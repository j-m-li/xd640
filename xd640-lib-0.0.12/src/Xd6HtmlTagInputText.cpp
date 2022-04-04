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

#include <FL/Fl.h>
#include "Xd6HtmlTagInputText.h"
#include "Xd6HtmlTagForm.h"
#include "Xd6HtmlDownload.h"
#include "Xd6HtmlFrame.h"
#include <FL/fl_draw.h>
#include <FL/fl_utf8.h>
#include <string.h>
#include <stdlib.h>


Xd6HtmlTagInputText::Xd6HtmlTagInputText(int i, Xd6XmlTreeElement *e, Xd6HtmlDisplay *u) : 
	Xd6HtmlDisplay(i, NULL, 0, e->style)
{
	name = NULL;
	value = NULL;
	form = Xd6HtmlTagForm::current;
	size = 10;
	maxlength = 500;

	if (e->get_attr_value("name")) {
		name = strdup(e->get_attr_value("name"));
	}
	if (e->get_attr_value("value")) {
		value = strdup(e->get_attr_value("value"));
	} else {
		value = strdup("");
	}
	if (e->get_attr_value("size")) {
		size = (int) atol(e->get_attr_value("size"));
	}
	if (e->get_attr_value("maxlength")) {
		maxlength = (int) atol(e->get_attr_value("maxlength"));
	}
	if (e->get_attr_value("dir") && 
		!strcasecmp("rlt", e->get_attr_value("dir"))) 
	{
		rtl = 1;	
	} else {
		rtl = 0;
	}
	text = strdup(value);
	cur_chr = text + strlen(text);
	sel_chr = NULL;
	off = 0;
	focus = 0;
	if (form) form->add(this);
}

void Xd6HtmlTagInputText::destroy()
{
	if (name) free(name);
	if (value) free(value);
	if (text) free(text);
}

void Xd6HtmlTagInputText::draw(int x, int y)
{
	x += left;
	y += top;

	fl_font(4, 14);
	fl_color(FL_BLACK);
	
	if (!rtl) {
		int w;
		w = (int) fl_width(text, (cur_chr - text));
		if (off + w + 8 > width) { 
			off = -(w - width + 8);
		} else if (off + w < 8) {
			off = - (w - 8);
		}
		if (off > 0) off = 0;
		if (focus) {
			if (cur_chr && (!sel_chr || cur_chr == sel_chr)) {
				fl_line(x + 1 + off + w, y + 4, 
					x + 1 + off + w, y + 20);
			} else if (cur_chr && sel_chr) {
				fl_color(137);
			}
		}
		fl_draw(text, x + 1 + off, y + 16);	
	} else {
		fl_rtl_draw(text, strlen(text), x + 1 + off, y + 16);
	}
	fl_color(FL_WHITE);
	fl_line(x + width - 1, y, x + width - 1, y + height - 1);
	fl_line(x, y + height - 1,  x + width - 1, y + height - 1);
	fl_color(FL_BLACK);
	fl_line(x, y, x + width - 1, y);
	fl_line(x, y, x, y + height - 1);
	
}

char *Xd6HtmlTagInputText::find_pos(int ex, int x)
{
	int i, l;
	char *ret = text;

	x += left + off + 1;
	l = strlen(text);
	fl_font(4, 14);
	
	i = 0;
	while (i < l) {
		int cw;
		int cl = fl_utflen((unsigned char*)text + i, l - i);
		if (cl < 1) cl = 1;
		cw = (int)fl_width(text + i, cl);
		if (x + cw / 2 > ex) {
			break;
		}
		x += cw;
		i += cl;
		ret = text + i;
	}	
	return ret;
	
}

int Xd6HtmlTagInputText::handle(int e, int x, int y)
{
	if (e == FL_PUSH) {
		if (event_is_inside(x, y)) {
			sel_chr = cur_chr = find_pos(Fl::event_x(), x);
			focus = 1;
			return 1;
		} else {
			focus = 0;
			return 0;
		}
	}
	if (focus != 1) return 0;

	switch (e) {
	case FL_DRAG:
		cur_chr = find_pos(Fl::event_x(), x);
		return 1;
	case FL_UNFOCUS:
		focus = 0;
		return 1;
	case FL_KEYBOARD:

		if (Fl::e_state & (FL_ALT|FL_CTRL)) {
			return 0;
		}

		if (Fl::event_key() == FL_Right) {
			if (Fl::e_state & FL_SHIFT && !sel_chr) {
				sel_chr = cur_chr;
			} else if (!(Fl::e_state & FL_SHIFT)) {
				sel_chr = NULL;
			}
			move_cursor(1);
			return 1;
		} else if (Fl::event_key() == FL_Left) {
			if (Fl::e_state & FL_SHIFT && !sel_chr) {
				sel_chr = cur_chr;
			} else if (!(Fl::e_state & FL_SHIFT)) {
				sel_chr = NULL;
			}
			move_cursor(-1);
			return 1;
		}
		if (Fl::e_length < 1) return 0;

	case FL_PASTE:
		cut();
		if (Fl::e_length == 1) {
			if (Fl::e_text[0] == '\b') {
				sel_chr = cur_chr;
				move_cursor(-1);
				cut();
				return 1;
			} else if (Fl::e_text[0] == '\x7F') {
				sel_chr = cur_chr;
				move_cursor(1);
				cut();
				return 1;
			} else if (Fl::e_text[0] == '\r' ||
				Fl::e_text[0] == '\n') 
			{
				return 0;
			}
		}
		insert_text(Fl::e_text, Fl::e_length);
		break;
	default:
		break;
	}
	return 1;

}

void Xd6HtmlTagInputText::measure()
{
	
	fl_font(4, 14);

	height = 25;
	width = (int) fl_width("_") * size + 4;
	descent = fl_descent();
	
}

char *Xd6HtmlTagInputText::get_text()
{
	return text;
}

void Xd6HtmlTagInputText::cut()
{
	char *b, *e;
	if (!sel_chr || !cur_chr) return;

	if (sel_chr > cur_chr) {
		e = sel_chr;
		b = cur_chr;
	} else {
		b = sel_chr;
		e = cur_chr;
	}
	
	sel_chr = NULL;
	cur_chr = b;
	while (*e) {
		*b = *e;
		b++; e++;
	}
	*b = '\0';
}

void Xd6HtmlTagInputText::move_cursor(int dir)
{
	int l;
	int len;
	len = strlen(text);
	if (dir > 0) {
		l = fl_utflen((unsigned char*)cur_chr, len - (cur_chr - text));	
		if (l < 1) l = 1;
		cur_chr += l;
		if (cur_chr > text + len) {
			cur_chr = text + len;
		}
	} else {
		while (cur_chr - 1 >= text) {
			cur_chr--;
			if (fl_utflen((unsigned char*)cur_chr, 
				len - (cur_chr - text)) > 0) 
			{
				break;
			}
		}
	}
}

void Xd6HtmlTagInputText::insert_text(char *txt, int len)
{
	int i, j;
	int l;
	char *new_str;

	l = strlen(text) + len;

	new_str = (char*) malloc(l + 1);
	
	i = 0;
	while (text[i] && text + i < cur_chr) {
		new_str[i] = text[i];
		i++;
	}
	j = i;
	cur_chr = new_str + i + len;
	while (text[i]) {
		new_str[i + len] = text[i];
		i++;
	}
	i = 0;
	while (i < len) {
		new_str[j] = txt[i];
		j++; i++;
	}
	new_str[l] = '\0';
	free(text);
	text = new_str;
	sel_chr = NULL;
}

