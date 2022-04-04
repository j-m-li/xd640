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

#include "Xd6Std.h"
#include "Xd6HtmlBlock.h"
#include "Xd6HtmlFrame.h"
#include "Xd6HtmlDisplay.h"
#include <FL/fl_draw.h>
#include <stdio.h>
#include <stdlib.h>


Xd6HtmlBlock::Xd6HtmlBlock(int i, int mw)
{
	id = i;

	top = 0;
	left = 0;
	width = 0;
	height = 0;
	style = 0;
	max_width = 0;
	min_width = 0;
	frame_width = mw;

	segs = NULL;
	nb_segs = 0;
	lines = NULL;
	nb_lines = 0;
}

Xd6HtmlBlock::~Xd6HtmlBlock()
{
	
	while (nb_segs > 0) {
		Xd6HtmlSegment *s = segs[--nb_segs];
		if (s->style & DISPLAY) delete((Xd6HtmlDisplay*) s);
		else delete(s);
	}
	free(segs);	

	while (nb_lines > 0) {
		delete(lines[--nb_lines]);
	}
	free(lines);	
}

void Xd6HtmlBlock::add_line() 
{
	lines = (Xd6HtmlLine**) realloc(lines, sizeof(Xd6HtmlLine*) * (nb_lines + 1));
	lines[nb_lines] = new Xd6HtmlLine(nb_lines);
	nb_lines++;
}

void Xd6HtmlBlock::add_segment(char *txt, int len, int style) 
{
	segs = (Xd6HtmlSegment**) realloc(segs, 
			sizeof(Xd6HtmlSegment*) * (nb_segs + 1));
	segs[nb_segs] = new Xd6HtmlSegment(nb_segs, txt, len, style);
	nb_segs++;
}

void Xd6HtmlBlock::measure() 
{
	int i;
	
	max_width = 0;
	min_width = 0;
	for (i = 0; i < nb_segs; i++) {
		int w;
		segs[i]->measure();
		w = segs[i]->width;
		if (w > min_width) min_width = w;
		max_width += w;
	}
}

void Xd6HtmlBlock::create_lines()
{
	int i;
	Xd6HtmlLine *l;
	int fw;

	while (nb_lines > 0) {
		delete(lines[--nb_lines]);
	}
	free(lines);
	lines = NULL;
	width = 0;
	height = 0;

	fw = frame_width - ((style & BLOCKQUOTES) / BLOCKQUOTE1 * 40);
	add_line();
	l = lines[nb_lines - 1];
	l->begin = 0;
	for (i = 0; i < nb_segs; i++) {
		Xd6HtmlSegment *s = segs[i];
		if ((s->style & DISPLAY) && 
			(((Xd6HtmlDisplay*)s)->display == DISPLAY_TAB)) 
		{
			s->set_font();
			if (s->style & PREFORMATED) {
				int w;
				w = (int)fl_width(" ", 1);
				s->width = (8 - ((l->width / w) % 8)) * w;
			} else {
				s->width = Xd6HtmlFrame::get_tab(
					l->width, style);
			}
			if (s->len == 0) {
				s->width = 0;
				((Xd6HtmlDisplay*)s)->display = DISPLAY_NONE;
			}
		}
		if (s->width + l->width > fw && 
			l->nb_segs > 0 && !(s->style & PREFORMATED)) 
		{
			add_line();
			if (l->width > width) width = l->width;
			l->end = i - 1;
			l = lines[nb_lines - 1];
			l->begin = i;
		}
		l->add_segment(s);
	}
	if (l->width > width) width = l->width;
	l->end = i - 1;
	width += ((style & BLOCKQUOTES) / BLOCKQUOTE1 * 40);
}

void Xd6HtmlBlock::align_lines()
{
	int i, ii, j;
	
	for (i = 0; i < nb_lines; i++) {
		if (style & TEXT_ALIGN_RIGHT) {
			lines[i]->left = width - lines[i]->width;
		} else if (style & TEXT_ALIGN_CENTER) {
			lines[i]->left = (width - lines[i]->width) / 2;
		}

		// reorder segments to be sorted !!
		j = 0;
		for (ii = lines[i]->begin; ii <= lines[i]->end; ii++) {
			lines[i]->segs[j++] = segs[ii];
		}
		lines[i]->sort_segments();
	}
}

void Xd6HtmlBlock::draw(int X, int Y)
{
	int i;
	X += left;
	Y += top;	

	for (i = 0; i < nb_lines; i++) {
		lines[i]->draw(X, Y);
	}
}

void Xd6HtmlBlock::cut_line(Xd6HtmlLine *l) 
{
	int i;
	for (i = 0; i < l->nb_segs; i++) {
		int id;
		id = l->segs[i]->id;
		delete(segs[id]);
		segs[id] = NULL;
	}
}

