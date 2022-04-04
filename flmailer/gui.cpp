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
        mnu = NULL;
        mnu_bar = NULL;
        browser = NULL;
        stat_bar = NULL;
	self = this;
}

GUI::~GUI()
{
	self = NULL;
	delete(mnu_bar);
	delete(mnu);
	delete(browser);
	delete(stat_bar);
}

void GUI::create_layout()
{
	
	create_menu();

	begin();

	mnu_bar = new Fl_Menu_Bar(0, 0, 240, 25);;
	mnu_bar->menu(mnu);
	mnu_bar->selection_color(137);


	browser = new Xd6HtmlBrowser(0, 25, 241, 146);
	resizable(browser);

	stat_bar = new Fl_Output(0, 171, 241, 20);
	stat_bar->color(FL_GRAY);
	Xd6HtmlFrame::status_bar = stat_bar;
	
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
		mnu->add(_("Open ..."), 0, (Fl_Callback*)cb_open, this, 0);
		mnu->add(_("Save as ..."), 0, (Fl_Callback*)cb_save_as, 
			0, FL_MENU_DIVIDER);
		mnu->add(_("Print ..."), 0, (Fl_Callback*)cb_print, this, 0);
		mnu->add(_("Preferences"), FL_ALT+'p', (Fl_Callback*)cb_pref, 
			this, FL_MENU_DIVIDER);
		mnu->add(_("Exit"), FL_ALT+'x', (Fl_Callback*)cb_exit, 
			this, 0);
	}
	mnu[i].flags = FL_SUBMENU;

	//FL_MENU_TOGGLE
	i = mnu->add(_("Edit"), FL_ALT+'e',  0, 0, 0);
	{
		mnu->add(_("Copy"), 0, (Fl_Callback*)cb_copy, this, 0);
		mnu->add(_("Find..."), 0, (Fl_Callback*)cb_find, this,
			FL_MENU_DIVIDER);
	}
	mnu[i].flags = FL_SUBMENU;

	i = mnu->add(_("Help"), FL_ALT+'h',  0, 0, 0);
	{
		mnu->add(_("About"), FL_ALT+'a', (Fl_Callback*)cb_about, 
			this, 0);
		mnu->add(_("Online Help"), FL_ALT+'o', 
			(Fl_Callback*)cb_help, this, 0);
	}
	mnu[i].flags = FL_SUBMENU;

}

int GUI::handle(int e)
{
	return Fl_Window::handle(e);	
}

void GUI::load(const char *f)
{
	browser->load(f);
	browser->redraw();
}

/*
 *  End of : "$Id: $"
 */

