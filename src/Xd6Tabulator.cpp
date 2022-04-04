/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2002  O'ksi'D
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


