/*******************************************************************************
 *  "$Id: $"
 *
 *  	Xd6Tabulator for Xd640.
 *
 *   		Copyright (c) 2002 O'ksi'D
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

#include "Xd6Tabulator.h"
#include "Xd6Std.h"
#include "Xd6XmlStyle.h"
#include <FL/x.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/fl_utf8.h>
#include <FL/Fl_Box.H>
#include <string.h>

#define _(String) gettext((String))

void Xd6Tabulator::setup()
{
        Xd6ConfigFile *cfg = NULL;
        Xd6ConfigFileSection *sec = NULL;
        Xd6ConfigFileGroup *grp = NULL;
        Xd6ConfigFileItem *itm = NULL;
        const char *val = NULL;
	int inch = 0;
	double fact;
	int i;
	Fl_Button *ok;
	Fl_Button *buttons[2];
	Fl_Window *w;
	Fl_Input *in;
	Fl_Hold_Browser *b;

	cfg = new Xd6ConfigFile("xd640", "Utilities");
	sec = cfg->get_xd640_section();
	grp = sec->get_group("page", cfg->locale);
	itm = grp->get_create_item("inch", cfg->locale);
	val = itm->get_value(); if (val) inch = (int) atol(val);
	delete(cfg);

        if (inch) {
                fact = 1.0 / 72;
        } else {
                fact = 2.54 / 72;
        }

	w = new Fl_Window(210, 125);
	w->label(_("Tab Setup"));

	in = new Fl_Input(5, 5, 140, 25);
	if (inch) {
		in->label(_(" in"));
	} else {
		in->label(_(" cm"));
	}
	in->align(FL_ALIGN_RIGHT);
	
	b = new Fl_Hold_Browser(5, 35, 90, 85);

	buttons[0] = new Fl_Button(110, 35, 90, 25);
	buttons[0]->label(_("Add"));

	buttons[1] = new Fl_Button(110, 65, 90, 25);
	buttons[1]->label(_("Remove"));

	ok = new Fl_Button(110, 95, 90, 25);
	ok->label(_("OK"));

	w->end();
	w->set_modal();	
	w->show();
	Xd6XmlStyle::clean_tab();
	
	i = 0;
	while (Xd6HtmlFrame::tab_stop && Xd6HtmlFrame::tab_stop[i]) {
		char buf[50];
		Xd6XmlStyle::add_tab(Xd6HtmlFrame::tab_stop[i]);
		sprintf(buf, "%f", Xd6HtmlFrame::tab_stop[i] * fact);
		b->add(buf);
		i++;
	}
	for (;;) {
    		Fl_Widget *o = Fl::readqueue();
    		if (!o) Fl::wait();
    		else if (o == ok) {break;}
    		else if (o == w) {break;}
		else if (o == buttons[1]) {
			int v = b->value() - 1;
			if (v >= 0) {
				b->remove(b->value());
				i = v;
				while (i < Xd6XmlStyle::nb_tab) {
					Xd6XmlStyle::tab_stop[i] = 
						 Xd6XmlStyle::tab_stop[i + 1];	
					i++;
				}
				Xd6XmlStyle::nb_tab--;
			}
		} else if (o == buttons[0]) {
			char buf[50];
			Xd6XmlStyle::add_tab((int)(atof(in->value()) / fact));
			b->clear();
			i = 0;
			while (i < Xd6XmlStyle::nb_tab) {
				sprintf(buf, "%f", Xd6XmlStyle::tab_stop[i] *
					fact);
				b->add(buf);
				i++;
			}
			
		}
  	}
	free(Xd6HtmlFrame::tab_stop);
	Xd6HtmlFrame::tab_stop = Xd6XmlStyle::get_tabs();
	w->hide();
	delete(w);
	
}


