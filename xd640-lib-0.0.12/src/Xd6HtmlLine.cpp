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

#include "Xd6HtmlLine.h"
#include <stdio.h>
#include <stdlib.h>

Xd6HtmlLine::Xd6HtmlLine(int i)
{
	id = i;

	top = 0;
	left = 0;
	width = 0;
	height = 0;
	descent = 0;
	segs = NULL;
	nb_segs = 0;
	begin = 0;
	end = -1;
}

Xd6HtmlLine::~Xd6HtmlLine()
{
	free(segs);
}


void Xd6HtmlLine::add_segment(Xd6HtmlSegment *s)
{
	segs = (Xd6HtmlSegment**) realloc(segs, 
			sizeof(Xd6HtmlSegment*) * (nb_segs + 1));
	segs[nb_segs] = s;
	nb_segs++;
	if (s->style & DISPLAY) {
		if (s->height > height) {
			height = s->height;
		}
	} else {
		if (s->height * 1.2 > height) height = (int)(s->height * 1.2);
	}

	if (s->descent > descent) descent = s->descent;
	width += s->width;
}

void Xd6HtmlLine::sort_segments()
{
	int i;
	Xd6HtmlSegment** s;
	int x;

	s = (Xd6HtmlSegment**) malloc(sizeof(Xd6HtmlSegment*) * (nb_segs + 1));
		

	for (i = 0; i < nb_segs; i++) {
		if (!(segs[i]->style & RTL_DIRECTION)) {
			s[i] = segs[i];
		} else {
			int ii, j;
			j = i;
			while (i < nb_segs && 
				segs[i]->style & RTL_DIRECTION) 
			{
				i++;
			}
			i--;
			ii = i;
			while (j <= i) {
				s[j] = segs[ii];
				ii--;
				j++;
			}
		}
	}
	free(segs);
	segs = s;

	x = 0;
	for (i = 0; i < nb_segs; i++) {
		Xd6HtmlSegment *s = segs[i];
		s->top = height - descent + s->descent - s->height;
		s->left = x;
		x += s->width;	
		if (s->style & SUB_TEXT) {
			s->top += (int)(s->height * 0.2);
		} else if (s->style & SUP_TEXT) {
			s->top -= (int)(s->height * 0.2);
		}
	}
	s[nb_segs] = NULL;
}

void Xd6HtmlLine::draw(int X, int Y)
{
	int i;

	X += left;
	Y += top;
	for (i = 0; i < nb_segs; i++) {
		segs[i]->draw(X, Y);
	}
}

void Xd6HtmlLine::draw_all(int X, int Y)
{
	int i;

	X += left;
	Y += top;
	for (i = 0; i < nb_segs; i++) {
		segs[i]->draw_all(X, Y);
	}
}

int Xd6HtmlLine::get_seg_id(Xd6HtmlSegment *s) 
{
	int i;
	for (i = 0; i < nb_segs && segs[i] != s; i++) {};
	if (i == nb_segs) { 
		printf("Panic %d %d\n", nb_segs, s->id);
	}
	return i;
}

void Xd6HtmlLine::draw_end(int X, int Y, Xd6HtmlSegment *seg, char *chr)
{
	int i;

	X += left;
	Y += top;

	for (i = 0; segs[i] != seg; i++) {};
	for (i++; i < nb_segs; i++) {
		segs[i]->draw_all(X, Y);
	}
	seg->draw_end(X, Y, chr);
}

void Xd6HtmlLine::cut_end(Xd6HtmlSegment *seg, char *chr)
{
	int i;
	seg->cut_end(chr);
	for (i = 0; segs[i] != seg; i++) {};
	for (i++; i < nb_segs; i++) {
		delete(segs[i]);
	}
	nb_segs = seg->id + 1;
}

void Xd6HtmlLine::draw_begin(int X, int Y, Xd6HtmlSegment *seg, char *chr)
{
	int i, sid;

	X += left;
	Y += top;
	seg->draw_begin(X, Y, chr);
	for (sid = 0; segs[sid] != seg; sid++) {};
	for (i = 0; i < sid; i++) {
		segs[i]->draw_all(X, Y);
	}
}

