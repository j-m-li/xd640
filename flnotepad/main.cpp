/******************************************************************************
 *   "$Id:  $"
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
#include <stdlib.h>
#include <libintl.h>
#include "flnotepad.xpm"
#include "xd640/Xd6IconWindow.h"

int main(int argc, char **argv, char **environ)
{
	int index = 0;
	GUI *gui;
	Xd6ConfigFile *cfg;
	Xd6ConfigFileSection *cfg_sec;
	Xd6ConfigFileItem *itm;
	const char *val;
	Fl_Font font = (Fl_Font) 0;
	int size = 16;

	Fl::args(argc, argv, index);

	// initialize an FLE configuration object
	cfg = new Xd6ConfigFile("flwriter", "Utilities");
	cfg_sec = cfg->get_config_section(cfg->app_name);

	// GNU gettext initialization
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	// load FLE default fonts	
	Xd6DefaultFonts::load(cfg);
        if (cfg_sec) {
                itm = cfg_sec->get_item("FontId", NULL);
                val = NULL; if (itm) val = itm->get_value();
                if (val) font = (Fl_Font) strtoul(val, NULL, 0);
                itm = cfg_sec->get_item("FontSize", NULL);
                val = NULL; if (itm) val = itm->get_value();
                if (val) size = (int) strtoul(val, NULL, 0);
        }
	if ((int)font >= (int) Xd6DefaultFonts::free_font) font = (Fl_Font) 0;

	// create the user interface
    	gui = new GUI(240, 190);
	gui->font = font;
	gui->size = size;
	gui->cfg = cfg;
	gui->cfg_sec = cfg_sec;
    	gui->create_layout();
	
	// show the interface
	gui->show(argc, argv);
	gui->resize(gui->x(), gui->y(), 620, 440);

	if (index != argc) {	
		gui->load(argv[index]);
	}
	
	set_wm_icon(flnotepad_xpm, gui);

	// main loop
    	return Fl::run();
}

