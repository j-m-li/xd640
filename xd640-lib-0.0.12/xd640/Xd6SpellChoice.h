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
#ifndef Xd6SpellChoice_h
#define Xd6SpellChoice_h

#include <FL/Fl_Window.h>
#include <FL/Fl_Button.h>
#include <FL/Fl_Input.h>
#include <FL/Fl_Select_Browser.h>
#include <stdio.h>

class Xd6SpellChoice : public Fl_Window {
public:
	FILE *fp;
	const char *word;
	static int status;
	Fl_Select_Browser *sb_guesses;
	Fl_Button *b_ignore;
	Fl_Button *b_always_ignore;
	Fl_Button *b_replace;
	Fl_Button *b_always_replace;
	Fl_Button *b_cancel;
	Fl_Button *b_find_guesses;
	Fl_Button *b_personal;
	Fl_Input *i_value;

	Xd6SpellChoice(const char *w, FILE *dict);
	~Xd6SpellChoice(void);
	void find_guesses(void);
	static void cb_i(Fl_Widget *w, void *d);	
	static void cb_ai(Fl_Widget *w, void *d);	
	static void cb_r(Fl_Widget *w, void *d);	
	static void cb_ar(Fl_Widget *w, void *d);	
	static void cb_c(Fl_Widget *w, void *d);	
	static void cb_g(Fl_Widget *w, void *d);	
	static void cb_fg(Fl_Widget *w, void *d);	
	static void cb_p(Fl_Widget *w, void *d);	
};

#endif

/*
 * "$Id: $"
 */
