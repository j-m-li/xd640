/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2001  O'ksi'D
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
