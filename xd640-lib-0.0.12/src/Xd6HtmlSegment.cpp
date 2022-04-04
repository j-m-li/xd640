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

#define HAVE_XUTF8  1

#include "Xd6HtmlSegment.h"
#include <stdio.h>
#include <stdlib.h>
#include <FL/x.h>
#include <FL/fl_draw.h>
#include <FL/fl_utf8.h>
#include <FL/Fl.h>

#include "Xd6HtmlDisplay.h"


int Xd6HtmlSegment::sizes1[] = { 8, 10, 12, 14, 18, 24, 36};
int Xd6HtmlSegment::sizes4[] = {12, 14, 18, 24, 30, 36, 56};
int Xd6HtmlSegment::sizes2[] = {14, 20, 24, 30, 36, 48, 72};
int Xd6HtmlSegment::sizes3[] = {24, 30, 36, 42, 56, 72, 96};
int *Xd6HtmlSegment::sizes = Xd6HtmlSegment::sizes1;

Xd6HtmlSegment::Xd6HtmlSegment(int i, char *txt, int l, int s)
{
	id = i;
	top = 0;
	left = 0;
	width = 0;
	height = 0;
	descent = 0;
	text = txt;
	if (!text) text = (char*) malloc(1);
	len = l;
	style = s;
}

Xd6HtmlSegment::Xd6HtmlSegment()
{
	id = 0;
	top = 0;
	left = 0;
	width = 0;
	height = 0;
	descent = 0;
	text = (char*) malloc(1);
	len = 0;
	style = 0;
}

Xd6HtmlSegment::~Xd6HtmlSegment()
{
	if (text) free(text);
	text = NULL;
}


void Xd6HtmlSegment::measure()
{
	if (style & DISPLAY) {
		((Xd6HtmlDisplay*)this)->measure();
		return;
	}
	set_font();

	width = (int) fl_width(text, len);
	descent = fl_descent();
	height = fl_height();
}

void Xd6HtmlSegment::position(int X, int Y) 
{
	top = Y;
	left = X;
}

void Xd6HtmlSegment::draw(int X, int Y)
{
	if (style & DISPLAY) {
		((Xd6HtmlDisplay*)this)->draw(X, Y);
		return;
	}
	X += left;
	Y += top + height - descent;
	
	set_font();
	set_color();
	if (style & UNDERLINE) {
		fl_line(X, Y + 1, X + width, Y + 1);
	}
	if (style & RTL_DIRECTION) {
		fl_draw(NULL, 0, 0, 0); // font context setting
		rtl_draw(text, len, X, Y, width);
	} else {
		fl_draw(text, len, X, Y);
	}
}

void Xd6HtmlSegment::rtl_draw(const char *text, int len, int X, int Y, int W)
{
	fl_rtl_draw(text, len, X + W, Y);
}

void Xd6HtmlSegment::draw_all(int X, int Y)
{
	if (style & DISPLAY) {
		((Xd6HtmlDisplay*)this)->draw(X, Y);
		return;
	}
	draw_middle(X, Y, text, text + len);
}


void Xd6HtmlSegment::draw_end(int X, int Y, char *chr)
{
	if (style & RTL_DIRECTION) {
		draw_middle(X, Y, text, chr);
	} else {
		draw_middle(X, Y, chr, text + len);
	}
}

void Xd6HtmlSegment::cut_end(char *chr)
{
	int i, ii;
	if (style & RTL_DIRECTION) {
		ii = 0;
		for (i = (chr - text); i < len; i++) {
			text[ii++] = text[i];
		}
		len = ii;
	} else {
		len = chr - text;
	}
}


void Xd6HtmlSegment::draw_begin(int X, int Y, char *chr)
{
	if (style & RTL_DIRECTION) {
		draw_middle(X, Y, chr, text + len);
	} else {
		draw_middle(X, Y, text, chr);
	}
}

void Xd6HtmlSegment::cut_begin(char *chr)
{
	int i, ii;
	if (style & RTL_DIRECTION) {
		len = chr - text;
	} else {
		ii = 0;
		for (i = (chr - text); i < len; i++) {
			text[ii++] = text[i];
		}
		len = ii;
	}
}