void Xd6HtmlLine::cut_begin(Xd6HtmlSegment *seg, char *chr)
{
	int i, ii, sid;
	for (sid = 0; segs[sid] != seg; sid++) {};
	for (i = 0; i < sid; i++) {
		delete(segs[i]);
	}
	seg->cut_begin(chr);
	ii = 0;
	for (i = sid; i < nb_segs; i++) {
		segs[ii] = segs[i];
		segs[ii++]->id = ii;
		
	}
	nb_segs = ii;
}

void Xd6HtmlLine::draw_middle(int X, int Y, Xd6HtmlSegment *bs, Xd6HtmlSegment *es,
		 char *b, char *e)
{
	int begin = 0;
	int end = 0;
	int i, bid, eid;

	X += left;
	Y += top;

	for (bid = 0; segs[bid] != bs; bid++) {};
	for (eid = 0; segs[eid] != es; eid++) {};

	if (bid < eid) {
		begin = bid;
		end = eid;
		bs->draw_end(X, Y, b);
		es->draw_begin(X, Y, e);
	} else if (bid > eid) {
		begin = eid;
		end = bid;
		es->draw_end(X, Y, e);
		bs->draw_begin(X, Y, b);
	} else {
		bs->draw_middle(X, Y, b, e);
	}

	for (i = begin + 1; i < end; i++) {
		segs[i]->draw_all(X, Y);
	}
}

void Xd6HtmlLine::cut_middle(Xd6HtmlSegment *bs, Xd6HtmlSegment *es, char *b, char *e)
{
	int begin = 0;
	int end = 0;
	int i, ii, bid, eid;

	for (bid = 0; segs[bid] != bs; bid++) {};
	for (eid = 0; segs[eid] != es; eid++) {};

	if (bid < eid) {
		begin = bid;
		end = eid;
		bs->cut_end(b);
		es->cut_begin(e);
	} else if (bid > eid) {
		begin = eid;
		end = bid;
		es->cut_end(e);
		bs->cut_begin(b);
	} else {
		bs->cut_middle(b, e);
	}

	if (begin < end) {
		ii = end;
		for (i = begin + 1; i < end; i++) {
			delete(segs[i]);
		}
		ii = end;
		for (i = begin + 1;ii < nb_segs; i++) {
			segs[i] = segs[ii++];
			segs[i]->id = i;
		}
		nb_segs = i;
	}
}

void Xd6HtmlLine::find_pos(int x, int X, Xd6HtmlSegment **seg, char **chr)
{
	int i;

	x += left;
	for (i = 0; i < nb_segs; i++) {
		if (X <= segs[i]->left + x + segs[i]->width) {
			segs[i]->find_pos(x, X, chr);
			*seg = segs[i];
			return;
		}
	}
	if (nb_segs) {
		segs[nb_segs - 1]->find_pos(x, X, chr);
		*seg = segs[nb_segs - 1];
	}
}

void Xd6HtmlLine::split_segment(Xd6HtmlSegment* sg, char* ch) 
{
	int i;
	int l;
	char *buf;
	char *ptr;
	Xd6HtmlSegment *seg;

	l = sg->len - (ch - sg->text);
	if (l < 1) l = 1;
	buf = (char*) malloc(l);
	ptr = ch;
	for(i = 0;ptr < sg->text + sg->len; ptr++) {
		buf[i++] = *ptr;
	}	
	sg->len = ch - sg->text;
	seg = new Xd6HtmlSegment(sg->id + 1, buf, i, sg->style);

	segs = (Xd6HtmlSegment**) realloc(segs,
		sizeof(Xd6HtmlSegment*) * (nb_segs + 1));
	for (i = nb_segs; i > seg->id; i--) {
		segs[i] = segs[i - 1];
		segs[i]->id = i;
	}
	segs[seg->id] = seg;
	nb_segs++;
}

/*
 * "$Id: $"
 */

