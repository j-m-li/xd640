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

#include "Xd6Std.h" 
#include <FL/Fl.h>
#include <FL/Fl_Widget.h>
#include <FL/fl_draw.h>
#include <FL/fl_utf8.h>
#include "Xd6HtmlScroll.h"
#include <stdio.h>
#include <stdlib.h>


Xd6HtmlScroll::Xd6HtmlScroll(int i) : Xd6HtmlDisplay(i, NULL, 0, 0)
{
	parent_widget = NULL;
	damage_ = FL_DAMAGE_ALL;
	max_width = 0;
	max_height = 0;
	
	ovscroll = vscroll = 0;
	ohscroll = hscroll = 0;
	flags = 0;
}


Xd6HtmlScroll::~Xd6HtmlScroll()
{
}


static int get_elevator_size(int h, int v, int l)
{
	int size;
	
	size = (int)((double)v * (double)l / (double)h);
	if (size < 15) size = 15;
	if (size > l) size = l;
	return size;
}


static void draw_ridge_box(int x, int y, int w, int h)
{
        w--;
        h--;
        fl_color(0xC0, 0xC0, 0XC0);
        fl_line(x + 1, y + 1, x + w - 2, y + 1);
        fl_line(x + 1, y + 1, x + 1,  y + h - 2);

        fl_color(0xFF, 0xFF, 0xFF);
        fl_line(x, y, x + w, y);
        fl_line(x, y, x, y + h);

        fl_color(0x0, 0x0, 0x0);
        fl_line(x, y + h, x + w, y + h);
        fl_line(x + w, y, x + w, y + h);

        fl_color(0x80, 0x80, 0x80);
        fl_line(x + 1, y + h - 1, x + w - 1, y + h - 1);
        fl_line(x + w - 1, y + 1, x + w - 1, y + h - 1);
}

static void draw_button(int t, int x, int y, int w, int h)
{
        fl_color(0xC0, 0xC0, 0XC0);
        fl_rectf(x, y, w, h);

        draw_ridge_box(x, y, w, h);

        fl_color(0x0, 0x0, 0x0);
        switch (t) {
        case 1:
                fl_polygon(x + 7, y + 4, x + 11, y + 8, x + 3, y + 8);
                break;
        case 2:
                fl_polygon(x + 7, y + 9, x + 11, y + 5, x + 3, y + 5);
                break;
        case 3:
                fl_polygon(x + 4, y + 7, x + 8, y + 11, x + 8, y + 3);
                break;
        default:
                fl_polygon(x + 9, y + 7, x + 5, y + 11, x + 5, y + 3);
        }
}

static int get_elevator_pos(int vh, int sl, int sc)
{
	int pos;
	pos = (int)((double)sl / (double)vh * (double) sc);

	return pos;
}

static int get_elevator_scroll(int vh, int sl, int pos)
{
	int sc;
	sc = (int)((double)vh / (double)sl * (double) pos);
	return -sc;
}

void Xd6HtmlScroll::draw_hscroll(int X, int Y)
{
	int size;
	int pos;
	int top = Y - 15 + max_height;
	int left = X;

	size = get_elevator_size(width,  max_width - 15, max_width - 45);
 	pos = get_elevator_pos(width - max_width + 15, 
				max_width - 45 - size, -hscroll);

        fl_color(0xC0, 0xC0, 0XC0);
	fl_rectf(left, top, max_width, 15);
        fl_rectf(left + pos + 15, top, size, 15);
        draw_ridge_box(left + pos + 15, top, size, 15);
        draw_button(3, left, top, 15, 15);
        draw_button(4, left + max_width - 15 - 15, top, 15, 15);

}

