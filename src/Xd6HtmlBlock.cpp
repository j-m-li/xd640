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
	flags = DAMAGE_ALL;
	max_width = 0;
	min_width = 0;
	frame_width = mw;

	segs = NULL;
	nb_segs = 0;
	lines = NULL;
	nb_lines = 0;
	stl = &Xd6XmlStl::def;
}

Xd6HtmlBlock::~Xd6HtmlBlock()
{
	
	while (nb_segs > 0) {
		Xd6HtmlSegment *s = segs[--nb_segs];
		if (s && s->stl->display) delete((Xd6HtmlDisplay*) s);
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

void Xd6HtmlBlock::add_segment(char *txt, int len, Xd6XmlStl *style) 
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

	fw = frame_width - stl->blockquote * BLOCKQUOTEWIDTH;
	add_line();
	l = lines[nb_lines - 1];
	l->begin = 0;
	for (i = 0; i < nb_segs; i++) {
		Xd6HtmlSegment *s = segs[i];
		if ((s->stl->display) && 
			(((Xd6HtmlDisplay*)s)->display == DISPLAY_TAB)) 
		{
			s->set_font();
			if (s->stl->preformated) {
				int w;
				w = (int)fl_width(" ", 1);
				s->width = (8 - ((l->width / w) % 8)) * w;
			} else {
				s->width = Xd6HtmlFrame::get_tab(l->width, stl);
			}
			if (s->len == 0) {
				s->width = 0;
				((Xd6HtmlDisplay*)s)->display = DISPLAY_NONE;
			}
		}
		if (s->width + l->width > fw && 
			l->nb_segs > 0 && !(s->stl->preformated)) 
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
	width += stl->blockquote * BLOCKQUOTEWIDTH;
}

void Xd6HtmlBlock::align_lines()
{
	int i, ii, j;
	int nbl = nb_lines - 1;

	for (i = 0; i < nb_lines; i++) {
		int just = 0;
		if (stl->text_align == TEXT_ALIGN_RIGHT) {
			lines[i]->left = width - lines[i]->width;
		} else if (stl->text_align == TEXT_ALIGN_CENTER) {
			lines[i]->left = (width - lines[i]->width) / 2;
		} else if (stl->text_align == TEXT_ALIGN_JUSTIFY) {
			if (i == nbl) {
				if (lines[i]->stl->rtl_direction) {
					lines[i]->stl->text_align = 
						TEXT_ALIGN_RIGHT;
					lines[i]->left = width - 
						lines[i]->width;
				} else {
					lines[i]->stl->text_align = 
						TEXT_ALIGN_LEFT;
				}
				just = 1;
			} else {
				lines[i]->width = width;
			}
		}

		// reorder segments to be sorted !!
		j = 0;
		for (ii = lines[i]->begin; ii <= lines[i]->end; ii++) {
			lines[i]->segs[j++] = segs[ii];
		}

		lines[i]->sort_segments();
		if (just) {
			lines[i]->stl->text_align = TEXT_ALIGN_JUSTIFY;
		}
	}
}

void Xd6HtmlBlock::draw(int X, int Y)
{
	int i;
	X += left;
	Y += top;	

	for (i = 0; i < nb_lines; i++) {
		lines[i]->draw(X, Y);
		lines[i]->flags = 0;
	}
}

void Xd6HtmlBlock::cut_line(Xd6HtmlLine *l) 
{
	int i;
	l->flags |= (DAMAGE_ALL|DAMAGE_CHILD);
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
	l->flags |= (DAMAGE_ALL|DAMAGE_CHILD);
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
	l->flags |= (DAMAGE_ALL|DAMAGE_CHILD);
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

	l->flags |= (DAMAGE_ALL|DAMAGE_CHILD);
	s1id = l->get_seg_id(s1);
	s2id = l->get_seg_id(s2);

	s1->flags |= DAMAGE_ALL|DAMAGE_CHILD;
	s2->flags |= DAMAGE_ALL|DAMAGE_CHILD;

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

