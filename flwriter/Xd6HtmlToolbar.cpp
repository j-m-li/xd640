/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000,2001  O'ksi'D
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
#include <FL/Fl_File_Chooser.H>
#include "Xd6HtmlToolbar.h"
#include "Xd6HtmlEditor.h"
#include "xd640/Xd6HtmlPrint.h"
#include "xd640/Xd6HtmlTagImg.h"
#include "xd640/Xd6XmlStyle.h"

#define _(String) gettext((String))

#include "xpm/center.xpm"
#include "xpm/left.xpm"
#include "xpm/right.xpm"
#include "xpm/bold.xpm"
#include "xpm/underline.xpm"
#include "xpm/italic.xpm"
#include "xpm/save.xpm"
#include "xpm/rtl.xpm"
#include "xpm/pict.xpm"
#include "xpm/print.xpm"
#include "xpm/break.xpm"
#include "xpm/spell.xpm"


Xd6HtmlToolbar::Xd6HtmlToolbar(Xd6HtmlEditor *e, int X, int Y, int W, int H) : 
	Fl_Group(X, Y, W, H)
{
	box(FL_UP_BOX);
        Y += 2;
        H -= 4;
        W = H;
        X += 3;

	old_style = 0;
	old_bstyle = 0;
	editor = e;

        save = new Fl_Button(X, Y, W, H); X += W + 5;
        print = new Fl_Button(X, Y, W, H); X += W + 5;
        font_family = new Fl_Choice(X, Y, 110, H); X += 111;
        font_size = new Fl_Choice(X, Y, 50, H); X += 51 + 5;
        text_align_left = new Fl_Toggle_Button(X, Y, W, H); X += W + 1;
        text_align_center = new Fl_Toggle_Button(X, Y, W, H); X += W + 1;
        text_align_right = new Fl_Toggle_Button(X, Y, W, H); X += W + 5;
        font_bold = new Fl_Toggle_Button(X, Y, W, H); X += W + 1;
        font_italic = new Fl_Toggle_Button(X, Y, W, H); X += W + 1;
        font_underline = new Fl_Toggle_Button(X, Y, W, H); X += W + 5;
	right_to_left = new Fl_Toggle_Button(X, Y, W, H); X += W + 5;
	insert_picture = new Fl_Button(X, Y, W, H); X += W + 5;
	page_break = new Fl_Button(X, Y, W, H); X += W + 5;
	spell = new Fl_Button(X, Y, W, H); X += W + 5;
        end();
	resizable(NULL);

        p_center = new Fl_Pixmap(center_xpm);
        p_left= new Fl_Pixmap(left_xpm);
        p_right = new Fl_Pixmap(right_xpm);
        p_bold = new Fl_Pixmap(bold_xpm);
        p_underline = new Fl_Pixmap(underline_xpm);
        p_italic = new Fl_Pixmap(italic_xpm);
        p_save = new Fl_Pixmap(save_xpm);
	p_rtl = new Fl_Pixmap(rtl_xpm);
	p_pict = new Fl_Pixmap(pict_xpm);
	p_print = new Fl_Pixmap(print_xpm);
	p_break = new Fl_Pixmap(break_xpm);
	p_spell = new Fl_Pixmap(spell_xpm);

        p_center->label(text_align_center);
        p_left->label(text_align_left);
        p_right->label(text_align_right);
        p_bold->label(font_bold);
        p_underline->label(font_underline);
        p_italic->label(font_italic);
        p_save->label(save);
	p_rtl->label(right_to_left);
	p_pict->label(insert_picture);	
	p_print->label(print);
	p_break->label(page_break);
	p_spell->label(spell);

	create_menus();

	font_size->menu(font_size_items);
	font_size->callback(cb_size);
	font_size->value(2);
	
	font_family->menu(font_family_items);
	font_family->callback(cb_family);

	save->callback(cb_save);
	text_align_left->callback(cb_left);
	text_align_right->callback(cb_right);
	text_align_center->callback(cb_center);
	font_bold->callback(cb_bold);
	font_italic->callback(cb_italic);
	font_underline->callback(cb_underline);
	right_to_left->callback(cb_rtl);
	insert_picture->callback(cb_pict);
	print->callback(cb_print);
	page_break->callback(cb_break);
	spell->callback(cb_spell);
}


