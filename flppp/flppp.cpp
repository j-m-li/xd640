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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libintl.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Group.H>
#include <FL/fl_ask.H>
#include "xd640/Xd6DefaultFonts.h"
#include "xd640/Xd6XmlUtils.h"
#include "PppGui.h"

#define _(String) gettext((String))


int main(int argc, char ** argv) 
{
	Xd6ConfigFileItem *itm;
	Xd6ConfigFileSection *cfg_sec;
	Xd6ConfigFile *cfg;
	XWMHints *hints;
	int size = 16;
	Fl_Font font = (Fl_Font) 0;
	const char *val;
	PppGui *gui;

	cfg = new Xd6ConfigFile("flppp", "Utilities");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	
	Xd6DefaultFonts::load(cfg);
	cfg_sec = cfg->get_config_section(cfg->app_name);
	if (cfg_sec) {
		itm = cfg_sec->get_item("FontId", NULL);
		val = NULL; if (itm) val = itm->get_value();
		if (val) font = (Fl_Font) strtoul(val, NULL, 0);
		itm = cfg_sec->get_item("FontSize", NULL);
		val = NULL; if (itm) val = itm->get_value();
		if (val) size = (int) strtoul(val, NULL, 0);
	}
	
	gui = new PppGui(240, 240);
	Fl::check();
	Fl::flush();
	gui->icon_window = new PppIcon(48, 48);
	//gui->icon_window->show();
	gui->show();
	Fl::check();
	Fl::flush();
	Fl::check();
	Fl::flush();
	
	hints = XGetWMHints(fl_display, fl_xid(gui));
//	hints->icon_window = fl_xid(gui->icon_window);
	hints->flags = IconWindowHint;
	Fl::check();
	Fl::flush();
	//XSetWMHints(fl_display, fl_xid(gui), hints);
	//XUnmapWindow(fl_display, fl_xid(gui->icon_window));
   	return Fl::run();
}

//
// End of "$Id:  $".
//
