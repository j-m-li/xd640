/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2001 O'ksi'D
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
Xd6HtmlFrameTextEngine Xd6HtmlFrame::text_engine = 
	&Xd6HtmlFrame::def_text_engine;
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
	blocks[0]->add_segment((char*)malloc(1), 0, &Xd6XmlStl::def);
	blocks[0]->measure();
	blocks[0]->create_lines();
	create_pages();

	background = 7;
	
	off_screen = 0;
	
	if (!downloader) {
		downloader = new Xd6HtmlDownload();
	}
	quote = "";

	is_hot = 0;
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

int Xd6HtmlFrame::def_text_engine(int e, Xd6HtmlFrame *frame,
	Xd6HtmlBlock *block,
        Xd6HtmlLine *line, Xd6HtmlSegment *seg, char *text)
{
	return 0;
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
	damage(DAMAGE_ALL);
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
	width += 2;
	create_pages();
	if (cur_chr) set_cut_cursor(cur_chr, cur_seg, cur_block);
}

int  Xd6HtmlFrame::break_line(Xd6HtmlLine *l, int height)
{
	// try to split line
	int i;
	int ret = 0;
	for (i = 0; i < l->nb_segs; i++) {
		if (l->segs[i]->stl->display) {
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
	int list_depth = 0;
	unsigned short list_num[128];

	list_num[list_depth] = 0;
 


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
		while (b != blks[i]) {
			if (blks[i]->stl->blockquote > 0) {
				Xd6XmlStl *bs = blks[i]->stl;
				for (;list_depth < bs->blockquote;) {
					list_num[++list_depth] = 0;
				}
				if (list_depth >  bs->blockquote) {
					list_depth = bs->blockquote;
				}
				if (bs->list != LIST_NONE) {
					blks[i]->list_id = 
						++list_num[bs->blockquote];
				}
			} else {
				list_depth = 0;
				list_num[0] = 0;				
			}
			i++;
		}
		if (i > 0) {
			i--;
			height = blks[i]->top;
			op = height / page_height;
			max_h = height + 4000;
			need_paging = 1;
			if (blks[i]->stl->blockquote > 0) {
				Xd6XmlStl *bs = blks[i]->stl;
				if (bs->list != LIST_NONE) {
					--list_num[bs->blockquote];
				}
			}
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
		
		if (b->stl->blockquote > 0) {
			Xd6XmlStl *bs = b->stl;
			for (;list_depth < bs->blockquote;) {
				list_num[++list_depth] = 0;
			}
			if (list_depth >  bs->blockquote) {
				list_depth = bs->blockquote;
			}
			if (bs->list != LIST_NONE) {
				b->list_id = ++list_num[bs->blockquote];
			}
		} else {
			list_depth = 0;
			list_num[0] = 0;				
		}

		height += b->stl->top_margin;

		if (height > max_h) {
			height += b->height;
			continue;
		}

		if (wysiwyg && b->stl->page_break) {
			int pad;
			pad = page_height - height % page_height;
			height += pad;
			op = height  / page_height;
		}
		b->left = b->stl->blockquote * BLOCKQUOTEWIDTH;
		b->top = height;
		b->height = 0;
		b->width = page_width - b->left;
		if (b->stl->text_align == TEXT_ALIGN_RIGHT) {
			b->left = 0;
		} else if (b->stl->text_align == TEXT_ALIGN_CENTER) {
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
	//printf("DAMAGE_ALL pageing\n");
	//damage(DAMAGE_ALL);
}

Xd6HtmlDisplay *Xd6HtmlFrame::special_elements(Xd6XmlTreeElement *elem, Xd6HtmlFrame*cp)
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

void Xd6HtmlFrame::add_segment(Xd6HtmlBlock *b, char *t, int l, Xd6XmlStl *s)
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

	
	if (!(elem->stl->is_block || elem->stl->is_inline)) return;

	if (elem->stl->is_block) {
		add_block();
		blocks[nb_blocks - 1]->stl = elem->stl;
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
				elem->stl);  
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

	if (!cp) cp = this;

	if (elem->display) {
		Xd6HtmlDisplay::pre_process(elem, this);
	}

	if (!(elem->stl->is_block || elem->stl->is_inline)) return;

	if (nb_blocks < 1) {
		add_block();
		if (elem->stl->display) {
			elem->stl->para.copy(elem->stl);
			elem->stl->para.display = 0;
			blocks[nb_blocks - 1]->stl = 
				elem->stl->get_style(&elem->stl->para);
		} else {
			blocks[nb_blocks - 1]->stl = elem->stl;
		}
	}

	if (elem->stl->is_block) {
		Xd6HtmlBlock *b = blocks[nb_blocks - 1];
		Xd6XmlStl *ls = b->stl;
		int need_margin = 1;

		if ((b->nb_segs > 0) || (b->stl->page_break)) {
			add_block();
			need_margin = 0;
		}
		if (elem->stl->display) {
			elem->stl->para.copy(elem->stl);
			elem->stl->para.display = 0;
			blocks[nb_blocks - 1]->stl = 
				elem->stl->get_style(&elem->stl->para);
		} else {
			blocks[nb_blocks - 1]->stl = elem->stl;
		}

		if (elem->stl->page_break) {
			add_block();
			need_margin = 0;
			elem->stl->para.copy(elem->stl);
			elem->stl->para.page_break = 0;
			blocks[nb_blocks - 1]->stl =
				elem->stl->get_style(&elem->stl->para);
		}

		if (need_margin && ls->top_margin > elem->stl->top_margin) {
			Xd6XmlStl *s = blocks[nb_blocks - 1]->stl;
			s->para.copy(s);
			s->para.top_margin = ls->top_margin;
			blocks[nb_blocks - 1]->stl = s->get_style(&s->para);
		}
#if 0
		if (elem->stl->top_margin /*&& (b->nb_segs > 1 || 
			(b->nb_segs == 1 && b->segs[0]->len > 0))*/) 
		{
			elem->stl->para.copy(elem->stl);
			elem->stl->para.display = 0;
			elem->stl->para.top_margin = 0;
			elem->stl->para.page_break = 0;
			elem->stl->para.list = LIST_NONE;
			elem->stl->para.a_link = 0;
			elem->stl->para.is_block = 0;
			elem->stl->para.is_inline = 1;
			elem->stl->para.font_size = 0;
			add_segment(blocks[nb_blocks - 1], (char*)malloc(1), 0,
				elem->stl->get_style(&elem->stl->para));

			add_block();
			if (elem->stl->display) {
				elem->stl->para.copy(elem->stl);
				elem->stl->para.display = 0;
				blocks[nb_blocks - 1]->stl = 
					elem->stl->get_style(&elem->stl->para);
			} else {
				blocks[nb_blocks - 1]->stl = elem->stl;
			}
		}
#endif
	}

	if ((elem->stl->display) && (elem->display != DISPLAY_TABLE_CELL)) {
		special_elements(elem, (Xd6HtmlFrame*)cp);
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
		
			if (b->nb_segs == 0) {
				Xd6XmlStl *s = elem->stl;
				s->para.copy(s);
				s->para.top_margin = 0;
				b->stl = s->get_style(&s->para);
			}
			if (e->stl->is_block && (b->nb_segs > 1 ||
				(b->nb_segs == 1 && b->segs[0]->len > 0) ||
				e->stl->display)) 
			{
				add_block();
				elem->stl->para.copy(elem->stl);
				elem->stl->para.display = 0;
				elem->stl->para.page_break = 0;
				elem->stl->para.list = LIST_NONE;
				elem->stl->para.top_margin = 0;
				blocks[nb_blocks - 1]->stl = 
					elem->stl->get_style(&elem->stl->para);

				if (e->stl->top_margin) {
					elem->stl->para.copy(elem->stl);
					elem->stl->para.display = 0;
					elem->stl->para.top_margin = 0;
					elem->stl->para.page_break = 0;
					elem->stl->para.list = LIST_NONE;
					elem->stl->para.a_link = 0;
					elem->stl->para.is_block = 0;
					add_segment(blocks[nb_blocks - 1], 
						(char*)malloc(1), 0,
						elem->stl->get_style(
							&elem->stl->para));
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
			elem->stl->para.copy(elem->stl);
			elem->stl->para.display = 0;
			elem->stl->para.top_margin = 0;
			elem->stl->para.page_break = 0;
			elem->stl->para.list = LIST_NONE;
			elem->stl->para.a_link = 0;
			elem->stl->para.is_block = 0;
			add_segment(blocks[nb_blocks - 1], txt->data, txt->len,
					elem->stl->get_style(&elem->stl->para));
			txt->data = NULL;
			txt->len = 0;
		}
	}
	if (elem->stl->is_block && elem->nb_children > 0) {
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
                        delete((Xd6XmlTreeText*)
				elem->children[elem->nb_children]);
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
/*
	*b = NULL;
	*l = NULL;
	*s = NULL;
	*c = NULL;
*/	
	for (i = 0; i < nb_blocks; i++) {
		if (Y <= blocks[i]->top + blocks[i]->height + y) 
		{
			/*
			while ((i > 0) && (Y < top)) {
				i--;
				if (blocks[i]->height) break;
			}
			*/
			if (blocks[i]->nb_segs < 1) return;
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

void Xd6HtmlFrame::set_cut_cursor(char *cursor, Xd6HtmlSegment *s, 
	Xd6HtmlBlock *b)
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
				b->flags |= DAMAGE_CHILD;
				l->flags |= DAMAGE_CHILD;
				s->flags |= DAMAGE_ALL;
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
		if (cur_chr && cur_seg == focus && cur_seg->stl->display &&
			((Xd6HtmlDisplay*)cur_seg)->display == DISPLAY_TABLE) 
		{
			damage(DAMAGE_CHILD);
			cur_line->flags |= DAMAGE_ALL;
			cur_block->flags |= DAMAGE_CHILD;
			((Xd6HtmlTagTable*)cur_seg)->cut();
		}
		return;
	}
	
	if (cur_chr == sel_chr) {
		return;
	}
	damage(DAMAGE_CHILD);

//	printf("%d %d '%s' '%s' %d %d / %d %d\n", cur_seg->id, sel_seg->id, cur_chr, sel_chr,
//		cur_seg->style & DISPLAY, sel_seg->style & DISPLAY, cur_seg->len, sel_seg->len); 
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
	clean_block(cursor_block, &cursor_seg, &cursor);
	cursor_block->measure();
	cursor_block->create_lines();
	cursor_block->align_lines();
	if (cursor_block->width > width - 2) width = cursor_block->width + 2;
	if (!fast) create_pages();
	set_cut_cursor(cursor, cursor_seg, cursor_block);
}

int Xd6HtmlFrame::get_tab(int l, Xd6XmlStl *s)
{
	int i = 0;

	if (s->text_align == TEXT_ALIGN_RIGHT) {
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

