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


#include <FL/fl_file_chooser.H>
#include "Xd6HtmlNavigation.h"
#include "Xd6HtmlBrowser.h"
#include "xd640/Xd6XmlStyle.h"
#include "gui.h"

#include <stdio.h>
#include <stdlib.h>
#include <libintl.h>

#define _(String) gettext((String))

#include "xpm/stop.xpm"
#include "xpm/mail_get.xpm"
#include "xpm/mail_send.xpm"
#include "xpm/mail_reply.xpm"
#include "xpm/mail_replyall.xpm"
#include "xpm/mail_forward.xpm"
#include "xpm/next.xpm"
#include "xpm/fileprint.xpm"
#include "xpm/remove.xpm"
#include "xpm/pencil.xpm"


Xd6HtmlNavigation::Xd6HtmlNavigation(Xd6HtmlBrowser *e, int X, int Y, int W, int H) : 
	Fl_Group(X, Y, W, H)
{
	int i;
	box(FL_UP_BOX);
        Y += 2;
        H -= 4;
        W = H;
        X += 3;

	browser = e;

        mail_get = new Fl_Button(X, Y, W, H); X += W + 1;
        mail_send = new Fl_Button(X, Y, W, H); X += W + 5;
        mail_compose = new Fl_Button(X, Y, W, H); X += W + 1;
        mail_reply = new Fl_Button(X, Y, W, H); X += W + 1;
        mail_replyall = new Fl_Button(X, Y, W, H); X += W + 1;
        mail_forward = new Fl_Button(X, Y, W, H); X += W + 5;
        next = new Fl_Button(X, Y, W, H); X += W + 1;
        fileprint = new Fl_Button(X, Y, W, H); X += W + 1;
        remove = new Fl_Button(X, Y, W, H); X += W + 1;
        stop = new Fl_Button(X, Y, W, H); X += W + 5;
	folder = new Fl_Choice(X, Y, 10, H);
        end();
	resizable(folder);
	
        p_stop = new Fl_Pixmap(stop_xpm);
	p_mail_get = new Fl_Pixmap(mail_get_xpm);
	p_mail_send = new Fl_Pixmap(mail_send_xpm);
	p_mail_compose = new Fl_Pixmap(pencil_xpm);
	p_mail_reply = new Fl_Pixmap(mail_reply_xpm);
	p_mail_replyall	= new Fl_Pixmap(mail_replyall_xpm);
	p_mail_forward	= new Fl_Pixmap(mail_forward_xpm);
	p_next	= new Fl_Pixmap(next_xpm);
	p_fileprint = new Fl_Pixmap(fileprint_xpm);
	p_remove = new Fl_Pixmap(remove_xpm);

        p_stop->label(stop);
	p_mail_get->label(mail_get);
	p_mail_send->label(mail_send);
	p_mail_compose->label(mail_compose);
	p_mail_reply->label(mail_reply);
	p_mail_replyall->label(mail_replyall);
	p_mail_forward->label(mail_forward);
	p_next->label(next);
	p_fileprint->label(fileprint);
	p_remove->label(remove);

	mail_get->callback(cb_mail_get);
	mail_send->callback(cb_mail_send);
	mail_compose->callback(cb_mail_compose);
	mail_reply->callback(cb_mail_reply);
	mail_replyall->callback(cb_mail_replyall);
	mail_forward->callback(cb_mail_forward);
	next->callback(cb_next);
	fileprint->callback(cb_fileprint);
	remove->callback(cb_remove);
	stop->callback(cb_stop);
	folder->callback(cb_folder);
	

}


Xd6HtmlNavigation::~Xd6HtmlNavigation()
{
}

void Xd6HtmlNavigation::cb_stop(Fl_Widget *w, void *data)
{
	Xd6HtmlNavigation *t = (Xd6HtmlNavigation*) w->parent();
//	t->loader->stop();
}

void Xd6HtmlNavigation::cb_mail_get(Fl_Widget *w, void *data)
{
	((Xd6HtmlBrowser*)w->parent()->parent())->mail_get();
}

void Xd6HtmlNavigation::cb_mail_send(Fl_Widget *w, void *data)
{
	((Xd6HtmlBrowser*)w->parent()->parent())->send();
}

void Xd6HtmlNavigation::cb_mail_reply(Fl_Widget *w, void *data)
{
	((Xd6HtmlBrowser*)w->parent()->parent())->mail_reply();
}

void Xd6HtmlNavigation::cb_mail_replyall(Fl_Widget *w, void *data)
{
}

void Xd6HtmlNavigation::cb_mail_forward(Fl_Widget *w, void *data)
{
}

void Xd6HtmlNavigation::cb_next(Fl_Widget *w, void *data)
{
}

void Xd6HtmlNavigation::cb_fileprint(Fl_Widget *w, void *data)
{
}

void Xd6HtmlNavigation::cb_remove(Fl_Widget *w, void *data)
{
}

void Xd6HtmlNavigation::cb_folder(Fl_Widget *w, void *data)
{
}

void Xd6HtmlNavigation::cb_mail_compose(Fl_Widget *w, void *data)
{
	system("flmailer &");
}

/*
 * "$Id: $"
 */

