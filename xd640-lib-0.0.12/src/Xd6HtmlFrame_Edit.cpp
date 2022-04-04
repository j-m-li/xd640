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
#include <FL/Fl.h>
#include <FL/Fl_Widget.h>
#include <FL/fl_draw.h>
#include <FL/fl_utf8.h>
#include "Xd6HtmlFrame.h"
#include "Xd6XmlParser.h"
#include "Xd6HtmlDisplay.h"
#include "Xd6HtmlTagTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <FL/Fl_Window.h>



void Xd6HtmlFrame::insert_block_after(Xd6HtmlBlock *b)
{
	Xd6HtmlBlock *b1;
	Xd6HtmlBlock **blks;
	int i;
	int bid;

	add_block();
	b1 = blocks[nb_blocks - 1];
	b1->style = b->style & ~PAGE_BREAK;

	blks = blocks;
	bid =  b->id + 1;
	for (i = nb_blocks - 1; i > bid; i--) {
		blks[i] = blks[i - 1];
		blks[i]->id = i;
	}

	blocks[b->id + 1] = b1;
	b1->id = b->id + 1;
}

void Xd6HtmlFrame::shift_blocks(int nb)
{
	int i;
	int id = cur_block->id;

	for (i = nb_blocks - 1; i >= id + nb; i--) {
		blocks[i] = blocks[i - nb];
		blocks[i]->id = i;
	}
}

void Xd6HtmlFrame::insert_frame(Xd6HtmlFrame *f)
{
	Xd6HtmlBlock *lb = NULL;
	Xd6HtmlBlock *b = NULL;
	int nbb = 0;
	int i, j;
	int nb;
	int shift_done = 0;
	int bid;

	cut();
	if (!cur_chr) return;
	if (cur_seg->style & DISPLAY && 
		((Xd6HtmlDisplay*)cur_seg)->display == DISPLAY_TABLE)
	{
		((Xd6HtmlTagTable*)cur_seg)->insert_frame(f);
		measure();
		create_pages();
		set_cut_cursor(cur_chr, cur_seg, cur_block);
		return;
	}
	
	for (i = 0; i <  f->nb_blocks; i++) {
		Xd6HtmlBlock *b;
		b = f->blocks[i];
		if ((b->nb_segs == 1 && 
			(b->segs[0]->len > 0 || b->segs[0]->style & DISPLAY)) ||
			b->nb_segs > 1)
		{
			lb = b;
			nbb++;
			if (nbb == 1 && (b->segs[0]->style & IS_BLOCK)) {
				split_block(cur_block, cur_seg, cur_chr);
			} 
		}
	}
	if (nbb < 1) return;
	if (lb->segs[0]->style & IS_BLOCK) {
		Xd6HtmlBlock *b = cur_block;
		Xd6HtmlSegment *s = cur_seg;
		char *t = cur_chr;
		split_block(cur_block, cur_seg, cur_chr);
		set_cut_cursor(t, s, b);
	}
	if (nbb == 1) {
		Xd6HtmlBlock *bb;
		int id;
		for (i = 0; i <  f->nb_blocks; i++) {
			b = f->blocks[i];
			if ((b->nb_segs == 1 && 
				(b->segs[0]->len > 0 || 
				b->segs[0]->style & DISPLAY)) ||
				b->nb_segs > 1)
			{
				break;
			}
		}
		bb = cur_block;
		if (b->segs[0]->style & IS_BLOCK) {
			int i;
			for (i = 0; i < bb->nb_segs; i++) {
				delete(bb->segs[i]);
			}
			bb->nb_segs = 0;
			cur_seg = b->segs[0];
			cur_chr = cur_seg->text;	
			id = -1;
		} else {
			split_segment(cur_block, cur_seg, cur_chr);
			id = cur_seg->id;
		}
		bb->segs = (Xd6HtmlSegment**) realloc(bb->segs,
				sizeof(Xd6HtmlSegment*) * 
				(bb->nb_segs + b->nb_segs));
		for (j = bb->nb_segs + b->nb_segs - 1; 
			j > id + b->nb_segs; j--)
		{
			bb->segs[j] = bb->segs[j - b->nb_segs];
			bb->segs[j]->id = j;
		}
		for (j = 0; j < b->nb_segs; j++) {
			bb->segs[id + 1 + j] = b->segs[j];
			b->segs[j]->id = bb->nb_segs;
			bb->nb_segs++;
			b->segs[j] = NULL;
		}		
		measure();
		create_pages();
		set_cut_cursor(cur_chr, cur_seg, cur_block);
		return;
	}

	split_block(cur_block, cur_seg, cur_chr);
	blocks = (Xd6HtmlBlock**) realloc(blocks, 
			sizeof(Xd6HtmlBlock*) * (nb_blocks + nbb));

	nb_blocks += nbb - 2;
	nb = 0;
	bid = cur_block->id;

	for (i = 0; i <  f->nb_blocks; i++) {
		Xd6HtmlBlock *b;
		b = f->blocks[i];
		if ((b->nb_segs == 1 && 
			(b->segs[0]->len > 0 || b->segs[0]->style & DISPLAY)) ||
			b->nb_segs > 1)
		{
			if (nb == 0) {
				Xd6HtmlBlock *bb;
				bb = blocks[cur_block->id -1];
				bb->segs = (Xd6HtmlSegment**) realloc(bb->segs,
					sizeof(Xd6HtmlSegment*) * 
					(bb->nb_segs + b->nb_segs));
				for (j = 0; j < b->nb_segs; j++) {
					bb->segs[bb->nb_segs] = b->segs[j];
					b->segs[j]->id = bb->nb_segs;
					bb->nb_segs++;
					b->segs[j] = NULL;
				}		
			} else if (nb == nbb - 1) {
				Xd6HtmlBlock *bb;
				bb = cur_block;
				b->segs = (Xd6HtmlSegment**) realloc(b->segs,
					sizeof(Xd6HtmlSegment*) * 
					(bb->nb_segs + b->nb_segs));
				for (j = 0; j < bb->nb_segs; j++) {
					b->segs[b->nb_segs] = bb->segs[j];
					bb->segs[j]->id = b->nb_segs;
					b->nb_segs++;
					bb->segs[j] = NULL;
				}
				free(bb->segs);
				bb->segs = b->segs;
				bb->nb_segs = b->nb_segs;
				b->segs = NULL;
				b->nb_segs = 0;		
			} else {
				if (!shift_done) shift_blocks(nbb - 2);
				shift_done = 1;
				blocks[bid] = b;
				b->id = bid;
				f->blocks[i] = NULL;
				bid++;
			}
			nb++;
		}
	}	

	measure();
	create_pages();
	set_cut_cursor(cur_chr, cur_seg, cur_block);
}

