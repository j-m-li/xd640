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
#include <FL/Fl.h>
#include <FL/Fl_Widget.h>
#include <FL/fl_draw.h>
#include <FL/fl_utf8.h>
#include "Xd6HtmlFrame.h"
#include "Xd6HtmlTagA.h"
#include "Xd6HtmlTagTable.h"
#include "Xd6XmlParser.h"
#include "Xd6HtmlView.h"
#include <sys/stat.h>
#include <FL/Fl_Window.h>

#define _(String) gettext((String))

int Xd6HtmlFrame::handle_keys()
{
	int nbb;
	int nbl;
	int lh;
	int lid;
	int bh;
	int bid;
	int lw;
	int lnbc;
	int lnbc2 = -1;
	int lnbc3 = -1;

	if (Fl::e_length < 1 || !Fl::e_text[0]) return 0;
	if (!cur_chr) return 0;	

	bh = cur_block->height;
	bid = cur_block->id;
	nbl = cur_block->nb_lines;
	lh = cur_line->height;
	lid = cur_line->id;
	lw = cur_line->width;
	nbb = nb_blocks;
	lnbc = cur_line->nb_segs;
	if (lid > 0) {
		lnbc2 = cur_block->lines[lid-1]->nb_segs;
	}
	cut();
	if (!cur_chr) return 0;
	insert_text(Fl::e_text, Fl::e_length);
//	Fl::e_text[0] = 0;
//	Fl::e_length = 0;

	if (nbl == cur_block->nb_lines && cur_line->id > 0) {
		lnbc3 = cur_block->lines[cur_line->id-1]->nb_segs;
	}
	if (cur_line->width != lw || cur_block->stl->text_align ||
		cur_seg->stl->rtl_direction) 
	{
		cur_line->flags |= DAMAGE_ALL;
	}
	if (bh != cur_block->height || bid != cur_block->id || nbb != nb_blocks 
		|| lnbc != cur_line->nb_segs || lnbc3 != lnbc2)
	{
		damage(DAMAGE_ALL);
	} else {
		damage(DAMAGE_CHILD);
	}
	if (cur_block->stl->text_align == TEXT_ALIGN_CENTER) {
		cur_block->flags |= DAMAGE_ALL;
	}
	return 1;
}

int Xd6HtmlFrame::handle_click(int e, Xd6HtmlFrame *frame, Xd6HtmlBlock *block,
	Xd6HtmlLine *line, Xd6HtmlDisplay *seg, char *chr) 
{
	static Xd6HtmlTagA *pushed = NULL;
	Xd6HtmlDisplay *d = frame;
	Xd6HtmlTagA *a_tag;

	if (frame->editor) return 0;
	if (!(seg->display == DISPLAY_A_LINK)) return 0;
	if (e == FL_PUSH) {
		pushed = (Xd6HtmlTagA*)seg;
		return 1;
	}
	if (e != FL_RELEASE) return 0;
	a_tag = (Xd6HtmlTagA*) seg;
	if (pushed != a_tag) {
		pushed = 0;
		return 0;
	}
	while (d->display != DISPLAY_TOP_FRAME &&
		d->display != DISPLAY_FRAME &&
		d->display != DISPLAY_IFRAME)
	{
		d = d->parent;
	}
	downloader->request(a_tag->href, a_tag->target, NULL, (Xd6HtmlFrame*)d);
	return 1;
}


void Xd6HtmlFrame::load(void) 
{
	class Xd6XmlParser *parser;
	parser = new Xd6XmlParser();	
	parser->parse_file(file);
	while (nb_blocks > 0) {
		delete(blocks[--nb_blocks]);
	}
	cur_chr = NULL;
	sel_chr = NULL;
	hscroll = 0;
	vscroll = 0;
	if (status_bar) {
		status_bar->value(_("Parsing file..."));
		Fl::flush();
	}
	tree2block(parser->tree->root);
	measure();
	delete(parser);
	damage(DAMAGE_ALL);
	if (status_bar) {
		status_bar->value(" ");
		Fl::flush();
	}
	Fl::redraw();
}


