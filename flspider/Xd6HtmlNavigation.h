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
	char *history[10];
	Xd6HtmlBrowser *browser;
	Fl_Button *forward;
	Fl_Button *back;
	Fl_Button *stop;
	Fl_Input *loc;

        Fl_Pixmap *p_back;
        Fl_Pixmap *p_forward;
        Fl_Pixmap *p_stop;

	Download *loader;

	Xd6HtmlNavigation(Xd6HtmlBrowser *e, int X, int Y, int W, int H);
	~Xd6HtmlNavigation(void);
	
	void new_url(const char *u);
	static void cb_back(Fl_Widget*, void*);
	static void cb_forward(Fl_Widget*, void*);
	static void cb_loc(Fl_Widget*, void*);
	static void cb_stop(Fl_Widget*, void*);
};

#endif

/*
 * "$Id: $"
 */
