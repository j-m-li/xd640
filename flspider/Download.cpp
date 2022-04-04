/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2001  O'ksi'D
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 *   Author : Jean-Marc Lienher ( http://oksid.ch )
 *
 ******************************************************************************/

#include <FL/fl_ask.H>
#include "Download.h"
#include "xd640/Xd6HtmlTagForm.h"
#include "xd640/Xd6HtmlTagInputText.h"
#include "xd640/Xd6HtmlFrame.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include "xd640/Xd6Base64.h"
#include "gui.h"

extern char **environ;

Xd6HtmlRequest::Xd6HtmlRequest()
{
	href = NULL;
	target = NULL;
	file = NULL;
	anchor = NULL;
	form = NULL;
	frame = NULL;
	pid = -1;
	status = 0;
}

Xd6HtmlRequest::~Xd6HtmlRequest()
{
	if (href) free(href);
	if (target) free(target);
	if (file) free(file);
	if (anchor) free(anchor);
}

void Xd6HtmlRequest::form_bufferadd(char **b, char *sep, char *name, char *value)
{
	int o;
	char *v;
	o = strlen(*b);
	v = curl_escape(value, strlen(value));
	*b = (char*) realloc(*b, o + strlen(name) + strlen(v) + 50);
	sprintf(*b + o, "%s=%s%s", name, v, sep);
	free(v);
}

void Xd6HtmlRequest::fill_form_buffer(CURL *curl, char **b, char *sep)
{
	int i;
	for (i = 0; i < form->nb_elem; i++) {
		Xd6HtmlDisplay *d;
		d = form->elem[i];
		switch(d->display) {
		case DISPLAY_SELECT:
		case DISPLAY_TEXTAREA:
			break;
		case DISPLAY_INPUT_TEXT:
		case DISPLAY_INPUT_PASSWORD:
		{
			Xd6HtmlTagInputText *t = (Xd6HtmlTagInputText*) d;
			if (!t->name) break;
			form_bufferadd(b, sep, t->name, t->text);
			break;
		}
		case DISPLAY_INPUT_CHECKBOX:
		case DISPLAY_INPUT_RADIO:
			break;
		case DISPLAY_INPUT_SUBMIT:

			break;
		case DISPLAY_INPUT_IMAGE:
		case DISPLAY_INPUT_RESET:
		case DISPLAY_INPUT_BUTTON:
		case DISPLAY_INPUT_HIDDEN:
		case DISPLAY_INPUT_FILE:
		case DISPLAY_BUTTON:
			break;
		}
	}
}

void Xd6HtmlRequest::fill_multipart_form(CURL *curl)
{
	HttpPost* post = NULL;
	HttpPost* last = NULL;
	int i;
	
	for (i = 0; i < form->nb_elem; i++) {
		Xd6HtmlDisplay *d;
		d = form->elem[i];
		switch(d->display) {
		case DISPLAY_SELECT:
		case DISPLAY_TEXTAREA:
			break;
		case DISPLAY_INPUT_TEXT:
		case DISPLAY_INPUT_PASSWORD:
		{
			Xd6HtmlTagInputText *t = (Xd6HtmlTagInputText*) d;
			if (!t->name) break;
			curl_formadd(&post, &last, CURLFORM_PTRNAME, t->name,
				CURLFORM_PTRCONTENTS, t->text, CURLFORM_END);
			break;
		}
		case DISPLAY_INPUT_CHECKBOX:
		case DISPLAY_INPUT_RADIO:
			break;
		case DISPLAY_INPUT_SUBMIT:

			break;
		case DISPLAY_INPUT_IMAGE:
		case DISPLAY_INPUT_RESET:
		case DISPLAY_INPUT_BUTTON:
		case DISPLAY_INPUT_HIDDEN:
		case DISPLAY_INPUT_FILE:
		case DISPLAY_BUTTON:
			break;
		}
	}
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
}

void Xd6HtmlRequest::fill_form(CURL *curl)
{
	char *buf = NULL;

	if (form->enctype && !strcmp("multipart/form-data", form->enctype)) {
		fill_multipart_form(curl);
	} else if (form->method && !strcasecmp("post", form->method)) {
		buf = (char*) malloc(1);
		buf[0] = '\0';
		fill_form_buffer(curl, &buf, "\r\n");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);
		curl_easy_setopt(curl, CURLOPT_POST, 1);
	} else {
		buf = (char*) malloc(strlen(href) + 1024);
		sprintf(buf, "%s?", href);
		fill_form_buffer(curl, &buf, "&");
		buf[strlen(buf) - 1] = '\0';
		free(href);
		href = buf;
	}
}

