/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2003  O'ksi'D
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
#include "Xd6XmlParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <FL/Fl_Window.h>


void Xd6HtmlFrame::draw_selection(int x, int y, Xd6HtmlBlock *b, Xd6HtmlLine *l)
{
	//if (damage() & FL_DAMAGE_ALL) l->style |= DAMAGE_ALL;
	if (b1 == b2 && b == b1) {
		if (l1 == l2) {
			if (l == l1) {
				l->flags |= DAMAGE_ALL;
				l->draw(x, y);
				l->draw_middle(x, y, s1, s2, c1, c2);
			} else {
				if (l->flags & (DAMAGE_ALL|DAMAGE_CHILD))
					l->draw(x, y);
			}
		} else {
			if (l == l1) {
				l->flags |= DAMAGE_ALL;
				l->draw(x, y);
				l->draw_end(x, y, s1, c1);
			} else if (l == l2) {
				l->flags |= DAMAGE_ALL;
				l->draw(x, y);
				l->draw_begin(x, y, s2, c2);
			} else if (l->id > l1->id && l->id < l2->id) {
				l->draw_all(x, y);
			} else {
				if (l->flags & (DAMAGE_ALL|DAMAGE_CHILD))
					l->draw(x, y);
			}
		}
	} else {
		if (b == b1) {
			if (l == l1) {
				l->flags |= DAMAGE_ALL;
				l->draw(x, y);
				l->draw_end(x, y, s1, c1);
			} else if (l->id > l1->id) {
				l->draw_all(x, y);
			} else {
				if (l->flags & (DAMAGE_ALL|DAMAGE_CHILD))
					l->draw(x, y);
			}
		} else if (b == b2) {
			if (l == l2) {
				l->flags |= DAMAGE_ALL;
				l->draw(x, y);
				l->draw_begin(x, y, s2, c2);
			} else if (l->id < l2->id) {
				l->draw_all(x, y);
			} else {
				if (l->flags & (DAMAGE_ALL|DAMAGE_CHILD))
					l->draw(x, y);
			}
		} else if (b->id > b1->id && b->id < b2->id) {
			l->draw_all(x, y);
		} else {
			if (l->flags & (DAMAGE_ALL|DAMAGE_CHILD))
				l->draw(x, y);
		}
	}
	l->clear_damage();
	//l->style &= ~(DAMAGE_ALL|DAMAGE_CHILD);
}

void Xd6HtmlFrame::draw_cursor(int x, int y)
{
	if (cur_chr) {
		int len = cur_chr - cur_seg->text;
		int cy, cur_x, wi;
		cur_seg->set_font();

		if (cur_seg->stl->display) {
			wi = 0;
			if (len) wi = cur_seg->width;
		} else { 
			wi = (int) fl_width(cur_seg->text, len);
		}

		if (cur_seg->stl->rtl_direction && !(cur_seg->stl->display)) {
			cur_x = x + cur_block->left + cur_line->left + 
				cur_seg->left + cur_seg->width - wi;
		} else {
			cur_x = x + cur_block->left +
				cur_line->left + cur_seg->left + wi;
		}
		cy = y + cur_block->top + cur_line->top + cur_seg->top;
		fl_color(FL_BLACK);
		fl_line(cur_x, cy, cur_x, cy + cur_seg->height - 1);
		fl_set_spot(fl_font(), fl_size(), cur_x, cy + cur_seg->height -
			fl_descent(), 10, 10);
		
	}
}


void Xd6HtmlFrame::draw(int X, int Y)
{
	int mw, mh;
	Window _sw;
	int x, y;
	int scrollit = 0;

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
		flags |= DAMAGE_ALL;
        	find_pos(X, Y, &cur_block, &cur_line, &cur_seg, &cur_chr);
	}
	if (!(flags & (DAMAGE_ALL|DAMAGE_CHILD))) return;

	if (vscroll != ovscroll || hscroll != ohscroll) {
		flags |= DAMAGE_ALL;
	}

	if (off_screen && (flags & DAMAGE_ALL)) {
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
		scrollit = 1;
	}

	draw_clipped(X, Y, mw, mh);

	if (scrollit) {
		if ((flags & (DAMAGE_ALL|DAMAGE_CHILD)) != DAMAGE_CHILD) {
			fl_clip(X, Y, max_width, max_height);
			draw_vscroll(X, Y);
			draw_hscroll(X, Y);
			fl_pop_clip();
		}
	}


	ovscroll = vscroll;
	ohscroll = hscroll;

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
	if (editor && focus == this) {
		fl_clip(x, y, mw, mh);
		draw_cursor(x + hscroll, y + vscroll);
		fl_pop_clip();
	}
	damage_ = 0;
	flags &= ~(DAMAGE_ALL|DAMAGE_CHILD);
}

