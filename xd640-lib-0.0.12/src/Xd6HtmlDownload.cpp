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

#include <FL/Fl.h>
#include "Xd6Std.h"
#include "Xd6HtmlDownload.h"
#include "Xd6HtmlFrame.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "Xd6Base64.h"

Xd6HtmlDownload* downloader = NULL;
char Xd6HtmlDownload::local_file[1024];

Xd6HtmlDownload::Xd6HtmlDownload() 
{
	tmp_dir = (char *) malloc(1024);
	cfg = new Xd6ConfigFile("xd640", "Utilities");
	cfg_sec = cfg->get_config_section(cfg->app_name);
	
	snprintf(tmp_dir, 1024,"%s/%s", cfg->user_paths->apps, cfg->pid);
	mkdir(tmp_dir, 0777);
	file = NULL;
}

Xd6HtmlDownload::~Xd6HtmlDownload()
{
	delete(cfg);
	delete(cfg_sec);
	free(tmp_dir);
}

static void loader_cb(Xd6HtmlFrame *frame) 
{
	frame->load();
}


const char *Xd6HtmlDownload::request(const char *url, const char *target,
		Xd6HtmlTagForm *form, Xd6HtmlFrame *frame, Xd6HtmlDisplay *wi)
{
	const char *ret;
	if (file) free(file);
	file = NULL;
	if (!strncmp("data:", url, 5)) {
		ret = download_data(url);
	} else {
		ret = curl_download(url, frame->url);
	}
	if (ret) {
		if (wi) {
			wi->load(ret, file);
		} else {
			free(frame->file);
			free(frame->url);
			frame->file = strdup(ret);
			frame->url = strdup(file);
			Fl::add_timeout(0.1, (Fl_Timeout_Handler)
				loader_cb,(void*)frame);
		}
	}
	return ret;
}

const char *Xd6HtmlDownload::curl_download(const char *u, const char *b)
{
	char tmp[1024];
	char cmd[2048];
	int i;

	if (!b) b = "";
	file = create_url(b, u);
	i = 0;
	while (file[i]) {
		if (file[i] == '#') {
			file[i] = '\0';
			break;
		}
		i++;
	}
	if (i == 0) return NULL;
	if (strncmp("file:/", file, 6)) return NULL;
	
	snprintf(local_file, 1024,"%s/%p.data", tmp_dir, u);
	snprintf(tmp, 1024,"%s/%p.dnl", tmp_dir, u);

	snprintf(cmd, 2048, "cp -f %s %s", file + 6, local_file);
	
	system(cmd);
	return local_file;
}

const char *Xd6HtmlDownload::download_data(const char *url)
{
	char tmp[1024];
	FILE *t;
	FILE *l;

	snprintf(local_file, 1024,"%s/%p.data", tmp_dir, url);
	snprintf(tmp, 1024,"%s/%p.tmp", tmp_dir, url);

	while(*url && *url != ',') url++;
	if (!*url) return NULL;
	url++;
	t = fopen(tmp, "w");
	while(*url) {
		putc(*url, t);
		url++;
	}
	fclose(t);
	t = fopen(tmp, "r");
	l = fopen(local_file, "w");
	Xd6Decode_base64(t, l);
	fclose(t);
	fclose(l);
	unlink(tmp);

	return local_file; 
}

static void clean_url(char *url) 
{
	int i = 0;
	while (url[i]) {
		
		if (url[i] == '.' && (url[i + 1] == '/' || url[i + 1] == '\0')){
			int ii = i;
			while (url[ii + 2]) {
				url[ii] = url[ii + 2];
				ii++;
			}
			url[ii] = '\0';
			continue;
		} else if (i > 1 && url[i] == '.' && url[i + 1] == '.' &&
			(url[i + 2] == '/' || url[i + 2] == '\0') && 
			url[i - 1] == '/' ) 
		{
			int ii;
			int o;

			o = i + 2;
			i -= 2;
			while (i > 0 && url[i] != '/') i--;
			ii = i;
			while (url[o]) {
				url[ii++] = url[o++];	
			}
			url[ii] = '\0';
			continue;
		}
		i++;
	}

}


char *Xd6HtmlDownload::create_url(const char *base, const char *u) 
{
	int i;
	char *url;

	if (base) {
		url = strdup(base);
	} else {
		char buf[1024];
		sprintf(buf, "file://");
		getcwd(buf + 7, 1000);
		strcat(buf, "/");
		url = strdup(buf);
	}
	if (!u) return url;

	i = 0;
	while (u[i]) {
		if (u[i] == ':') {
			break;
		} else if (u[i] == '/') {
			break;
		} else if (u[i] == '#') {
			break;
		}	
		i++;
	}
	if (u[i] == ':') {
		free(url);
		i++;
		if (u[i] == '/' && u[i + 1] == '/') {
			i += 2;
			while (u[i] && u[i] != '/') i++;
			if (u[i] == '/') {
				url = strdup(u);
			} else {
				url = (char*) malloc(strlen(u) + 10);
				sprintf(url, "%s/", u);
			}
		} else {
			url = strdup(u);
		}
	} else if (u[0] == '/') {
		char *nu = (char*) malloc(1024);
		int j;
		j = 1;
		while (url[j] && url[j] != ':') {
			j++;
		}
		if (!url[j]) {
			free(url);
			url = strdup("file:///");
			j = 6;
		}
		while (url[j]) {
			if (url[j] == '/' && url[j - 1] == '/') {
				j++;
				break;
			}
			j++;
		}
		while (url[j]) {
			if (url[j] == '/') {
				break;
			}
			j++;
		}
		url[j] = '\0';
		snprintf(nu, 1024, "%s%s", url, u);
		free(url);
		url = nu;
	} else if (u[0] == '#') {
		int j = 0;
		char *nu = (char*) malloc(1024);
		while (url[j]) {
			if (url[j] == '#') {
				url[j] = '\0';
				break;
			}
			j++;
		}
		snprintf(nu, 1024, "%s%s", url, u);
		free(url);
		url = nu;
	} else {
		char *nu = (char*) malloc(1024);
		i = strlen(url);
		while (i > 0) {
			i--;
			if (url[i] == '/') {
				url[i] = '\0';	
				break;
			}
		}
		snprintf(nu, 1024, "%s/%s", url, u);	
		free(url);
		url = nu;
	}
	
	clean_url(url);
	return url;
}


/*
 * "$Id: $"
 */
