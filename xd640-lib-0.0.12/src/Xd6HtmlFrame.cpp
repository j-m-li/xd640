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
#include "Xd6HtmlDownload.h"
#include "Xd6XmlParser.h"
#include "Xd6HtmlTagTable.h"
#include <sys/stat.h>
#include <FL/Fl_Window.h>

#define _(String) gettext((String))

int Xd6HtmlFrame::paste_buffer_malloc = 0;
int Xd6HtmlFrame::paste_buffer_len = 0;
char *Xd6HtmlFrame::paste_buffer = NULL;
Xd6HtmlFrameEngine Xd6HtmlFrame::engine = &Xd6HtmlFrame::handle_click;
Fl_Output *Xd6HtmlFrame::status_bar = NULL;
int *Xd6HtmlFrame::tab_stop = NULL;
int Xd6HtmlFrame::rule_width = 500;


Xd6HtmlFrame::Xd6HtmlFrame(int i) : Xd6HtmlScroll(i)
{
	focus = NULL;

	display = DISPLAY_TOP_FRAME;

	blocks = NULL;
	nb_blocks = 0;
	
	wysiwyg = 1;
	editor = 1;

	scroll = 1;
	page_margin_top = 56;
	page_margin_left = 84;
	page_margin_right = 28;
	page_margin_bottom = 56;
	page_height = 842 - 112;
	page_width = 595 - 112;
	page_land = 0;
	footer_height = 25;

	flags = 0;
	add_block();

	sel_block = NULL;
	sel_line = NULL;
	sel_seg = NULL;
	sel_chr = NULL;
	cur_block = NULL;
	cur_line = NULL;
	cur_seg = NULL;
	cur_chr = NULL;

	blk_paddings = NULL;
	blk_heights = NULL;
	blk_nb_lines = 0;
	blk_current = NULL;

	open_tags = NULL;
	close_tags = NULL;
	nb_tags = 0;

	url = NULL;
	file = NULL;
	parent = this;
	
	add_block();
	blocks[0]->add_segment((char*)malloc(1), 0, 
		IS_INLINE|FONT_SIZE_3|FONT_SANS_SERIF);
	blocks[0]->measure();
	blocks[0]->create_lines();
	create_pages();

	background = 7;
	
	off_screen = 0;
	
	if (!downloader) {
		downloader = new Xd6HtmlDownload();
	}
	quote = "";
}


Xd6HtmlFrame::~Xd6HtmlFrame()
{
	while (nb_blocks > 0) {
		delete(blocks[--nb_blocks]);
	}
	if (file) free(file);
	if (url) free(url);
	create_off_screen(); // free offscreen pixmap
}

void Xd6HtmlFrame::add_block() 
{
	blocks = (Xd6HtmlBlock**) realloc(blocks, 
			sizeof(Xd6HtmlBlock*) * (nb_blocks + 1));
	blocks[nb_blocks] = new Xd6HtmlBlock(nb_blocks, page_width);
	nb_blocks += 1;
}

void Xd6HtmlFrame::resize(int W, int H)
{
	int i;

	height = 0;
	width = 0;
	max_width = W;
	max_height = H;

	if (!wysiwyg) {
		if (scroll) {
			page_width = W - 15;
		} else {
			page_width = W;
		}
	}

	sel_chr = NULL;


	for (i = 0; i < nb_blocks; i++) {
		blocks[i]->frame_width = page_width;
		blocks[i]->create_lines();
		if (blocks[i]->width > width) width = blocks[i]->width;
	}
	create_pages();
	if (wysiwyg) {
		if (width < page_width) width = page_width;
		if (scroll) {
			width += 15;
		}
	}
	damage(FL_DAMAGE_ALL);
	if (off_screen) create_off_screen();
	if (cur_chr) set_cut_cursor(cur_chr, cur_seg, cur_block);
}

void Xd6HtmlFrame::page_size(int W, int H, int FH)
{
	page_height = H;
	page_width = W;
	footer_height = FH;
}

