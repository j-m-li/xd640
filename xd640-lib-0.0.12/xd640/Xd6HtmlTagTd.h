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
#ifndef Xd6HtmlTagTd_h
#define Xd6HtmlTagTd_h

#include "Xd6HtmlFrame.h"

enum {
	VALIGN_TOP 	= 1,
	VALIGN_MIDDLE,
	VALIGN_BOTTOM,
	VALIGN_BASELINE,
};

class Xd6HtmlTagTd : public Xd6HtmlFrame {
public:
	int rowspan;
	int colspan;
	int valign;
	int cellpadding;
	int border;
	int real_top;
	int real_left;
	int valign_pad;
	char *xnum;
	char *xfmla;

	Xd6HtmlTagTd(int id, Xd6XmlTreeElement *elem, Xd6HtmlDisplay *p);
	~Xd6HtmlTagTd();
	
	
	void draw(int X, int Y);
	void destroy(void);
	void measure(void);
	int handle(int e, int x, int y);
	void resize(int w, int h);
	void break_line(int h, int ph, int fh);
	void print(Xd6HtmlPrint *p, int X, int Y);
	int event_is_inside(int x, int y);
	void to_html(FILE *fp);
	void to_rtf(FILE *fp);
};

#endif

/*
 * "$Id: $"
 */
