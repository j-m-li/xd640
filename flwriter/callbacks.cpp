/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2002  O'ksi'D
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
			GUI::self->load(GUI::self->editor->file);
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
	Xd6XmlStl st;

	st.clear_flags();
	st.a_link = 1;
	st.underline = 1;
	st.fg_color = COLOR_BLUE;
	st.flags[1] = (1 << 11) | (1 << 6) | (1 << 3);

	if (!GUI::self) return;
	v = fl_input(_("Tag Value :"));
	if (!v) return;

	GUI::self->editor->view->frame->modified = 1;
	f = GUI::self->editor->view->frame->get_cursor_frame();
	if (!f || !f->cur_chr) return;

	f->modified = 1;
	f->change_style(&st);

	e = new Xd6XmlTreeElement(NULL, 0);
	a = new Xd6XmlAttribute("href", 4);
	a->set_value(v, strlen(v));
	e->add_attribute(a);
	
	st.copy(f->cur_seg->stl);
	st.display = 1;
	st.a_link = 1;
	st.underline = 1;
	st.fg_color = COLOR_BLUE;

	e->stl = f->cur_seg->stl->get_style(&st);

	st.copy(f->cur_seg->stl);
	st.display = 0;
	st.a_link = 0;
	st.underline = 0;
	st.fg_color = COLOR_BLACK;
	f->cur_seg->stl = f->cur_seg->stl->get_style(&st);

	s = new Xd6HtmlTagA(0, e, NULL);
	f->insert_segment(s);
	se = f->cur_seg;
	se = f->cur_block->segs[se->id + 1];

	st.copy(se->stl);
	st.display = 1;
	st.a_link = 1;
	st.underline = 1;
	st.fg_color = COLOR_BLUE;

	se->stl =se->stl->get_style(&st);

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

void cb_lang(Fl_Widget*, void*)
{
	Xd6ConfigFile *cfg;
        Xd6ConfigFileSection *sec = NULL;
        Xd6ConfigFileGroup *grp = NULL;
        Xd6ConfigFileItem *itm = NULL;
        const char *val = NULL;
	const char *f;

        cfg = new Xd6ConfigFile("xd640", "Utilities");
        sec = cfg->get_xd640_section();
        grp = sec->add_group("spell", cfg->locale);
        itm = grp->get_create_item("dictionary", cfg->locale);
	val = itm->get_value();
	if (!val) val = "/usr/share/dict";
	f = fl_file_chooser("Dictionary ...", "*.utf8", val);

	if (f) {
		itm->set_value(f);
		cfg->write_xd640_section(sec);
	}
	delete(cfg);
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
              _("  flwriter - Copyright (c) 2000-2003 O'ksi'D\n"
                "This application is an Open Source software.\n"
                "             http://www.oksid.ch"));

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

