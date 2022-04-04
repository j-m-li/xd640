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
#ifndef Xd6HtmlToolbar_h
#define Xd6HtmlToolbar_h

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Toggle_Button.H>

class Xd6HtmlEditor;

class Xd6HtmlToolbar : public Fl_Group {
public:
        Fl_Button *save;
        Fl_Button *print;
        Fl_Choice *font_family;
        Fl_Menu_Item *font_family_items;
        Fl_Choice *font_size;
        Fl_Menu_Item *font_size_items;
        Fl_Toggle_Button *text_align_left;
        Fl_Toggle_Button *text_align_right;
        Fl_Toggle_Button *text_align_center;
        Fl_Toggle_Button *font_bold;
        Fl_Toggle_Button *font_italic;
        Fl_Toggle_Button *font_underline;
	Fl_Toggle_Button *right_to_left;
	Fl_Button *insert_picture;
	Fl_Button *page_break;
	Fl_Button *spell;
	
        Fl_Pixmap *p_center;
        Fl_Pixmap *p_left;
        Fl_Pixmap *p_right;
        Fl_Pixmap *p_bold;
        Fl_Pixmap *p_underline;
        Fl_Pixmap *p_italic;
        Fl_Pixmap *p_save;
	Fl_Pixmap *p_rtl;
	Fl_Pixmap *p_pict;
	Fl_Pixmap *p_print;
	Fl_Pixmap *p_break;
	Fl_Pixmap *p_spell;

	Fl_Input *pict_name;
	Fl_Input *w_size;
	Fl_Input *h_size;

	Xd6HtmlEditor *editor;
	
	int old_style;
	int old_bstyle;

	void pict_dialog(void);

	static void cb_family(Fl_Widget*, void*);
	static void cb_size(Fl_Widget*, void*);
	static void cb_center(Fl_Widget*, void*);
	static void cb_left(Fl_Widget*, void*);
	static void cb_right(Fl_Widget*, void*);
	static void cb_bold(Fl_Widget*, void*);
	static void cb_underline(Fl_Widget*, void*);
	static void cb_italic(Fl_Widget*, void*);
	static void cb_save(Fl_Widget*, void*);
	static void cb_rtl(Fl_Widget*, void*);
	static void cb_pict(Fl_Widget*, void*);
	static void cb_print(Fl_Widget*, void*);
	static void cb_break(Fl_Widget*, void*);
	static void cb_spell(Fl_Widget*, void*);
	Xd6HtmlToolbar(Xd6HtmlEditor *e, int X, int Y, int W, int H);
	~Xd6HtmlToolbar(void);
	void create_menus(void);
	void set_style(int s, int bs);
	void update_style(void);
	void cut(void);
	void copy(void);
	void paste(void);
	void insert_file(void);
};

#endif

/*
 * "$Id: $"
 */
