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


#include <FL/Fl_File_Chooser.H>
#include "Xd6HtmlNavigation.h"
#include "Xd6HtmlBrowser.h"
#include "xd640/Xd6XmlStyle.h"
#include "gui.h"

#include <stdio.h>
#include <stdlib.h>
#include <libintl.h>

#define _(String) gettext((String))

#include "xpm/back.xpm"
//#include "xpm/forward.xpm"
#include "xpm/stop.xpm"


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

        back = new Fl_Button(X, Y, W, H); X += W + 1;
        //forward = new Fl_Button(X, Y, W, H); X += W + 1;
        stop = new Fl_Button(X, Y, W, H); X += W + 1;
	loc = new Fl_Input(X, Y, w() - (X - x()) - 1, H);
        end();
	resizable(loc);

        p_back = new Fl_Pixmap(back_xpm);
        //p_forward = new Fl_Pixmap(forward_xpm);
        p_stop = new Fl_Pixmap(stop_xpm);

        p_back->label(back);
        //p_forward->label(forward);
        p_stop->label(stop);


	back->callback(cb_back);
	//forward->callback(cb_forward);
	stop->callback(cb_stop);
	loc->callback(cb_loc);
	
	loader = new Download();	
	downloader = loader;

	for (i = 0; i < 10; i++) {
		history[i] = NULL;
	}
}


Xd6HtmlNavigation::~Xd6HtmlNavigation()
{
	delete(loader);
	downloader = NULL;
}

void Xd6HtmlNavigation::new_url(const char *u)
{
	int i;
	loc->value(u);
	
	if (history[9]) free(history[9]);
	for (i = 9; i > 0; i--) {
		history[i] = history[i - 1];
	}
	history[0] = strdup(u);
}

void Xd6HtmlNavigation::cb_back(Fl_Widget *w, void *data)
{
	int i;
	char *u;
	Xd6HtmlNavigation *t = (Xd6HtmlNavigation*) w->parent();

	if (!t->history[1])  return;

	free(t->history[0]);
	u = t->history[1];

	for (i = 0; i < 8; i++) {
		t->history[i] = t->history[i + 2];

	}
	t->history[8] = NULL;
	t->history[9] = NULL;
	
	GUI::self->load(u);
	free(u);
}

void Xd6HtmlNavigation::cb_forward(Fl_Widget *w, void *data)
{
	//Xd6HtmlNavigation *t = (Xd6HtmlNavigation*) w->parent();
}

void Xd6HtmlNavigation::cb_loc(Fl_Widget *w, void *data)
{
	Xd6HtmlNavigation *t = (Xd6HtmlNavigation*) w->parent();
	//t->browser->load(((Fl_Input*)w)->value());
	downloader->request(((Fl_Input*)w)->value(), "", NULL, 
		t->browser->view->frame, NULL);
	t->browser->redraw();
	printf ("loaaa\n");
}

void Xd6HtmlNavigation::cb_stop(Fl_Widget *w, void *data)
{
	Xd6HtmlNavigation *t = (Xd6HtmlNavigation*) w->parent();
	t->loader->stop();
}
/*
 * "$Id: $"
 */

