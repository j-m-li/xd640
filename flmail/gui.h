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

#include <FL/Fl_Double_Window.H>
#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Button.H>
#include "xd640/Xd6ConfigFile.h"
#include "xd640/Xd6DefaultFonts.h"
#include "Xd6HtmlBrowser.h"
#include "Download.h"

class GUI : public Fl_Window {
public:
	Fl_Menu_Item *mnu;
	Fl_Menu_Bar *mnu_bar;
	Xd6HtmlBrowser *browser;
	Fl_Output *stat_bar;
	Xd6ConfigFile *cfg;
	static GUI *self;

	GUI(int W, int H);
	~GUI();
	void create_layout(void);
	void create_menu(void);
	int handle(int e);
	void load(const char *file);
};


#endif

/*
 *  End of : "$Id: $"
 */

