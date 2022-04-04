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
#include "callbacks.h"
#include "xd640/Xd6ConfigFile.h"
#include "xd640/Xd6DefaultFonts.h"
#include <stdlib.h>
#include <libintl.h>
#include "xd640/Xd6IconWindow.h"

#include "xpm/flfm.xpm"

GUI *gui;
Xd6ConfigFile *cfg;
Xd6ConfigFileSection *cfg_sec;
Atom XdndActionAsk;
Atom XdndActionMove;
Atom XdndActionLink;
Atom XdndActionCopy;

static void restore_values()
{
	Xd6ConfigFileItem *itm;
	const char *val;
        itm = cfg_sec->get_item("sort_size", NULL);
        val = NULL; if (itm) val = itm->get_value();
        if (val) StatesValues.sort_size = (int) strtoul(val, NULL, 0);
       
        itm = cfg_sec->get_item("sort_type", NULL);
        val = NULL; if (itm) val = itm->get_value();
        if (val) StatesValues.sort_type = (int) strtoul(val, NULL, 0);

        itm = cfg_sec->get_item("sort_name", NULL);
        val = NULL; if (itm) val = itm->get_value();
        if (val) StatesValues.sort_name = (int) strtoul(val, NULL, 0);

        itm = cfg_sec->get_item("view_icon", NULL);
        val = NULL; if (itm) val = itm->get_value();
        if (val) StatesValues.view_icon = (int) strtoul(val, NULL, 0);

        itm = cfg_sec->get_item("view_detail", NULL);
        val = NULL; if (itm) val = itm->get_value();
        if (val) StatesValues.view_detail = (int) strtoul(val, NULL, 0);

        itm = cfg_sec->get_item("show_hide", NULL);
        val = NULL; if (itm) val = itm->get_value();
        if (val) StatesValues.show_hide = (int) strtoul(val, NULL, 0);

}

int main(int argc, char **argv, char **environ)
{
	int index = 0;
	Xd6ConfigFileItem *itm;
	const char *val;
	Fl_Font font = (Fl_Font) 0;
	int size = 16;
	int x = 50;
	int y = 50;
	int w = 500;
	int h = 350;

	Fl::args(argc, argv, index);

	// initialize D'n'D Atoms
	fl_open_display();
	XdndActionCopy     = XInternAtom(fl_display, "XdndActionCopy",     0);
	XdndActionAsk     = XInternAtom(fl_display, "XdndActionAsk",     0);
	XdndActionMove     = XInternAtom(fl_display, "XdndActionMove",     0);
	XdndActionLink     = XInternAtom(fl_display, "XdndActionLink",     0);
	
	// initialize an FLE configuration object
	cfg = new Xd6ConfigFile("flfm", "Utilities");
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
                itm = cfg_sec->get_item("W", NULL);
                val = NULL; if (itm) val = itm->get_value();
                if (val) w = (int) strtoul(val, NULL, 0);
                itm = cfg_sec->get_item("H", NULL);
                val = NULL; if (itm) val = itm->get_value();
                if (val) h = (int) strtoul(val, NULL, 0);
        }
	if ((int)font >= (int) Xd6DefaultFonts::free_font) font = (Fl_Font) 0;

	// create the user interface
    	gui = new GUI(x, y, w, h);
	gui->font = font;
	gui->size = size;
        if (cfg_sec) {
		restore_values();
	}
    	gui->create_layout();

	if (index != argc) {
		StatesValues.newurl = strdup(argv[index]);
	} else {
		StatesValues.newurl = strdup(cfg->home_dir);
	}
	
	// show the interface
	gui->show(argc, argv);
	cb_rescan(NULL, NULL);

	// set the icon pixmap
	set_wm_icon(flfm_xpm, gui);

	// main loop
    	return Fl::run();
}

