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


#include "xd640/Xd6Std.h"
#include "gui.h"
#include "xd640/Xd6HtmlTagTable.h"
#include <xd640/Xd6SpellChoice.h>
#include <FL/fl_ask.h>
#include <FL/fl_draw.h>
#include "callbacks.h"
#include <FL/Fl_File_Chooser.H>

#define _(String) gettext((String))

GUI *GUI::self = NULL;
static int text_engine(int e, Xd6HtmlFrame *frame, Xd6HtmlBlock *block,
	Xd6HtmlLine *line, Xd6HtmlSegment *seg, char *text);

GUI::GUI(int W, int H) : Fl_Window(W, H)
{
        mnu = NULL;
        mnu_bar = NULL;
        editor = NULL;
        stat_bar = NULL;
        cfg = NULL;
        cfg_sec = NULL;
        font = (Fl_Font) 0;
        size = 16;
	self = this;
	Xd6HtmlFrame::engine = &engine;
	Xd6HtmlFrame::text_engine = &text_engine;
	atexit(cb_atexit);
	fl_set_status(50, H - 25, 100, 25);
}

GUI::~GUI()
{
	self = NULL;
	delete(mnu_bar);
	delete(mnu);
	delete(editor);
	delete(stat_bar);
}

void GUI::create_layout()
{
	
	create_menu();

	begin();

	mnu_bar = new Fl_Menu_Bar(0, 0, 240, 25);;
	mnu_bar->menu(mnu);
	mnu_bar->selection_color(137);


	editor = new Xd6HtmlEditor(0, 25, 241, 146);
	resizable(editor);

	stat_bar = new Fl_Output(0, 171, 241, 20);
	stat_bar->color(FL_GRAY);

	end();
	editor->tool->save->callback(cb_save);
	editor->tool->print->callback(cb_print);
}