int Xd6HtmlFrame::handle_external(int e, int x, int y)
{
	Xd6HtmlBlock *block = NULL;
	Xd6HtmlLine *line = NULL;
	Xd6HtmlSegment *seg = NULL;
	char *chr = NULL;

       	find_pos(x, y, &block, &line, &seg, &chr);
	if (!chr) return 0;
	if ((seg->stl->a_link)) {
		int i;
		Xd6HtmlTagA *a_tag = NULL;;
		for (i = 0; i < block->nb_segs; i++) {
			Xd6HtmlSegment *s;
			s = block->segs[i];
			if (s == seg) break;
			if (s->stl->display && 
				((Xd6HtmlDisplay*)s)->display == DISPLAY_A_LINK)
			{
				a_tag = (Xd6HtmlTagA*) s;
			}
		}
		if (!a_tag) return 0;
		return engine(e, this, block, line, a_tag, a_tag->text);
	} else if (seg->stl->display) {
		return engine(e, this, block, line, (Xd6HtmlDisplay*)seg, chr);
	} else {
		return text_engine(e, this, block, line, seg, chr);
	}
	return 0;
}

int Xd6HtmlFrame::handle(int e, int x, int y)
{
	int ret = 0;
	static int ox, oy;
	Xd6HtmlLine *l;
	Xd6HtmlBlock *b;
	int ov, oh;
	int ex, ey;
	char *osel_chr;
	
	if (focus != cur_seg && focus != this) focus = NULL;

	if (focus && cur_chr && focus != this && e != FL_PUSH && 
		!(flag & CLICK_OVER_SCROLL)) 
	{
		int X, Y;
		X = x + left + hscroll + cur_block->left + 
				cur_line->left;
		Y = y + top + vscroll + cur_block->top + cur_line->top;
		ret = focus->handle(e, X, Y);	
		if (e == FL_HIDE_CURSOR) {
			focus = NULL;
			ret = 1;
		}
		if (ret) {
			if (focus && focus->display == DISPLAY_TABLE &&
				((Xd6HtmlTagTable*)focus)->need_resize) 
			{
				((Xd6HtmlTagTable*)focus)->need_resize = 0;
				cur_block->measure();
				cur_block->create_lines();
				cur_block->align_lines();
				create_pages();
				damage(DAMAGE_ALL);
			}
			damage(DAMAGE_CHILD);
			if (is_hot) {
				is_hot = 0;
				if (check_hotspot(x, y)) {
					printf("DAMAGE_ALL handle hot\n");
					damage(DAMAGE_ALL);
				}
			}
			return ret;
		}
		if (e == FL_KEYBOARD) {
			focus->handle(FL_HIDE_CURSOR, X, Y);	
			focus = this;
		}
	}

	if (e == FL_DRAG) {
		const Fl_Window *w;
		Fl::get_mouse(ex, ey);
		w = Fl_Window::current();
		ex -= w->x();
		ey -= w->y();
	} else {
		ex = Fl::event_x();
		ey = Fl::event_y();
	}
	if (height > max_height || width > max_width) {
		ret = handle_scroll(e, ex, ey, x, y);
		if (ret) {
			if (need_paging) {
				create_pages();
				if (focus && focus != this) {
					focus->handle(FL_HIDE_CURSOR, 0, 0);	
				}
				focus = NULL;
			}
			return ret;
		}
	}


	switch (e) {
	case FL_FOCUS:
		focus = this;
		ret = handle_external(e, ex - x, ey - y);
		return 1;
	case FL_UNFOCUS:
		return 1;
	case FL_HIDE_CURSOR:
		if (focus == this) {
			if (cur_chr) {
				cur_block->flags |= DAMAGE_CHILD;
				cur_line->flags |= DAMAGE_ALL;
			} 
			if (sel_chr) {
				damage(DAMAGE_ALL);
			}
			focus = NULL;
		} else if (focus) {
			focus->handle(FL_HIDE_CURSOR, 0, 0);	
		}
		focus = NULL;
		damage(DAMAGE_CHILD);
		ret = handle_external(e, ex - x, ey - y);

		return 1;
	case FL_PUSH:

		if (sel_chr) damage(DAMAGE_ALL);
		sel_block = NULL;
                sel_line = NULL;
                sel_seg = NULL;
                sel_chr = NULL;
        	ox = Fl::event_x();
        	oy = Fl::event_y();

		if (need_paging) create_pages();

		l = cur_line;
		if (focus && focus != this) {
			focus->handle(FL_HIDE_CURSOR, 0, 0);
		} else {
			if (cur_chr) {
				cur_block->flags |= DAMAGE_CHILD;
				cur_line->flags |= DAMAGE_ALL;
			}
		}
		damage(DAMAGE_CHILD);
        	find_pos(ex - x, ey - y, &cur_block, &cur_line, 
			&cur_seg, &cur_chr);
 		if (cur_chr && cur_seg->stl->display) {
			int X, Y;
			X = x + left + hscroll + cur_block->left + 
				cur_line->left;
			Y = y + top + vscroll + cur_block->top + cur_line->top;
			Xd6HtmlDisplay *di = (Xd6HtmlDisplay*) cur_seg;
			if (di->handle(FL_PUSH, X, Y)) {
				focus = di;
			} else {
				focus = this;
			}
		} else {
			focus = this;
			if (cur_chr && Fl::event_clicks() == 1) {
				cur_chr = cur_seg->text;
				sel_chr = cur_seg->text + cur_seg->len;
				sel_seg = cur_seg;
				sel_block = cur_block;
				sel_line = cur_line;
				if (cur_seg->len > 0 && (*(sel_chr-1)) == ' ') {
					sel_chr--;
				}
				cur_block->flags |= DAMAGE_CHILD;
				cur_line->flags |= DAMAGE_ALL;
			}
			if (cur_chr && Fl::event_clicks() == 2) {
				cur_seg = cur_block->segs[0];
				cur_line = cur_block->lines[0];
				cur_chr = cur_seg->text;
				sel_seg = cur_block->segs[
					cur_block->nb_segs - 1];
				sel_chr = sel_seg->text + sel_seg->len;
				sel_block = cur_block;
				sel_line = cur_block->lines[
					cur_block->nb_lines - 1];
				cur_block->flags |= DAMAGE_ALL;
			}
		}
		ret = handle_external(e, ex - x, ey - y);
		if (Fl::event_clicks() == 2) Fl::event_clicks(0);
		return 1;
	case FL_DRAG:
		if (!(flag & CLICK_OVER_SCROLL) &&
			(ex - ox - 3 > 0 ||
			ex - ox  + 3 < 0 ||
			ey - oy - 3 > 0 ||
			ey - oy + 3 < 0))
		{
			char *oc;
			Xd6HtmlSegment *s;
			Xd6HtmlBlock *b;
			
			oc = sel_chr;
			l = sel_line;
			b = sel_block;
			s = sel_seg;

			find_pos(ex - x, ey - y, &sel_block,
				&sel_line, &sel_seg, &sel_chr);
			if (oc == sel_chr) {
				if (ey - y < top) {
					damage(DAMAGE_ALL);
					vscroll += 20;
				}
				if (ey - y> top + max_height) {
					damage(DAMAGE_ALL);
					vscroll -= 20;
				}
        			if (vscroll <= -(height - max_height + 15)) {
                			vscroll = -(height - max_height + 15);
				}
				if (vscroll > 0) vscroll = 0;
				return 1;
			}
			if (sel_chr) {
				ov = vscroll;
				oh = hscroll;
				auto_scroll(-1, -1);
				auto_scroll(1, 1);
				if (vscroll - ov > max_height) {
					vscroll = ov + max_height;
					sel_chr = oc; sel_line = l;
					sel_block = b; sel_seg = s;
				} else if (ov - vscroll > max_height) {
					vscroll = ov - max_height;
					sel_chr = oc; sel_line = l;
					sel_block = b; sel_seg = s;
				}
				if (ov != vscroll || oh != hscroll) {
					ox = ex;
					oy = ey;
					damage(DAMAGE_ALL);
				}
			}
			if (!l || l != sel_line) {
				damage(DAMAGE_ALL);
			} else {
				damage(DAMAGE_CHILD);
				l->flags |= DAMAGE_ALL;
				sel_block->flags |= DAMAGE_CHILD;
			}
		}
		ret = handle_external(e, ex - x, ey - y);
		return 1;
	case FL_MOUSEWHEEL:
		if (display != DISPLAY_TOP_FRAME) return 0;
		vscroll -= Fl::e_dy * 40;
        	if (vscroll <= -(height - max_height + 15)) {
                	vscroll = -(height - max_height + 15);
		}
		if (vscroll > 0) vscroll = 0;
		damage(DAMAGE_ALL);
		ret = handle_external(e, ex - x, ey - y);
		return 1;
	case FL_RELEASE:
		if (sel_chr && cur_chr) select_to_buffer();
		ret = handle_external(e, ex - x, ey - y);
		return 1;
	case FL_KEYBOARD:
		osel_chr = sel_chr;
		if (Fl::e_state & (FL_ALT|FL_CTRL)) {
			ret = handle_external(e, ex - x, ey - y);
			switch (Fl::event_key()) {
			case FL_End:
			case FL_Home:
			case FL_Left:
			case FL_Up:
			case FL_Right:
			case FL_Down:
				break;
			default:
				return ret;
			}
		}
		b = cur_chr ? cur_block : NULL;
		ret = handle_arrows();
		if (Fl::event_key() == FL_End) {
			if (Fl::e_state & FL_CTRL) {
				cursor_to_end();
			} else {
				if (cur_chr) {
					cur_seg = cur_line->segs[
						cur_line->nb_segs-1];
					cur_chr = cur_seg->text + 
						cur_seg->len;
				}
			}
			auto_scroll(-1, -1);
			auto_scroll(1, 1);
			ret = 1;
		} else if (Fl::event_key() == FL_Home) {
			if (Fl::e_state & FL_CTRL) {
				cursor_to_begin();
			} else {
				if (cur_chr) {
					cur_seg = cur_line->segs[0];
					cur_chr = cur_seg->text;
				}
			}
			auto_scroll(-1, -1);
			auto_scroll(1, 1);
			ret = 1;
		}
		if (ret) {
			if (osel_chr && !sel_chr) damage(DAMAGE_ALL);
			if (need_paging) {
				create_pages();
				damage(DAMAGE_ALL);
			} else {
				if (b) {
					b->flags |= DAMAGE_ALL;
					damage(DAMAGE_CHILD);
				} else {
					damage(DAMAGE_ALL);
				}
			}
			return ret;
		}
		if (editor) {
			ret = handle_keys();
			if (osel_chr && !sel_chr) damage(DAMAGE_ALL);
			if (ret) return ret;
		}
		ret = handle_external(e, ex - x, ey - y);
		if (Fl::e_length == 0) ret = 1;
		break;
	case FL_PASTE:
		if (editor) {
			ret = handle_keys();
			damage(DAMAGE_ALL);
		}
		return ret;
	default:
		ret = handle_external(e, ex - x, ey - y);
	};
	return ret;
}

