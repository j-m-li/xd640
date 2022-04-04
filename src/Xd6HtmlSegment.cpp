/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2001  O'ksi'D
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
int Xd6HtmlSegment::bg_color = 0;
int Xd6HtmlSegment::bg_x = 0;
int Xd6HtmlSegment::bg_y = 0;
int Xd6HtmlSegment::bg_w = 0;
int Xd6HtmlSegment::bg_h = 0;


Xd6HtmlSegment::Xd6HtmlSegment(int i, char *txt, int l, Xd6XmlStl *s)
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
	flags = DAMAGE_ALL;
	
	if (!s) s = &Xd6XmlStl::def;
	stl = s;
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
	flags = DAMAGE_ALL;
	stl = &Xd6XmlStl::def;
}

Xd6HtmlSegment::~Xd6HtmlSegment()
{
	if (text) free(text);
	text = NULL;
}

void Xd6HtmlSegment::draw_bg()
{
	fl_color(bg_color);
	fl_rectf(bg_x, bg_y, bg_w, bg_h);
}

void Xd6HtmlSegment::draw_bg(int X, int Y)
{
	X += left;
	fl_color(bg_color);
	fl_rectf(X, Y, width, bg_h - (Y - bg_y));
}

void Xd6HtmlSegment::measure()
{
	if (stl->display) {
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
	int l = len;
	if (stl->display) {
		((Xd6HtmlDisplay*)this)->draw(X, Y);
		return;
	}
	X += left;
	Y += top + height - descent;
	
	set_font();
	set_color();
	if (stl->strike) {
		fl_line(X, Y - height / 3, X + width, Y - height / 3);
	}
	if (stl->underline) {
		fl_line(X, Y + 1, X + width, Y + 1);
	}
	if (stl->double_under) {
		fl_line(X, Y + 2, X + width, Y + 2);
		fl_line(X, Y, X + width, Y);
	}
	if (l > 0 && text[l-1] == '\t') {
		l--;
	}
	if (stl->rtl_direction) {
		fl_draw(NULL, 0, 0, 0); // font context setting
		rtl_draw(text, len, X, Y, width);
	} else {
		fl_draw(text, l, X, Y);
	}
	if (stl->bad_spell) {
		fl_color(FL_RED);
		fl_line(X, Y + 1, X + width, Y + 1);
		fl_line(X, Y + 2, X + width, Y + 2);
	}
}

void Xd6HtmlSegment::rtl_draw(const char *text, int len, int X, int Y, int W)
{
	fl_rtl_draw(text, len, X + W, Y);
}

void Xd6HtmlSegment::draw_all(int X, int Y)
{
	if (stl->display) {
		flags |= DAMAGE_ALL;
		((Xd6HtmlDisplay*)this)->draw_selected(X, Y);
		return;
	}
	draw_middle(X, Y, text, text + len);
}


void Xd6HtmlSegment::draw_end(int X, int Y, char *chr)
{
	if (stl->rtl_direction) {
		draw_middle(X, Y, text, chr);
	} else {
		draw_middle(X, Y, chr, text + len);
	}
}

void Xd6HtmlSegment::cut_end(char *chr)
{
	int i, ii;
	if (stl->rtl_direction) {
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
	if (stl->rtl_direction) {
		draw_middle(X, Y, chr, text + len);
	} else {
		draw_middle(X, Y, text, chr);
	}
}

void Xd6HtmlSegment::cut_begin(char *chr)
{
	int i, ii;
	if (stl->rtl_direction) {
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

	if (stl->display) return;
	set_font();

	if (b > e) {
		t = e; e = b; b = t;
	}
	if (stl->rtl_direction) {
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

	if (stl->rtl_direction && len > 0) {
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
	if (stl->rtl_direction) {
		*chr = text;
	} else {
		*chr = text + len;
	}
}

void Xd6HtmlSegment::set_font()
{
	int fnt;
	int size;

	switch(stl->font_size) {
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
	case 0:
		size = 0;
	default:	
		size = sizes[1];
	} 

	fnt = stl->font;

	if (stl->font_bold) {
		fnt += 1;
	} 
	if (stl->font_italic) {
		fnt += 2;
	}

	fl_font(fnt, size);
}


void Xd6HtmlSegment::set_color()
{
	if ((unsigned) fl_color() != (unsigned) stl->fg_color) {
		fl_color(stl->fg_color);
	}
}

/*
 * "$Id: $"
 */

