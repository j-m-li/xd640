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
#include "Xd6HtmlFrame.h"
#include "Xd6HtmlTagA.h"
#include <FL/fl_draw.h>
#include <string.h>

Xd6HtmlTagA::Xd6HtmlTagA(int i, Xd6XmlTreeElement *e, Xd6HtmlDisplay *u) : 
	Xd6HtmlDisplay(i, NULL, 0, e->style)
{
	display = DISPLAY_A_LINK;
	name = NULL;
	target = NULL;
	href = NULL;
	if (e->get_attr_value("href")) {
		href = strdup(e->get_attr_value("href"));
	}
	if (e->get_attr_value("target")) {
		target = strdup(e->get_attr_value("target"));
	}
	if (e->get_attr_value("name")) {
		name = strdup(e->get_attr_value("name"));
	}
	text = strdup(" ");
	len = 1;
}

Xd6HtmlTagA::~Xd6HtmlTagA()
{
	free(name);
	free(href);
	free(target);
}

void Xd6HtmlTagA::destroy()
{
}

void Xd6HtmlTagA::draw(int X, int Y)
{
	if (width < 1) return;
	X += left;
	Y += top + height - descent;
	set_color();
	fl_line(X, Y + 1, X + 2, Y + 1);
}

void Xd6HtmlTagA::measure()
{
	set_font();
	descent = fl_descent();
	height = fl_height();
	if (len < 1) {
		width = 0;
		style &= ~(A_LINK|UNDERLINE|COLORS);
	} else {
		width = 2;
	}
}

void Xd6HtmlTagA::to_html(FILE *fp)
{
}
