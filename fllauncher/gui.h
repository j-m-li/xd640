/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000  O'ksi'D
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
