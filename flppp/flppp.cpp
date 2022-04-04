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
