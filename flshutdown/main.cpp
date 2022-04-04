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

#include <FL/x.H>
#include <FL/fl_ask.H>
#include <stdlib.h>
#include <libintl.h>
#include "xd640/Xd6DefaultFonts.h"
#include "xd640/Xd6ConfigFile.h"
#define _(String) gettext((String))

int main(int argc, char **argv, char **environ)
{
	int i;
	int index = 0;
	Xd6ConfigFile *cfg;
	Xd6ConfigFileSection *cfg_sec;

	Fl::args(argc, argv, index);

	// initialize an FLE configuration object
        cfg = new Xd6ConfigFile("flshutdown", "Utilities");
        cfg_sec = cfg->get_config_section(cfg->app_name);

	// GNU gettext initialization
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	// load XD640 default fonts	
	Xd6DefaultFonts::load(cfg);

	i = 0;
	while (environ[i]) {
		if (!strncmp("DISPLAY=", environ[i], 8)) {
			if (environ[i][8] != ':') {
				printf("Bad boy :-(\n");
				exit(0);
			}
		}
		i++;
	}
	fl_open_display();
	if (fl_ask(_("Do you really want to SHUTDOWN the computer ?"))) {
		system("/sbin/shutdown -ah now");
	}
    	return 0;
}