void Xd6HtmlFrame::measure()
{
	int i;

	height = 0;
	width = 0;
	if (status_bar) {
		status_bar->value(_("preformating page..."));
#if HAVE_FLTK_UTF || HAVE_FLTK
		Fl::first_window()->damage(FL_DAMAGE_CHILD);
#else
		Fl::first_window()->damage(FL_DAMAGE_CHILD);
#endif
		Fl::flush();
	}
	for (i = 0; i < nb_blocks; i++) {
		blocks[i]->frame_width = page_width;
		blocks[i]->measure();
		blocks[i]->create_lines();
		if (blocks[i]->width > width) width = blocks[i]->width;
	}
	create_pages();
	if (cur_chr) set_cut_cursor(cur_chr, cur_seg, cur_block);
}

int  Xd6HtmlFrame::break_line(Xd6HtmlLine *l, int height)
{
	// try to split line
	int i;
	int ret = 0;
	for (i = 0; i < l->nb_segs; i++) {
		if (l->segs[i]->style & DISPLAY) {
			Xd6HtmlDisplay *d;
			d = (Xd6HtmlDisplay*) l->segs[i];
			if (d->display == DISPLAY_TABLE) {
				d->break_line(height, page_height, 
					footer_height);
				ret = 1;
				l->height = d->height;
			}
			if (d->height > l->height) {
				l->height = d->height;
			}
		}
	}
	return ret;
}

void Xd6HtmlFrame::create_pages(Xd6HtmlBlock *b)
{
	int i;
	int p = 0;
	int op = 0;
	Xd6HtmlBlock **blks;
	int nb_blks;
	int max_h = 0x7FFFFFFF;

	height = 0;
	if (status_bar) {
		status_bar->value(_("formating page..."));
#if HAVE_FLTK_UTF || HAVE_FLTK
		Fl::first_window()->damage(FL_DAMAGE_CHILD);
#else
		Fl::first_window()->damage(FL_DAMAGE_CHILD);
#endif
		Fl::flush();
	}
	blks = blocks;
	nb_blks = nb_blocks;
	i = 0;
	if (b && display == DISPLAY_TOP_FRAME) {
		while (b != blks[i]) i++;
		if (i > 0) {
			i--;
			height = blks[i]->top;
			op = height / page_height;
			max_h = height + 4000;
			need_paging = 1;
		}
	} else {
		need_paging = 0;
	}
	for (; i < nb_blks; i++) {
		int ii;
		Xd6HtmlBlock *b;
		Xd6HtmlLine **lns;
		int nbl;
		b = blks[i];
		
		if (height > max_h) {
			height += b->height;
			continue;
		}

		if (wysiwyg && b->style & PAGE_BREAK) {
			int pad;
			pad = page_height - height % page_height;
			height += pad;
			op = height  / page_height;
		}
		b->left = (b->style & BLOCKQUOTES) / BLOCKQUOTE1 * 40;
		b->top = height;
		b->height = 0;
		b->width = page_width - b->left;
		if (b->style & TEXT_ALIGN_RIGHT) {
			b->left = 0;
		} else if (b->style & TEXT_ALIGN_CENTER) {
			b->left = b->left / 2;
		}
		b->align_lines();
		nbl = b->nb_lines;
		lns = b->lines;
		for(ii = 0; ii < nbl; ii++) {
			Xd6HtmlLine *l;
			l = lns[ii];
			
			p = (height + l->height + footer_height) / page_height;
			if (wysiwyg && p != op) {
				/* try to break line */
				l->top = height - b->top;
				if (!break_line(l, height)) {
					/* we can't -> put line on a new page */
					int pad;
					pad = page_height - 
						(height % page_height);
					b->height += pad;
					height += pad;
					l->top = height - b->top;
				}	
			} else {
				l->top = height - b->top;
			}
			height += l->height;
			b->height += l->height;
			op = height / page_height;
		}
	}
	if (wysiwyg) {
		height += page_height - height % page_height;
	}
	if (status_bar) {
		status_bar->value(" ");
#if HAVE_FLTK_UTF || HAVE_FLTK
		Fl::first_window()->damage(FL_DAMAGE_CHILD);
#else
		Fl::first_window()->damage(FL_DAMAGE_CHILD);
#endif
		Fl::flush();
	}
}

