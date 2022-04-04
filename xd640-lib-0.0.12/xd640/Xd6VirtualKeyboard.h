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
