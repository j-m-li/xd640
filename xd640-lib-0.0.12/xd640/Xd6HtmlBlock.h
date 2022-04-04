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
#ifndef Xd6HtmlBlock_h
#define Xd6HtmlBlock_h

#include "Xd6HtmlSegment.h"
#include "Xd6HtmlLine.h"

class Xd6HtmlSegment;
class Xd6HtmlLine;

class Xd6HtmlBlock {
public:
	int id;

	int top;
	int left;
	int width;
	int height;
	int style;
	int max_width;
	int min_width;
	int frame_width;

	Xd6HtmlSegment **segs;
	int nb_segs;
	Xd6HtmlLine **lines;
	int nb_lines;

	Xd6HtmlBlock(int i, int mw);
	~Xd6HtmlBlock(void);
	void add_line(void);
	void add_segment(char *txt, int len, int style);
	void measure(void);
	void draw(int X, int Y);
	void create_lines(void);
	void align_lines(void);

	void find_pos(int X, int Y, int x, int y, Xd6HtmlLine **line, 
			Xd6HtmlSegment **seg, char **chr);
	void cut_begin(Xd6HtmlLine *l, Xd6HtmlSegment *s, char *c);
	void cut_end(Xd6HtmlLine *l, Xd6HtmlSegment *s, char *c);
	char *cut_middle(Xd6HtmlLine *l, Xd6HtmlSegment *s, char *c,
			Xd6HtmlLine *l1, Xd6HtmlSegment *s1, char *c1);
	void cut(void);
	void cut_line_begin(Xd6HtmlLine *l, Xd6HtmlSegment *s, char *c);
	void cut_line_end(Xd6HtmlLine *l, Xd6HtmlSegment *s, char *c);
	char *cut_line_middle(Xd6HtmlLine *l, Xd6HtmlSegment *s, char *c,
			Xd6HtmlSegment *s1, char *c1);
	void cut_line(Xd6HtmlLine *l);
	void clean_segs_array(void);
};

#endif

/*
 * "$Id: $"
 */
