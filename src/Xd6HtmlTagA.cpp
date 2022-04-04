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
#include "Xd6HtmlFrame.h"
#include "Xd6HtmlTagA.h"
#include <FL/fl_draw.h>
#include <string.h>

Xd6HtmlTagA::Xd6HtmlTagA(int i, Xd6XmlTreeElement *e, Xd6HtmlDisplay *u) : 
	Xd6HtmlDisplay(i, NULL, 0, e->stl)
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
		stl->para.copy(stl);
		stl->para.a_link = 0;
		stl->para.underline = 0;
		stl->para.fg_color = stl->def.fg_color;
		stl = stl->get_style(&stl->para);
	} else {
		width = 2;
	}
}

void Xd6HtmlTagA::to_html(FILE *fp)
{
}
