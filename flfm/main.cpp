/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000  O'ksi'D
 *
 *                      All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *      Neither the name of O'ksi'D nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER 
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

        itm = cfg_sec->get_item("view_tree", NULL);
        val = NULL; if (itm) val = itm->get_value();
        if (val) StatesValues.view_tree = (int) strtoul(val, NULL, 0);

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
	setlocale(LC_MESSAGES, "");
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

