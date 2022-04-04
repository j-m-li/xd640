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
#include "Xd6HtmlTagA.h"
#include "Xd6HtmlTagTable.h"
#include "Xd6XmlParser.h"
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

	if (Fl::e_length < 1) return 0;
	if (!cur_chr) return 0;	

	bh = cur_block->height;
	bid = cur_block->id;
	nbl = cur_block->nb_lines;
	lh = cur_line->height;
	lid = cur_line->id;
	lw = cur_line->width;
	nbb = nb_blocks;

	cut();
	if (!cur_chr) return 0;
	
	insert_text(Fl::e_text, Fl::e_length);
	Fl::e_length = 0;

	if (bh != cur_block->height || bid != cur_block->id || nbb != nb_blocks)
	{
		 damage(FL_DAMAGE_ALL);
	} else if (lh != cur_line->height || lid != cur_line->id ||
		 nbl != cur_block->nb_lines || lw > cur_line->width)
	{
		damage(FL_DAMAGE_BLOCK);
	} else {
		damage(FL_DAMAGE_CHILD);
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
	damage(FL_DAMAGE_ALL);
	if (status_bar) {
		status_bar->value(" ");
		Fl::flush();
	}
	Fl::redraw();
}


int Xd6HtmlFrame::handle_external(int e, int x, int y)
{
	Xd6HtmlBlock *block;
	Xd6HtmlLine *line;
	Xd6HtmlSegment *seg;
	char *chr;

       	find_pos(x, y, &block, &line, &seg, &chr);
	if (!chr) return 0;
	if ((seg->style & A_LINK)) {
		int i;
		Xd6HtmlTagA *a_tag = NULL;;
		for (i = 0; i < block->nb_segs; i++) {
			Xd6HtmlSegment *s;
			s = block->segs[i];
			if (s == seg) break;
			if (s->style & DISPLAY && 
				((Xd6HtmlDisplay*)s)->display == DISPLAY_A_LINK)
			{
				a_tag = (Xd6HtmlTagA*) s;
			}
		}
		if (!a_tag) return 0;
		return engine(e, this, block, line, a_tag, a_tag->text);
	} else if (seg->style & DISPLAY) {
		return engine(e, this, block, line, (Xd6HtmlDisplay*)seg, chr);
	}
	return 0;
}