Xd6HtmlToolbar::~Xd6HtmlToolbar()
{
}

void Xd6HtmlToolbar::create_menus()
{
	int i;
	font_family_items = new Fl_Menu_Item[4];
	font_size_items = new Fl_Menu_Item[8];
	for (i = 0; i < 4; i++) font_family_items[i].text = NULL;
	for (i = 0; i < 8; i++) font_size_items[i].text = NULL;

	font_family_items->add("Serif", 0, cb_family, (void*) FONT_SERIF, 0);
	font_family_items->add("Sans-Serif", 0, cb_family,  
		(void*) FONT_SANS_SERIF, 0);
	font_family_items->add("Monospace", 0, cb_family, 
		(void*) FONT_MONOSPACE, 0);
	
	font_size_items->add("8", 0, cb_size, (void*)  FONT_SIZE_1, 0);
	font_size_items->add("10", 0, cb_size, (void*)  FONT_SIZE_2, 0);
	font_size_items->add("12", 0, cb_size, (void*)  FONT_SIZE_3, 0);
	font_size_items->add("14", 0, cb_size, (void*)  FONT_SIZE_4, 0);
	font_size_items->add("18", 0, cb_size, (void*)  FONT_SIZE_5, 0);
	font_size_items->add("24", 0, cb_size, (void*)  FONT_SIZE_6, 0);
	font_size_items->add("36", 0, cb_size, (void*)  FONT_SIZE_7, 0);
}