void Xd6HtmlFrame::move_cursor(int x, int y)
{
	char *c;
	Xd6HtmlSegment *s;
	Xd6HtmlLine *l;
	Xd6HtmlBlock *b;

	if (!(Fl::e_state & (FL_SHIFT))) {
		sel_block = NULL;
		sel_line = NULL;
		sel_seg = NULL;
		sel_chr = NULL;
	} else if (!sel_chr) {
		sel_chr = cur_chr;
		sel_seg = cur_seg;
		sel_line = cur_line;
		sel_block = cur_block;
	}
	

	if (!cur_chr) return;
	
	c = cur_chr;
	s = cur_seg;
	l = cur_line;
	b = cur_block;
 
	if (cur_chr) {
		cur_block->flags |= DAMAGE_CHILD;
		cur_line->flags |= DAMAGE_ALL;
	}
	if (y > 0) {
		move_cursor_down();
		if (cur_chr && cur_seg->stl->display &&
			((Xd6HtmlDisplay*)cur_seg)->display == DISPLAY_TABLE)
		{	
			((Xd6HtmlTagTable*)cur_seg)->take_focus(0);
			focus = (Xd6HtmlDisplay*)cur_seg;
		} else {
			auto_scroll(0, 1, 1);
		}
	} else if (y < 0) {
		move_cursor_up();
		if (cur_chr && cur_seg->stl->display &&
			((Xd6HtmlDisplay*)cur_seg)->display == DISPLAY_TABLE)
		{
			((Xd6HtmlTagTable*)cur_seg)->take_focus(1);
			focus = (Xd6HtmlDisplay*)cur_seg;
		} else {
			auto_scroll(0, -1, 1);
		}
	}

	if (x > 0) {
		move_cursor_right();
		auto_scroll(1, 1, 1);
	} else if (x < 0) {
		move_cursor_left();
		auto_scroll(-1, -1, 1);
	}
	
	if (!cur_chr) {
		cur_chr = c;
		cur_seg = s;
		cur_line = l;
		cur_block = b;
	}
	if (cur_chr) {
		cur_block->flags |= DAMAGE_CHILD;
		cur_line->flags |= DAMAGE_ALL;
	}
}