void GUI::create_menu()
{
	int i, j;

	mnu = new Fl_Menu_Item[50];
	for (i = 0; i < 50; i++) {
		mnu[i].text = NULL;
	}
	
	i = mnu->add(_("File"), FL_ALT+'f',  0, 0, 0);
	{
		mnu->add(_("New ..."), FL_CTRL+'n', (Fl_Callback*)cb_new, 0, 0);
		mnu->add(_("Open ..."), FL_CTRL+'o', 
			(Fl_Callback*)cb_open, 0, 0);
		//mnu->add(_("Import RTF"), 0, (Fl_Callback*)cb_import, 0, 0);
		mnu->add(_("Save"), FL_CTRL+'s', (Fl_Callback*)cb_save, 0, 0);
		mnu->add(_("Save as ..."), 0, (Fl_Callback*)cb_save_as, 0, 0);
		mnu->add(_("Export RTF"), 0, (Fl_Callback*)cb_export, 
			0, FL_MENU_DIVIDER);
		mnu->add(_("Print ..."), FL_CTRL+'p', 
			(Fl_Callback*)cb_print, 0, 0);
		j = mnu->add(_("Preferences"), 0, 
			NULL, 0, FL_MENU_DIVIDER);
		{
			mnu->add(_("Page Setup"), 0, (Fl_Callback*)cb_pref,
				0, 0);
			mnu->add(_("Font Setup"), 0, (Fl_Callback*)cb_pref_font,
				0, 0);
			
		}
		mnu[j].flags = FL_SUBMENU|FL_MENU_DIVIDER;
		mnu->add(_("Quit"), FL_ALT+'q', (Fl_Callback*)cb_exit, 
			0, 0);
	}
	mnu[i].flags = FL_SUBMENU;

	i = mnu->add(_("Edit"), FL_ALT+'e',  0, 0, 0);
	{
		mnu->add(_("Cut"), FL_CTRL+'x', (Fl_Callback*)cb_cut, 0, 0);
		mnu->add(_("Copy"), FL_CTRL+'c', (Fl_Callback*)cb_copy, 0, 0);
		mnu->add(_("Paste"), FL_CTRL+'v', (Fl_Callback*)cb_paste, 0, 
			FL_MENU_DIVIDER);
		mnu->add(_("Find..."), FL_CTRL+'f', (Fl_Callback*)cb_find, 0,
			FL_MENU_DIVIDER);
		j = mnu->add(_("Zoom"), 0, 0, 0, 0);
		{
			int k;
			k = mnu->add(_("100%"), 0, (Fl_Callback*)cb_zoom,
				(void *)100, FL_MENU_RADIO);
			mnu[k].set();
			mnu->add(_("150%"), 0, (Fl_Callback*)cb_zoom,
				(void *)150, FL_MENU_RADIO);
			mnu->add(_("200%"), 0, (Fl_Callback*)cb_zoom,
				(void *)200, FL_MENU_RADIO);
			mnu->add(_("300%"), 0, (Fl_Callback*)cb_zoom,
				(void *)300, FL_MENU_RADIO);
		}
		mnu[j].flags = FL_SUBMENU;
	}
	mnu[i].flags = FL_SUBMENU;

	i = mnu->add(_("Tools"), FL_ALT+'t',  0, 0, 0);
	{
		mnu->add(_("Tabs"), 0, (Fl_Callback*)cb_tabs,
			0, FL_MENU_DIVIDER);
		mnu->add(_("Insert HTML File"), 0, (Fl_Callback*)cb_insert,
			0, 0);
		mnu->add(_("Insert Picture"), 0, 
			(Fl_Callback*)cb_picture, 0, FL_MENU_DIVIDER);
/*
		mnu->add(_("Mailing"), 0, 
			(Fl_Callback*)cb_mailing, 0, 0);
		mnu->add(_("Insert Mailing Tag"), 0, 
			(Fl_Callback*)cb_tag, 0, 0);
*/
		mnu->add(_("Insert page break"), FL_CTRL+'\r', 
			(Fl_Callback*)cb_page_break, 0, FL_MENU_DIVIDER);
		j = mnu->add(_("Spelling"), 0, 0, 0, 0);
		{
			mnu->add(_("Spell"), 0, 
				(Fl_Callback*)cb_spell, 0, 0);
			mnu->add(_("Set Language"), 0, 
				(Fl_Callback*)cb_lang, 0, 0);
		}	
		mnu[j].flags = FL_SUBMENU|FL_MENU_DIVIDER;
		mnu->add(_("Virtual keyboard"), 0, (Fl_Callback*)cb_keyboard,
			0, 0);
	}
	mnu[i].flags = FL_SUBMENU;

	i = mnu->add(_("Help"), FL_ALT+'h',  0, 0, 0);
	{
		mnu->add(_("About"), FL_ALT+'a', (Fl_Callback*)cb_about, 0, 0);
		mnu->add(_("Online Help"), FL_ALT+'o', 
			(Fl_Callback*)cb_help, 0, 0);
	}
	mnu[i].flags = FL_SUBMENU;

}

int GUI::handle(int e)
{
	return Fl_Window::handle(e);	
}

void GUI::load(const char *f)
{
	editor->file = strdup(f);
	editor->load(f);
	editor->redraw();
}

void GUI::load_rtf(const char *f)
{

}

void GUI::resize(int X, int Y, int W, int H)
{
	Window c; int mx,my,cx,cy; unsigned int mask;
	Window root;
	fl_set_status(50, H - 25, 100, 25);
	if (W == w() && H == h()) {
		Fl_Window::resize(X, Y, W, H);
		return;
	}
#ifdef WIN32
	Fl_Window::resize(X, Y, W, H);
	Fl_Window::redraw();
#else
	root = RootWindow(fl_display, fl_screen);
	XQueryPointer(fl_display,root,&root,&c,&mx,&my,&cx,&cy,&mask);
   	if ((mask & (Button1Mask|Button2Mask|Button3Mask)) == 0) {
		Fl_Window::resize(X, Y, W, H);
	//	Fl_X::i(this)->wait_for_expose = 0;
		Fl_Window::redraw();
	}
#endif
}

int GUI::engine(int e, Xd6HtmlFrame *frame, Xd6HtmlBlock *block,
                Xd6HtmlLine *line, Xd6HtmlDisplay *seg, char *chr)
{
	return GUI::self->rengine(e, frame, block, line, seg, chr);
}

int GUI::rengine(int e, Xd6HtmlFrame *frame, Xd6HtmlBlock *block,
                Xd6HtmlLine *line, Xd6HtmlDisplay *seg, char *chr)
{
        if (frame != editor->view->frame) return 0;
        switch (e) {
        case FL_PUSH:
                if (Fl::event_state(FL_BUTTON3)) {
			if (seg->display == DISPLAY_TABLE) {
				table_menu(frame, block, seg);
			} else if (seg->display == DISPLAY_IMG) {
				img_menu(frame, block, seg);
			}
                }
                break;
        default:
                break;
        }
        return 0;
}

