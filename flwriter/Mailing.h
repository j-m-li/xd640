/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000-2002  O'ksi'D
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
#ifndef Mailing_h
#define Mailing_h

#include <FL/Fl_Window.h>
#include <FL/Fl_Button.h>
#include <FL/Fl_Input.h>
#include <FL/Fl_Box.h>
#include <xd640/Xd6HtmlFrame.h>
#include "Xd6HtmlEditor.h"

class Mailing : public Fl_Window {
public:
	Xd6HtmlEditor *e;
	Xd6HtmlFrame *f;
	Xd6XmlTreeElement *t;
	Fl_Button *quit;
	Fl_Input *in_file;
	Fl_Button *bu_file;
	Fl_Input *in_text;
	Fl_Button *bu_text;
	Fl_Box *bo_text;
	Fl_Button *bu_run;
	Fl_Box *bo_file;
	Fl_Input *in_from;
	Fl_Input *in_to;

	char ***table;
	int nb_col;
	int nb_row;
	int col;
	char status;
	int line;

	Mailing(Xd6HtmlEditor *ed);
	~Mailing(void);
	void load(const char *f);
	void go(void);
	void get_table(Xd6XmlTreeElement *tb);
	void get_cell(Xd6XmlTreeElement *tb);
	int to_text(Xd6XmlTreeElement *tb, char **txt, int l);
	void run(void);
	static void cb_in_file(Fl_Widget *w, void *d);
	void tree_replace(Xd6XmlTreeElement *elem);
	int set_word(Xd6XmlTreeElement* e, int style);
};

#endif

/*
 * "$Id: $"
 */