void Xd6HtmlFrame::move_cursor_up()
{
	int x, w;

	cur_seg->set_font();
	x = cur_block->left + cur_line->left + cur_seg->left;
	w = (int) fl_width(cur_seg->text, cur_chr - cur_seg->text);
	if (cur_seg->stl->rtl_direction) {
		x += cur_seg->width - w;
	} else {
		x += w;
	}
	if (cur_line->id > 0) {
		cur_line = cur_block->lines[cur_line->id - 1];
		cur_line->find_pos(0, x, &cur_seg, &cur_chr);
		return;
	}
	while (cur_block->id > 0) {
		cur_block = blocks[cur_block->id - 1];
		cur_chr = NULL;
		if (cur_block->nb_lines > 0) {
			cur_line = cur_block->lines[cur_block->nb_lines - 1];
			if (cur_line->nb_segs > 0) {
				cur_line->find_pos(0, x,  &cur_seg, &cur_chr);
				return;
			}
		 	while (cur_line->id > 0) {
				cur_line = cur_block->lines[cur_line->id - 1];
				if (cur_line->nb_segs > 0) {
					cur_line->find_pos(0, x, 
						&cur_seg, &cur_chr);
					return;
				}
			}
		}
	}
}

void Xd6HtmlFrame::move_cursor_down()
{
	int x, w;

	cur_seg->set_font();
	x = cur_block->left + cur_line->left + cur_seg->left;
	w = (int) fl_width(cur_seg->text, cur_chr - cur_seg->text);
	if (cur_seg->stl->rtl_direction) {
		x += cur_seg->width - w;
	} else {
		x += w;
	}
	if (cur_line->id < cur_block->nb_lines - 1) {
		cur_line = cur_block->lines[cur_line->id + 1];
		cur_line->find_pos(0, x, &cur_seg, &cur_chr);
		return;
	}
	while (cur_block->id < nb_blocks - 1) {
		cur_block = blocks[cur_block->id + 1];
		cur_chr = NULL;
		if (cur_block->nb_lines > 0) {
			cur_line = cur_block->lines[0];
			if (cur_line->nb_segs > 0) {
				cur_line->find_pos(0, x, &cur_seg, &cur_chr);
				return;
			}
		 	while (cur_line->id < cur_block->nb_lines - 1) {
				cur_line = cur_block->lines[cur_line->id + 1];
				if (cur_line->nb_segs > 0) {
					cur_line->find_pos(0, x, 
						&cur_seg, &cur_chr);
					return;
				}
			}
		}
	}
}