void Xd6HtmlFrame::split_segment(Xd6HtmlBlock *b, Xd6HtmlSegment *s, char *c)
{
	int len;
	char *str;
	int i;
	Xd6HtmlSegment *s1;

	len = s->len - (c - s->text);
	str = (char *) malloc(len + 1);
	
	for (i = 0; i < len; i++) {
		str[i] = c[i];
	}

	s->len = c - s->text;
		
	str[len] = '\0';
	b->add_segment(str, len, s->style & ~(DISPLAY|IS_BLOCK));
	s1 = b->segs[b->nb_segs - 1];

	for (i = b->nb_segs - 1; i > s->id + 1; i--) {
		b->segs[i] = b->segs[i - 1];
		b->segs[i]->id = i;
	}
	b->segs[s->id + 1] = s1;
	s1->id = s->id + 1;
}

void Xd6HtmlFrame::split_block(Xd6HtmlBlock *b, Xd6HtmlSegment *s, char *c)
{
	Xd6HtmlBlock *b1;
	int i, nbs;

	split_segment(b, s , c);
	insert_block_after(b);
	b1 = blocks[b->id + 1];
	
	nbs = b->nb_segs - s->id - 1;
	b1->segs = (Xd6HtmlSegment**) malloc(sizeof(Xd6HtmlSegment*) * nbs);
	for (i = 0; i < nbs; i++) {
		b1->segs[i] = b->segs[s->id + 1 + i];
		b1->segs[i]->id = i;
	}
	b1->nb_segs = nbs;
	b->nb_segs = s->id + 1;
	

	b->measure();
	b1->measure();
	b->create_lines();
	b1->create_lines();
	b->align_lines();
	b1->align_lines();
	set_cut_cursor(b1->segs[0]->text, b1->segs[0], b1);
}

