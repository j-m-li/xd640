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
#ifndef Xd6HtmlLine_h
#define Xd6HtmlLine_h

#include "Xd6HtmlSegment.h"

class Xd6HtmlSegment;

class Xd6HtmlLine {
public:
	int id;

	int top;
	int left;
	short width;
	short height;
	short descent;
	int begin;
	int end;

	Xd6HtmlSegment **segs;
	short nb_segs;

	Xd6HtmlLine(int i);
	~Xd6HtmlLine();

	void sort_segments(void);
	void add_segment(Xd6HtmlSegment *s);
        void find_pos(int x, int X, Xd6HtmlSegment **seg, char **chr);
        void draw(int X, int Y);
        void draw_all(int X, int Y);
        void draw_end(int X, int Y, Xd6HtmlSegment *seg, char *chr);
        void draw_begin(int X, int Y, Xd6HtmlSegment *seg, char *chr);
        void draw_middle(int X, int Y, Xd6HtmlSegment *bs, Xd6HtmlSegment *es,
                        char *b, char *e);
	int get_seg_id(Xd6HtmlSegment *s);

        void cut_end(Xd6HtmlSegment *seg, char *chr);
        void cut_begin(Xd6HtmlSegment *seg, char *chr);
        void cut_middle(Xd6HtmlSegment *bs, Xd6HtmlSegment *es, char *b, char *e);
        void split_segment(Xd6HtmlSegment* sg, char* ch);

};

#endif

/*
 * "$Id: $"
 */
