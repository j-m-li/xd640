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
#ifndef Xd6HtmlTagLi_h
#define Xd6HtmlTagLi_h

#include "Xd6HtmlDisplay.h"

class Xd6HtmlTagLi : public Xd6HtmlDisplay {
public:
	int num;
	Xd6XmlTreeElement *el;
	int type;

	Xd6HtmlTagLi(int id, Xd6XmlTreeElement *elem);
	~Xd6HtmlTagLi();

	void draw(int X, int Y);
	int scan(Xd6XmlTreeElement *e);
	void destroy(void);
};

#endif

/*
 * "$Id: $"
 */
