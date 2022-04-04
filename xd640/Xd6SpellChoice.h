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
