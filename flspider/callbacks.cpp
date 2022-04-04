/******************************************************************************
 *   "$Id: $"
 *
 *   This file is part of the FLE project. 
 *
 *                 Copyright (c) 2001-2002  O'ksi'D
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

#include "xd640/Xd6HtmlPrint.h"
#include "xd640/Xd6VirtualKeyboard.h"
#include "callbacks.h"
#include <libintl.h>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <unistd.h>

#define _(String) gettext((String))



void cb_open(Fl_Widget*, void *d)
{
	GUI *g = (GUI*) d;
	if (g) {
		const char *f;
		f = fl_file_chooser("Open ...", "*.{html,htm,HTM}", NULL);
		if (f) {
			g->load(f);
		}
	}
}


void cb_save_as(Fl_Widget*, void *d)
{
	GUI *g = (GUI*) d;
	if (g) {
		const char *f;
		f = fl_file_chooser("Save as ...", "*.{html,htm,HTM}", NULL);
		if (f) {
			g->browser->view->frame->to_html(f);
		}
	}
}

void cb_print(Fl_Widget*, void *d)
{
	if (!GUI::self) return;
	GUI *g = (GUI*) d;
	if (g) {
		g->browser->view->print();
	}
}

void cb_pref(Fl_Widget*, void *d)
{
	GUI *g = (GUI*) d;
	if (g) {
		g->browser->view->page_setup();
	}
}

void cb_exit(Fl_Widget*, void*)
{
	exit(0);
}


void cb_copy(Fl_Widget*, void *d)
{
}


void cb_find(Fl_Widget*, void*)
{
}


void cb_about(Fl_Widget*, void*)
{
        fl_message(
              _("  flspider - Copyright (c) 2001-2002 O'ksi'D\n"
                "This application is a GPL free software.\n"
                "        http://oksid.ch"));

}

void cb_help(Fl_Widget*, void*)
{
	fl_message(_("Not yet Implemented."));
}


/*
 *  End of : "$Id: $"
 */

