/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2001  O'ksi'D
 *
 *                      All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *      Neither the name of O'ksi'D nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER 
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

Xd6HtmlDownload::Xd6HtmlDownload() 
{
	cfg = new Xd6ConfigFile("xd640", "Utilities");
}

Xd6HtmlDownload::~Xd6HtmlDownload()
{
	delete(cfg);
}


const char *Xd6HtmlDownload::request(const char *url, const char *target,
		Xd6HtmlTagForm *form, Xd6HtmlFrame *frame, Xd6HtmlDisplay *wi)
{
	const char *ret = NULL;

	if (!url || !frame) return NULL;

	if (!strncmp("data:", url, 5)) {
		ret = data_download(url);
	} else if (!strncmp("file:", url, 5) || (!strstr(url, ":")
		&& (!frame->url || !strncmp("file:", frame->url, 5)))) 
	{
		char *go;
		go = create_url(frame->url ? frame->url : NULL, url); 	
		ret = file_download(go);
		free(go);
	}
	if (ret) {
		if (wi) {
			wi->load(url, ret);
		}
	}
	return ret;
}

const char *Xd6HtmlDownload::file_download(char *file)
{
	char *loc;
	char buf[1024];
	int i;
	FILE *fi, *fo;

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
	
	fi = fl_fopen(file + 6, "r");
	if (!fi) return NULL;
	loc = Xd6ConfigFile::temp();
	fo = fl_fopen(loc, "w");
	if (!fo) { free(loc); fclose(fi); return NULL; }
	
	i = fread(buf, 1, 1024, fi);
	while (i > 0) {
		fwrite(buf, i, 1, fo);
		i = fread(buf, 1, 1024, fi);
	}
	fclose(fi);
	fclose(fo);

	return loc;
}

/*
 * FIXME: only supports base64 data: url !
 */
const char *Xd6HtmlDownload::data_download(const char *url)
{
	char *tmp, *loc;
	FILE *t;
	FILE *l;

	while(*url && *url != ',') url++;
	if (!*url) return NULL;

	url++;
	tmp = Xd6ConfigFile::temp();
	t = fl_fopen(tmp, "w");
	if (!t) { free(tmp); return NULL; }

	while(*url) {
		putc(*url, t);
		url++;
	}
	fclose(t);
	t = fopen(tmp, "r");
	if (!t) { free(tmp); return NULL; }
	loc = Xd6ConfigFile::temp();
	l = fopen(loc, "w");
	if (!t) { free(tmp);  free(loc); fclose(t); return NULL; }
	Xd6Decode_base64(t, l);
	fclose(t);
	fclose(l);
	fl_unlink(tmp);

	return loc; 
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