Xd6HtmlDisplay *Xd6HtmlFrame::special_elements(Xd6XmlTreeElement *elem, Xd6HtmlDisplay *cp)
{
	Xd6HtmlDisplay *d;
	Xd6HtmlBlock *blk;
	int seg;

	blk = blocks[nb_blocks - 1];
	blk->add_segment(NULL, 0, 0);
	seg = blk->nb_segs - 1;
	delete(blk->segs[seg]);

	d = Xd6HtmlDisplay::create(seg, elem, cp);
	blk->segs[seg] = d;
	return d;
}

void Xd6HtmlFrame::add_segment(Xd6HtmlBlock *b, char *t, int l, int s)
{
	int i = 1;
	char *p = t;
	int ll = l;
	while (i < l) {
		// remove &nbsp;
		if (t[i] == '\xA0' && t[i - 1] == '\xC2') {
			t[i - 1] = ' ';
			t[i] = '\0';
			b->add_segment(t, i, s);
			p = (char *)malloc(l - i + 1);
			ll = 0;
			i++;
			while (i < l) p[ll++] = t[i++];
			p[ll] = '\0';
			break;
		}	
		i++;
	}
	b->add_segment(p, ll, s);
}

void Xd6HtmlFrame::text_tree2block(Xd6XmlTreeElement *elem)
{
	int i;
	if (!elem) return;

	
	if (!(elem->style & (IS_BLOCK|IS_INLINE))) return;

	if (elem->style & IS_BLOCK) {
		add_block();
		blocks[nb_blocks - 1]->style = elem->style;
	}

	if (nb_blocks < 1) add_block();

	for (i = 0; i < elem->nb_children; i++) {
		Xd6XmlTreeText *txt;
		Xd6XmlTreeElement *e;
		switch(elem->children[i]->type) {
		case Xd6XmlTreeSegment_none:
			break;
		case Xd6XmlTreeSegment_element:
			e = (Xd6XmlTreeElement*)elem->children[i];
			text_tree2block(e);
			break;
		default:
			txt = (Xd6XmlTreeText*)elem->children[i];
			if (blocks[nb_blocks - 1]->nb_segs == 0 &&
				txt->len == 1 && txt->data[0] == ' ')
			{
				break;
			}
			add_segment(blocks[nb_blocks - 1], txt->data, txt->len,
				elem->style);  
			txt->data = NULL;
			txt->len = 0;
		}
	}

        while (elem->nb_children > 0) {
                elem->nb_children--;
                if (!elem->children[elem->nb_children]) continue;
                if (elem->children[elem->nb_children]->type == 
			Xd6XmlTreeSegment_element) 
		{
                        delete((Xd6XmlTreeElement*)
				elem->children[elem->nb_children]);
                } else if (elem->children[elem->nb_children]->type & 
			Xd6XmlTreeSegment_text) 
		{
                        delete((Xd6XmlTreeText*)elem->children[elem->nb_children]);
                } else {
                        delete(elem->children[elem->nb_children]);
                }
	}
}

