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
#ifndef Xd6HtmlNavigation_h
#define Xd6HtmlNavigation_h

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Toggle_Button.H>
#include "Download.h"

class Xd6HtmlBrowser;

class Xd6HtmlNavigation : public Fl_Group {
public:
	Xd6HtmlBrowser *browser;
	Fl_Button *mail_get;
	Fl_Button *mail_send;
	Fl_Button *mail_compose;
	Fl_Button *mail_reply;
	Fl_Button *mail_replyall;
	Fl_Button *mail_forward;
	Fl_Button *next;
	Fl_Button *fileprint;
	Fl_Button *remove;
	Fl_Button *stop;
	Fl_Choice *folder;
	
        Fl_Pixmap *p_mail_get;
        Fl_Pixmap *p_mail_send;
	Fl_Pixmap *p_mail_compose;
	Fl_Pixmap *p_mail_reply;
	Fl_Pixmap *p_mail_replyall;
	Fl_Pixmap *p_mail_forward;
	Fl_Pixmap *p_next;
	Fl_Pixmap *p_fileprint;
	Fl_Pixmap *p_remove;
        Fl_Pixmap *p_stop;

	Xd6HtmlNavigation(Xd6HtmlBrowser *e, int X, int Y, int W, int H);
	~Xd6HtmlNavigation(void);
	
	static void cb_mail_get(Fl_Widget*, void*);
	static void cb_mail_send(Fl_Widget*, void*);
	static void cb_mail_compose(Fl_Widget*, void*);
	static void cb_mail_reply(Fl_Widget*, void*);
	static void cb_mail_replyall(Fl_Widget*, void*);
	static void cb_mail_forward(Fl_Widget*, void*);
	static void cb_next(Fl_Widget*, void*);
	static void cb_fileprint(Fl_Widget*, void*);
	static void cb_remove(Fl_Widget*, void*);
	static void cb_stop(Fl_Widget*, void*);
	static void cb_folder(Fl_Widget*, void*);
};

#endif

/*
 * "$Id: $"
 */
