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
	void find_word(Xd6HtmlSegment *, char **, int *);
	void spell_choice(Xd6HtmlFrame *f, Xd6HtmlBlock *b, Xd6HtmlSegment *s);
	void add_spell_ignore(const char *w);
	void add_spell_replace(const char *w, const char *r);
	static void replace_by(const char *w, Xd6HtmlFrame *f, Xd6HtmlBlock *b, 
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
