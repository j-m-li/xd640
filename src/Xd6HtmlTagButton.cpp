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


#include "Xd6HtmlTagButton.h"
#include "Xd6HtmlTagForm.h"
#include "Xd6HtmlDownload.h"
#include "Xd6HtmlFrame.h"
#include <FL/fl_draw.h>
#include <string.h>
#include <stdlib.h>


Xd6HtmlTagButton::Xd6HtmlTagButton(int i, Xd6XmlTreeElement *e, Xd6HtmlDisplay *u) : 
	Xd6HtmlDisplay(i, NULL, 0, e->stl)
{
	name = NULL;
	value = NULL;
	type = NULL;
	form = Xd6HtmlTagForm::current;
	while (u->display != DISPLAY_TOP_FRAME &&
		u->display != DISPLAY_FRAME &&
		u->display != DISPLAY_IFRAME)
	{
		u = u->parent;
	}
	frame = (Xd6HtmlFrame*)u;
	pushed = 0;

	if (e->get_attr_value("name")) {
		name = strdup(e->get_attr_value("name"));
	}
	if (e->get_attr_value("value")) {
		value = strdup(e->get_attr_value("value"));
	}
	if (e->get_attr_value("type")) {
		type = strdup(e->get_attr_value("type"));
	}
	
	if (!value) {
		value = strdup("button");
	}
	
}

void Xd6HtmlTagButton::destroy()
{
	if (name) free(name);
	if (value) free(value);
	if (type) free(type);
}

void Xd6HtmlTagButton::draw(int x, int y)
{
	x += left;
	y += top;
	fl_font(0, 14);
	fl_color(FL_GRAY);
	fl_rectf(x, y, width, height);

	if (!pushed) {
		fl_color(FL_WHITE);
		fl_line(x, y, x + width - 1, y);
		fl_line(x, y, x, y + height - 1);
		fl_color(FL_BLACK);
		fl_line(x + width - 1, y, x + width - 1, y + height - 1);
		fl_line(x, y + height - 1,  x + width - 1, y + height - 1);
	} else {
		fl_color(FL_WHITE);
		fl_line(x + width - 1, y, x + width - 1, y + height - 1);
		fl_line(x, y + height - 1,  x + width - 1, y + height - 1);
		fl_color(FL_BLACK);
		fl_line(x, y, x + width - 1, y);
		fl_line(x, y, x, y + height - 1);
	}

	fl_color(FL_BLACK);
	fl_draw(value, x + 10, y + 18);
	
}

int Xd6HtmlTagButton::handle(int e, int x, int y)
{

	if (e == FL_RELEASE) {
		if (pushed && event_is_inside(x, y)) {
			if (type && !strcasecmp("button", type)) {

			} else if (type && !strcasecmp("reset", type)) {
				form->reset();
			} else {
				downloader->request(form->action, "", 
					form, frame);
			}
		}
		pushed = 0;
		return 1;
	}
	if (!event_is_inside(x, y)) return 0;
	
	if (e == FL_PUSH) {
		pushed = 1;
		return 1;
	}
	return 0;

}

void Xd6HtmlTagButton::measure()
{
	
	fl_font(0, 14);

	height = 25;
	width = (int) fl_width(value) + 20;
	descent = fl_descent();
}