void Xd6HtmlFrame::insert_char(Xd6HtmlSegment *s, char *c, const char *txt, int len)
{
	int i, ii;

	ii = c - s->text;
	s->text = (char*) realloc(s->text, s->len + len + 1);
	
	for (i = s->len + len - 1; i - len >= ii; i--) {
		s->text[i] = s->text[i - len];
	}

	for (i = 0; i < len; i++) {
		s->text[ii + i] = txt[i];
	}

	s->len += len;

}

void Xd6HtmlFrame::save_padding()
{
	int i, y;
	nb_blk = nb_blocks;
	blk_current = cur_block;
	blk_nb_lines = cur_block->nb_lines;
	blk_paddings = (int*) realloc(blk_paddings, 
		sizeof(int) * cur_block->nb_lines);
	blk_heights = (int*) realloc(blk_heights, 
		sizeof(int) * cur_block->nb_lines);
	y = 0;
	for (i = 0; i < blk_nb_lines; i++) {
		blk_heights[i] = cur_block->lines[i]->height;
		blk_paddings[i] = cur_block->lines[i]->top - y;
		y = cur_block->lines[i]->top + blk_heights[i];
	}
}

void Xd6HtmlFrame::restore_padding()
{
	int i;

	cur_block->measure();
	cur_block->create_lines();
	if (cur_block->width > width) width = cur_block->width;
	cur_block->width = page_width;
	cur_block->align_lines();
	set_cut_cursor(cur_chr, cur_seg, cur_block);
	cur_block->height = 0;
	if (nb_blocks == nb_blk && cur_block == blk_current && 
		cur_block->nb_lines == blk_nb_lines) 
	{
		for (i = 0; i < cur_block->nb_lines; i++) {
			Xd6HtmlLine *l;
			l = cur_block->lines[i];
			cur_block->height += blk_paddings[i];
			l->top = cur_block->height;
			cur_block->height += l->height;
			if (l->height != blk_heights[i]) {
				create_pages(cur_block);
				break;
			}
		}
	} else {
		if (nb_blk < nb_blocks - 1) {
			create_pages();
		} else {
			create_pages(cur_block);
		}
	}
}

Xd6HtmlFrame *Xd6HtmlFrame::get_cursor_frame()
{
	if (!cur_seg) return this;
	if (cur_seg->style & DISPLAY && 
		((Xd6HtmlDisplay*)cur_seg)->display == DISPLAY_TABLE)
	{
		return ((Xd6HtmlTagTable*)cur_seg)->get_cursor_frame();
	}
	return this;
}

void Xd6HtmlFrame::insert_segment(Xd6HtmlSegment *s)
{
	if (!cur_seg) return;
	if (cur_seg->style & DISPLAY && 
		((Xd6HtmlDisplay*)cur_seg)->display == DISPLAY_TABLE)
	{
		((Xd6HtmlTagTable*)cur_seg)->insert_segment(s);
		measure();
		create_pages();
		set_cut_cursor(cur_chr, cur_seg, cur_block);
		return;
	}
	split_segment(cur_block, cur_seg, cur_chr);
	cur_seg = cur_block->segs[cur_seg->id + 1];
	cur_chr = cur_seg->text;
	split_segment(cur_block, cur_seg, cur_seg->text);
	s->id = cur_seg->id;
	delete(cur_seg);
	cur_block->segs[s->id] = s;
	cur_seg = s;
	cur_chr = cur_seg->text;

	cur_block->measure();
	cur_block->create_lines();
	cur_block->align_lines();
	set_cut_cursor(cur_chr, cur_seg, cur_block);	
}