void Xd6HtmlToolbar::cb_family(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	t->editor->view->frame->change_style(FONTS, (int) data);
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_size(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	t->editor->view->frame->change_style(FONT_SIZES, (int) data);
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_center(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	t->editor->view->frame->change_style(TEXT_ALIGN, TEXT_ALIGN_CENTER);
	t->text_align_left->value(0);
	t->text_align_right->value(0);
	t->text_align_center->value(1);
	t->redraw();
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_left(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	t->editor->view->frame->change_style(TEXT_ALIGN, TEXT_ALIGN_LEFT);
	t->text_align_center->value(0);
	t->text_align_right->value(0);
	t->text_align_left->value(1);
	t->redraw();
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_right(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	t->editor->view->frame->change_style(TEXT_ALIGN, TEXT_ALIGN_RIGHT);
	t->text_align_center->value(0);
	t->text_align_left->value(0);
	t->text_align_right->value(1);
	t->redraw();
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_bold(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	if (t->font_bold->value()) {
		t->editor->view->frame->change_style(0, FONT_BOLD);
	} else {
		t->editor->view->frame->change_style(FONT_BOLD, 0);
	}
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_underline(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	if (t->font_underline->value()) {
		t->editor->view->frame->change_style(0, UNDERLINE);
	} else {
		t->editor->view->frame->change_style(UNDERLINE, 0);
	}
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_italic(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	if (t->font_italic->value()) {
		t->editor->view->frame->change_style(0, FONT_ITALIC);
	} else {
		t->editor->view->frame->change_style(FONT_ITALIC, 0);
	}
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_save(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	
	t->editor->view->frame->to_html("output.html");
}

void Xd6HtmlToolbar::cb_rtl(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	if (t->right_to_left->value()) {
		t->editor->view->frame->change_style(0, RTL_DIRECTION);
	} else {
		t->editor->view->frame->change_style(RTL_DIRECTION, 0);
	}
	t->editor->view->redraw();
	t->update_style();
}

static void cb_pict_open(Fl_Widget *w, void *data)
{
	const char *f;
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar *) data;
	
	f = fl_file_chooser(_("Open Gif file..."), "*.{gif,GIF}", NULL);
	if (f) {
		int w, h;
		FILE *fp;
		unsigned char buf[15];
		t->pict_name->value(f);
		fp = fopen(f, "r");
		if (!fp) return;
		fread(buf, 1, 13, fp);
		fclose(fp);
		if (!(memcmp(buf, "GIF87a", 6) == 0 ||
			memcmp(buf, "GIF89a", 6) == 0))
		{
			return;
		}
		w = (buf[7] << 8) | buf[6];
		h = (buf[9] << 8) | buf[8];
		snprintf((char*)buf, 15, "%d", w);
		t->w_size->value((char*)buf);
		snprintf((char*)buf, 15, "%d", h);
		t->h_size->value((char*)buf);
	}
	

}

void Xd6HtmlToolbar::pict_dialog()
{
	Fl_Widget *o;
	Fl_Window pict_dialog(240, 240);	
	pict_dialog.label(_("Insert picture"));

	pict_name = new Fl_Input(5, 25, 230, 20);
	pict_name->label(_("File name:"));
	pict_name->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
	
	Fl_Button load(120, 60, 90, 25);
	load.label(_("Open ..."));
	load.callback(cb_pict_open, this);
	
	w_size = new Fl_Input(5, 115, 70, 20);
	w_size->label(_("width:"));
	w_size->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);

	h_size = new Fl_Input(125, 115, 70, 20);
	h_size->label(_("height:"));
	h_size->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);

	Fl_Input border(5, 160, 70, 20);
	border.label(_("border:"));
	border.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
	
	Fl_Button ok(65, 205, 80, 25, _("OK"));
        Fl_Button cancel(155, 205, 80, 25, _("Cancel"));

	pict_dialog.end();
	pict_dialog.set_modal();
	pict_dialog.show();

        for (;;) {
                o = Fl::readqueue();
                if (o != &ok && o != &cancel) {
                        Fl::wait();
                } else {
                        break;
                }
        }

	if (o == &ok && w_size->value() && h_size->value() &&
		pict_name->value()) 
	{

		Xd6HtmlTagImg *i;
		i = new Xd6HtmlTagImg(0, NULL, editor->view->frame);
		i->attr_h = (int) atol(h_size->value());
		i->attr_w = (int) atol(w_size->value());
		if(border.value()) i->attr_border = (int) atol(border.value());
		i->source = strdup(pict_name->value());
		i->style = DISPLAY|IS_INLINE;
		i->display = DISPLAY_IMG;
		editor->view->frame->cut();
		editor->view->frame->insert_segment(i);
		editor->view->frame->create_pages();
		editor->view->frame->damage(FL_DAMAGE_ALL);
	}
	pict_dialog.hide();
	pict_dialog.remove(pict_name);
	pict_dialog.remove(w_size);
	pict_dialog.remove(h_size);
	delete(pict_name);
	delete(w_size);
	delete(h_size);
}

void Xd6HtmlToolbar::cb_pict(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	t->pict_dialog();
}

void Xd6HtmlToolbar::cb_print(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	t->editor->view->print();
}

void Xd6HtmlToolbar::cb_break(Fl_Widget *w, void *data)
{
	Xd6HtmlFrame *f;
	Xd6HtmlBlock *b;
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	f = t->editor->view->frame;
	f->cut();
	if (!f->cur_seg) return;
	f->split_block(f->cur_block, f->cur_seg, f->cur_chr);
	f->blocks[f->cur_block->id]->style |= PAGE_BREAK;
	b = f->blocks[f->cur_block->id - 1];
	if (b->nb_segs == 1) {
		Xd6HtmlSegment *s;
		s = b->segs[0];
		if (s->len == 0) {
			s->text = (char*) realloc(s->text, 2);
			s->text[0] = ' ';
			s->text[1] = 0;
			s->len = 1;
		}
	}
	f->measure();
	f->create_pages();
	f->set_cut_cursor(f->cur_chr, f->cur_seg, f->cur_block);
	t->editor->redraw();
}

void Xd6HtmlToolbar::cb_spell(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	t->editor->view->spell();
}

void Xd6HtmlToolbar::set_style(int style, int blk_style)
{
	if (old_style == style && old_bstyle == blk_style) return;
	old_style = style;
	old_bstyle = blk_style;

        switch(style & FONT_SIZES) {
        case FONT_SIZE_1:
                font_size->value(0); break;
        case FONT_SIZE_2:
                font_size->value(1); break;
        case FONT_SIZE_3:
                font_size->value(2); break;
        case FONT_SIZE_4:
                font_size->value(3); break;
        case FONT_SIZE_5:
                font_size->value(4); break;
        case FONT_SIZE_6:
                font_size->value(5); break;
        case FONT_SIZE_7:
                font_size->value(6); break;
        default:
		font_size->value(1);
        }

        switch(style & FONTS) {
        case FONT_SERIF:
                font_family->value(0); break;
        case FONT_SANS_SERIF:
                font_family->value(1); break;
        case FONT_MONOSPACE:
                font_family->value(2); break;
        default:
                font_family->value(0);
        }

	switch (blk_style & TEXT_ALIGN) {
	case TEXT_ALIGN_LEFT:
		text_align_left->value(1);
		text_align_right->value(0);
		text_align_center->value(0);
		break;
	case TEXT_ALIGN_CENTER:
		text_align_left->value(0);
		text_align_right->value(0);
		text_align_center->value(1);
		break;
	case TEXT_ALIGN_RIGHT:			
		text_align_left->value(0);
		text_align_right->value(1);
		text_align_center->value(0);
	}

	if (style & UNDERLINE) {
		font_underline->value(1);
	} else {
		font_underline->value(0);
	}
	if (style & RTL_DIRECTION) {
		right_to_left->value(1);
	} else {
		right_to_left->value(0);
	}
	
        if (style & FONT_BOLD) {
		font_bold->value(1);
	} else {
		font_bold->value(0);
        }
        if (style & FONT_ITALIC) {
		font_italic->value(1);
        } else {
		font_italic->value(0);
	}

	redraw();
}

void Xd6HtmlToolbar::cut()
{
	copy();
	editor->view->frame->cut();
	editor->redraw();
}

void Xd6HtmlToolbar::copy()
{
	char bu[1024];
	snprintf(bu, 1024, "%s/cut_buffer.html",
		downloader->cfg->user_paths->apps);
	editor->view->frame->select_to_html(bu);
	editor->redraw();
}

void Xd6HtmlToolbar::paste()
{
	Xd6XmlParser *p = new Xd6XmlParser();
	Xd6HtmlFrame *f;
	char bu[1024];
	snprintf(bu, 1024, "%s/cut_buffer.html",
		downloader->cfg->user_paths->apps);
	f = new Xd6HtmlFrame(0);
	p->parse_file(bu);
	f->tree2block(p->tree->root);
	editor->view->frame->insert_frame(f);
	delete(f);
	delete(p);
	editor->redraw();
}

void Xd6HtmlToolbar::insert_file()
{
	const char *f;
	Xd6XmlParser *p;
	Xd6HtmlFrame *fr;
	f = fl_file_chooser(_("Insert Xd6Html file..."), 
		"*.{html,htm}", NULL);

	if (!f) return;
	p = new Xd6XmlParser();
	fr = new Xd6HtmlFrame(0);
	p->parse_file(f);
	fr->tree2block(p->tree->root);
	editor->view->frame->insert_frame(fr);
	delete(fr);
	delete(p);
	Fl::redraw();
}

void Xd6HtmlToolbar::update_style()
{
	if (editor->view->frame->cur_chr) {
		set_style(editor->view->frame->cur_seg->style,
			editor->view->frame->cur_block->style);
	}
}

/*
 * "$Id: $"
 */

