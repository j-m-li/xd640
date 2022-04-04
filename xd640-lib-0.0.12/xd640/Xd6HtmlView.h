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
#ifndef Xd6HtmlView_h
#define Xd6HtmlView_h

#include "Xd6HtmlFrame.h"
#include "Xd6HtmlPrint.h"
#include "Xd6FindDialog.h"
#include <FL/Fl_Widget.h>
#include "Xd6XmlParser.h"
#include <stdio.h>

class Xd6HtmlView : public Fl_Widget {
public:
	Xd6HtmlFrame *frame;
	int flags;
	Xd6XmlParser *parser;
	Xd6HtmlPrint *printer;
	int paper_size;
	int margin_right;
	int margin_left;
	int margin_top;
	int margin_bottom;
	int landscape;
	FILE *word_file;
	FILE *pword_file;
	char **spell_ignore;
	char **spell_replace;
	char **spell_replace_word;
	int spell_nb_i;
	int spell_nb_r;
	int cancel_spelling;
	int inch;

	Xd6FindDialog *find_dialog;
	Xd6HtmlView(int X, int Y, int W, int H);
	~Xd6HtmlView(void);
	
	static void clean_tmp_dir(void);	

	void new_frame(int W, int H);
	void page_setup(void);
	void set_sizes(void);
	void set_frame_sizes(void);
	void measure(void);
	void print(void);
	void draw(void);
	void load(const char *n);
	void blank(void);
	int handle(int e);
	void resize(int X, int Y, int W, int H);
	void spell(void);
	void spell_cb(Xd6HtmlSegment *s, Xd6HtmlLine *line, Xd6HtmlBlock *b);
	void spell_choice(Xd6HtmlBlock *b, Xd6HtmlLine *l, 
		Xd6HtmlSegment *s, char *text, int len);
	void add_spell_ignore(const char *w);
	void add_spell_replace(const char *w, const char *r);
	static void replace_by(const char *w, Xd6HtmlBlock *b, Xd6HtmlLine *l, 
		Xd6HtmlSegment *s, char *text, int len);
	static void replace_cb(const char *w, Xd6HtmlFrame *f);

	void find(void);
	void find_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s);
	void add_to_dict(const char *w);
};

#endif

/*
 * "$Id: $"
 */