void Xd6HtmlFrame::insert_text(const char *txt, int len)
{
	int i = 0;

	save_padding();

	if (cur_seg->style & DISPLAY) { 
		if (cur_seg->style & IS_BLOCK) {
			split_block(cur_block, cur_seg, cur_seg->text +
				cur_seg->len);
		} else if (cur_chr != cur_seg->text) {
			// place cursor after cur_seg
			split_segment(cur_block, cur_seg, cur_seg->text +
				cur_seg->len);
			cur_seg = cur_block->segs[cur_seg->id + 1];
			cur_chr = cur_seg->text;
			cur_block->create_lines();
			cur_block->align_lines();
			set_cut_cursor(cur_chr, cur_seg, cur_block);
		} else {
			// place cursor before cur_seg
			Xd6HtmlSegment * s1;
			int i;
			cur_block->add_segment((char*)malloc(1), 0, 
				cur_seg->style & ~DISPLAY);
			s1 = cur_block->segs[cur_block->nb_segs - 1];
			s1->id = cur_seg->id;

			for (i = cur_block->nb_segs - 1; 
				i > cur_seg->id; i--) 
			{
				cur_block->segs[i] = cur_block->segs[i - 1];
				cur_block->segs[i]->id = i;
			}
			cur_block->segs[s1->id] = s1;
			cur_seg = s1;
			cur_chr = cur_seg->text;
			cur_block->create_lines();
			cur_block->align_lines();
			set_cut_cursor(cur_chr, cur_seg, cur_block);
		}
	}

	while (i < len) {
		Xd6HtmlBlock *b = cur_block;
		Xd6HtmlLine *l = cur_line;
		Xd6HtmlSegment *s = cur_seg;
		char *c = cur_chr;

		if (s->len > 0 && txt[i] != '\x7F' && txt[i] != '\b') {
			if (c == s->text && (c[0] == ' ' || c[0] == '\t')) {
				split_segment(b, s, c + 1);
				s = b->segs[s->id + 1];
				c = s->text;
				cur_block->create_lines();
				cur_block->align_lines();
				set_cut_cursor(c, s, b);
			} else if (c > s->text && (c[-1] == ' ' ||
				c[-1] == '\t'))
			{
				split_segment(b, s, c);
				s = b->segs[s->id + 1];
				c = s->text;
				cur_block->create_lines();
				cur_block->align_lines();
				set_cut_cursor(c, s, b);
			}
		}
		b = cur_block;
		l = cur_line;
		s = cur_seg;
		c = cur_chr;

		if (txt[i] == '\n' || txt[i] == '\r') {
			split_block(b, s, c);
			//create_pages();
			auto_scroll(-1, 1);
			set_cut_cursor(cur_chr, cur_seg, cur_block);
		} else if (txt[i] == ' ') {
			int ii;
			ii = cur_chr - cur_seg->text;
			insert_char(s, c, " ", 1);
			split_segment(b, s, s->text + ii + 1);
			cur_seg = b->segs[s->id + 1];
			cur_chr = cur_seg->text;
			cur_block->create_lines();
			cur_block->align_lines();
			set_cut_cursor(cur_chr, cur_seg, cur_block);
		} else if (txt[i] == '\t') {
			int id, st;
			if (cur_seg->len > 0) {
				split_segment(b, s, c);
				cur_seg = b->segs[s->id + 1];
				cur_chr = cur_seg->text;
			}
			insert_char(cur_seg, cur_chr, "\t", 1);
			cur_chr++;
			split_segment(b, cur_seg, cur_chr);
			id = cur_seg->id;
			st = cur_seg->style;
			delete(b->segs[id]);
			b->segs[id] = new Xd6HtmlDisplay(id,
				(char*)malloc(2) , 1, st);
			b->segs[id]->text[0] = '\t';
			b->segs[id]->style = st | DISPLAY;
			((Xd6HtmlDisplay*)b->segs[id])->display = DISPLAY_TAB;
			cur_seg = b->segs[id + 1];
			cur_chr = cur_seg->text; 
			cur_block->create_lines();
			cur_block->align_lines();
			set_cut_cursor(cur_chr, cur_seg, cur_block);
		} else if (txt[i] == '\b') {
			move_cursor(-1, 0);
			sel_block = b;
			sel_line = l;
			sel_seg = s;
			sel_chr = c;
			if (len == 1) {
				cut(1);
			} else {
				cut();
			}
		} else if (txt[i] == '\x7F') {
			move_cursor(1, 0);
			sel_block = b;
			sel_line = l;
			sel_seg = s;
			sel_chr = c;
			if (len == 1) {
				cut(1);
			} else {
				cut();
			}
		} else  {
			int ii;
			ii = cur_chr - cur_seg->text;
			insert_char(cur_seg, cur_chr, txt + i, 1);
			cur_chr = cur_seg->text + ii + 1;
		}
		i++;
	}
	
	restore_padding();

	auto_scroll(-1, -1);
	auto_scroll(1, 1);
}