void Xd6HtmlFrame::move_cursor_right(int r)
{
	int was_display = 0;
	int sid;

	if (!cur_chr) return;

	r = was_display = cur_seg->stl->display;

	if (cur_seg->stl->rtl_direction) {
		if (cur_chr > cur_seg->text) {
			go_char_back();
			return;
		}
	} else {
		if (cur_chr < cur_seg->text + cur_seg->len) {
			go_char_forward();
			return;
		}
	}
	
	sid = cur_line->get_seg_id(cur_seg);
	if (sid < cur_line->nb_segs - 1) {
               	cur_seg = cur_line->segs[sid + 1];
		cur_chr = cur_seg->text;
               	if (cur_seg->stl->rtl_direction) {
                       	cur_chr += cur_seg->len;
               	}
		if (!r) move_cursor_right(was_display);	
               	return;
	}

	if (cur_line->id < cur_block->nb_lines - 1) {
		 cur_line = cur_block->lines[cur_line->id + 1];
		while (cur_line->nb_segs < 1 && 
			cur_line->id < cur_block->nb_lines - 1)
		{
               		cur_line = cur_block->lines[cur_line->id + 1];
		}
	}

	if (cur_line->id < cur_block->nb_lines - 1) {
		if (cur_line->nb_segs > 0) {
                	cur_seg = cur_line->segs[0];
			cur_chr = cur_seg->text;
                	if (cur_seg->stl->rtl_direction) {
                        	cur_chr += cur_seg->len;
                	}
			auto_scroll(-1, 0);
		} else {
			cur_seg = NULL;
			cur_chr = NULL;
		}
		if (!r) move_cursor_right(was_display);
                return;
        } 

	if (cur_block && cur_block->id < nb_blocks - 1) {
		do {
			cur_block = blocks[cur_block->id + 1];
		} while (cur_block->nb_segs < 1 && 
			cur_block->id < nb_blocks -  1);

		if (cur_block->nb_lines > 0) {
			cur_line = cur_block->lines[0];
			if (cur_line->nb_segs > 0) {
				cur_seg = cur_line->segs[0];
				cur_chr = cur_seg->text;
				if (cur_seg->stl->rtl_direction) {
					cur_chr += cur_seg->len;
				}
				auto_scroll(-1, 0);
				return;
			} else {
				cur_seg = NULL;
				cur_chr = NULL;
			}
		} else {
			cur_line = NULL;
			cur_seg = NULL;
			cur_chr = NULL;
		}
		if (!r) move_cursor_right(was_display); 
		return;
	}
}

