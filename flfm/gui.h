/******************************************************************************
 *   "$Id: $"
 *
 *   This file is part of the FLE project. 
 *
 *                 Copyright (c) 2000  O'ksi'D
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
#ifndef gui_h
#define gui_h

#include <FL/Fl_Window.H>
#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Button.H>
#include "Location.h"
#include "IconCanvas.h"


extern Atom XdndActionAsk;
extern Atom XdndActionCopy;
extern Atom XdndActionMove;
extern Atom XdndActionLink;
extern Atom fl_dnd_source_action;
extern Atom fl_dnd_action;
extern Window fl_dnd_source_window;
extern Atom fl_XdndActionCopy;
extern Atom fl_XdndStatus;

enum {
	DND_MOVE = 1,
	DND_LINK,
	DND_COPY,
	DND_ASK
};

class GUI : public Fl_Window {
public:
	Fl_Menu_Item *mnu;
	Fl_Menu_Bar *mnu_bar;
	Location *loc_inp;
	IconCanvas *icon_can;
	Fl_Output *stat_bar;
	Fl_Font font;
	int size;
	Fl_Menu_Button *dnd_menu;
	Fl_Menu_Item *dnd_items;
	static int dnd_action;
	Fl_Widget *dnd_target;

	GUI(int X, int Y, int W, int H);
	~GUI();
	void create_layout(void);
	void create_menu(void);
	int handle(int e);
	int dnd_move(void);
	int dnd_release(int a);
	static void dnd_cb(Fl_Widget *w, void *d);
};


#endif
