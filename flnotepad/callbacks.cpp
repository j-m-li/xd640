/******************************************************************************
 *   "$Id: $"
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

#include "xd640/Xd6HtmlPrint.h"
#include "xd640/Xd6VirtualKeyboard.h"
#include "callbacks.h"
#include <libintl.h>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <unistd.h>

#define _(String) gettext((String))


void cb_new(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	GUI::self->editor->blank();
}

void cb_open(Fl_Widget*, void*)
{
	if (GUI::self) {
		const char *f;
		f = fl_file_chooser("Open ...", "*.{html,htm,HTM}", NULL);
		if (f) {
			GUI::self->load(f);
		}
	}
}

void cb_save(Fl_Widget*, void*)
{
	if (GUI::self) {
		if (GUI::self->file) {
			GUI::self->editor->frame->to_text(
				GUI::self->file);
		} else {
			cb_save_as(NULL, NULL);
		}
	}
}

void cb_save_as(Fl_Widget*, void*)
{
	if (GUI::self) {
		const char *f;
		f = fl_file_chooser("Save as ...", "*", NULL);
		if (f) {
			GUI::self->editor->frame->to_text(f);
			free(GUI::self->file);
			GUI::self->file = strdup(f);
		}
	}
}


void cb_exit(Fl_Widget*, void*)
{
	exit(0);
}


void cb_cut(Fl_Widget*, void*)
{
	if (!GUI::self) return;
}

void cb_copy(Fl_Widget*, void*)
{
	if (!GUI::self) return;
}

void cb_paste(Fl_Widget*, void*)
{
	if (!GUI::self) return;
}

void cb_find(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	GUI::self->editor->find();
}


void cb_about(Fl_Widget*, void*)
{
        fl_message(
              _("  flnotepad - Copyright (c) 2001 O'ksi'D\n"
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

