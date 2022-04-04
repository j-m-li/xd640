/******************************************************************************* *   $Id: gui.h,v 1.1 2000/08/05 19:11:21 nickasil Exp $
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
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Box.H>
#include "xd640/Xd6ConfigFile.h"

class UserInterface;
class My_Menu_Button;

class My_Box : public Fl_Box
{
	UserInterface *ui;
	int handle(int);
public:
	My_Box(Fl_Boxtype b, int x, int y, int w, int h, const char *l, 
		UserInterface* p) : Fl_Box(b,x,y,w,h,l) 
	{
		ui = p; 
		color(FL_INACTIVE_COLOR);
		labelcolor(FL_GRAY);
	};
};

class UserInterface {
public:
	Xd6ConfigFile *cfg;
	Xd6ConfigFileSection *cfg_section;
	static UserInterface *self;

	Fl_Pixmap *file_pix;
	Fl_Pixmap *dir_pix;
	Fl_Pixmap **pixmaps;
	char **pix_names;
	char **pix_data;
	int nb_pixmaps;

	Fl_Menu_Item *popup_menu;
	Fl_Font font;
	int font_size;

	int verti;
	int height;
	int width;
	const char *terminal; 
	int nb_buttons;

	Fl_Window *w;

	UserInterface(Xd6ConfigFile*);
	~UserInterface();
  	Fl_Window* make_window();
	void make_buttons(void);
	void make_items(My_Menu_Button*, const char*);
	void make_button(Xd6ConfigFileSection*, const char*);
	static void cb_item(Fl_Widget* , void*);
	Fl_Pixmap* load_icon(const char*);
	static void callback(Fl_Widget*, void*);
	int create_menus();
};

class My_Menu_Button : public Fl_Menu_Button {
public:
	int verti;
	Fl_Pixmap *pixmap;
	Fl_Menu_Item *items;
	Xd6ConfigFileSection *sec;

	My_Menu_Button(int x, int y, int w, int h, const char *c) : 
		Fl_Menu_Button(x,y,w,h,c) { 
		verti = 0; 
		pixmap = NULL;
		items = NULL;
		align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
		selection_color(137);
		down_box(FL_FLAT_BOX);
		
	}; 
	~My_Menu_Button();
	int handle(int);
	void image(Fl_Pixmap *pix);
	void draw();
};

#endif
