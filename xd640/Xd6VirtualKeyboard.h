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


#ifndef Xd6VirtualKeyboard_h
#define Xd6VirtualKeyboard_h

#include <FL/Fl_Window.h>
#include <FL/Fl_Choice.h>
#include <FL/Fl_Button.h>

class Xd6VirtualKeyboard : public Fl_Window {
public:
	int base;
	int offset;
	Fl_Menu_Item *item;
	Fl_Menu_Item *oitem;
	Fl_Widget *target;
	Fl_Choice *c_base;
	Fl_Choice *c_off;
	Fl_Button *b_del;
	Fl_Button *b_back;
	Fl_Button *b_enter;
	Fl_Button *keys[100];
	char lab[100][8];
	Xd6VirtualKeyboard(Fl_Widget *t);
	~Xd6VirtualKeyboard(void);
	void update(void);
	int handle(int e);
	static void cb_base(Fl_Widget *w, void *d);	
	static void cb_off(Fl_Widget *w, void *d);	
	static void cb_button(Fl_Widget *w, void *d);	
};

#endif

/*
 * "$Id: $"
 */
