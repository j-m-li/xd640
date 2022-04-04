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