void Xd6HtmlScroll::draw_vscroll(int X, int Y)
{
	int size;
	int pos;
	int left = X - 15 + max_width;
	int top = Y;

	size = get_elevator_size(height,  max_height - 15, max_height - 45);
 	pos = get_elevator_pos(height - max_height + 15, 
				max_height - 45 - size, -vscroll);

        fl_color(0xC0, 0xC0, 0XC0);
	fl_rectf(left, top, 15, max_height);
        fl_rectf(left, top + pos + 15, 15, size);
        draw_ridge_box(left, top + pos + 15, 15, size);
        draw_button(1, left, top, 15, 15);
        draw_button(2, left, top + max_height - 15 - 15, 15, 15);

}

static int op = 0; // previous scrollbar cursor position

int Xd6HtmlScroll::handle_drag_vscroll(int oy, int ey)
{
	int dy;
	int size, pos;

	dy = ey - oy;
	size = get_elevator_size(height,  max_height - 15, max_height - 45);
		
	pos = op + dy;	
	vscroll = get_elevator_scroll(height - max_height + 15,
					max_height - 45 - size, pos);

	if (vscroll <= -(height - max_height + 15)) {
		vscroll = -(height - max_height + 15);
	}
	if (vscroll > 0) vscroll = 0;
	return 1;
}

int Xd6HtmlScroll::handle_drag_hscroll(int ox, int ex)
{
	int dx;
	int size, pos;

	dx = ex - ox;
	size = get_elevator_size(width,  max_width - 15, max_width - 45);
		
	pos = op + dx;	
	hscroll = get_elevator_scroll(width - max_width + 15,
					max_width - 45 - size, pos);

	if (hscroll <= -(width - max_width + 15)) {
		hscroll = -(width - max_width + 15);
	}
	if (hscroll > 0) hscroll = 0;
	return 1;
}

int Xd6HtmlScroll::handle_push_vscroll(int oy, int y) 
{
	int ret = 0;

	if (oy >= y + top + max_height - 30) {
		// down arrow
		vscroll -= 15;
		ret = 1;
	} else if (oy <= y + top + 15) {
		// up arrow
		vscroll += 15;
		ret = 1;
	} else {
		int size, pos;
		size = get_elevator_size(height,  
					max_height - 15, max_height - 45);
 		pos = get_elevator_pos(height - max_height + 15, 
					max_height - 45 - size, -vscroll);

		if (oy < pos + y + top + 15) {
			// over the cursor
			vscroll += max_height - 30;
			ret = 1;
		} else if (oy > pos + size + y + top + 15) {
			if (oy < y + top + max_height - 15) {
				// under the cursor
				vscroll -= max_height - 30;
				ret = 1;
			}
		} else {
			// in the cursor
			flags = flags | CLICK_OVER_VSCROLL;
			op = pos;
			ret = 1;
		}
	}
	if (vscroll <= -(height - max_height + 15)) {
		vscroll = -(height - max_height + 15);
	}
	if (vscroll > 0) vscroll = 0;
	return ret;
}

int Xd6HtmlScroll::handle_push_hscroll(int ox, int x) 
{
	int ret = 0;

	if (ox >= x + left + max_width - 30 && ox <= x + left + max_width - 15)
 	{
		// right arrow
		hscroll -= 15;
		ret = 1;
	} else if (ox <= x + left + 15) {
		// left arrow
		hscroll += 15;
		ret = 1;
	} else {
		int size, pos;
		size = get_elevator_size(width,  
					max_width - 15, max_width - 45);
 		pos = get_elevator_pos(width - max_width + 15, 
					max_width - 45 - size, -hscroll);

		if (ox < pos + x + left + 15) {
			// to the left of the cursor
			hscroll += max_width - 30;
			ret = 1;
		} else if (ox > pos + size + x + left + 15) {
			if (ox < x + left + max_width - 15) {
				// to the right the cursor
				hscroll -= max_width - 30;
				ret = 1;
			}
		} else {
			// in the cursor
			flags = flags | CLICK_OVER_HSCROLL;
			op = pos;
			ret = 1;
		}
	}
	if (hscroll <= -(width - max_width + 15)) {
		hscroll = -(width - max_width + 15);
	}
	if (hscroll > 0) hscroll = 0;
	return ret;
}

