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


#ifndef Xd6HtmlBrowser_h
#define Xd6HtmlBrowser_h

#include "xd640/Xd6HtmlView.h"
#include <FL/Fl_Group.h>
#include <FL/Fl_Select_Browser.H>
#include <stdio.h>

struct folder {
        Xd6ConfigFileGroup *server;
        Xd6ConfigFileGroup *box;
        int id;
};

enum {
	MODE_REPLY,
	MODE_REPLY_ALL,
	MODE_FORWARD,
};

class Xd6HtmlBrowser : public Fl_Group {
public:
	Xd6HtmlView *view;
	Fl_Select_Browser *list;

	int nb_folder;
	struct folder *folders;
	int pid;
	struct folder *current_folder;
	char path[1024];
	int reply_mode;
	FILE *reply_fp;
	char from[256];
	int upid;

	Xd6HtmlBrowser(int X, int Y, int W, int H);
	~Xd6HtmlBrowser(void);
	void load(const char *f);
	void resize(int X, int Y, int W, int H); 
	int handle(int e);
	void init(void);
	void add_folder(Xd6ConfigFileGroup *server, Xd6ConfigFileGroup *box, int k);
	struct folder *get_folder(void);
	void mail_get(void);
	void check_mail_get(void);
	void mailbox2html(const char *path);
	void load_mailbox(const char *path);
	void mail2html(const char *f, const char *p, FILE *index);
	void list_cb(void);
	void save_list(void);
	void mail_reply(void);	
	void reply_cb(Xd6XmlTreeElement* e);
	void write_reply(void);
	void send(void);
	void send_mail(char *path, char *name);
	void check_send_mail(void);
};

#endif

/*
 * "$Id: $"
 */
