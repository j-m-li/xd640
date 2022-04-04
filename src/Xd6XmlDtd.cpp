/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000  O'ksi'D
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
	for (int i = 0; i < len; i++) buf[i] = name[i];
	buf[len] = 0;

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
