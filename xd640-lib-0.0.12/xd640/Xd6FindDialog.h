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
#ifndef Xd6FindDialog_h
#define Xd6FindDialog_h

#include "Xd6HtmlFrame.h"
#include <FL/Fl_Window.h>
#include <FL/Fl_Button.h>
#include <FL/Fl_Input.h>
#include <stdio.h>

struct Xd6FindDialogFound {
	Xd6HtmlLine *l;
	Xd6HtmlSegment *s;
	int c;
};

typedef void (*ReplaceCb )(const char *, Xd6HtmlFrame *);

class Xd6FindDialog : public Fl_Window {
public:
	int status;
	int pointer;
	int is_rescan;
	Xd6FindDialogFound *found;
	int nb_found;
	Xd6HtmlBlock *current_block;
	Xd6HtmlFrame *frame;
	Fl_Button *b_replace_next;
	Fl_Button *b_next;
	Fl_Button *b_cancel;
	Fl_Input *i_find;
	Fl_Input *i_replace;
	ReplaceCb cb_replace;

	Xd6FindDialog(Xd6HtmlFrame *f);
	~Xd6FindDialog(void);

	int finder(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s);
	void scan(Xd6HtmlLine *l, Xd6HtmlSegment *s, int off = 0);
	void rescan(void);
	void add_found(Xd6HtmlLine *l, Xd6HtmlSegment *s, char *c);
	static void cb_rn(Fl_Widget *w, void *d);	
	static void cb_n(Fl_Widget *w, void *d);	
	static void cb_c(Fl_Widget *w, void *d);	
};

#endif

/*
 * "$Id: $"
 */
