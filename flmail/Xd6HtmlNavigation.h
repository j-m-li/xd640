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
