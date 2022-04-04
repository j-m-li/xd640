/******************************************************************************
 *   "$Id: $"
 *
 *   This file is part of the Xd640 project. 
 *
 *                 Copyright (c) 2000-2002  O'ksi'D
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

#include "xd640/Xd6Std.h"
#include "xd640/Xd6HtmlPrint.h"
#include "xd640/Xd6VirtualKeyboard.h"
#include "xd640/Xd6Tabulator.h"
#include "xd640/Xd6HtmlTagA.h"
#include "xd640/Xd6HtmlToRtf.h"
#include "callbacks.h"
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>

#define _(String) gettext((String))

extern Xd6ConfigFile *cfg;

void cb_atexit()
{
	if (!GUI::self) return;
	if (GUI::self->editor->view->frame->modified) {
		int r;
		r = fl_ask(_("File modified. Save it ?"));
		if (r) cb_save_as(NULL, NULL);
	}
}

void cb_new(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	cb_atexit();
	GUI::self->editor->view->frame->modified = 0;
	GUI::self->editor->view->blank();
}

void cb_open(Fl_Widget*, void*)
{
	if (GUI::self) {
		const char *f;
		f = fl_file_chooser("Open ...", "*.{html,htm,HTM}", NULL);
		if (f) {
			GUI::self->editor->view->frame->modified = 0;
			GUI::self->load(f);
		}
	}
}

void cb_import(Fl_Widget*, void*)
{
	if (GUI::self) {
		const char *f;
		f = fl_file_chooser("Import RTF ...", "*.{rtf,RTF}", NULL);
		if (f) {
			GUI::self->editor->view->frame->modified = 0;
			GUI::self->load_rtf(f);
		}
	}
}

void cb_save(Fl_Widget*, void*)
{
	if (GUI::self) {
		if (GUI::self->editor->file) {
			GUI::self->editor->view->frame->modified = 0;
			GUI::self->editor->view->frame->to_html(
				GUI::self->editor->file);
		} else {
			cb_save_as(NULL, NULL);
		}
	}
}

void cb_save_as(Fl_Widget*, void*)
{
	if (GUI::self) {
		const char *f;
		f = fl_file_chooser("Save as ...", "*.{html,htm,HTM}", 
			GUI::self->editor->file);
		if (f) {
			GUI::self->editor->view->frame->modified = 0;
			GUI::self->editor->view->frame->to_html(f);
			free(GUI::self->editor->file);
			GUI::self->editor->file = strdup(f);
		}
	}
}

void cb_export(Fl_Widget*, void*)
{
	if (GUI::self) {
		const char *f;
		f = fl_file_chooser("Export RTF as ...", "*.{RTF, rtf}", 
			GUI::self->editor->file);
		if (f) {
			Xd6HtmlToRtf r(GUI::self->editor->view->frame);
			r.to_rtf(f);
		}
	}
}

void cb_print(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	if (!GUI::self->editor->view->frame->wysiwyg) {
		fl_alert(_("You must set Zoom to 100% before printing."));
		return;
	};
	GUI::self->editor->view->print();
}

void cb_pref(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	GUI::self->editor->view->page_setup();
}

void cb_pref_font(Fl_Widget*, void*)
{
	Xd6DefaultFonts::setup();
}

void cb_exit(Fl_Widget*, void*)
{
	exit(0);
}


void cb_cut(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	GUI::self->editor->tool->cut();
}

void cb_copy(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	GUI::self->editor->tool->copy();
}

void cb_paste(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	GUI::self->editor->tool->paste();
}

void cb_find(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	GUI::self->editor->view->find();
}


void cb_zoom(Fl_Widget*, void* v)
{
	int z = (int) v;
	if (!GUI::self) return;
	if (GUI::self->editor->page_width == -1) {
		GUI::self->editor->page_width =
			 GUI::self->editor->view->frame->page_width;
	}
	GUI::self->editor->view->frame->zoom(z);
	if (z != 100) {
		GUI::self->editor->view->frame->wysiwyg = 0;
	} else {
		GUI::self->editor->view->frame->page_width =
			GUI::self->editor->page_width;
		GUI::self->editor->view->frame->wysiwyg = 1;
	}

	GUI::self->editor->view->frame->measure();
	GUI::self->editor->view->frame->resize(
		GUI::self->editor->view->frame->max_width,
		GUI::self->editor->view->frame->max_height);
	GUI::self->editor->view->frame->create_pages();
	GUI::self->editor->view->redraw();
}


void cb_tabs(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	Xd6Tabulator::setup();
	GUI::self->editor->view->frame->measure();
	GUI::self->editor->view->frame->create_pages();
	GUI::self->editor->view->redraw();
}

void cb_insert(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	GUI::self->editor->tool->insert_file();
}

void cb_picture(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	GUI::self->editor->tool->insert_picture->do_callback();
}

void cb_page_break(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	GUI::self->editor->tool->page_break->do_callback();
}

void cb_tag(Fl_Widget*, void*)
{
	Xd6HtmlFrame *f;
	Xd6HtmlDisplay *s;
	Xd6HtmlSegment *se;
	Xd6XmlTreeElement *e;
	Xd6XmlAttribute *a;
	const char *v;
	int st = A_LINK|UNDERLINE|COLOR_BLUE;
	if (!GUI::self) return;
	v = fl_input(_("Tag Value :"));
	if (!v) return;
	GUI::self->editor->view->frame->modified = 1;
	f = GUI::self->editor->view->frame->get_cursor_frame();
	if (!f || !f->cur_chr) return;
	f->modified = 1;
	f->change_style(0, st);
	e = new Xd6XmlTreeElement(NULL, 0);
	a = new Xd6XmlAttribute("href", 4);
	a->set_value(v, strlen(v));
	e->add_attribute(a);
	e->style = f->cur_seg->style|DISPLAY|st;
	s = new Xd6HtmlTagA(0, e, NULL);
	f->cur_seg->style &= ~st;
	f->insert_segment(s);
	se = f->cur_seg;
	se = f->cur_block->segs[se->id + 1];
	se->style |= st;
	f->cur_block->measure();
	f->cur_block->create_lines();
	f->create_pages();
	GUI::self->editor->view->redraw();
	delete(e);
}

void cb_mailing(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	GUI::self->editor->mailing();
	GUI::self->editor->view->redraw();
}

void cb_spell(Fl_Widget*, void*)
{
	if (!GUI::self) return;
	GUI::self->editor->view->spell();
}

void cb_keyboard(Fl_Widget*, void*)
{
	static Xd6VirtualKeyboard *k = NULL;
	if (!GUI::self) return;
	if (k) delete(k);
	k = new Xd6VirtualKeyboard(GUI::self->editor->view);
	k->show();
}

void cb_about(Fl_Widget*, void*)
{
        fl_message(
              _("  flwriter - Copyright (c) 2000-2002 O'ksi'D\n"
                "This application is a GPL free software.\n"
                "        http://oksid.ch"));

}

void cb_help(Fl_Widget*, void*)
{
	char buf[2048];
	char b[2048];
	
	snprintf(buf, 2048, "%s/HTML/%s/%s/index.html", 
		cfg->global_paths->doc, cfg->lang, cfg->app_name);
	if (!access(buf, 0)) {
		snprintf(b, 2048, "flspider '%s' &", buf);
	} else {
		snprintf(b, 2048, "flspider '%s/HTML/en/%s/index.html' &", 
			cfg->global_paths->doc, cfg->app_name);
	}
	system(b);
}


/*
 *  End of : "$Id: $"
 */

