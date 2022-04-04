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
#ifndef Xd6HtmlSegment_h
#define Xd6HtmlSegment_h

#include "Xd6XmlStyle.h"

class Xd6HtmlSegment {
public:
	unsigned short id;

	int top;
	int left;
	unsigned short descent;
	int width;
	int height;
	int style;

	char *text;
	unsigned short len;
	
	static int *sizes;
	static int sizes1[];
	static int sizes2[];
	static int sizes3[];
	static int sizes4[];

	Xd6HtmlSegment(int i, char *txt, int l, int style);
	Xd6HtmlSegment();
	~Xd6HtmlSegment();

	void measure(void);
	void position(int X, int Y); // relative to Xd6HtmlLine pos.
	void find_pos(int x, int X, char **chr);
	void rtl_draw(const char *text, int len, int X, int Y, int W);
	void draw(int X, int Y);
	void draw_all(int X, int Y);
	void draw_end(int X, int Y, char *chr);
	void draw_begin(int X, int Y, char *chr);
	void draw_middle(int X, int Y, char *b, char *e);
	void cut_end(char *chr);
	void cut_begin(char *chr);
	char *cut_middle(char *b, char *e);
	
	void set_font(void);
	void set_color(void);
};

#endif

/*
 * "$Id: $"
 */