void Xd6HtmlFrame::go_next_seg()
{
	if (!cur_chr) return;

	if (cur_seg->id < cur_block->nb_segs - 1) {
		cur_seg = cur_block->segs[cur_seg->id+1];
		cur_chr = cur_seg->text;
	} else {
		Xd6HtmlBlock *b;
		b = cur_block;
		while (cur_block->id < nb_blocks - 1) {
			cur_block = blocks[cur_block->id+1];
			if (cur_block->nb_segs > 0) {
				cur_seg = cur_block->segs[0];
				cur_chr = cur_seg->text;
				b = cur_block;
				break;
			}
		}
		cur_block = b;
	}
	set_cut_cursor(cur_chr, cur_seg, cur_block);
	auto_scroll(-1, -1);
	auto_scroll(1, 1);
}

void Xd6HtmlFrame::go_previous_seg()
{
	if (!cur_chr) return;

	if (cur_seg->id > 0) {
		cur_seg = cur_block->segs[cur_seg->id-1];
		cur_chr = cur_seg->text;
	} else {
		Xd6HtmlBlock *b;
		b = cur_block;
		while (cur_block->id > 0) {
			cur_block = blocks[cur_block->id-1];
			if (cur_block->nb_segs > 0) {
				cur_seg = cur_block->segs[cur_block->nb_segs-1];
				cur_chr = cur_seg->text;
				b = cur_block;
				break;
			}
		}
		cur_block = b;
	}
	set_cut_cursor(cur_chr, cur_seg, cur_block);
	auto_scroll(-1, -1);
	auto_scroll(1, 1);
}


