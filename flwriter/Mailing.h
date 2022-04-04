/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2002  O'ksi'D
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
	int set_word(Xd6XmlTreeElement* e, Xd6XmlStl *style);
};

#endif

/*
 * "$Id: $"
 */
