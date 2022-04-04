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
#include <libintl.h>

#define _(String) gettext((String))

GUI *GUI::self = NULL;

GUI::GUI(int W, int H) : Fl_Window(W, H)
{
        mnu = NULL;
        mnu_bar = NULL;
        browser = NULL;
        stat_bar = NULL;
	self = this;
}

GUI::~GUI()
{
	self = NULL;
	delete(mnu_bar);
	delete(mnu);
	delete(browser);
	delete(stat_bar);
}

void GUI::create_layout()
{
	
	create_menu();

	begin();

	mnu_bar = new Fl_Menu_Bar(0, 0, 240, 25);;
	mnu_bar->menu(mnu);
	mnu_bar->selection_color(137);


	browser = new Xd6HtmlBrowser(0, 25, 241, 146);
	resizable(browser);

	stat_bar = new Fl_Output(0, 171, 241, 20);
	stat_bar->color(FL_GRAY);
	Xd6HtmlFrame::status_bar = stat_bar;
	
	end();
}

void GUI::create_menu()
{
	int i;

	mnu = new Fl_Menu_Item[50];
	for (i = 0; i < 50; i++) {
		mnu[i].text = NULL;
	}
	
	i = mnu->add(_("File"), FL_ALT+'f',  0, 0, 0);
	{
		mnu->add(_("Open ..."), 0, (Fl_Callback*)cb_open, this, 0);
		mnu->add(_("Save as ..."), 0, (Fl_Callback*)cb_save_as, 
			0, FL_MENU_DIVIDER);
		mnu->add(_("Print ..."), 0, (Fl_Callback*)cb_print, this, 0);
		mnu->add(_("Preferences"), FL_ALT+'p', (Fl_Callback*)cb_pref, 
			this, FL_MENU_DIVIDER);
		mnu->add(_("Exit"), FL_ALT+'x', (Fl_Callback*)cb_exit, 
			this, 0);
	}
	mnu[i].flags = FL_SUBMENU;

	//FL_MENU_TOGGLE
	i = mnu->add(_("Edit"), FL_ALT+'e',  0, 0, 0);
	{
		mnu->add(_("Copy"), 0, (Fl_Callback*)cb_copy, this, 0);
		mnu->add(_("Find..."), 0, (Fl_Callback*)cb_find, this,
			FL_MENU_DIVIDER);
	}
	mnu[i].flags = FL_SUBMENU;

	i = mnu->add(_("Help"), FL_ALT+'h',  0, 0, 0);
	{
		mnu->add(_("About"), FL_ALT+'a', (Fl_Callback*)cb_about, 
			this, 0);
		mnu->add(_("Online Help"), FL_ALT+'o', 
			(Fl_Callback*)cb_help, this, 0);
	}
	mnu[i].flags = FL_SUBMENU;

}

int GUI::handle(int e)
{
	return Fl_Window::handle(e);	
}

void GUI::load(const char *f)
{
	browser->load(f);
	browser->redraw();
}

/*
 *  End of : "$Id: $"
 */