void Xd6HtmlSegment::draw_middle(int X, int Y, char *b, char *e)
{
	char *t;

	X += left;
	Y += top + height - descent;

	if (style & DISPLAY) return;
	set_font();

	if (b > e) {
		t = e; e = b; b = t;
	}
	if (style & RTL_DIRECTION) {
		int w, dx;
		w = (int) fl_width(b, e-b);
		dx = (int)fl_width(e, len - (e - text));
		if (len > 0 && text[len - 1] == '\t' && e != b) {
			w = width;
			dx = 0;
		}
		fl_color(137);
		fl_rectf(X + dx, Y - height + descent, w, height);
		fl_color(FL_WHITE);
		rtl_draw(b, e - b, X + dx, Y, w);
	} else {
		int w, dx;
		w = (int) fl_width(b, e-b);
		dx = (int)fl_width(text, b - text);
		if (len  > 0  && text[len - 1] == '\t' && e != b) {
			w = width;
			dx = 0;
		}
		fl_color(137);
		fl_rectf(X + dx, Y - height + descent, w, height);
		fl_color(FL_WHITE);
		fl_draw(b, e - b, X + dx, Y);
	}
}

char *Xd6HtmlSegment::cut_middle(char *b, char *e)
{
	char *t;

	if (b > e) {
		t = e; e = b; b = t;
	}
	t = b;	
	for(; e < text + len;) {
		*(b++) = *(e++);		
	}
	len = b - text;
	return t;
}

void Xd6HtmlSegment::find_pos(int x, int X, char **chr)
{
	int i;
	x += left;
	
	set_font();

	if (style & RTL_DIRECTION && len > 0) {
		int cl, cw;
		for(i = len - 1; i >= 0; i--) {
			cl = 0;
			while (cl < 1 && i >= 0) {
				cl = fl_utflen((unsigned char*)text + i, 
					len - i);
				if (cl < 1) i--;
			}
			if (i >= 0) {
				cw = (int)fl_width(text + i, cl);
			} else {
				cw = 0;
			}
			if (X <= x + cw) {
				*chr = text + i;
				return;
			}
			x += cw;
		}

	} else if (len > 0) {
		for(i = 0; i < len;) {
			int cl, cw;

			cl = fl_utflen((unsigned char*)text + i, len - i);
			if (cl < 1) cl = 1;
			cw = (int)fl_width(text + i, cl);
			
			if (X <= x + cw / 2) {
				*chr = text + i;
				return;
			}
			x += cw;
			i += cl;
		}
	}
	if (style & RTL_DIRECTION) {
		*chr = text;
	} else {
		*chr = text + len;
	}
}

void Xd6HtmlSegment::set_font()
{
	int fnt;
	int size;

	switch(style & FONT_SIZES) {
	case FONT_SIZE_1:
		size = sizes[0]; break;
	case FONT_SIZE_2:
		size = sizes[1]; break;
	case FONT_SIZE_3:
		size = sizes[2]; break;
	case FONT_SIZE_4:
		size = sizes[3]; break;
	case FONT_SIZE_5:
		size = sizes[4]; break;
	case FONT_SIZE_6:
		size = sizes[5]; break;
	case FONT_SIZE_7:
		size = sizes[6]; break;
	default:	
		size = sizes[1];
	} 

	switch(style & FONTS) {
	case FONT_SERIF:
		fnt = 8; break;
	case FONT_SANS_SERIF:
		fnt = 0; break;
	case FONT_MONOSPACE:
		fnt = 4; break;
	default:
		fnt = 8;
	}

	if (style & FONT_BOLD) {
		fnt += 1;
	} 
	if (style & FONT_ITALIC) {
		fnt += 2;
	}

	fl_font(fnt, size);
}


void Xd6HtmlSegment::set_color()
{
	int c = 0;
	switch (style & COLORS) {
	case COLOR_BLACK: 	c = 0; break;
	case COLOR_SILVER: 	c = 49; break;
	case COLOR_GRAY: 	c = 43; break;
	case COLOR_WHITE: 	c = 7; break;
	case COLOR_MAROON: 	c = 72; break;
	case COLOR_RED: 	c = 1; break;
	case COLOR_PURPLE: 	c = 152; break;
	case COLOR_FUCHSIA: 	c = 248; break;
	case COLOR_GREEN: 	c = 60; break;
	case COLOR_LIME: 	c = 2; break;
	case COLOR_OLIVE: 	c = 76; break;
	case COLOR_YELLOW: 	c = 3; break;
	case COLOR_NAVY: 	c = 136; break;
	case COLOR_BLUE: 	c = 216 ; break;
	case COLOR_TEAL: 	c = 140; break;
	case COLOR_AQUA: 	c = 223; break;
	}
	if ((unsigned) fl_color() != (unsigned) c) {
		fl_color(c);
	}
}

/*
 * "$Id: $"
 */

