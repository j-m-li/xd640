/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000,2001  O'ksi'D
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


#include "gui.h"
#include "Xd6HtmlBrowser.h"
#include "MailParser.h"
#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libintl.h>
#include <dirent.h>
#include <sys/stat.h>
#include "xd640/Xd6XmlUtils.h"

#define _(String) gettext((String))

extern Xd6ConfigFile *cfg;
extern char **environ;

static void l_cb(Fl_Widget *w, void *)
{
	((Xd6HtmlBrowser*)w->parent())->list_cb();
}

Xd6HtmlBrowser::Xd6HtmlBrowser(int X, int Y, int W, int H) : Fl_Group(X, Y, W, H)
{
	tool = new Xd6HtmlNavigation(this, X, Y, W, 25);
	tool->end();
	list = new Fl_Select_Browser(X, Y + 25, W, 80);
	list->callback(l_cb);
	view = new Xd6HtmlView(X, Y + 105, W, H - 105);
	view->frame->editor = 0;
	view->frame->wysiwyg = 0;
	folders = NULL;
	nb_folder = 0;
	pid = 0;
	upid = 0;
	init();
}


Xd6HtmlBrowser::~Xd6HtmlBrowser()
{
}

void Xd6HtmlBrowser::save_list()
{
	int i;
	int size = list->size();
	char path[1024];
	FILE *index;
	snprintf(path, 1024, "%s/%s/index.txt", 
				cfg->user_paths->apps, 
				current_folder->box->get_create_item(
				"directory", NULL)->get_value());
	index = fopen(path, "w");
	if (!index) return;
	for (i = 1; i <= size; i++) {
		const char *d;
		d = list->text(i);
		if (*d == '\v') {
			fprintf(index, "%s\n", d);

		}
	}
	fclose(index);
}

void Xd6HtmlBrowser::list_cb(void)
{
	int v = list->value();
	char *d;
	if (v < 1) return;
	d = (char*)list->text(v);
	if (!current_folder) return;
	if (d) {
		int save = 0;
		if (*d == '\v' && *(d +1) == 'b') {
			d++;
			*d = ' ';
			save = 1;
		}
		while (*d && *d != '\f') d++; d++;
		while (*d && *d != '\f') d++; d++;
		while (*d && *d != '\f') d++; d++;
		if (*d) d += 3;
		snprintf(path, 1024, "%s/%s/%s", 
				cfg->user_paths->apps, 
				current_folder->box->get_create_item(
				"directory", NULL)->get_value(),
				d);
		view->load(path);
		if (save) {
			Fl::check();
			Fl::flush();
			save_list();
		}
	}
}

struct folder *Xd6HtmlBrowser::get_folder()
{
	int x, i;
	x = tool->folder->value();

	i = 0;
	while (i < nb_folder) {
		if (folders[i].id >= x) {
			return folders + i;
		}
		i++;
	}
	return NULL;
}

void Xd6HtmlBrowser::add_folder(Xd6ConfigFileGroup *server, 
	Xd6ConfigFileGroup *box, int k)
{
	folders = (struct folder*) realloc(folders, sizeof(struct folder) *
			(nb_folder + 1));
	folders[nb_folder].server = server;
	folders[nb_folder].box = box;
	folders[nb_folder].id = k;
	nb_folder++;
}

void Xd6HtmlBrowser::init()
{
	char path[1024];
	int i, j, k;
	Xd6ConfigFileSection *boxes;
	Xd6ConfigFileGroup *server;
	Xd6ConfigFileGroup *box;
	
	nb_folder = 0;
	tool->folder->clear();
	boxes = cfg->get_config_section("mailbox");
	if (!boxes) return;

	
	i = 0;
	k = 0;
	while (i < boxes->nb_items) {
		tool->folder->add(boxes->items[i]->get_name());
		k++;
		if (boxes->items[i]->type == CONFIG_FILE_GROUP) {
			server = (Xd6ConfigFileGroup*)boxes->items[i];
			j = 0;
			while (j < server->nb_items) {
				if (server->items[j]->type == 
					CONFIG_FILE_GROUP) 
				{
					box = (Xd6ConfigFileGroup*)
						server->items[j];
					tool->folder->add(box->get_name());
					add_folder(server, box, k);
					k++;
				}
				j++;
			}
			
		}
		i++;
	}

	tool->folder->value(0);
	current_folder = get_folder(); 
	snprintf(path, 1024, "%s/%s", 
				cfg->user_paths->apps, 
				current_folder->box->get_create_item(
				"directory", NULL)->get_value());
	load_mailbox(path);
}


static void cb_dnl(Xd6HtmlBrowser *b)
{
	b->check_mail_get();
}