void Xd6HtmlBlock::cut_line_begin(Xd6HtmlLine *l, Xd6HtmlSegment *s, char *c)
{
        int i;
        for (i = 0; s != l->segs[i]; i++) {
                int id;
                id = l->segs[i]->id;
                delete(segs[id]);
                segs[id] = NULL;
        }
	s->cut_begin(c);
}

void Xd6HtmlBlock::cut_line_end(Xd6HtmlLine *l, Xd6HtmlSegment *s, char *c)
{
        int i;
        for (i = l->get_seg_id(s) + 1; i < l->nb_segs; i++) {
                int id;
                id = l->segs[i]->id;
                delete(segs[id]);
                segs[id] = NULL;
        }
        s->cut_end(c);

}

char *Xd6HtmlBlock::cut_line_middle(Xd6HtmlLine *l, Xd6HtmlSegment *s2, char *c2,
	Xd6HtmlSegment *s1, char *c1)
{
	int s1id, s2id;
	int i = 0;
        int m = 0;
	char *cursor;

	s1id = l->get_seg_id(s1);
	s2id = l->get_seg_id(s2);

        if (s1id > s2id) {
       		i = s2id;
		m = s1id;
		cursor = c2;		
		s2->cut_end(c2);
		s1->cut_begin(c1);
        } else if (s1id < s2id) {
                i = s1id;
                m = s2id;
		cursor = c1;		
		s2->cut_begin(c2);
		s1->cut_end(c1);
        } else {
		cursor = s1->cut_middle(c1, c2);
        }
	
	

        for (i++; i < m; i++) {
                int id;
                id = l->segs[i]->id;
                delete(segs[id]);
                segs[id] = NULL;
        }
	return cursor;
}


void Xd6HtmlBlock::clean_segs_array()
{
	int i;
	int nb;
	int o = 1;

	nb = nb_segs;
	for (i = 0; i < nb_segs; i++) {
		if (segs[i] == NULL && i + o < nb_segs) {
			segs[i] = segs[i + o];
			segs[i + o] = NULL;
			if (segs[i]) {
				segs[i]->id = i;
			} else {
				o++;
			}
			i--; nb--;
		}
	}
	for (nb = 0; nb < nb_segs; nb++) {
		if (segs[nb] == NULL) break;
	}
	nb_segs = nb;
}

void Xd6HtmlBlock::cut_begin(Xd6HtmlLine *l, Xd6HtmlSegment *s, char *c)
{
	int i;
	
	for (i = 0; i < l->id; i++) {
		cut_line(lines[i]);
	}
	cut_line_begin(l, s, c);
	clean_segs_array();
}

void Xd6HtmlBlock::cut_end(Xd6HtmlLine *l, Xd6HtmlSegment *s, char *c)
{
	int i;
	
	for (i = l->id + 1; i < nb_lines; i++) {
		cut_line(lines[i]);
	}
	cut_line_end(l, s, c);
	clean_segs_array();
}

char *Xd6HtmlBlock::cut_middle( 
	Xd6HtmlLine *sel_line, Xd6HtmlSegment *sel_seg, char *sel_chr,
	Xd6HtmlLine *cur_line, Xd6HtmlSegment *cur_seg, char *cur_chr)
{
	int i = 0;
        int m = 0;
	char *cursor;

        if (sel_line->id > cur_line->id) {
       		i = cur_line->id;
		m = sel_line->id;
		cursor = cur_chr;
                cut_line_end(cur_line, cur_seg, cur_chr);
        } else if (sel_line->id < cur_line->id) {
                i = sel_line->id;
                m = cur_line->id;
		cursor = sel_chr;
                cut_line_end(sel_line, sel_seg, sel_chr);
        } else {
                cursor = cut_line_middle(sel_line, sel_seg, 
				sel_chr, cur_seg, cur_chr);
        }
        for (i++; i < m; i++) {
             	cut_line(lines[i]);
        }

        if (sel_line->id > cur_line->id) {
                cut_line_begin(sel_line, sel_seg, sel_chr);
        } else if (sel_line->id < cur_line->id) {
                cut_line_begin(cur_line, cur_seg, cur_chr);
        }
	clean_segs_array(); 
	return cursor;
}

void Xd6HtmlBlock::find_pos(int X, int Y, int x, int y, 
	Xd6HtmlLine **line, Xd6HtmlSegment **seg, char **chr)
{
        int i;

        x += left;
        y += top;

        for (i = 0; i < nb_lines; i++) {
                if (Y <= lines[i]->top + y + lines[i]->height) {
                        lines[i]->find_pos(x, X, seg, chr);
                        *line = lines[i];
                        return;
                }
        }
	if (nb_lines) {
        	lines[nb_lines - 1]->find_pos(x, X, seg, chr);
        	*line = lines[nb_lines - 1];
	}
}


/*
 * "$Id: $"
 */

