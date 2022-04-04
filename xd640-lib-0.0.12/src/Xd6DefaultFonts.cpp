/*******************************************************************************
 *  "$Id: $"
 *
 *  	Xd6DefaultFonts for Xd640.
 *
 *   		Copyright (c) 2000-2001 O'ksi'D
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

#define HAVE_XUTF8 1

#include "Xd6DefaultFonts.h"
#include "Xd6Std.h"
#include <FL/x.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Button.H>
#include <FL/fl_utf8.h>
#include <FL/Fl_Box.H>
#include <string.h>


#define _(String) gettext((String))

Fl_Font Xd6DefaultFonts::free_font = FL_FREE_FONT;
Fl_Font Xd6DefaultFonts::serif = FL_TIMES;
Fl_Font Xd6DefaultFonts::sans_serif = FL_HELVETICA;
Fl_Font Xd6DefaultFonts::monospace = FL_COURIER;

static void set_font(Xd6ConfigFileGroup *fnt, Xd6ConfigFile *cfg, int font)
{
	const char *val;
	Xd6ConfigFileItem *itm;

	
	itm = fnt->get_item("normal", cfg->locale);
	val = NULL; if (itm) val = itm->get_value();
	if (val) Fl::set_font((Fl_Font)font, val);
	font++;
	itm = fnt->get_item("bold", cfg->locale);
	val = NULL; if (itm) val = itm->get_value();
	if (val) Fl::set_font((Fl_Font)font, val);
	font++;
	itm = fnt->get_item("italic", cfg->locale);
	val = NULL; if (itm) val = itm->get_value();
	if (val) Fl::set_font((Fl_Font)font, val);
	font++;
	itm = fnt->get_item("bold italic", cfg->locale);
	val = NULL; if (itm) val = itm->get_value();
	if (val) Fl::set_font((Fl_Font)font, val);
}

extern int fl_font_;
	
void Xd6DefaultFonts::load(Xd6ConfigFile *cfg)
{
	Xd6ConfigFileSection *sec;
	Xd6ConfigFileGroup *grp;
	Xd6ConfigFileGroup *fnt;
	int font;

	fl_open_display();
	fl_font(0, 14);

	if (!cfg) return;

	sec = cfg->get_xd640_section();
	if (!sec) return;

	grp = sec->get_group("fonts", cfg->locale);
	if (!grp) return;

	font = (int) 0;

	fnt = grp->get_group("sans-serif", cfg->locale);
	if (fnt) {
		sans_serif = (Fl_Font)font;
		set_font(fnt, cfg, font);
	}
	font += 4;
 
	fnt = grp->get_group("monospace", cfg->locale);
	if (fnt) {
		monospace = (Fl_Font)font;
		set_font(fnt, cfg, font);
	}
	font += 4;
	
	fnt = grp->get_group("serif", cfg->locale);
	if (fnt) {
		serif = (Fl_Font)font;
		set_font(fnt, cfg, font);
	}

	fl_font_ = -1;
	fl_font(0, 14);
}

class FontDisplay : public Fl_Widget {
  	void draw();
public:
  	int font, size;
  	FontDisplay(Fl_Boxtype B, int X, int Y, int W, int H, 
		const char* L = 0) : Fl_Widget(X,Y,W,H,L) 
	{
		box(B); 
		font = 0; 
		size = 12;
	}
};

void FontDisplay::draw() 
{
  	draw_box();
  	fl_font((Fl_Font)font, size);
  	fl_color(FL_BLACK);
  	fl_draw(label(), x()+3, y()+3, w()-6, h()-6, align());
}

static Fl_Window *form = NULL;
static FontDisplay *textobj;
static Fl_Hold_Browser *fontobj;
static Fl_Button *ok, *cancel;
static char label[1024];

static void font_cb(Fl_Widget *, long) 
{
  	int fn = fontobj->value();
  	if (!fn) return;
  	fn += FL_FREE_FONT - 1;
  	textobj->font = fn;
  	textobj->redraw();
}

static Fl_Window *create_selector_form()
{
	int i = 0;
	uchar c;

	if (form) return form;
	form = new Fl_Window(550,370);
	form->label(_("Font Selection"));

	strcpy(label, "Hello, world!\n");
	
	for (c = ' '+1; c < 127; c++) {
		if (!(c&0x1f)) label[i++]='\n'; 
		label[i++]=c;
	}
	label[i++] = '\n';
	for (c = 0xA1; c; c++) {
		if (!(c&0x1f)) label[i++]='\n';
		i += fl_ucs2utf(c, label + i);
	}
	label[i] = 0;
	textobj = new FontDisplay(FL_FRAME_BOX,10,10,530,170,label);
	textobj->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE|
		FL_ALIGN_CLIP);
	fontobj = new Fl_Hold_Browser(10, 190, 390, 170);
	fontobj->box(FL_FRAME_BOX);
	fontobj->callback(font_cb);
	form->resizable(fontobj);
	
	ok = new Fl_Button(410, 250, 130, 25);
	ok->label(_("OK"));

	cancel = new  Fl_Button(410, 280, 130, 25);
	cancel->label(_("Cancel"));

	form->end();
	form->set_modal();

	int k = Fl::set_fonts("-*");
	for (i = FL_FREE_FONT; i < k; i++) {
		int t; const char *name = Fl::get_font_name((Fl_Font)i,&t);
		char buffer[128];
		if (t) {
			char *p = buffer;
			if (t & FL_BOLD) {*p++ = '@'; *p++ = 'b';}
			if (t & FL_ITALIC) {*p++ = '@'; *p++ = 'i';}
			strcpy(p,name);
			name = buffer;
		}
		fontobj->add(name);
	}
	fontobj->value(1);
	font_cb(fontobj,0);
	return form;
}

static const char * font_selector()
{
	int r;

	create_selector_form();
	
	form->show();
	for (;;) {
    		Fl_Widget *o = Fl::readqueue();
    		if (!o) Fl::wait();
    		else if (o == cancel) {r = 0; break;}
    		else if (o == ok) {r = 1; break;}
    		else if (o == form) {r = 0; break;}
  	}
	form->hide();
	Fl::check();
	Fl::redraw();
	if (r == 0) return NULL;
	return Fl::get_font((Fl_Font)textobj->font);
}


static void font_set(int fn) 
{
	Xd6ConfigFile *cfg = NULL;
	Xd6ConfigFileSection *sec = NULL;
	Xd6ConfigFileItem *itm = NULL;
	Xd6ConfigFileGroup *grp = NULL;
	const char *val = NULL;

	Fl_Button *ok, *cancel, *add, *remove;
	Fl_Hold_Browser *fontobj;
	Fl_Window *win;

	cfg = new Xd6ConfigFile("xd640", "Utilities");
	sec = cfg->get_xd640_section();
	grp = sec->get_group("fonts", cfg->locale);
	if (fn < 4) {
		grp = grp->get_group("sans-serif", cfg->locale);
	} else if (fn < 8) {
		fn -= 4;
		grp = grp->get_group("monospace", cfg->locale);
	} else {
		fn -= 8;
		grp = grp->get_group("serif", cfg->locale);
	}
	
	switch (fn) {
	case 0:
		itm = grp->get_create_item("normal", cfg->locale); break;
	case 1:
		itm = grp->get_create_item("bold", cfg->locale); break;
	case 2:
		itm = grp->get_create_item("italic", cfg->locale); break;
	case 3:
		itm = grp->get_create_item("bold italic", cfg->locale); break;
	}

	val = itm->get_value();

	win = new Fl_Window(400, 300);
	win->label(_("Font set selection"));

	fontobj = new Fl_Hold_Browser(5, 5, 390, 200);
	fontobj->box(FL_FRAME_BOX);

	if (val) {
		char *l = strdup(val);
		char *ptr = l;
		char *ll = l;
		while (*ptr) {
			if (*ptr == ',') {
				*ptr = '\0';
				fontobj->add(l);
				l = ptr + 1;	
			}
			ptr++;
		}
		if (*l) fontobj->add(l);	
		free(ll);
	}

	ok  = new Fl_Button(5, 270, 190, 25);
	ok->label(_("OK"));

	cancel  = new Fl_Button(205, 270, 190, 25);
	cancel->label(_("Cancel"));

	add  = new Fl_Button(5, 230, 190, 25);
	add->label(_("Add font"));

	remove  = new Fl_Button(205, 230, 190, 25);
	remove->label(_("Remove font"));

	win->end();
	win->set_modal();	
	win->show();
	for (;;) {
    		Fl_Widget *o = Fl::readqueue();
    		if (!o) Fl::wait();
    		else if (o == ok) {
			char buf[2048];
			int s = fontobj->size() + 1;
			int i = 1;
				
			buf[0] = '\0';
			while (i < s) {
				const char *b = fontobj->text(i);
				if (b) {
					if (i > 1) strcat(buf, ",");
					strcat(buf, b);
				}
				i++;
			}
			grp->add_item(itm->get_name(), buf, cfg->locale);
			cfg->write_xd640_section(sec);
			break;
		}
		else if (o == remove && fontobj->value()) {
			fontobj->remove(fontobj->value());
		}
		else if (o == add) {
			const char *s;
			s = font_selector();
			if (s) fontobj->add(s);
		}
    		else if (o == win) {break;}
    		else if (o == cancel) {break;}
  	}
	win->hide();
	delete(ok);
	delete(cancel);
	delete(add);
	delete(remove);
	delete(fontobj);
	delete(win);
	Fl::flush();
	Xd6DefaultFonts::load(cfg);
	delete(cfg);
}

void Xd6DefaultFonts::setup()
{
	int i;
	Fl_Button *ok;
	Fl_Button *buttons[12];
	Fl_Window *w;
	w = new Fl_Window(210, 400);
	w->label(_("Fonts Setup"));
	
	buttons[0] = new Fl_Button(5, 5, 200, 25);
	buttons[0]->label(_("Sans-Serif"));

	buttons[1] = new Fl_Button(5, 35, 200, 25);
	buttons[1]->label(_("Sans-Serif Bold"));

	buttons[2] = new Fl_Button(5, 65, 200, 25);
	buttons[2]->label(_("Sans-Serif Italic"));

	buttons[3] = new Fl_Button(5, 95, 200, 25);
	buttons[3]->label(_("Sans-Serif Bold Italic"));

	buttons[4] = new Fl_Button(5, 125, 200, 25);
	buttons[4]->label(_("Monospace"));

	buttons[5] = new Fl_Button(5, 155, 200, 25);
	buttons[5]->label(_("Monospace Bold"));

	buttons[6] = new Fl_Button(5, 185, 200, 25);
	buttons[6]->label(_("Monospace Italic"));

	buttons[7] = new Fl_Button(5, 215, 200, 25);
	buttons[7]->label(_("Monospace Bold Italic"));

	buttons[8] = new Fl_Button(5, 245, 200, 25);
	buttons[8]->label(_("Serif"));

	buttons[9] = new Fl_Button(5, 275, 200, 25);
	buttons[9]->label(_("Serif Bold"));

	buttons[10] = new Fl_Button(5, 305, 200, 25);
	buttons[10]->label(_("Serif Italic"));

	buttons[11] = new Fl_Button(5, 335, 200, 25);
	buttons[11]->label(_("Serif Bold Italic"));

	ok  = new Fl_Button(15, 365, 180, 25);
	ok->label(_("OK"));

	w->end();
	w->set_modal();	
	w->show();
	for (;;) {
    		Fl_Widget *o = Fl::readqueue();
    		if (!o) Fl::wait();
    		else if (o == ok) {break;}
    		else if (o == w) {break;}
		for (i = 0; i < 12; i++) {
			if (o == buttons[i]) {
				font_set(i);
			}
		}
  	}
	w->hide();
	delete(w);
}


