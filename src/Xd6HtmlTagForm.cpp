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


#include "Xd6HtmlTagForm.h"
#include <FL/fl_draw.h>
#include <string.h>
#include <stdlib.h>

Xd6HtmlTagForm *Xd6HtmlTagForm::current = NULL;

Xd6HtmlTagForm::Xd6HtmlTagForm(int i, Xd6XmlTreeElement *e, Xd6HtmlFrame *u) : 
	Xd6HtmlDisplay(i, NULL, 0, e->stl)
{
	parent = u;
	current = this;
	name = NULL;
	action = NULL;
	method = NULL;
	enctype = NULL;
	accept_charset = NULL;
	accept = NULL;
	
	if (e->get_attr_value("name")) {
		name = strdup(e->get_attr_value("name"));
	}
	if (e->get_attr_value("action")) {
		action = strdup(e->get_attr_value("action"));
	}
	if (e->get_attr_value("method")) {
		method = strdup(e->get_attr_value("method"));
	}
	if (e->get_attr_value("enctype")) {
		enctype = strdup(e->get_attr_value("enctype"));
	}
	if (e->get_attr_value("accept-charset")) {
		accept_charset = strdup(e->get_attr_value("accept-charset"));
	}
	if (e->get_attr_value("accept")) {
		accept = strdup(e->get_attr_value("accept"));
	}

	elem = NULL;
	nb_elem = 0;
}

Xd6HtmlTagForm::~Xd6HtmlTagForm()
{
}

void Xd6HtmlTagForm::reset()
{

}

void Xd6HtmlTagForm::add(Xd6HtmlDisplay *d)
{
	elem = (Xd6HtmlDisplay**) realloc(elem, 
		sizeof(Xd6HtmlDisplay*) * (nb_elem + 1));
	elem[nb_elem] = d;
	nb_elem++;
}