void Xd6HtmlFrame::move_cursor_left(int r)
{
	int was_display = 0;
	int sid;

	if (!cur_chr) return;
/*
	printf ("in -> %d %d %d %d\n", cur_chr -  cur_seg->text,
			cur_seg->id, cur_line->id, cur_block->id);
*/
	r = was_display = cur_seg->stl->display;

	if (!(cur_seg->stl->rtl_direction)) {
		if (cur_chr > cur_seg->text) {
			go_char_back();
			return;
		}
	} else {
		if (cur_chr < cur_seg->text + cur_seg->len) {
			go_char_forward();
			return;
		}
	}
	
	sid = cur_line->get_seg_id(cur_seg);
	if (sid > 0) {
               	cur_seg = cur_line->segs[sid - 1];
		cur_chr = cur_seg->text;
               	if (!(cur_seg->stl->rtl_direction)) {
                       	cur_chr += cur_seg->len;
               	}
		if (!r) move_cursor_left(was_display);	
               	return;
	}

	
	if (cur_line->id > 0) {
               	cur_line = cur_block->lines[cur_line->id - 1];
		while (cur_line->nb_segs < 1 && cur_line->id > 0) {
               		cur_line = cur_block->lines[cur_line->id - 1];
		}
	}

	if (cur_line->id > 0) {

		if (cur_line->nb_segs > 0) {
                	cur_seg = cur_line->segs[cur_line->nb_segs - 1];
			cur_chr = cur_seg->text;
                	if (!(cur_seg->stl->rtl_direction)) {
                        	cur_chr += cur_seg->len;
                	}
			auto_scroll(1, 0);
		} else {
			cur_seg = NULL;
			cur_chr = NULL;
		}
		if (!r) move_cursor_left(was_display);
                return;
        } 

	if (cur_block->id > 0) {
		do {
			cur_block = blocks[cur_block->id - 1];
		} while (cur_block->nb_segs < 1 && cur_block->id > 0);

		if (cur_block->nb_lines > 0) {
			cur_line = cur_block->lines[cur_block->nb_lines - 1];
			if (cur_line->nb_segs > 0) {
				cur_seg = cur_line->segs[cur_line->nb_segs - 1];
				cur_chr = cur_seg->text;
				if (!(cur_seg->stl->rtl_direction)) {
					cur_chr += cur_seg->len;
				}
				auto_scroll(1, 0);
				return;
			} else {
				cur_seg = NULL;
				cur_chr = NULL;
			}
		} else {
			cur_line = NULL;
			cur_seg = NULL;
			cur_chr = NULL;
		}
		if (!r) move_cursor_left(was_display); 
		return;
	}
}

void Xd6HtmlFrame::go_char_back()
{
        int i;
        if (cur_chr > cur_seg->text) {
                i = 1;
                while (i < cur_chr - cur_seg->text &&
                        fl_utflen((unsigned char*)cur_chr - i, i) < 1)
                {
                        i++;
                }
                cur_chr -= i;
		//cur_line->style |= DAMAGE_ALL;
		//cur_block->style |= DAMAGE_CHILD;
        }
}

void Xd6HtmlFrame::go_char_forward()
{
        int i;
        if (cur_chr - cur_seg->text < cur_seg->len) {
                i = fl_utflen((unsigned char*)cur_chr,
                        cur_seg->len - (cur_chr - cur_seg->text));
                if (i < 1) i = 1;
                cur_chr += i;
		//cur_line->style |= DAMAGE_ALL;
		//cur_block->style |= DAMAGE_CHILD;
        }
}