static int push_x = 0;
static int push_y = 0;

static void cb_scroll(Xd6HtmlScroll *self)
{
	Fl::repeat_timeout(0.1, (Fl_Timeout_Handler)cb_scroll, self);
	self->handle(FL_PUSH, push_x, push_y);
	self->damage(FL_DAMAGE_ARROWS);
}

int Xd6HtmlScroll::handle_scroll(int e, int ex, int ey, int x, int y)
{
	static int ox = 0;
	static int oy = 0;
	int ret;
	
	if (e == FL_DRAG) {
		Fl::remove_timeout((Fl_Timeout_Handler)cb_scroll, this);
		if (CLICK_OVER_VSCROLL & flags) {
			int v = vscroll;
			ret = handle_drag_vscroll(oy, ey);
			if (v != vscroll) damage(FL_DAMAGE_ARROWS);
		} else if (CLICK_OVER_HSCROLL & flags) {
			int h = hscroll;
			ret = handle_drag_hscroll(ox, ex);
			if (h != hscroll) damage(FL_DAMAGE_ARROWS);
		} else {
			ret = 0;
		}
		return ret;
	} else if (e == FL_PUSH) {
		int ret = 0;

		ox = ex;
		oy = ey;
		push_x = x;
		push_y = y;
		if (ox >= x + left + max_width - 15 &&
			ox <= x + left + max_width &&
			oy <= y + top + max_height - 15) 
		{
			ret = handle_push_vscroll(oy, y);
			flags = flags | CLICK_OVER_SCROLL;
		} else if (oy >= y + top + max_height - 15 &&
			oy <= y + top + max_height) 
		{
			ret = handle_push_hscroll(ox, x);
			flags = flags | CLICK_OVER_SCROLL;
		}
		if (ret) {
			if (!Fl::has_timeout((Fl_Timeout_Handler)cb_scroll,
				this))
			{
				Fl::add_timeout(.5, 
					(Fl_Timeout_Handler)cb_scroll, this);
			}
		}
		return ret;
	} else if (e == FL_RELEASE) {
		damage(FL_DAMAGE_ALL);
		Fl::remove_timeout((Fl_Timeout_Handler)cb_scroll, this);
		if (flags & (CLICK_OVER_HSCROLL|CLICK_OVER_VSCROLL|
			CLICK_OVER_SCROLL))
		{
			flags = flags & ~(CLICK_OVER_HSCROLL|CLICK_OVER_VSCROLL|
				CLICK_OVER_SCROLL);
			return 1;
		}
		return 0;
	} else if (e == FL_KEYBOARD) {
		int ret = 0;
		if (Fl::event_key() == FL_Page_Up) {
			vscroll += max_height - 30;
			ret = 1;
			damage(FL_DAMAGE_ARROWS);	
		} else if (Fl::event_key() == FL_Page_Down) {
			vscroll -= max_height - 30;
			ret = 1;
			damage(FL_DAMAGE_ARROWS);
		}
		if (vscroll <= -(height - max_height + 15)) {
			vscroll = -(height - max_height + 15);
		}
		if (vscroll > 0) vscroll = 0;
		return ret;
	}
	return 0;
}

void Xd6HtmlScroll::damage(unsigned char d)
{
        damage_ |= d;
        if (parent_widget) {
#if HAVE_FLTK_UTF || HAVE_FLTK
                parent_widget->damage(FL_DAMAGE_CHILD);
#else
		parent_widget->damage(FL_DAMAGE_CHILD);
#endif
        }
}

void Xd6HtmlScroll::go_bottom()
{
	vscroll = -(height - max_height + 15); 
}

void Xd6HtmlScroll::go_top()
{
	vscroll = 0;
}

/*
 * "$Id: $"
 */