void Xd6HtmlFrame::draw_clipped(int x, int y, int mw, int mh)
{
	int X, Y;
	int i;
	int ly, ph;
	int top_draw, bottom_draw;
	int dm;

	fl_clip(x, y, mw, mh);
	ly = y;
	X = x + hscroll;
	Y = y + vscroll;

	sort_selection(&b1, &l1, &s1, &c1, &b2, &l2, &s2, &c2);
	
	top_draw = y;
	bottom_draw = y + max_height; 
	dm = flags & DAMAGE_ALL;
	if (dm) {
		fl_color(background);
		fl_rectf(x, y, mw, mh);
	}
	for (i = 0; i < nb_blocks; i++) {
		Xd6HtmlBlock *b;
		b = blocks[i];
		b->flags |= dm;
		if (b->top + Y >= bottom_draw) break; 
		if (b->top + b->height + Y > top_draw &&
			(b->flags & (DAMAGE_ALL|DAMAGE_CHILD))) 
		{
			int x1, y1, j;
			int ar;
			int dam = b->flags & DAMAGE_ALL;
			x1 = X + b->left;
			y1 = Y + b->top;

			ar = b->stl->text_align == TEXT_ALIGN_RIGHT;
			if (dam) {
				int y2 = y1;
				if (!dm) {
					fl_color(background);
					fl_rectf(X, y1, mw, b->height);
				}
				if (b->nb_lines > 0) {
					y2 += b->lines[0]->top + 
						b->lines[0]->height -
					 	b->lines[0]->descent;
				}
				if (b->stl->list == LIST_NUMBER) {
					char buf[100];
					fl_font(FONT_SANS_SERIF,
						FONT_SIZE_3);
					fl_color(COLOR_BLACK);
					snprintf(buf, 100, "%d.", b->list_id);
					if (ar) { 
						fl_rtl_draw(buf,
							strlen(buf),
							x1 + b->width + 
							(int)fl_width(buf) + 5, 
							y2);
					} else {
						fl_draw(buf, 
							x1 - fl_width(buf) - 5, 
							y2);
					}
				} else if (b->stl->list == LIST_DISC) {
					int x2;
					x2 = x1;
					fl_color(COLOR_BLACK);
					if (ar) {
						x2 += b->width + 9;
					} else {
						x2 -= 9;
					}	
					if (b->stl->blockquote & 0x1) {
						fl_circle(x2, y2 - 4, 2);
					} else {
						fl_rect(x2, y2 - 5, 5, 5);
					}
				}
			}
			for (j = 0; j < b->nb_lines; j++) {
				int my;
				Xd6HtmlLine *l = b->lines[j];
				int clx = x1;

				if (ar) {
					clx = x1 + b->width - width;
				}
				l->flags |= dam;
				my = y1 + l->top + l->height;
				Xd6HtmlSegment::color(background);
				Xd6HtmlSegment::rectf(clx, 
					y1 + l->top, width, l->height);
				if (sel_chr && cur_chr && focus) {
					draw_selection(x1, y1, b, l);
				} else {
					if (l->flags & (DAMAGE_ALL|
						DAMAGE_CHILD)) 
					{
						l->draw(x1, y1);
					}
				}
				ly = my;
				l->clear_damage();
			}
		}
		b->flags &= ~(DAMAGE_ALL|DAMAGE_CHILD);
	}
	fl_color(background);
	if ((flags & DAMAGE_ALL) && cur_chr) {
		fl_rectf(x, ly, mw, y + mh - ly);
	}

	if (wysiwyg) {
        	ph = 0;
        	fl_font(FL_HELVETICA, 12);
        	while(ph < mh) {
                	int yy;
                	char txt[100];
                	ph += page_height;
                	yy = Y - vscroll + ((vscroll) % page_height) + ph;
                	fl_color(FL_GRAY);
                	fl_line(X - hscroll, yy, X + mw - hscroll, yy);
			fl_color(FL_WHITE); 
			fl_rectf(X, yy - footer_height , mw, footer_height);
			
               	 	snprintf(txt, 100, "%d / %d",
                        	(yy - Y) / page_height, height / page_height);
                	fl_color(FL_BLACK);
                	fl_draw(txt, X  + page_width / 2 - 
				(int) fl_width(txt) / 2, yy - 5);
        	}
	} 
	if (wysiwyg) {
        	fl_color(FL_GRAY);
        	fl_line(X + page_width + 1, Y - vscroll, 
			X + page_width + 1, Y + mh - vscroll);
	}

	fl_pop_clip();

	//damage_ = 0;
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