void Xd6HtmlFrame::tree2block(Xd6XmlTreeElement *elem, Xd6HtmlDisplay *cp)
{
	int i;
	if (!elem) return;

	if (elem->display) {
		Xd6HtmlDisplay::pre_process(elem, this);
	}

	if (!(elem->style & (IS_BLOCK|IS_INLINE))) return;

	if (nb_blocks < 1) {
		add_block();
		blocks[nb_blocks - 1]->style = elem->style & ~(DISPLAY);
	}

	if (elem->style & IS_BLOCK) {
		if (nb_blocks < 1) add_block();
		Xd6HtmlBlock *b = blocks[nb_blocks - 1];
		if ((b->nb_segs > 0) || (b->style & PAGE_BREAK)) {
			add_block();
		}
		blocks[nb_blocks - 1]->style = elem->style & ~(DISPLAY);
		if (elem->style & TOP_MARGIN && (b->nb_segs > 1 || 
			(b->nb_segs == 1 && b->segs[0]->len > 0))) 
		{
			add_segment(blocks[nb_blocks - 1], (char*)malloc(1), 0,
				elem->style &
				~(TOP_MARGIN|DISPLAY|PAGE_BREAK|
					LIST|A_LINK|IS_BLOCK));
			add_block();
			blocks[nb_blocks - 1]->style = elem->style & ~(DISPLAY);
		} 
	}

	if ((elem->style & DISPLAY) && (elem->display != DISPLAY_TABLE_CELL)) {
		if (!cp) cp = this;
		cp = special_elements(elem, cp);
		if (elem->display == DISPLAY_TAB) {
			return;
		}
	}

	for (i = 0; i < elem->nb_children; i++) {
		Xd6XmlTreeText *txt;
		Xd6XmlTreeElement *e;
		Xd6HtmlBlock *b;
		switch(elem->children[i]->type) {
		case Xd6XmlTreeSegment_none:
			break;
		case Xd6XmlTreeSegment_element:
			e = (Xd6XmlTreeElement*)elem->children[i];
			tree2block(e, cp);
			if (nb_blocks < 1) add_block();
			b = blocks[nb_blocks - 1];
			if (e->style & IS_BLOCK && (b->nb_segs > 1 ||
				(b->nb_segs == 1 && b->segs[0]->len > 0) ||
				e->style & DISPLAY)) 
			{
				add_block();
				blocks[nb_blocks - 1]->style = 
					elem->style & 
					~(DISPLAY|PAGE_BREAK|LIST);
				if (e->style & TOP_MARGIN) {
					add_segment(blocks[nb_blocks - 1], 
					(char*)malloc(1), 0,
					e->style &
					~(TOP_MARGIN|DISPLAY|PAGE_BREAK|
						LIST|A_LINK|IS_BLOCK));
					add_block();
				}
			}
			
			break;
		default:
			if (nb_blocks < 1) add_block();
			txt = (Xd6XmlTreeText*)elem->children[i];
			if ((blocks[nb_blocks - 1]->nb_segs == 0 ||
				(blocks[nb_blocks - 1]->nb_segs == 1 &&
				blocks[nb_blocks - 1]->segs[0]->len == 0)) &&
				txt->len == 1 && txt->data[0] == ' ')
			{
				break;
			}
			add_segment(blocks[nb_blocks - 1], txt->data, txt->len,
				elem->style & 
				~(TOP_MARGIN|DISPLAY|PAGE_BREAK|LIST|IS_BLOCK)); 
			txt->data = NULL;
			txt->len = 0;
		}
	}
	if (elem->style & IS_BLOCK && elem->nb_children > 0) {
//		add_block();
	}
        while (elem->display == 0 && elem->nb_children > 0) {
                elem->nb_children--;
                if (!elem->children[elem->nb_children]) continue;
                if (elem->children[elem->nb_children]->type == 
			Xd6XmlTreeSegment_element) 
		{
                        delete((Xd6XmlTreeElement*)
				elem->children[elem->nb_children]);
                } else if (elem->children[elem->nb_children]->type & 
			Xd6XmlTreeSegment_text) 
		{
                        delete((Xd6XmlTreeText*)elem->children[elem->nb_children]);
                } else {
                        delete(elem->children[elem->nb_children]);
                }
        }

}


void Xd6HtmlFrame::find_pos(int X, int Y, Xd6HtmlBlock **b, Xd6HtmlLine **l,
	Xd6HtmlSegment **s, char **c)
{
	int i, x, y;
	
	x = left + hscroll;
	y = top + vscroll;

	*b = NULL;
	*l = NULL;
	*s = NULL;
	*c = NULL;
	
	for (i = 0; i < nb_blocks; i++) {
		if (Y <= blocks[i]->top + blocks[i]->height + y) {
			while ((i > 0) && (Y < top)) {
				i--;
				if (blocks[i]->height) break;
			} 
			blocks[i]->find_pos(X, Y, x, y, l, s, c);
			*b = blocks[i];
			return;
		}
	}
	i = nb_blocks;
	while (i > 0) {
		i--;
		if (blocks[i]->nb_segs > 0) {
			blocks[i]->find_pos(X, Y, x, y, l, s, c);
			*b = blocks[i];
			break;
		}
	}
}