void Xd6HtmlBrowser::load_mailbox(const char *path)
{
	char buf[1024];
	static int w[4];
	
	snprintf(buf, 1024, "%s/index.txt", path);
	w[0] = 350;
	w[1] = 200;
	w[2] = 200;
	w[3] = 0;
	list->format_char('\v');
	list->column_char('\f');
	list->column_widths(w);
	list->load(buf);
	list->select(1);
	list->do_callback();
}

void Xd6HtmlBrowser::check_mail_get()
{
	int ret, status;
	char path[1024];
	FILE *f;

	snprintf(path, 1024, "%s/%s/dnl.log", 
				cfg->user_paths->apps, 
				current_folder->box->get_create_item(
				"directory", NULL)->get_value());
	f = fopen(path, "r");
	ret = -1;
	if (f) {
		ret = fread(path, 1, 1024, f); 
		fclose(f);
	}
	if (ret > 0) {
		path[ret] = '\0';
		((GUI*)parent())->stat_bar->value(path);
		((GUI*)parent())->stat_bar->redraw();
		Fl::redraw();
		Fl::check();
		Fl::flush();
	}


	ret = waitpid(pid, &status, WNOHANG);
	if (1 || ret == pid) {
		if (1 || !status) {
			MailParser *p;
			p = new MailParser();
			snprintf(path, 1024, "%s/%s", cfg->user_paths->apps, 
				current_folder->box->get_create_item(
				"directory", NULL)->get_value());
			
			p->mailbox2html(path);
			delete(p);
			load_mailbox(path);
		} else {
			snprintf(path, 1024, "%s/%s/dnl.err", 
				cfg->user_paths->apps, 
				current_folder->box->get_create_item(
				"directory", NULL)->get_value());
			f = fopen(path, "r");
			ret = -1;
			if (f) {
				ret = fread(path, 1, 1024, f); 
				fclose(f);
			}
			if (ret > 0) {
				path[ret] = '\0';
				fl_alert(path);
			}
		}
		pid = 0;
		return;		
	}
	Fl::add_timeout(0.2, (Fl_Timeout_Handler)cb_dnl, this);	
}

void Xd6HtmlBrowser::mail_get()
{
	char cmd[4096];
	char path[1024];
	struct folder *f;
	FILE *dn;
	char *argv[4];

	if (pid > 0) {
		fl_alert(_("flmail is already downloading mails."));
		return;
	}
	current_folder = f = get_folder();
	if (!f) return;
	
	pid = fork();
	if (pid != 0) {
		if (pid > 0) {
			((GUI*)parent())->stat_bar->value(
				_("Loading messages"));
			Fl::add_timeout(0.2, (Fl_Timeout_Handler)cb_dnl, this);	
		} else {
			pid = 0;
		}
		return;
	}

	snprintf(path, 1024, "%s/%s", cfg->user_paths->apps, 
		f->box->get_create_item("directory", NULL)->get_value());
	mkdir(path, S_IXUSR|S_IWUSR|S_IRUSR);
	snprintf(path, 1024, "%s/%s/dnl", cfg->user_paths->apps, 
		f->box->get_create_item("directory", NULL)->get_value());

	dn = fopen(path, "w");
	chmod(path, S_IXUSR|S_IRUSR|S_IWUSR);	
	path[strlen(path) - 4] = '\0';
	fprintf(dn, "#/bin/sh\n cat >> \"%s/download.$$.txt\"", path);
	fclose(dn);
	snprintf(cmd, 4096, 
		"umask 077; echo \"poll %s proto pop3"
		" user \\\"%s\\\" pass \\\"%s\\\""
		" \nmda \\\"%s/dnl\\\" "
		"\" > ~/.fetchmailrc; "
		"fetchmail -a  2>'%s/dnl.err' 1>'%s/dnl.log' ", 
		f->server->get_create_item("pop", NULL)->get_value(),
		f->server->get_create_item("user", NULL)->get_value(),
		f->server->get_create_item("password", NULL)->get_value(), 
		path, path, path);
	argv[0] = "sh";
	argv[1] = "-c";
	argv[2] = cmd;
	argv[3] = 0;
	_exit(execve("/bin/sh", argv, environ));
}

void Xd6HtmlBrowser::load(const char *f)
{
	view->load(f);
}

void Xd6HtmlBrowser::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	tool->resize(X, Y, W, 25);
	list->resize(X, Y + 25, W, 80);
	view->resize(X, Y + 105, W, H - 105);
}

int Xd6HtmlBrowser::handle(int e)
{
	int ret;

	ret = Fl_Group::handle(e);
	
	return ret;
}

static void scan_cb(Xd6XmlTreeElement* e, void *d)
{
	((Xd6HtmlBrowser*)d)->reply_cb(e);
}

