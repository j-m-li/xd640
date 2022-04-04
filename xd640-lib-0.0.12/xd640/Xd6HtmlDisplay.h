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
#ifndef Xd6HtmlDisplay_h
#define Xd6HtmlDisplay_h

#include "Xd6HtmlSegment.h"
#include <stdio.h>

#define FL_HIDE_CURSOR 128

class Xd6HtmlPrint;
class Xd6HtmlFrame;

class Xd6HtmlDisplay : public Xd6HtmlSegment {
public:
	int display;
	Xd6HtmlDisplay *parent;
	
	Xd6HtmlDisplay(int i, char *txt, int l, int style);
	Xd6HtmlDisplay();
	virtual ~Xd6HtmlDisplay();
	virtual void measure(void);
	virtual void draw(int X, int Y);
	virtual void print(Xd6HtmlPrint *p, int X, int Y);
	virtual void to_html(FILE *fp);
	virtual void to_rtf(FILE *fp);
	virtual int handle(int e, int x, int y);
	virtual void destroy(void);
	virtual void break_line(int h, int ph, int fh);
	virtual void load(const char *url, const char *file);
	int event_is_inside(int x, int y);
	static Xd6HtmlDisplay *create(int id, Xd6XmlTreeElement *elem,Xd6HtmlDisplay *u);
	static void Xd6HtmlDisplay::pre_process(Xd6XmlTreeElement *e, Xd6HtmlFrame *u);
	
	void redraw(void);
	int get_length(const char *str, int ref);
};

#endif

/*
 * "$Id: $"
 */
