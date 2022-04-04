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

#include "Xd6HtmlTagForm.h"
#include <FL/fl_draw.h>
#include <string.h>
#include <stdlib.h>

Xd6HtmlTagForm *Xd6HtmlTagForm::current = NULL;

Xd6HtmlTagForm::Xd6HtmlTagForm(int i, Xd6XmlTreeElement *e, Xd6HtmlDisplay *u) : 
	Xd6HtmlDisplay(i, NULL, 0, e->style)
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

