/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000  O'ksi'D
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

#include "Xd6XmlDtd.h"
#include "Xd6Std.h"
#include "entities.h"
#include <FL/fl_utf8.h>
#include <string.h>

Xd6XmlDtd::Xd6XmlDtd() 
{
}

Xd6XmlDtd::~Xd6XmlDtd()
{
}

static int is_right(const char *a, const entity_struct* b)
{
	return strcmp(a, b->name);
}

const char *Xd6XmlDtd::get_entity_value(const char *name, int len)
{
	static char buf[1024];
	int ucs;
	int l;
	entity_struct *s;

	if (len > 1020) len = 1020;
	snprintf(buf, len + 1, "%s", name);
	
	if (len > 1 && buf[0] == '#') {
		if (buf[1] == 'x' || buf[1] == 'X') {
			ucs = (unsigned int) strtol(buf + 2, NULL, 16);
		} else {
			ucs = (unsigned int) atol(buf + 1);
		}
	} else {

		s = (entity_struct*) bsearch(buf, entities, NB_ENTITIES, 
			sizeof(entity_struct), 
			(int (*)(const void *, const void *))is_right);
		if (!s) return NULL;	
		ucs = s->value;
	}
	l = fl_ucs2utf(ucs, buf);
	buf[l] = '\0';
	return buf;
}

/*
 * "$Id: $"
 */
