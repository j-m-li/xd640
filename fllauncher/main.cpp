/******************************************************************************
 *   "$Id: main.cpp,v 1.1 2000/08/05 19:11:21 nickasil Exp $"
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
#include <FL/Fl_Window.H>
#include <FL/x.H>
#include "xd640/Xd6ConfigFile.h"
#include "xd640/Xd6DefaultFonts.h"
#include <stdlib.h>
#include <libintl.h>

int main(int argc, char **argv, char **environ)
{
	Xd6ConfigFile *cfg;
	UserInterface *ui;
	Fl_Window *window;

	// initialize an FLE configuration object
	cfg = new Xd6ConfigFile("fllauncher", "Utilities");

	// GNU gettext initialization
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	// load FLE default fonts	
	Xd6DefaultFonts::load(cfg);

	// create the user interface
    	ui = new UserInterface(cfg);
    	window = ui->make_window();

	// show the interface
	window->show(argc, argv);
	XLowerWindow(fl_display, fl_xid(window));

	// main loop
    	return Fl::run();
}