int Xd6HtmlFrame::handle(int e, int x, int y)
{
	int ret = 0;
	static int ox, oy;
	Xd6HtmlLine *l;
	int ov, oh;
	int ex, ey;

	if (focus != cur_seg && focus != this) focus = NULL;

	if (focus && cur_chr && focus != this && e != FL_PUSH) {
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
			}
			damage(FL_DAMAGE_ARROWS);
			return ret;
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
			if (need_paging) create_pages();
			if (focus && focus != this) {
				focus->handle(FL_HIDE_CURSOR, 0, 0);	
			}
			focus = NULL;
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
			focus = NULL;
		} else if (focus) {
			focus->handle(FL_HIDE_CURSOR, 0, 0);	
		}
		focus = NULL;
		damage(FL_DAMAGE_ARROWS);
		ret = handle_external(e, ex - x, ey - y);

		return 1;
	case FL_PUSH:

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
		}
        	find_pos(ex - x, ey - y, &cur_block, &cur_line, 
			&cur_seg, &cur_chr);
 		if (cur_chr && cur_seg->style & DISPLAY) {
			int X, Y;
			X = x + left + hscroll + cur_block->left + 
				cur_line->left;
			Y = y + top + vscroll + cur_block->top + cur_line->top;
			Xd6HtmlDisplay *di = (Xd6HtmlDisplay*) cur_seg;
			if (di->handle(FL_PUSH, X, Y)) {
				focus = di;
				damage(FL_DAMAGE_ARROWS);
			} else {
				focus = this;
				damage(FL_DAMAGE_ARROWS);
			}
		} else {
			focus = this;
			damage(FL_DAMAGE_ARROWS);
		}
		ret = handle_external(e, ex - x, ey - y);
		return 1;
	case FL_DRAG:
		if (!(flags & CLICK_OVER_SCROLL) &&
			(ex - ox - 3 > 0 ||
			ex - ox  + 3 < 0 ||
			ey - oy - 3 > 0 ||
			ey - oy + 3 < 0))
		{
			const char *oc;

			oc = sel_chr;
			l = sel_line;
			find_pos(ex - x, ey - y, &sel_block,
				&sel_line, &sel_seg, &sel_chr);
			if (oc == sel_chr) return 1;
			if (sel_chr) {
				ov = vscroll;
				oh = hscroll;
				auto_scroll(-1, -1);
				auto_scroll(1, 1);
				if (ov != vscroll || oh != hscroll) {
					damage(FL_DAMAGE_ALL);
				}
			}
			if (l != sel_line) {
				damage(FL_DAMAGE_ALL);
			} else {
				damage(FL_DAMAGE_CHILD);
			}
		}
		ret = handle_external(e, ex - x, ey - y);
		return 1;
	case FL_RELEASE:
		if (sel_chr && cur_chr) select_to_buffer();
		damage(FL_DAMAGE_ARROWS);
		ret = handle_external(e, ex - x, ey - y);
		return 1;
	case FL_KEYBOARD:
		if (Fl::e_state & (FL_ALT|FL_CTRL)) {
			ret = handle_external(e, ex - x, ey - y);
			return ret;
		}
		ret = handle_arrows();
		if (ret) {
			if (need_paging) create_pages();
			damage(FL_DAMAGE_ARROWS);
			return ret;
		}
		if (editor) {
			ret = handle_keys();
			if (ret) return ret;
		}
		ret = handle_external(e, ex - x, ey - y);
		break;
	case FL_PASTE:
		if (editor) {
			ret = handle_keys();
			damage(FL_DAMAGE_ALL);
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
 
	if (y > 0) {
		move_cursor_down();
		auto_scroll(0, 1, 1);
	} else if (y < 0) {
		move_cursor_up();
		auto_scroll(0, -1, 1);
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
}

void Xd6HtmlFrame::move_cursor_up()
{
	int x, w;

	cur_seg->set_font();
	x = cur_block->left + cur_line->left + cur_seg->left;
	w = (int) fl_width(cur_seg->text, cur_chr - cur_seg->text);
	if (cur_seg->style & RTL_DIRECTION) {
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
	if (cur_seg->style & RTL_DIRECTION) {
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
	r = 0;
	if (cur_seg && cur_seg->style & RTL_DIRECTION) {
		if (cur_chr && cur_chr > cur_seg->text) {
			go_char_back();
			return;
		}
	} else if (cur_seg) {
		if (cur_chr && cur_chr < cur_seg->text + cur_seg->len) {
			go_char_forward();
			return;
		}
	}
	
        if (cur_seg && cur_line) {
		int sid;
		sid = cur_line->get_seg_id(cur_seg);
		if (sid < cur_line->nb_segs - 1) {
                	cur_seg = cur_line->segs[sid + 1];
			cur_chr = cur_seg->text;
                	if (cur_seg->style & RTL_DIRECTION) {
                        	cur_chr += cur_seg->len;
                	}
			if (!r) move_cursor_right(1);	
                	return;
		}
        } 

	if (cur_line && cur_block && cur_line->id < cur_block->nb_lines - 1) {
                cur_line = cur_block->lines[cur_line->id + 1];
		if (cur_line->nb_segs > 0) {
                	cur_seg = cur_line->segs[0];
			cur_chr = cur_seg->text;
                	if (cur_seg->style & RTL_DIRECTION) {
                        	cur_chr += cur_seg->len;
                	}
			auto_scroll(-1, 0);
		} else {
			cur_seg = NULL;
			cur_chr = NULL;
		}
		if (!r) move_cursor_right(1);
                return;
        } 

	if (cur_block && cur_block->id < nb_blocks - 1) {
		cur_block = blocks[cur_block->id + 1];
		if (cur_block->nb_lines > 0) {
			cur_line = cur_block->lines[0];
			if (cur_line->nb_segs > 0) {
				cur_seg = cur_line->segs[0];
				cur_chr = cur_seg->text;
				if (cur_seg->style & RTL_DIRECTION) {
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
		if (!r) move_cursor_right(1); 
		return;
	}
}

void Xd6HtmlFrame::move_cursor_left(int r)
{
	r = 0;
	if (cur_seg && !(cur_seg->style & RTL_DIRECTION)) {
		if (cur_chr && cur_chr > cur_seg->text) {
			go_char_back();
			return;
		}
	} else if (cur_seg) {
		if (cur_chr && cur_chr < cur_seg->text + cur_seg->len) {
			go_char_forward();
			return;
		}
	}
	
        if (cur_seg && cur_line) {
		int sid;
		sid = cur_line->get_seg_id(cur_seg);
		if (sid > 0) {
                	cur_seg = cur_line->segs[sid - 1];
			cur_chr = cur_seg->text;
                	if (!(cur_seg->style & RTL_DIRECTION)) {
                        	cur_chr += cur_seg->len;
                	}
			if (!r) move_cursor_left(1);	
                	return;
		}
        } 

	if (cur_line && cur_block && cur_line->id > 0) {
                cur_line = cur_block->lines[cur_line->id - 1];
		if (cur_line->nb_segs > 0) {
                	cur_seg = cur_line->segs[cur_line->nb_segs - 1];
			cur_chr = cur_seg->text;
                	if (!(cur_seg->style & RTL_DIRECTION)) {
                        	cur_chr += cur_seg->len;
                	}
			auto_scroll(1, 0);
		} else {
			cur_seg = NULL;
			cur_chr = NULL;
		}
		if (!r) move_cursor_left(1);
                return;
        } 

	if (cur_block && cur_block->id > 0) {
		cur_block = blocks[cur_block->id - 1];
		if (cur_block->nb_lines > 0) {
			cur_line = cur_block->lines[cur_block->nb_lines - 1];
			if (cur_line->nb_segs > 0) {
				cur_seg = cur_line->segs[cur_line->nb_segs - 1];
				cur_chr = cur_seg->text;
				if (!(cur_seg->style & RTL_DIRECTION)) {
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
		if (!r) move_cursor_left(1); 
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
                return;
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
                move_cursor(-1, 0);
                return 1;
        case FL_Right:
                move_cursor(1, 0);
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
                if (sel_seg->style & RTL_DIRECTION) {
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
                if (cur_seg->style & RTL_DIRECTION) {
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

/*
 * "$Id: $"
 */

