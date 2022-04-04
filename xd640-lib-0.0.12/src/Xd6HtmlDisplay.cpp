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
#include "Xd6HtmlDisplay.h"
#include "Xd6HtmlFrame.h"
#include "Xd6HtmlPrint.h"
#include "Xd6HtmlTagImg.h"
#include "Xd6HtmlTagLi.h"
#include "Xd6HtmlTagA.h"
#include "Xd6HtmlTagTable.h"
#include "Xd6HtmlTagForm.h"
#include "Xd6HtmlTagButton.h"
#include "Xd6HtmlTagInputText.h"

Xd6HtmlDisplay::Xd6HtmlDisplay(int i, char *txt, int l, int s) : 
	Xd6HtmlSegment(i, txt, l, s)
{
	display = DISPLAY_NONE;
	parent = NULL;
}

Xd6HtmlDisplay::Xd6HtmlDisplay() : Xd6HtmlSegment()
{
	display = DISPLAY_NONE;
	parent = NULL;
}

Xd6HtmlDisplay::~Xd6HtmlDisplay()
{
}

void Xd6HtmlDisplay::destroy()
{

}

void Xd6HtmlDisplay::measure(void) 
{
	int s = style;
	style &= ~DISPLAY;
	Xd6HtmlSegment::measure();
	style = s;
}

void Xd6HtmlDisplay::draw(int X, int Y)
{
	int s = style;
	style &= ~DISPLAY;
	Xd6HtmlSegment::draw(X, Y);
	style = s;
}

void Xd6HtmlDisplay::print(Xd6HtmlPrint *p, int X, int Y)
{

}

void Xd6HtmlDisplay::to_rtf(FILE *fp)
{

}

void Xd6HtmlDisplay::to_html(FILE *fp)
{

}

Xd6HtmlDisplay *Xd6HtmlDisplay::create(int id, Xd6XmlTreeElement *elem, Xd6HtmlDisplay *u)
{
	Xd6HtmlDisplay *s;

	switch(elem->display) {
	case DISPLAY_IMG:
		s = new Xd6HtmlTagImg(id, elem, u);
		break;
	case DISPLAY_LIST_ITEM:
		s = new Xd6HtmlTagLi(id, elem);
		break;
	case DISPLAY_A_LINK:
		s = new Xd6HtmlTagA(id, elem, u);
		break;
	case DISPLAY_TABLE:
		s = new Xd6HtmlTagTable(id, elem, u);
		break;
	case DISPLAY_FORM:
		s = new Xd6HtmlTagForm(id, elem, u);
		break;
	case DISPLAY_BUTTON:
		s = new Xd6HtmlTagButton(id, elem, u);
		break;
	case DISPLAY_INPUT_TEXT:
		s = new Xd6HtmlTagInputText(id, elem, u);
		break;
	default:
		s = new Xd6HtmlDisplay(id, (char *)malloc(2), 1, elem->style);
		s->text[0] = '\t';
	}
	if (s->text == NULL) {
		s->text = (char*) malloc(2);
		s->text[0] = ' ';
		s->len = 1;
	}
	if (!s->parent) s->parent = u;
	s->display = elem->display;
	return s;
}

void Xd6HtmlDisplay::pre_process(Xd6XmlTreeElement *elem, Xd6HtmlFrame *u)
{

}

int Xd6HtmlDisplay::handle(int e, int x, int y)
{
	return 0;
}

int Xd6HtmlDisplay::get_length(const char *str, int ref)
{
	int val;

	if (!str) return 0;

	val = (int) atof(str);
	
 	if (strstr(str, "%"))  {
		val = val * ref / 100;
	}
	return val;
}

void Xd6HtmlDisplay::break_line(int h, int ph, int fh)
{

}

int Xd6HtmlDisplay::event_is_inside(int x, int y)
{
	x += left;
	y += top;

	if (Fl::event_x() >= x && Fl::event_y() >= y &&
		Fl::event_x() <= x + width &&
		 Fl::event_y() <= y + height)
	{
		return 1;
	}
	return 0;

}

void Xd6HtmlDisplay::load(const char *url, const char *file)
{

}

void Xd6HtmlDisplay::redraw()
{
	if (parent) parent->redraw();
	switch(display) {
	case DISPLAY_TOP_FRAME:
		((Xd6HtmlFrame*)this)->damage(FL_DAMAGE_ALL);
		break;
	}
}