void Xd6HtmlFrame::set_cut_cursor(char *cursor, Xd6HtmlSegment *s, Xd6HtmlBlock *b)
{
	int i;

	cur_chr = cursor;
	cur_block = b;
	cur_seg = s;
	
	for (i = 0; i < b->nb_lines; i++) {
		int ii;
		Xd6HtmlLine *l;
		l = b->lines[i];
		for (ii = 0; ii < l->nb_segs; ii++) {
			if (l->segs[ii] == s) {
				cur_line = l;
				l = NULL;
				break;
			}
		}
		if (!l) break;
	}

	sel_block = NULL;
	sel_chr = NULL;
	sel_line = NULL;
	sel_seg = NULL;

}

void Xd6HtmlFrame::cut(int fast)
{
	int i = 0;
	int m = 0;
	int n = 0;
	int o = 0;
	char *cursor;	
	Xd6HtmlSegment *cursor_seg;
	Xd6HtmlBlock *cursor_block;

	modified = 1;

	if (!sel_chr || !cur_chr) {
		if (cur_chr && cur_seg == focus && cur_seg->style & DISPLAY &&
			((Xd6HtmlDisplay*)cur_seg)->display == DISPLAY_TABLE) 
		{
			((Xd6HtmlTagTable*)cur_seg)->cut();
		}
		return;
	}
	
	if (cur_chr == sel_chr) {
		return;
	}
	damage(FL_DAMAGE_CHILD);

	if (sel_block->id > cur_block->id) {
		n = cur_block->id;
		m = sel_block->id;
		cursor = cur_chr;
		cursor_seg = cur_seg;
		cursor_block = cur_block;
		cur_block->cut_end(cur_line, cur_seg, cur_chr);
		sel_block->cut_begin(sel_line, sel_seg, sel_chr);
	} else if (sel_block->id < cur_block->id) {
		n = sel_block->id;
		m = cur_block->id;
		cursor = sel_chr;
		cursor_seg = sel_seg;
		cursor_block = sel_block;
		sel_block->cut_end(sel_line, sel_seg, sel_chr);
		cur_block->cut_begin(cur_line, cur_seg, cur_chr);
	} else {
		n = m = sel_block->id;
		cursor = sel_block->cut_middle(sel_line, sel_seg, sel_chr,
			cur_line, cur_seg, cur_chr);
		if (cursor == sel_chr) {
			cursor_seg = sel_seg;
			cursor_block = sel_block;
		} else {
			cursor_seg = cur_seg;
			cursor_block = cur_block;
		}
	}

	for (i  = n + 1; i < m; i++) {
		delete(blocks[i]);
		blocks[i] = NULL;
	}

	if (n < m) {
		o = n + 1;
		merge_blocks(cursor_block, blocks[m], &cursor);
		for(i = m + 1; i < nb_blocks; i++) {
			blocks[o] = blocks[i];
			blocks[o]->id = o;
			o++;
		}
		nb_blocks = o;
	//	fast = 0;
	}
	cursor_block->measure();
	cursor_block->create_lines();
	cursor_block->align_lines();
	if (cursor_block->width > width) width = cursor_block->width;
	if (!fast) create_pages();
	set_cut_cursor(cursor, cursor_seg, cursor_block);
}

int Xd6HtmlFrame::get_tab(int l, int s)
{
	int i = 0;

	if (s & TEXT_ALIGN_RIGHT) {
		while (tab_stop && tab_stop[i]) {
			i++;
		}
		while (i > 0) {
			i--;
			if (tab_stop[i] < (rule_width - l)) {
				return ((rule_width - l) - tab_stop[i]);
			}
		}
		return ((rule_width - l) % 80) + 1;
	}
	while (tab_stop && tab_stop[i]) {
		if (tab_stop[i] > l) return (tab_stop[i] - l);
		i++;
	}
	return (80 - (l % 80));
}

void Xd6HtmlFrame::zoom(int z) 
{
	switch(z) {
	case 200:
		Xd6HtmlSegment::sizes = Xd6HtmlSegment::sizes2;
		break;
	case 300:
		Xd6HtmlSegment::sizes = Xd6HtmlSegment::sizes3;
		break;
	case 150:
		Xd6HtmlSegment::sizes = Xd6HtmlSegment::sizes4;
		break;
	default:
		Xd6HtmlSegment::sizes = Xd6HtmlSegment::sizes1;
	}
}


/*
 * "$Id: $"
 */

