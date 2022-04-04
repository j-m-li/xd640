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

#include "gui.h"
#include "callbacks.h"
#include <libintl.h>

#define _(String) gettext((String))

GUI *GUI::self = NULL;

GUI::GUI(int W, int H) : Fl_Window(W, H)
{
	file = NULL;
        mnu = NULL;
        mnu_bar = NULL;
        editor = NULL;
        stat_bar = NULL;
        cfg = NULL;
        cfg_sec = NULL;
        font = (Fl_Font) 0;
        size = 16;
	self = this;
}

GUI::~GUI()
{
	free(file);
	self = NULL;
	delete(mnu_bar);
	delete(mnu);
	delete(editor);
	delete(stat_bar);
}

void GUI::create_layout()
{
	
	create_menu();

	begin();

	mnu_bar = new Fl_Menu_Bar(0, 0, 240, 25);;
	mnu_bar->menu(mnu);
	mnu_bar->selection_color(137);


	editor = new TextEditor(0, 25, 241, 146);
	resizable(editor);

	stat_bar = new Fl_Output(0, 171, 241, 20);
	stat_bar->color(FL_GRAY);

	end();
}

void GUI::create_menu()
{
	int i;

	mnu = new Fl_Menu_Item[50];
	for (i = 0; i < 50; i++) {
		mnu[i].text = NULL;
	}
	
	i = mnu->add(_("File"), FL_ALT+'f',  0, 0, 0);
	{
		mnu->add(_("New ..."), 0, (Fl_Callback*)cb_new, 0, 0);
		mnu->add(_("Open ..."), 0, (Fl_Callback*)cb_open, 0, 0);
		mnu->add(_("Save"), 0, (Fl_Callback*)cb_save, 0, 0);
		mnu->add(_("Save as ..."), 0, (Fl_Callback*)cb_save_as, 
			0, FL_MENU_DIVIDER);
		mnu->add(_("Exit"), FL_ALT+'x', (Fl_Callback*)cb_exit, 
			0, 0);
	}
	mnu[i].flags = FL_SUBMENU;

	//FL_MENU_TOGGLE
	i = mnu->add(_("Edit"), FL_ALT+'e',  0, 0, 0);
	{
/*
		mnu->add(_("Copy"), 0, (Fl_Callback*)cb_copy, 0, 0);
		mnu->add(_("Cut"), 0, (Fl_Callback*)cb_cut, 0, 0);
		mnu->add(_("Paste"), 0, (Fl_Callback*)cb_paste, 0, 
			FL_MENU_DIVIDER);
*/
		mnu->add(_("Find..."), 0, (Fl_Callback*)cb_find, 0,
			FL_MENU_DIVIDER);
	}
	mnu[i].flags = FL_SUBMENU;


	i = mnu->add(_("Help"), FL_ALT+'h',  0, 0, 0);
	{
		mnu->add(_("About"), FL_ALT+'a', (Fl_Callback*)cb_about, 0, 0);
/*		mnu->add(_("Online Help"), FL_ALT+'o', 
			(Fl_Callback*)cb_help, 0, 0);
*/
	}
	mnu[i].flags = FL_SUBMENU;

}

int GUI::handle(int e)
{
	return Fl_Window::handle(e);	
}

void GUI::load(const char *f)
{
	free(file);
	file = strdup(f);
	editor->load(f);
	editor->redraw();
}

/*
 *  End of : "$Id: $"
 */

