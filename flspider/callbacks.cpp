/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2001-2002  O'ksi'D
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
                "This application is an Open Source software.\n"
                "        http://oksid.ch"));

}

void cb_help(Fl_Widget*, void*)
{
	fl_message(_("Not yet Implemented."));
}


/*
 *  End of : "$Id: $"
 */

