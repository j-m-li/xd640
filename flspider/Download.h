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
#ifndef Download_h
#define Download_h

#include "xd640/Xd6ConfigFile.h"
#include <curl/curl.h>
#include "xd640/Xd6HtmlDownload.h"

enum {
	DNL_FINISHED = 0x0001,
	DNL_STARTED = 0x0002,
	DNL_STOPED = 0x0004,
	DNL_ERROR = 0x0008,
};

class Xd6HtmlRequest {
public:
	char *href;
	char *target;
	char *file;
	char *anchor;
	Xd6HtmlTagForm *form;
	Xd6HtmlFrame *frame;
	Xd6HtmlDisplay *disp;
	int pid;
	int status;
	int kbd_fp;
	Xd6HtmlRequest();
	~Xd6HtmlRequest();
	void start_download(void);
	void start_curl(void);
	void finish_download(void);
	void check_download(void);
	void stop_download(void);
	void fill_form(CURL *curl);
	void fill_form_buffer(CURL *curl, char **b, char *sep);
	void form_bufferadd(char **b, char *sep, char *name, char *value);
	void fill_multipart_form(CURL *curl);
	int list_local_dir(void);
};

class Download : public Xd6HtmlDownload {
public:

	int nb_requests;
	Xd6HtmlRequest **requests;
	int empty;

	Download();
	~Download();
	
	const char *request(const char *href, const char *target,
		Xd6HtmlTagForm *form, Xd6HtmlFrame *frame, Xd6HtmlDisplay *wi = NULL);
	const char *new_request(const char *href, const char *target,
		Xd6HtmlTagForm *form, Xd6HtmlFrame *frame, Xd6HtmlDisplay *wi);
	void spooler(void);
	void stop(void);
};

#endif

/*
 * "$Id: $"
 */