void Xd6HtmlRequest::start_curl()
{
	char errorbuffer[CURL_ERROR_SIZE];
	FILE *fout;
	CURL *curl;
	int res;

	errorbuffer[0] = '\0';

	fout = fopen(file, "w");
	if (!fout) return;
	curl = curl_easy_init();
	if (!curl) return;


	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorbuffer);
	if (form) {
		fill_form(curl);	
	}
	curl_easy_setopt(curl, CURLOPT_FILE, fout);
	curl_easy_setopt(curl, CURLOPT_URL, href);
	res = curl_easy_perform(curl);
	fclose(fout);
	if (res != CURLE_OK) {
		char buf[1024];
		snprintf(buf, 1024, "%s.log", file);
		fout = fopen(buf, "w");
		fprintf(fout, errorbuffer);
		fclose(fout);
		_exit(127);
	}
}

static int selfile(const struct dirent *)
{
	return 1;
}

int Xd6HtmlRequest::list_local_dir()
{
	struct stat s;
	char *sep = "";
	int i;
	int nbd;
	struct dirent **namelist = NULL;
	FILE *fout;

	if (strncmp("file:/", href, 6)) return 0;
	stat(href + 5, &s);
	if (!S_ISDIR(s.st_mode)) return 0;	
	fout = fopen(file, "w");
	if (!fout) return 0;
	if (href[strlen(href) - 1] != '/') {
		sep = "/";
	}
	fprintf(fout, "<html><body><pre>\n");
	
	nbd = scandir(href + 5, &namelist, selfile, alphasort);
	
	for (i = 0; i < nbd; i++) {
		fprintf(fout, "<a href=\"%s%s%s\">", href, sep, 
			namelist[i]->d_name);
		fprintf(fout, "%s</a>\n", namelist[i]->d_name);
		free(namelist[i]);
	}
	if (namelist) free(namelist);

	fprintf(fout, "</pre></body></html>\n");
	fclose(fout);
	return 1;
}

void Xd6HtmlRequest::start_download()
{
//	char command[4096];
//	char *argv[4];
//	int filedes[2] = { -1, -1};

//	if (pipe(filedes)) return;
	pid = fork();
	if (pid != 0) {
		status = DNL_STARTED;
		//kbd_fp = filedes[1];
		return;
	}
//	close(STDIN_FILENO);
//	dup(filedes[0]);
//	close(filedes[0]);
/*
	snprintf(command, 4096, 
		"curl -s -S  -o '%s' %s  2> %s.log ", 
		file, href, file);
	argv[0] = "sh";
	argv[1] = "-c";
	argv[2] = command;
	argv[3] = 0;
	execve("/bin/sh", argv, environ);
*/
	if (!list_local_dir()) {
		start_curl();
	}
	_exit(0);	
}

static void loader_cb(Xd6HtmlFrame *frame)
{
        frame->load();
}

void Xd6HtmlRequest::finish_download()
{
	status = DNL_FINISHED;
	if (disp) {
		disp->load(file, href);
	} else {
		free(frame->file);
		free(frame->url);
		frame->file = strdup(file);
		frame->url = strdup(href);
		Fl::add_timeout(0.1, (Fl_Timeout_Handler) loader_cb,
			(void*)frame);
	}
}

void Xd6HtmlRequest::check_download()
{
	int ret, stat;
	if (status != DNL_STARTED) return;

	ret = waitpid(pid, &stat, WNOHANG);

	if (ret == -1)	{
		status = DNL_ERROR;
	} if (ret == pid) {
		if (WIFEXITED(stat) && WEXITSTATUS(stat) != 0) {
			char buf[1024];
			FILE *f;
			snprintf(buf, 1024, "%s.log", file);
			f = fopen(buf, "r");
			if (f) {
				int r;
				r = fread(buf, 1, 1024, f);
				if (r < 1) r = 1;
				buf[r] = '\0';
				fclose(f);
			} else {
				buf[0] = '\0';
			}
			if (!disp) {
				fl_alert(buf);
			}
			status = DNL_ERROR;
			GUI::self->stat_bar->value(buf);
			GUI::self->stat_bar->redraw();
			GUI::self->damage(FL_DAMAGE_CHILD);
			Fl::flush();
		} else {
			finish_download();
		}
	}
}

