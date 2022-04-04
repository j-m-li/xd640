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
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <FL/Fl_Window.h>


void Xd6HtmlFrame::draw_selection(int x, int y, Xd6HtmlBlock *b, Xd6HtmlLine *l)
{
	if (b1 == b2 && b == b1) {
		if (l1 == l2) {
			if (l == l1) {
				l->draw(x, y);
				l->draw_middle(x, y, s1, s2, c1, c2);
			} else {
				l->draw(x, y);
			}
		} else {
			if (l == l1) {
				l->draw(x, y);
				l->draw_end(x, y, s1, c1);
			} else if (l == l2) {
				l->draw(x, y);
				l->draw_begin(x, y, s2, c2);
			} else if (l->id > l1->id && l->id < l2->id) {
				l->draw_all(x, y);
			} else {
				l->draw(x, y);
			}
		}
	} else {
		if (b == b1) {
			if (l == l1) {
				l->draw(x, y);
				l->draw_end(x, y, s1, c1);
			} else if (l->id > l1->id) {
				l->draw_all(x, y);
			} else {
				l->draw(x, y);
			}
		} else if (b == b2) {
			if (l == l2) {
				l->draw(x, y);
				l->draw_begin(x, y, s2, c2);
			} else if (l->id < l2->id) {
				l->draw_all(x, y);
			} else {
				l->draw(x, y);
			}
		} else if (b->id > b1->id && b->id < b2->id) {
			l->draw_all(x, y);
		} else {
			l->draw(x, y);
		}
	}
}

void Xd6HtmlFrame::draw_cursor(int x, int y)
{
	if (cur_chr) {
		int len = cur_chr - cur_seg->text;
		int cy, cur_x, wi;
		cur_seg->set_font();
		wi = (int) fl_width(cur_seg->text, len);

		if (cur_seg->style & RTL_DIRECTION) {
			cur_x = x + cur_block->left + cur_line->left + 
				cur_seg->left + cur_seg->width - wi;
		} else {
			cur_x = x + cur_block->left +
				cur_line->left + cur_seg->left + wi;
		}
		cy = y + cur_block->top + cur_line->top + cur_seg->top;
		fl_color(FL_BLACK);
		fl_line(cur_x, cy, cur_x, cy + cur_seg->height - 1);
		
	}
}


void Xd6HtmlFrame::draw(int X, int Y)
{
	int mw, mh;
	Window _sw;
	int x, y;
	
	_sw = fl_window;
#ifdef WIN32
	HDC _sgc;
#endif	

	X += left;
	Y += top;
	x = X;
	y = Y;
	mw = max_width;
	mh = max_height;

	
	if (!cur_chr) {
        	find_pos(X, Y, &cur_block, &cur_line, &cur_seg, &cur_chr);
	}
	if (off_screen && (sel_chr || damage() & FL_DAMAGE_ARROWS)) {
		if ((long)off_screen == 1) {
			off_screen = (Window) 
				fl_create_offscreen(max_width, max_height);
		}
#ifdef WIN32
		_sgc=fl_gc;
		fl_gc=fl_makeDC((HBITMAP)off_screen);
#endif
		fl_window=off_screen;
		fl_push_no_clip();
		X = Y = 0;
	}
	if (scroll && (height > mh || width > mw)) {
		mw -= 15;
		mh -= 15;
		if (damage() != FL_DAMAGE_CHILD) {
			fl_clip(X, Y, max_width, max_height);
			draw_vscroll(X, Y);
			draw_hscroll(X, Y);
			fl_pop_clip();
		}
	}
	if (vscroll != ovscroll || hscroll != ohscroll) {
		damage(FL_DAMAGE_ALL);
		draw_clipped(X, Y, mw, mh);
		ovscroll = vscroll;
		ohscroll = hscroll;
	} else {
		draw_clipped(X, Y, mw, mh);
	}	
	if (fl_window != _sw) {
		fl_pop_clip(); 
		fl_window = _sw;
#ifdef WIN32
		DeleteDC(fl_gc);
		fl_gc = _sgc;
		fl_copy_offscreen(x, y,  max_width, max_height,
			(HBITMAP)off_screen, 0, 0);
#else
		fl_copy_offscreen(x, y,  max_width, max_height,
			off_screen, 0, 0);
#endif
	}
}

