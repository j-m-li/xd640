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
#ifndef Xd6HtmlTagInputText_h
#define Xd6HtmlTagInputText_h

#include "Xd6HtmlDisplay.h"
#include "Xd6HtmlTagForm.h"

class Xd6HtmlTagInputText : public Xd6HtmlDisplay {
public:
	char *name;
	char *value;
	int size;
	int maxlength;
	Xd6HtmlTagForm *form;
	char *text;
	char *sel_chr;
	char *cur_chr;
	int focus;
	int rtl;
	int off;

	Xd6HtmlTagInputText(int id, Xd6XmlTreeElement *elem, Xd6HtmlDisplay *u);
	void destroy(void);
	void draw(int x, int y);
	void measure(void);
	int handle(int e, int x, int y);
	char *get_text(void);
	void cut(void);
	void move_cursor(int dir);
	void insert_text(char *txt, int len);
	
	char *find_pos(int ex, int x);
};

#endif

/*
 * "$Id: $"
 */
