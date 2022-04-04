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

#include "Xd6HtmlTagLi.h"
#include "Xd6Std.h"
#include <FL/fl_draw.h>

Xd6HtmlTagLi::Xd6HtmlTagLi(int i, Xd6XmlTreeElement *e) : Xd6HtmlDisplay(i, NULL, 0, 
	e->style)
{
	num = 0;
	el = e;
	
	if (e->parent) {
		while (e->parent && e->display == DISPLAY_LIST_ITEM) {
			e = e->parent;
		}
		while (e->parent && !(e->style & LIST)) {
			e = e->parent;
		}
		type = e->style;
		scan(e);
	}
}

Xd6HtmlTagLi::~Xd6HtmlTagLi()
{
}

void Xd6HtmlTagLi::destroy()
{
}

int Xd6HtmlTagLi::scan(Xd6XmlTreeElement *e) 
{
	int i;
	for (i = 0; i < e->nb_children; i++) {
		if (e->children[i] && e->children[i]->type == 
			Xd6XmlTreeSegment_element) 
		{
			Xd6XmlTreeElement *ee;
			ee = (Xd6XmlTreeElement*)e->children[i];
			if (ee->display == DISPLAY_LIST_ITEM) {
				num++;
			}
			if (!(ee->style & LIST)) {
				if (scan(ee)) return 1;
			}
			if (ee == el) return 1;
		}
	}
	return 0;
};
void Xd6HtmlTagLi::draw(int X, int Y)
{
	char buf[100];
	int s = style;
	style &= ~DISPLAY;
	Xd6HtmlSegment::draw(X, Y);
	style = s;
	
	X += left;
	Y += top + height - descent;
	
	
	snprintf(buf, 100, "%d.", num);
	if (style & TEXT_ALIGN_RIGHT && style & RTL_DIRECTION) {
		X += width + (int)fl_width(buf) + 5;	
	} else {
		X -= (int)fl_width(buf) + 5;	
	}
	if ((type & LIST) == LIST_NUMBER) {
		fl_draw(buf, X, Y);
	} else if ((type & LIST) == LIST_DISC) {
		fl_circle(X + 4, Y - 4, 2);
	}
}