void Xd6HtmlBrowser::reply_cb(Xd6XmlTreeElement* e)
{
	const char *n;
	n = e->get_attr_value("name");
	if (n) {
		const char *v;
		v =  e->get_attr_value("value");
		if (!v) return;
		if (!strcmp(n, "Subject")) {
			if (reply_mode == MODE_FORWARD) {
				fprintf(reply_fp, "Subject: Fw: %s\n", v); 
			} else {
				fprintf(reply_fp, "Subject: Re: %s\n", v); 
			}
		} else if (!strcmp(n, "To")) {
			if (reply_mode == MODE_REPLY_ALL) {
				fprintf(reply_fp, "Cc: %s\n", v);
			}
		} else if (!strcmp(n, "Cc")) {
			if (reply_mode == MODE_REPLY_ALL) {
				fprintf(reply_fp, "Cc: %s\n", v);
			}
		} else if (!strcmp(n, "From")) {
			snprintf(from, 256, _("%s wrote :\n"), v);
			if (reply_mode != MODE_FORWARD) {
				fprintf(reply_fp, "To: %s\n", v);
			}
		}
	}
}

void Xd6HtmlBrowser::write_reply()
{
	fprintf(reply_fp, "\n");
	fprintf(reply_fp, "%s", from);
	fprintf(reply_fp, "> ");
	view->frame->quote = "> ";
	view->frame->fp = reply_fp;
	view->frame->scan_all(Xd6HtmlFrame::scan_to_text_cb, view->frame);
	view->frame->quote = "";	

}
void Xd6HtmlBrowser::mail_reply()
{
	Xd6XmlParser *p;
	char buf[1024];
	
	if (!current_folder) {
		return;
	}
	p = new Xd6XmlParser();
	
	snprintf(buf, 1024, "%s/tmp", cfg->user_paths->apps); 
	mkdir(buf, S_IRWXU);
	snprintf(buf, 1024, "/bin/rm %s/tmp/*.txt", cfg->user_paths->apps); 
	system(buf);
	snprintf(buf, 1024, "%s/tmp/%s.txt", cfg->user_paths->apps, cfg->pid);

	reply_fp = fopen(buf, "w");
	if (reply_fp) {	
		reply_mode = MODE_REPLY;
		p->scan_file(path, scan_cb, this);
		write_reply();
		fclose(reply_fp);
	}
	delete(p);
	snprintf(buf, 1024, "flmailer '%s/tmp/%s.txt' &", 
		cfg->user_paths->apps, cfg->pid);
	system(buf);
}


static int selfile(const struct dirent *d)
{
        return (int) strstr(d->d_name, ".txt");
}

static void cb_upl(Xd6HtmlBrowser *b)
{
        b->check_send_mail();
}


void Xd6HtmlBrowser::check_send_mail()
{
	int ret, status;
	ret = waitpid(upid, &status, WNOHANG);

	if (ret == upid) {
		if (!status) {

		} else {
			fl_alert(_("Error during send mail !"));
		}
		upid = 0;
	}
	Fl::add_timeout(0.2, (Fl_Timeout_Handler)cb_upl, this);
}

void Xd6HtmlBrowser::send_mail(char *path, char *name)
{
	char cmd[2048];
	char *argv[4];

	upid = fork();
	if (upid != 0) {
		if (pid > 0) {
			((GUI*)parent())->stat_bar->value(
				_("Sending messages"));
			Fl::add_timeout(0.2, (Fl_Timeout_Handler)cb_upl, this);
		} else {
			upid = 0;
		}
		return;
	}
	snprintf(cmd, 2048, "cd %s; cat %s | smtp-send %s 2>upl.log "
		"&& mv %s ../sent/",
		path, name, 
		current_folder->server->
			get_create_item("smtp", NULL)->get_value(), 
		name);

	argv[0] = "sh";
	argv[1] = "-c";
	argv[2] = cmd;
	argv[3] = 0;
	_exit(execve("/bin/sh", argv, environ));
}

void Xd6HtmlBrowser::send()
{
	char buf[1024];
	int nbd;
	struct dirent **namelist = NULL;
	int i;

	if (upid > 0) {
		fl_alert(_("flmail is already uploading mails."));
		return;
	}
	snprintf(buf, 1024, "%s/sent/", cfg->user_paths->apps);
	mkdir(buf, S_IRWXU);
	snprintf(buf, 1024, "%s/unsent/", cfg->user_paths->apps);
	nbd = scandir(buf, &namelist, selfile, alphasort);
	
	printf("%s %d\n", buf, nbd);
	
	for (i = 0; i < nbd; i++) {
		send_mail(buf, namelist[i]->d_name);
		free(namelist[i]);
	}
	free(namelist);
	
}

/*
 * "$Id: $"
 */

