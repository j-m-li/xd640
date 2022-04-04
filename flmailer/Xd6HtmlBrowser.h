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
