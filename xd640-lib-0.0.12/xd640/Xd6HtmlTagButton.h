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
#ifndef Xd6HtmlTagButton_h
#define Xd6HtmlTagButton_h

#include "Xd6HtmlDisplay.h"
#include "Xd6HtmlTagForm.h"

class Xd6HtmlTagButton : public Xd6HtmlDisplay {
public:
	char *name;
	char *value;
	char *type;
	Xd6HtmlTagForm *form;
	Xd6HtmlFrame *frame;
	int pushed;

	Xd6HtmlTagButton(int id, Xd6XmlTreeElement *elem, Xd6HtmlDisplay *u);
	void destroy(void);
	void draw(int x, int y);
	void measure(void);
	int handle(int e, int x, int y);
};

#endif

/*
 * "$Id: $"
 */