static void export_table(Xd6HtmlTagTable *t)
{
	FILE *fp;
	const char *f;

	f = fl_file_chooser("Save Table as ...", "*.{html,htm,HTM}", NULL);

	fp = fopen(f, "w");
	if (!fp) {
		fl_alert(_("Cannot open file..."));
		return;
	}

	Xd6HtmlFrame::html_header(f, fp);
	t->to_html(fp);
        fprintf(fp, "</body>\n");
        fprintf(fp, "</html>\n");

	fclose(fp);
}

void GUI::table_menu(Xd6HtmlFrame *frame, Xd6HtmlBlock *block, 
	Xd6HtmlDisplay *seg)
{
	int i;
	const Fl_Menu_Item *ret;
	mnu = new Fl_Menu_Item[5];
	for (i = 0; i < 5; i++) {
		mnu[i].text = NULL;
	}

	mnu->add(_("Delete"), 0, 0, 0, 0);
	mnu->add(_("Export"), 0, 0, 0, 0);
	ret = mnu->popup(Fl::event_x(), Fl::event_y(), _("Table Menu"));
	if (ret == mnu) {
		if (fl_ask(_("Really delete table ?"))) {
			Xd6XmlStl *s;
			i = seg->id;
			s = seg->stl;
			s->para.copy(s);
			s->para.display = 0;
			s = s->get_style(&s->para);

			delete(seg);
			block->segs[i] = new 
				Xd6HtmlSegment(i, strdup(""), 0, s);
			frame->sel_chr = NULL;
			frame->cur_chr = block->segs[i]->text;
			frame->cur_seg = block->segs[i];
			frame->cur_block = block;
			frame->measure();
			frame->create_pages();
		}
	} else if (ret == mnu + 1) {
		export_table((Xd6HtmlTagTable*)seg);
	}
	delete(mnu);
	redraw();
}

static int text_engine(int e, Xd6HtmlFrame *frame, Xd6HtmlBlock *block,
	Xd6HtmlLine *line, Xd6HtmlSegment *seg, char *chr)
{
	if (e != FL_PUSH) return 0;
        if (!Fl::event_state(FL_BUTTON3)) return 0;
	GUI::self->txt_menu(frame, block, seg);
	return 1;
}

void GUI::txt_menu(Xd6HtmlFrame *frame, Xd6HtmlBlock *block, 
	Xd6HtmlSegment *seg)
{
	int i;
	const Fl_Menu_Item *ret;
	mnu = new Fl_Menu_Item[5];
	for (i = 0; i < 5; i++) {
		mnu[i].text = NULL;
	}

	mnu->add(_("Spelling"), 0, 0, 0, 0);
	ret = mnu->popup(Fl::event_x(), Fl::event_y(), _("Text Menu"));
	if (ret == mnu) {
		 editor->view->spell_choice(frame, block, seg);	
	}
	redraw();
}

void GUI::img_menu(Xd6HtmlFrame *frame, Xd6HtmlBlock *block, 
	Xd6HtmlDisplay *seg)
{
	int i;
	const Fl_Menu_Item *ret;
	mnu = new Fl_Menu_Item[5];
	for (i = 0; i < 5; i++) {
		mnu[i].text = NULL;
	}

	mnu->add(_("Delete"), 0, 0, 0, 0);
	ret = mnu->popup(Fl::event_x(), Fl::event_y(), _("Image Menu"));
	if (ret == mnu) {
		Xd6XmlStl *s;
		i = seg->id;
		s = seg->stl;
		s->para.copy(s);
		s->para.display = 0;
		s = s->get_style(&s->para);
		delete(seg);
		block->segs[i] = new Xd6HtmlSegment(i, strdup(""), 0, s);
		frame->sel_chr = NULL;
		frame->cur_chr = block->segs[i]->text;
		frame->cur_seg = block->segs[i];
		frame->cur_block = block;
		frame->measure();
		frame->create_pages();
	}
	delete(mnu);
	redraw();
}


/*
 *  End of : "$Id: $"
 */

