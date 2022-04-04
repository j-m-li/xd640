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
#ifndef Xd6HtmlScroll_h
#define Xd6HtmlScroll_h

#include <FL/Fl_Widget.h>
#include <FL/x.h>
#include "Xd6HtmlDisplay.h"
#include "Xd6HtmlBlock.h"
#include "Xd6XmlTree.h"
#include "Xd6ConfigFile.h"
#include <stdio.h>
#include <string.h>

enum Xd6HtmlScrollFlags {
        CLICK_OVER_VSCROLL = 1,
        CLICK_OVER_HSCROLL = 2,
        CLICK_OVER_SCROLL = 4,
};

#define FL_DAMAGE_BLOCK 0x10
#define FL_DAMAGE_ARROWS 0x20


class Xd6HtmlScroll : public Xd6HtmlDisplay {
public:
	Fl_Widget *parent_widget;
	unsigned char damage_;
	int flags;
	int max_width;
	int max_height;
	int vscroll;
	int hscroll;
	int ovscroll;
	int ohscroll;

	Xd6HtmlScroll(int i);
	~Xd6HtmlScroll(void);

	void draw_hscroll(int X, int Y);
	void draw_vscroll(int X, int Y);
	int handle_push_hscroll(int ox, int x);
	int handle_push_vscroll(int oy, int y);
        int handle_drag_hscroll(int ox, int ex);
        int handle_drag_vscroll(int oy, int ey);
	int handle_scroll(int e, int ex, int ey, int x, int y);

	unsigned char damage(void) {return damage_; }
	void damage(unsigned char d);

	void go_bottom(void);
	void go_top(void);
};

#endif

/*
 * "$Id: $"
 */
