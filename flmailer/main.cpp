/******************************************************************************
 *   "$Id:  $"
 *
 *   This file is part of the XD640 project.
 *
 *                 Copyright (c) 2002  O'ksi'D
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
#include <stdlib.h>
#include <libintl.h>

Xd6ConfigFile *cfg;
int news_mode = 0;

int main(int argc, char **argv, char **environ)
{
	int index = 0;
	GUI *gui;

	Fl::args(argc, argv, index);

	if (index < argc) {
		if (!strcmp(argv[index], "--news")) {
			 news_mode = 1;
			index++;
		}
	}

	// initialize a XD640 configuration object
	if (news_mode) {
		cfg = new Xd6ConfigFile("flnews", "Utilities");
	} else {
		cfg = new Xd6ConfigFile("flmail", "Utilities");
	}
	// GNU gettext initialization
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	// load XD640 default fonts	
	Xd6DefaultFonts::load(cfg);

	// create the user interface
    	gui = new GUI(240, 190);
    	gui->create_layout();
	
	// show the interface
	gui->show(argc, argv);
	gui->resize(gui->x(), gui->y(), 620, 440);

	if (index != argc) {	
		gui->load(argv[index]);
	}

	// main loop
    	return Fl::run();
}

