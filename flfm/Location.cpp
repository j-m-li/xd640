/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000  O'ksi'D
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


#include "Location.h"
#include "callbacks.h"
#include "xpm/new.xpm"
#include "xpm/back.xpm"
#include "xpm/up.xpm"

Location::Location(int X, int Y, int W, int H) : Fl_Group(X, Y, W, H)
{
	url = NULL;
	nfm = new Fl_Button(X + 1, Y + 1, 26, H - 2);
	up = new Fl_Button(X + 28, Y + 1, 26, H - 2);
	back = new Fl_Button(X + 55, Y + 1, 26, H - 2);
	loc = new Fl_Input(X + 82, Y + 1, W - 82, H - 2);
	p1 = new Fl_Pixmap(new_xpm);
	p2 = new Fl_Pixmap(back_xpm);
	p3 = new Fl_Pixmap(up_xpm);
	p1->label(nfm);
	p2->label(back);
	p3->label(up);
	up->callback(cb_up);
	back->callback(cb_back);
	nfm->callback(cb_new_fm);
	loc->callback(cb_loc_input, this);
	loc->when(FL_WHEN_ENTER_KEY_ALWAYS);
	end();
	resizable(loc);
	box(FL_THIN_UP_BOX);
}

Location::~Location()
{
	delete(p1);
	delete(p2);
	delete(p3);
	free(url);
}

void Location::value(const char *val)
{
	free(url);
	url = strdup(val);
	loc->value(url);
}

int Location::handle(int e)
{
	int r;

	r = Fl_Group::handle(e);
	
	switch(e) {
	case FL_ENTER:
		return 1;
	default:
		break;
	}
	
	return r;
}

void Location::new_url(const char *u)
{
	int i = 0;
	if (!u) return;

	while (i < 10) {
		if (!StatesValues.history[i]) break;
		i++;
	}
	if (i >= 10) {
		i = 0;
		while (i < 9) {
			StatesValues.history[i] = StatesValues.history[++i];
		}
	}
	StatesValues.history[i] = strdup(u);
}