void Xd6HtmlFrame::draw_clipped(int x, int y, int mw, int mh)
{
	int X, Y;
	int i;
	int ly, ph;
	int top_draw, bottom_draw;

	fl_clip(x, y, mw, mh);
	ly = y;
	X = x + hscroll;
	Y = y + vscroll;

	sort_selection(&b1, &l1, &s1, &c1, &b2, &l2, &s2, &c2);

	if (damage() == FL_DAMAGE_CHILD && cur_chr) {
		int x1, y1;
		x1 = X + cur_block->left;
		y1 = Y + cur_block->top;
		if (sel_chr && focus) {
			x1 = X + sel_block->left;
			y1 = Y + sel_block->top;
			fl_color(background);
			fl_rectf(x, y1 + sel_line->top, mw, sel_line->height);
			draw_selection(x1, y1, sel_block, sel_line);
		} else {
			fl_color(background);
			fl_rectf(x, y1 + cur_line->top, mw, 
				cur_line->height);
			cur_line->draw(x1, y1);
			if (editor) draw_cursor(X, Y);
		}

		fl_pop_clip();
		damage_ = 0;
		return;
	}

	if (damage() == FL_DAMAGE_BLOCK && cur_chr) {
		ly = y + cur_block->top + vscroll;
		top_draw = ly;
		bottom_draw = top_draw + cur_block->height;
	} else {
		top_draw = y;
		bottom_draw = y + max_height; 
	}
	for (i = 0; i < nb_blocks; i++) {
		Xd6HtmlBlock *b;
		b = blocks[i];
		if (b->top + Y >= bottom_draw) break; 
		if (b->top + b->height + Y > top_draw) {
			int x1, y1, j;
			for (j = 0; j < b->nb_lines; j++) {
				int my;
				Xd6HtmlLine *l = b->lines[j];
				x1 = X + b->left;
				y1 = Y + b->top;
				my = y1 + l->top + l->height;
				fl_color(background);
				fl_rectf(x, ly, mw, my - ly + 1);
				if (sel_chr && cur_chr && focus) {
					draw_selection(x1, y1, b, l);
				} else {
					l->draw(x1, y1);
				}
				ly = my;
			}
		}
	}
	fl_color(background);
	if (!(damage() == FL_DAMAGE_BLOCK && cur_chr)) {
		fl_rectf(x, ly, mw, y + mh - ly);
	}
	if (editor && focus == this) draw_cursor(X, Y);

	if (wysiwyg) {

        	ph = 0;
        	fl_font(FL_HELVETICA, 12);
        	while(ph < mh) {
                	int yy;
                	char txt[100];
                	ph += page_height;
                	yy = Y - vscroll + ((vscroll) % page_height) + ph;
                	fl_color(FL_GREEN);
                	fl_line(X - hscroll, yy, X + mw - hscroll, yy);
			fl_color(FL_WHITE); 
			fl_rectf(X, yy - footer_height , mw, footer_height);
			
               	 	snprintf(txt, 100, "%d / %d",
                        	(yy - Y) / page_height, height / page_height);
                	fl_color(FL_BLACK);
                	fl_draw(txt, X  + page_width / 2 - 
				(int) fl_width(txt) / 2, yy - 5);
        	}
        	fl_color(FL_GREEN);
        	fl_line(X + page_width + 1, Y - vscroll, 
			X + page_width + 1, Y + mh - vscroll);
	}

	fl_pop_clip();

	damage_ = 0;
}


void Xd6HtmlFrame::create_off_screen(void)
{
	if ((long)off_screen > 1) {
#ifdef WIN32
		fl_delete_offscreen((HBITMAP)off_screen);
#else 
		fl_delete_offscreen(off_screen);
#endif
	}
	off_screen = (Window) 1;
}

/*
 * "$Id: $"
 */

