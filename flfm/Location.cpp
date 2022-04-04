/******************************************************************************
 *   "$Id: $"
 *
 *   This file is part of the FLE project. 
 *
 *                 Copyright (c) 2000  O'ksi'D
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

