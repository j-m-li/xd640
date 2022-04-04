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


#include "Xd6Std.h"
#include "Xd6HtmlDisplay.h"
#include "Xd6HtmlFrame.h"
#include "Xd6HtmlView.h"
#include "Xd6HtmlPrint.h"
#include "Xd6HtmlTagImg.h"
#include "Xd6SvgTag.h"
#include "Xd6MathMl.h"
#include "Xd6HtmlTagA.h"
#include "Xd6HtmlTagTable.h"
#include "Xd6HtmlTagForm.h"
#include "Xd6HtmlTagButton.h"
#include "Xd6HtmlTagInputText.h"

Xd6HtmlDisplay::Xd6HtmlDisplay(int i, char *txt, int l, Xd6XmlStl *s) : 
	Xd6HtmlSegment(i, txt, l, s)
{
	display = DISPLAY_NONE;
	parent = NULL;
	parent_widget = NULL;
	damage_ = FL_DAMAGE_ALL;
}

Xd6HtmlDisplay::Xd6HtmlDisplay() : Xd6HtmlSegment()
{
	display = DISPLAY_NONE;
	parent = NULL;
}

Xd6HtmlDisplay::~Xd6HtmlDisplay()
{
}

void Xd6HtmlDisplay::damage(unsigned char d)
{
	if ((flags & ~d) == d) return;
	if (DAMAGE_CHILD == d) {
		flags |= DAMAGE_CHILD;
	} else {
		flags |= DAMAGE_ALL;
	}
	if (parent && parent != this) {
		parent->damage(DAMAGE_CHILD);
		Xd6HtmlBlock **blocks = parent->blocks;
		int nb = parent->nb_blocks;
		while (nb > 0) {
			nb--;
			Xd6HtmlLine **lines = blocks[nb]->lines;
			int nl = blocks[nb]->nb_lines;
			while (nl > 0) {
				nl--;
				Xd6HtmlSegment **segs = lines[nl]->segs;
				int ns = lines[nl]->nb_segs;
				while (ns > 0) {
					ns--;
					if (segs[ns] == this) break;
				}
				if (ns >= 0) lines[nl]->flags |= DAMAGE_CHILD;
			}
			if (nl >= 0) blocks[nb]->flags |= DAMAGE_CHILD;
		}
	}
        if (parent_widget) {
                parent_widget->damage(FL_DAMAGE_CHILD);
        }
}

void Xd6HtmlDisplay::destroy()
{

}

void Xd6HtmlDisplay::measure(void) 
{
	int s = stl->display;
	stl->display = 0;
	Xd6HtmlSegment::measure();
	stl->display = s;
}

void Xd6HtmlDisplay::draw(int X, int Y)
{
	int s = stl->display;
	stl->display = 0;
	Xd6HtmlSegment::draw(X, Y);
	stl->display = s;
}

void Xd6HtmlDisplay::draw_selected(int X, int Y)
{
	draw(X, Y);	
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

Xd6HtmlDisplay *Xd6HtmlDisplay::create(int id, Xd6XmlTreeElement *elem, Xd6HtmlFrame *u)
{
	Xd6HtmlDisplay *s;

	switch(elem->display) {
	case DISPLAY_MATH:
		s = new Xd6MathMl(id, elem, u);
		break;
	case DISPLAY_SVG:
		s = new Xd6SvgTag(id, elem, u);
		break;
	case DISPLAY_IMG:
		s = new Xd6HtmlTagImg(id, elem, u);
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
		s = new Xd6HtmlDisplay(id, (char *)malloc(2), 1, elem->stl);
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