void Xd6HtmlFrame::merge_blocks(Xd6HtmlBlock *b, Xd6HtmlBlock *b2, char **cursor)
{
	int ii, i, nbs;
	int id;

	b->segs =  (Xd6HtmlSegment**) realloc(b->segs,
			sizeof(Xd6HtmlSegment*) * 
				(b->nb_segs + b2->nb_segs));
	id = b2->id;
	ii = b->nb_segs;
	nbs = b->nb_segs + b2->nb_segs;
	for (i = 0; i < b2->nb_segs; i++) {
		Xd6HtmlSegment *s1 = b->segs[ii - 1];
		Xd6HtmlSegment *s2 = b2->segs[0];
		if (i == 0 && s1->style == s2->style &&
			!(s1->len > 0 && s1->text[s1->len - 1] == ' ') &&
			!(s2->len > 0 && s2->text[0] != ' '))
		{
			int j, jj;
			int selc;

			selc = *cursor - s1->text;
			s1->text = (char*) realloc(s1->text,
				s1->len + s2->len + 1);
			*cursor = s1->text + selc;
			jj = s1->len;
			for (j = 0; j < s2->len; j++) {
				s1->text[jj] = s2->text[j];
				jj++;
			}
			s1->text[jj] = '\0';
			s1->len = jj;
			delete(s2);
			b2->segs[0] = NULL;
			nbs--;
		} else {
			b->segs[ii] = b2->segs[i];
			b->segs[ii]->id = ii;
			ii++;
		}
	}
	b->nb_segs = nbs;
	b2->nb_segs = 0;
	delete(b2);
	blocks[id] = NULL;
}

void Xd6HtmlFrame::change_block_style(Xd6HtmlBlock *b, int and_mask, int or_mask)
{
	int i;
	
	b->style &= ~and_mask;
	b->style |= or_mask;
	for (i = 0; i < b->nb_segs; i++) {
		b->segs[i]->style &= ~and_mask;
		b->segs[i]->style |= or_mask;
	}
	b->measure();
	b->create_lines();
}

void Xd6HtmlFrame::change_middle_block_style(Xd6HtmlBlock *b, Xd6HtmlLine *l1,
        Xd6HtmlSegment *s1, Xd6HtmlLine *l2, Xd6HtmlSegment *s2, 
        int and_mask, int or_mask)
{
	int i;

	
	if (l1 == l2) {
		int ii, e;
		ii = l1->get_seg_id(s1);
		e = l1->get_seg_id(s2);
		for (;ii <= e; ii++) {
			l1->segs[ii]->style &= ~and_mask;
			l1->segs[ii]->style |= or_mask;
		}	
	} else {
		int ii, e;
		ii = l1->get_seg_id(s1);
		for (;ii < l1->nb_segs; ii++) {
			l1->segs[ii]->style &= ~and_mask;
			l1->segs[ii]->style |= or_mask;
		}
		e = l2->get_seg_id(s2);
		for (ii = 0; ii <= e; ii++) {
			l2->segs[ii]->style &= ~and_mask;
			l2->segs[ii]->style |= or_mask;
		}

	}

	for (i = l1->id + 1; i < l2->id; i++) {
		int ii;
		Xd6HtmlLine *ln;
		ln = b->lines[i];
		for (ii = 0; ii < ln->nb_segs; ii++) {
			ln->segs[ii]->style &= ~and_mask;
			ln->segs[ii]->style |= or_mask;
		}
	}
}

void Xd6HtmlFrame::change_end_block_style(Xd6HtmlBlock *b1, Xd6HtmlLine *l1,
        Xd6HtmlSegment *s1, int and_mask, int or_mask)
{
	Xd6HtmlSegment *s2;
	Xd6HtmlLine *l2;
	l2 = b1->lines[b1->nb_lines - 1];
	s2 = l2->segs[l2->nb_segs - 1];
	change_middle_block_style(b1, l1, s1, l2, s2, and_mask, or_mask);
}