void Xd6HtmlRequest::stop_download()
{
	int stat;
	status = DNL_STOPED;
	if (pid > 0) {
		kill(pid, 9);
	}
	waitpid(pid, &stat, WNOHANG);
}

Download::Download() 
{
	nb_requests = 0;
	requests = NULL;
}

Download::~Download()
{
	while (nb_requests > 0) {
		delete(requests[--nb_requests]);
	}
	delete(requests);
}


static void spool_cb(Download *d)
{
        d->spooler();
}

static void progress()
{
	static int p = 0;
	char *prog[4] = {"|", "/", "-", "\\"};
	p++;
	if (p > 3) p = 0;
	GUI::self->stat_bar->value(prog[p]);
	GUI::self->stat_bar->redraw();
	GUI::self->damage(FL_DAMAGE_CHILD);
	Fl::flush();
}

void Download::spooler()
{
	int i;
	empty = 1;
	for (i = 0; i < nb_requests; i++) {
		if (requests[i]->status == DNL_STARTED) {
			empty = 0;
			progress();
			requests[i]->check_download();
			progress();
		}
	}
	if (!empty) {
		Fl::add_timeout(0.2, (Fl_Timeout_Handler) spool_cb, 
			(void*)this);
	} else {
		GUI::self->browser->view->frame->measure();
		GUI::self->browser->view->frame->create_pages();
		GUI::self->browser->redraw();
	}
}

void Download::stop()
{
	int i;
	for (i = 0; i < nb_requests; i++) {
		if (requests[i]->status == DNL_STARTED) {
			requests[i]->stop_download();
		}
	}
}

const char *Download::new_request(const char *url, const char *target,
		Xd6HtmlTagForm *form, Xd6HtmlFrame *frame, Xd6HtmlDisplay *wi)
{
	Xd6HtmlRequest *req;
	int i;
	char *real_url;
	char *a = "";

	req = new Xd6HtmlRequest();
	requests = (Xd6HtmlRequest**) realloc(requests, 
		sizeof(Xd6HtmlRequest*) * (nb_requests + 1));
	requests[nb_requests] = req;
	nb_requests++;
	

	real_url = create_url(frame->url, url);
	i = 0;
	while (real_url[i]) {
		if (real_url[i] == '#') {
			real_url[i] = '\0';
			a = real_url + i + 1;
		}
		i++;
	}
	
	snprintf(local_file, 1024,"%s/%p.data", tmp_dir, real_url);
	req->href = real_url;
	req->anchor = strdup(a);
	req->file = strdup(local_file);
	req->target = target ? strdup(target) : strdup("");
	req->form = form;
	req->frame = frame;
	req->disp = wi; 

	if (!wi) {
		stop();
		GUI::self->browser->tool->new_url(req->href);
		GUI::self->browser->redraw();
		Fl::flush();
	}

	if (!form) {
		i = 0;
		while (i < nb_requests - 1) {
			if (requests[i]->form == NULL && 
				requests[i]->status == DNL_FINISHED &&
				!strcmp(req->href, requests[i]->href))
			{
				progress();
				free(req->file);
				req->file = strdup(requests[i]->file);
				req->finish_download();
				progress();
				return requests[i]->file;
			}
			i++;
		}
		req->start_download();
	} else {
		req->start_download();
	}
	
	Fl::remove_timeout((Fl_Timeout_Handler) spool_cb, (void*)this);
	Fl::add_timeout(0.1, (Fl_Timeout_Handler) spool_cb, (void*)this);
	return local_file;
}

const char *Download::request(const char *url, const char *target,
		Xd6HtmlTagForm *form, Xd6HtmlFrame *frame, Xd6HtmlDisplay *wi = NULL)
{
	const char *ret;
		
	if (!url) return NULL;

	if (!strncmp("data:", url, 5)) {
		ret = download_data(url);
		if (ret && wi) {
			wi->load(ret, NULL);
		}
	} else {
		ret = new_request(url, target, form, frame, wi);
	}
	return ret;
}

/*
 * "$Id: $"
 */
