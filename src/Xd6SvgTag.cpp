/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2002  O'ksi'D
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
#include <FL/fl_draw.h>
#include "Xd6SvgTag.h"
#include <stdio.h>
#include <stdlib.h>
#include "Xd6Base64.h"
#include <ctype.h>

Xd6SvgTag::Xd6SvgTag(int i, Xd6XmlTreeElement *e, Xd6HtmlFrame *u) : 
	Xd6HtmlDisplay(i, NULL , 0, e ? e->stl : NULL)
{
	const char *ptr;

	parent = u;
	text = (char *)malloc(1);
	attr_w = attr_h =  0;
	view_x = view_y = view_w = view_h = 0.0;
	if (e) {
		ptr = e->get_attr_value("width");
		if (ptr) attr_w = (int) atol(ptr);
		ptr = e->get_attr_value("height");
		if (ptr) attr_h = (int) atol(ptr);
		ptr = e->get_attr_value("viewBox");
		if (ptr) {
			view_x =  atof(ptr);
			while (*ptr && !isspace(*ptr)) ptr++;
			while (*ptr && isspace(*ptr)) ptr++;
			view_y =  atof(ptr);
			while (*ptr && !isspace(*ptr)) ptr++;
			while (*ptr && isspace(*ptr)) ptr++;
			view_w =  atof(ptr);
			while (*ptr && !isspace(*ptr)) ptr++;
			while (*ptr && isspace(*ptr)) ptr++;
			view_h =  atof(ptr);
		}
		e->nb_children = 0;
		e->children = NULL;
	}
}

Xd6SvgTag::~Xd6SvgTag()
{
	destroy();
}

void Xd6SvgTag::destroy()
{
        while (nb_children > 0) {
                nb_children--;
                if (!children[nb_children]) continue;
                if (children[nb_children]->type == 
                        Xd6XmlTreeSegment_element)  
                {
                        delete((Xd6XmlTreeElement*)
                                children[nb_children]);
                } else if (children[nb_children]->type &
                        Xd6XmlTreeSegment_text)
                {
                        delete((Xd6XmlTreeText*)children[nb_children]);
                } else {
                        delete(children[nb_children]);
                }
        }
	free(children);
	children = NULL;
}

void Xd6SvgTag::measure() 
{
	width = attr_w; 
	height = attr_h;
	descent = fl_descent(); 
}

void Xd6SvgTag::draw_child(Xd6XmlTreeElement *e)
{
	if (e->display == DISPLAY_SVG_DEFS) {
		return;
	}
	// DISPLAY_SVG_DESC

	fl_push_matrix();

	if (e->display == DISPLAY_SVG_G) {

	}
	for (int i = 0; i < nb_children; i++) {
		if (children[i]->type == Xd6XmlTreeSegment_element) {
			draw_child((Xd6XmlTreeElement*)children[i]);
		}
	}
	if (e->display == DISPLAY_SVG_PATH) {

	} else if (e->display == DISPLAY_SVG_IMAGE) {
	} else if (e->display == DISPLAY_SVG_TEXT) {
	}

	fl_pop_matrix();
}

void Xd6SvgTag::draw(int X, int Y)
{
	X += left;
	Y += top;
	fl_clip(X, Y, width, height);
	
	fl_push_matrix();
	fl_translate(view_x + X, view_y + Y);
	fl_scale(view_w, view_h);

	for (int i = 0; i < nb_children; i++) {
		if (children[i]->type == Xd6XmlTreeSegment_element) {
			draw_child((Xd6XmlTreeElement*)children[i]);
		}
	}

	fl_pop_matrix();
	fl_pop_clip();
}


#ifdef WIN32
void Xd6SvgTag::print(Xd6HtmlPrint *p, int X, int Y)
{
}
#else
void Xd6SvgTag::print(Xd6HtmlPrint *p, int X, int Y)
{
	
}
#endif

void Xd6SvgTag::to_html(FILE *fp)
{
	fprintf(fp, "<s:svg  width=\"%d\" height=\"%d\" \n/>",
		 attr_w, attr_h);
}



/*
 * "$Id: $"
 */