void Xd6HtmlFrame::change_begin_block_style(Xd6HtmlBlock *b1, Xd6HtmlLine *l1,
        Xd6HtmlSegment *s1, int and_mask, int or_mask)
{
	Xd6HtmlSegment *s2;
	Xd6HtmlLine *l2;
	l2 = b1->lines[0];
	s2 = l2->segs[0];
	change_middle_block_style(b1, l2, s2, l1, s1, and_mask, or_mask);
}

void Xd6HtmlFrame::change_style(int and_mask, int or_mask)
{
	if (sel_chr && cur_chr) {
		int i = 0;
		Xd6HtmlBlock *b1, *b2;
		Xd6HtmlLine *l1, *l2;
		Xd6HtmlSegment *s1, *s2;
		char *c1, *c2;
		
		sort_selection(&b1, &l1, &s1, &c1, &b2, &l2, &s2, &c2);
	
		if (c1 == cur_chr) {
			cur_block = sel_block;
			cur_line = sel_line;
			cur_seg = sel_seg;
			cur_chr = sel_chr;
			move_cursor_left();
			move_cursor_right();
			sel_block = cur_block;
			sel_line = cur_line;
			sel_seg = cur_seg;
			sel_chr = cur_chr;
			cur_block = b1;
			cur_line = l1;
			cur_seg = s1;
			cur_chr = c1;
			move_cursor_right();
			if (cur_block == b1) {
				move_cursor_left();
			}
		} else {
			cur_block = sel_block;
			cur_line = sel_line;
			cur_seg = sel_seg;
			cur_chr = sel_chr;
			move_cursor_right();
			if (cur_block == b1) {
				move_cursor_left();
			}
			sel_block = cur_block;
			sel_line = cur_line;
			sel_seg = cur_seg;
			sel_chr = cur_chr;
			cur_block = b2;
			cur_line = l2;
			cur_seg = s2;
			cur_chr = c2;
			move_cursor_left();
			move_cursor_right();
		}	
		
		sort_selection(&b1, &l1, &s1, &c1, &b2, &l2, &s2, &c2);

		if (s1 == s2) {
			int w;
			if (s1->style & RTL_DIRECTION) {
				w = c1 - c2;
				if (c2 != s1->text) {
					split_segment(b1, s1, c2);
					s1 = b1->segs[s1->id + 1];
					c2 = s1->text;
				}
				if (w != s1->len) {
					split_segment(b1, s1, c2 + w);
				}
				s2 = s1;
				c1 = s2->text + w;
			} else {
				w = c2 - c1;
				if (c1 != s1->text) {
					split_segment(b1, s1, c1);
					s1 = b1->segs[s1->id + 1];
					c1 = s1->text;
				}
				if (w != s1->len) {
					split_segment(b1, s1, c1 + w);
				}
				s2 = s1;
				c2 = s2->text + w;
			}
		} else {
			if (s1->style & RTL_DIRECTION) {
				if (c1 != s1->text + s1->len) {
					split_segment(b1, s1, c1);
				}
			} else {
				if (c1 != s1->text) {
					split_segment(b1, s1, c1);
					s1 = b1->segs[s1->id + 1];
					c1 = s1->text;
				}
			}
			if (s2->style & RTL_DIRECTION) {
				if (c2 != s2->text) {
					split_segment(b2, s2, c2);
					s2 = b2->segs[s2->id + 1];
					c2 = s2->text;
				}
			} else {
				if (c2 != s2->text + s2->len) {
					split_segment(b2, s2, c2);
				}
			}
		}
		b1->create_lines();
		b2->create_lines();
		b1->align_lines();
		b2->align_lines();
		set_cut_cursor(c2, s2, b2);
		b2 = cur_block;
		l2 = cur_line;
		s2 = cur_seg;
		c2 = cur_chr;
		set_cut_cursor(c1, s1, b1);
		sel_block = b2;
		sel_line = l2;
		sel_seg = s2;
		sel_chr = c2;

		sort_selection(&b1, &l1, &s1, &c1, &b2, &l2, &s2, &c2);
		
		if (b1 == b2) {
			change_middle_block_style(b1, l1, s1, l2, s2, 
				and_mask, or_mask);
		} else {
			change_end_block_style(b1, l1, s1, and_mask, or_mask);
			change_begin_block_style(b2, l2, s2, and_mask, or_mask);
		}

		for (i = b1->id + 1; i < b2->id; i++) {
			change_block_style(blocks[i], and_mask, or_mask);
		}
		if (and_mask & TEXT_ALIGN) {
			change_block_style(b1, and_mask, or_mask);
			change_block_style(b2, and_mask, or_mask);
		}
		b1->measure();
		b2->measure();
		b1->create_lines();
		b2->create_lines();
		create_pages();
		set_cut_cursor(c2, s2, b2);
		b2 = cur_block;
		l2 = cur_line;
		s2 = cur_seg;
		c2 = cur_chr;
		set_cut_cursor(c1, s1, b1);
		sel_block = b2;
		sel_line = l2;
		sel_seg = s2;
		sel_chr = c2;
	} else if (cur_chr) {
		if (cur_seg->style & DISPLAY && 
			((Xd6HtmlDisplay*)cur_seg)->display == DISPLAY_TABLE)
		{
			((Xd6HtmlTagTable*)cur_seg)->
				change_style(and_mask, or_mask);
			return;
		}
		if (and_mask & TEXT_ALIGN) {
			change_block_style(cur_block, and_mask, or_mask);
		}
		if (cur_chr != cur_seg->text) {
			split_segment(cur_block, cur_seg, cur_chr);
			cur_seg = cur_block->segs[cur_seg->id + 1];
			cur_chr = cur_seg->text;
		}
		if (cur_chr != cur_seg->text + cur_seg->len) {
			split_segment(cur_block, cur_seg, cur_chr);
		}
		cur_seg->style &= ~and_mask;
		cur_seg->style |= or_mask;
		cur_block->measure();
		cur_block->create_lines();
		create_pages();
		set_cut_cursor(cur_chr, cur_seg, cur_block);
	}
}