int Xd6HtmlFrame::handle_arrows()
{
        switch(Fl::event_key()) {
        case FL_Up:
                move_cursor(0, -1);
                return 1;
        case FL_Down:
                move_cursor(0, 1);
                return 1;
        case FL_Left:
		if (Fl::e_state & FL_CTRL) {
			go_previous_seg();
		} else {
                	move_cursor(-1, 0);
		}
                return 1;
        case FL_Right:
		if (Fl::e_state & FL_CTRL) {
			go_next_seg();
		} else {
                	move_cursor(1, 0);
		}
                return 1;
        default:
                return 0;
        }
}

void Xd6HtmlFrame::auto_scroll(int X, int Y, int flow_cur)
{
        int x, y, h;

        if (sel_chr && !flow_cur) {
                int len = sel_chr - sel_seg->text;
                if (sel_seg->stl->rtl_direction) {
                        x = left + hscroll + sel_block->left +
				sel_line->left + sel_seg->left +
                                sel_seg->width -
                                (int)fl_width(sel_seg->text, len);
                } else {
                        x = left + hscroll + sel_block->left + 
				sel_line->left + sel_seg->left +
                                (int)fl_width(sel_seg->text, len);
                }
                y = top + vscroll + sel_block->top +
			sel_line->top + sel_seg->top;

                h = sel_seg->height;
        } else if (cur_chr) {
                int len = cur_chr - cur_seg->text;
                if (cur_seg->stl->rtl_direction) {
                        x = left + hscroll + cur_block->left +
				cur_line->left + cur_seg->left +
                                cur_seg->width -
                                (int)fl_width(cur_seg->text, len);
                } else {
                        x = left + hscroll + cur_block->left +
				cur_line->left + cur_seg->left +
                                (int)fl_width(cur_seg->text, len);
                }
                y = top + vscroll + cur_block->top +
			cur_line->top + cur_seg->top;
                h = cur_seg->height;
        } else {
                return;
        }
        if (Y < 0 && y < top + 15) {
                vscroll -= y - ( top + 15);
        } else if (Y > 0 && y + h > top + max_height - 30) {
                vscroll += (top + max_height - 30) - (y + h);
        }
        if (X < 0 && x < left + 15) {
                hscroll += left - x + 15;
        } else if (X > 0 && x > left + max_width - 45) {
                hscroll -= x - (left + max_width - 45);
        }
        if (hscroll <= -(width - max_width + 15)) {
                hscroll = -(width - max_width + 15);
        }
        if (hscroll > 0) hscroll = 0;
        if (vscroll <= -(height - max_height + 15)) {
                vscroll = -(height - max_height + 15);
        }
        if (vscroll > 0) vscroll = 0;

	if (height <= max_height) vscroll = 0;
	if (width <= max_width) hscroll = 0;

}

int Xd6HtmlFrame::check_hotspot(int X, int Y)
{
	int r, t, b, l;
	r = X + left + max_width - 15;
	l = X + left;
	t = Y + top;
	b = Y + top + max_height - 15;
	
	if (hot_x >= l && hot_x <= r && hot_y >= t && hot_y <= b) {
		return 0;
	}
	if (hot_y < t) {
		vscroll += t - hot_y;
	} else if (hot_y > b) {
		vscroll -= hot_y - b;
	}
	if (hot_x < l) {
		hscroll += l - hot_x;
	} else if (hot_x > b) {
		hscroll -= hot_x - r;
	}
	if (vscroll > 0) vscroll = 0;
	if (hscroll > 0) hscroll = 0;
 	return 1;
}

void Xd6HtmlFrame::hotspot(int X, int Y)
{
	is_hot = 1;
	hot_x = X;
	hot_y = Y;
}

/*
 * "$Id: $"
 */

