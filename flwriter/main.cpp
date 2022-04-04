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


#include "xd640/Xd6Std.h"
#include "gui.h"
#include "flwriter.xpm"
#include "xd640/Xd6IconWindow.h"

Xd6ConfigFile *cfg;

int main(int argc, char **argv, char **environ)
{
	int index = 0;
	GUI *gui;
	Xd6ConfigFileSection *cfg_sec;
	Xd6ConfigFileItem *itm;
	const char *val;
	Fl_Font font = (Fl_Font) 0;
	int size = 16;

	Fl::args(argc, argv, index);

	fl_open_display();

	// initialize an FLE configuration object
	cfg = new Xd6ConfigFile("flwriter", "Utilities");
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
	gui->resize(gui->x(), gui->y(), 620, 440);
	gui->show(argc, argv);

	if (index != argc) {	
		gui->load(argv[index]);
	}

	set_wm_icon(flwriter_xpm, gui);
	// main loop
    	return Fl::run();
}