void Xd6HtmlFrame::cursor_to_end()
{
	int i;
	sel_chr = NULL;
	sel_seg = NULL;
	sel_line = NULL;
	sel_block = NULL;
	cur_chr = NULL;
	cur_seg = NULL;
	cur_line = NULL;
	cur_block = NULL;
	
	i = nb_blocks;
	while (i > 0) {
		i--;
		cur_block = blocks[i];
		if (cur_block->nb_segs > 0) {
			cur_line = cur_block->lines[cur_block->nb_lines - 1];
			cur_seg = cur_block->segs[cur_block->nb_segs - 1];
			if (cur_seg->style & RTL_DIRECTION) {
				cur_chr = cur_seg->text;
			} else {
				cur_chr = cur_seg->text + cur_seg->len;
			}
			break;
		}
	}
}

void Xd6HtmlFrame::cursor_to_begin()
{
	int i;
	sel_chr = NULL;
	sel_seg = NULL;
	sel_line = NULL;
	sel_block = NULL;
	cur_chr = NULL;
	cur_seg = NULL;
	cur_line = NULL;
	cur_block = NULL;
	i = 0;
	while (i < nb_blocks) {
		cur_block = blocks[i];
		if (cur_block->nb_segs > 0) {
			cur_line = cur_block->lines[0];
			cur_seg = cur_block->segs[0];
			if (cur_seg->style & RTL_DIRECTION) {
				cur_chr = cur_seg->text + cur_seg->len;
			} else {
				cur_chr = cur_seg->text;
			}
			break;
		}
		i++;
	}

}

void Xd6HtmlFrame::select_all()
{
	int i;
	cursor_to_end();
	i = 0;
	while (i < nb_blocks) {
		sel_block = blocks[i];
		if (sel_block->nb_segs > 0) {
			sel_line = sel_block->lines[0];
			sel_seg = sel_block->segs[0];
			if (sel_seg->style & RTL_DIRECTION) {
				sel_chr = sel_seg->text + sel_seg->len;
			} else {
				sel_chr = sel_seg->text;
			}
			break;
		}
		i++;
	}
}

/*
 * "$Id: $"
 */

