/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2003  O'ksi'D
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
#include "xpm/justify.xpm"
#include "xpm/num.xpm"
#include "xpm/bullet.xpm"
#include "xpm/quotep.xpm"
#include "xpm/quotem.xpm"


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
        text_align_right = new Fl_Toggle_Button(X, Y, W, H); X += W + 1;
        text_align_justify = new Fl_Toggle_Button(X, Y, W, H); X += W + 5;

        font_bold = new Fl_Toggle_Button(X, Y, W, H); X += W + 1;
        font_italic = new Fl_Toggle_Button(X, Y, W, H); X += W + 1;

        font_underline = new Fl_Toggle_Button(X, Y, W, H); X += W + 5;

	right_to_left = new Fl_Toggle_Button(X, Y, W, H); X += W + 5;
	
	list_num = new Fl_Toggle_Button(X, Y, W, H); X += W + 1;
	list_bullet = new Fl_Toggle_Button(X, Y, W, H); X += W + 5;
	
	quote_minus =  new Fl_Button(X, Y, W, H); X += W + 1;
	quote_plus =  new Fl_Button(X, Y, W, H); X += W + 5;

	page_break = new Fl_Button(X, Y, W, H); X += W + 5;

	insert_picture = new Fl_Button(X, Y, W, H); X += W + 5;

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
	p_justify = new Fl_Pixmap(justify_xpm);
	p_num = new Fl_Pixmap(num_xpm);
	p_bullet = new Fl_Pixmap(bullet_xpm);
	p_quotem = new Fl_Pixmap(quotem_xpm);
	p_quotep = new Fl_Pixmap(quotep_xpm);

        p_center->label(text_align_center);
        p_left->label(text_align_left);
        p_right->label(text_align_right);
        p_justify->label(text_align_justify);
        p_bold->label(font_bold);
        p_underline->label(font_underline);
        p_italic->label(font_italic);
        p_save->label(save);
	p_rtl->label(right_to_left);
	p_pict->label(insert_picture);	
	p_print->label(print);
	p_break->label(page_break);
	p_spell->label(spell);
	p_num->label(list_num);
	p_bullet->label(list_bullet);
	p_quotem->label(quote_minus);
	p_quotep->label(quote_plus);

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
	text_align_justify->callback(cb_justify);
	font_bold->callback(cb_bold);
	font_italic->callback(cb_italic);
	font_underline->callback(cb_underline);
	right_to_left->callback(cb_rtl);
	insert_picture->callback(cb_pict);
	print->callback(cb_print);
	page_break->callback(cb_break);
	spell->callback(cb_spell);
	list_num->callback(cb_num);
	list_bullet->callback(cb_bullet);
	quote_minus->callback(cb_quotem);
	quote_plus->callback(cb_quotep);
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
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[1] = (1 << 1);
	st.font = (int) data;

	t->editor->view->frame->change_style(&st);
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_size(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[1] = (1 << 2);
	st.font_size = (int) data;

	t->editor->view->frame->change_style(&st);
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_justify(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[0] = (1 << 2);
	st.text_align = TEXT_ALIGN_JUSTIFY;

	t->editor->view->frame->change_style(&st);
	t->text_align_left->value(0);
	t->text_align_right->value(0);
	t->text_align_center->value(0);
	t->text_align_justify->value(1);
	t->redraw();
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_center(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[0] = (1 << 2);
	st.text_align = TEXT_ALIGN_CENTER;

	t->editor->view->frame->change_style(&st);
	t->text_align_left->value(0);
	t->text_align_right->value(0);
	t->text_align_center->value(1);
	t->text_align_justify->value(0);
	t->redraw();
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_left(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[0] = (1 << 2);
	st.text_align = TEXT_ALIGN_LEFT;

	t->editor->view->frame->change_style(&st);
	t->text_align_center->value(0);
	t->text_align_right->value(0);
	t->text_align_left->value(1);
	t->text_align_justify->value(0);
	t->redraw();
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_right(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[0] = (1 << 2);
	st.text_align = TEXT_ALIGN_RIGHT;

	t->editor->view->frame->change_style(&st);
	t->text_align_center->value(0);
	t->text_align_left->value(0);
	t->text_align_right->value(1);
	t->text_align_justify->value(0);
	t->redraw();
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_bold(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[1] = (1 << 4);

	if (t->font_bold->value()) {
		st.font_bold = 1;
	} else {
		st.font_bold = 0;
	}
	t->editor->view->frame->change_style(&st);
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_bullet(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[0] = (1 << 6);

	if (t->list_bullet->value()) {
		st.list = LIST_DISC;
	} else {
		st.list = LIST_NONE;
	}
	t->list_num->value(0);
	t->editor->view->frame->change_style(&st);
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_num(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[0] = (1 << 6);

	if (t->list_num->value()) {
		st.list = LIST_NUMBER;
	} else {
		st.list = LIST_NONE;
	}
	t->list_bullet->value(0);
	t->editor->view->frame->change_style(&st);
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_underline(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[1] = (1 << 6);

	if (t->font_underline->value()) {
		st.underline = 1;
	} else {
		st.underline = 0;
	}
	t->editor->view->frame->change_style(&st);
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_italic(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[1] = (1 << 5);
	if (t->font_italic->value()) {
		st.font_italic = 1;
	} else {
		st.font_italic = 0;
	}
	t->editor->view->frame->change_style(&st);
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_save(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	
	t->editor->view->frame->to_html("output.html");
}

void Xd6HtmlToolbar::cb_quotem(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[0] = (1 << 3);
	st.blockquote = -1;

	t->editor->view->frame->change_style(&st);
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_quotep(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[0] = (1 << 3);
	st.blockquote = 1;

	t->editor->view->frame->change_style(&st);
	t->editor->view->redraw();
	t->update_style();
}

void Xd6HtmlToolbar::cb_rtl(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	Xd6XmlStl st;

	st.clear_flags();
	st.flags[1] = (1 << 0);

	if (t->right_to_left->value()) {
		st.rtl_direction = 1;
	} else {
		st.rtl_direction = 0;
	}
	t->editor->view->frame->change_style(&st);
	t->editor->view->redraw();
	t->update_style();
}

static void cb_pict_open(Fl_Widget *w, void *data)
{
	const char *f;
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar *) data;
	
	f = fl_file_chooser(_("Open PNG file..."), "*.{png,PNG}", NULL);
	if (f) {
		t->pict_name->value(f);
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
	w_size->value("");

	h_size = new Fl_Input(125, 115, 70, 20);
	h_size->label(_("height:"));
	h_size->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
	h_size->value("");

	Fl_Input border(5, 160, 70, 20);
	border.label(_("border:"));
	border.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
	border.value("0");

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
		Xd6XmlStl st;
		i = new Xd6HtmlTagImg(0, NULL, editor->view->frame);
		i->attr_h = (int) atol(h_size->value());
		i->attr_w = (int) atol(w_size->value());
		if(border.value()) i->attr_border = (int) atol(border.value());
		i->source = strdup(pict_name->value());
		
		st.copy(editor->view->frame->stl);
		st.display = 1;
		st.is_inline = 1;
		st.is_block = 0;
		i->stl = editor->view->frame->stl->get_style(&st);
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
	Xd6XmlStl st;

	f = t->editor->view->frame;
	f->cut();
	if (!f->cur_seg) return;
	f->split_block(f->cur_block, f->cur_seg, f->cur_chr);
	f->split_block(f->cur_block, f->cur_seg, f->cur_chr);

	st.copy(f->blocks[f->cur_block->id]->stl);
	st.page_break = 1;
	b = f->blocks[f->cur_block->id - 1];
	b->stl = b->stl->get_style(&st);
	while (b->nb_segs > 0) {
		b->nb_segs--;
		delete(b->segs[b->nb_segs]);
	}

//	f->blocks[f->cur_block->id]->stl = 
//		f->blocks[f->cur_block->id]->stl->get_style(&st);
/*
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
*/
	f->measure();
	f->create_pages();
	f->set_cut_cursor(f->cur_chr, f->cur_seg, f->cur_block);
	f->auto_scroll(-1, -1);
	f->auto_scroll(1, 1);
	t->editor->redraw();
}

void Xd6HtmlToolbar::cb_spell(Fl_Widget *w, void *data)
{
	Xd6HtmlToolbar *t = (Xd6HtmlToolbar*) w->parent();
	t->editor->view->spell();
}

void Xd6HtmlToolbar::set_style(Xd6XmlStl *style, Xd6XmlStl *blk_style)
{
	if (old_style == style && old_bstyle == blk_style) return;
	old_style = style;
	old_bstyle = blk_style;

        switch(style->font_size) {
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
		font_size->value(2);
        }

        switch(style->font) {
        case FONT_SERIF:
                font_family->value(0); break;
        case FONT_SANS_SERIF:
                font_family->value(1); break;
        case FONT_MONOSPACE:
                font_family->value(2); break;
        default:
                font_family->value(0);
        }

	switch (blk_style->list) {
	case LIST_NUMBER:
		list_num->value(1);
		list_bullet->value(0);
		break;
	case LIST_DISC:
		list_num->value(0);
		list_bullet->value(1);
		break;
	default:
		list_num->value(0);
		list_bullet->value(0);
	}

	switch (blk_style->text_align) {
	case TEXT_ALIGN_LEFT:
		text_align_left->value(1);
		text_align_right->value(0);
		text_align_center->value(0);
		text_align_justify->value(0);
		break;
	case TEXT_ALIGN_JUSTIFY:
		text_align_left->value(0);
		text_align_right->value(0);
		text_align_center->value(0);
		text_align_justify->value(1);
		break;
	case TEXT_ALIGN_CENTER:
		text_align_left->value(0);
		text_align_right->value(0);
		text_align_center->value(1);
		text_align_justify->value(0);
		break;
	case TEXT_ALIGN_RIGHT:			
		text_align_left->value(0);
		text_align_right->value(1);
		text_align_center->value(0);
		text_align_justify->value(0);
	}

	if (style->underline) {
		font_underline->value(1);
	} else {
		font_underline->value(0);
	}
	if (style->rtl_direction) {
		right_to_left->value(1);
	} else {
		right_to_left->value(0);
	}
	
        if (style->font_bold) {
		font_bold->value(1);
	} else {
		font_bold->value(0);
        }
        if (style->font_italic) {
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
	Fl::flush();
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
		set_style(editor->view->frame->cur_seg->stl,
			editor->view->frame->cur_block->stl);
	}
}

/*
 * "$Id: $"
 */

