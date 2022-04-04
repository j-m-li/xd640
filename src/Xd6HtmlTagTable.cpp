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
#include "Xd6HtmlTagTable.h"
#include "Xd6HtmlPrint.h"
#include <FL/fl_draw.h>
#include <FL/Fl.h>
#include "Xd6XmlUtils.h"

Xd6HtmlTagTable::Xd6HtmlTagTable(int i, Xd6XmlTreeElement *e, Xd6HtmlFrame *p): 
	Xd6HtmlFrame(i)
{
	stl = e->stl;
	parent = p;
	while (p->display != DISPLAY_IFRAME &&
		p->display != DISPLAY_FRAME &&
		p->display != DISPLAY_TOP_FRAME &&
		p->display != DISPLAY_TABLE_CELL)
	{
		p = p->parent;
	}
	need_resize = 0;
	parentf = (Xd6HtmlFrame*)p;
	create_table(e);
}

void Xd6HtmlTagTable::create_table(Xd6XmlTreeElement *e)
{
	int i;
	int pw;

	pw = parentf->page_width;

	t_width = get_length(e->get_attr_value("width"), pw);
	if (e->get_attr_value("border")) {
		t_border = get_length(e->get_attr_value("border"), t_width);
	} else {
		t_border = 0;
	}
	t_cellspacing = get_length(e->get_attr_value("cellspacing"), t_width);
	t_cellpadding = get_length(e->get_attr_value("cellpadding"), t_width);
	//if (t_cellspacing < 0) t_cellspacing = 1;
	if (t_cellpadding < 0 || !e->get_attr_value("cellpadding")) { 
		t_cellpadding = 1;
	}
	col = 0;
	max_td = 0;
	row = 0;
	max_tr = 0;
	
	// get the number of rows and cells by rows
	scan_table(e, scan_td_cb);
	if (col > max_td) max_td = col;


	cells = NULL;
	nb_cells = max_td * max_tr;
	if (nb_cells < 1) return;
	cells = (Xd6HtmlTagTd**) malloc(sizeof(Xd6HtmlTagTd*) * nb_cells);

	
	for (i = 0; i < nb_cells; i++) {
		cells[i] = NULL;
	}

	col_widths = (int*) malloc(sizeof(int) * max_td);
	cols_pos = (int*) malloc(sizeof(int) * max_td);
	for (i = 0; i < max_td; i++) {
		cols_pos[i] = 0;
		col_widths[i] = 0;
	}

	row_heights = (int*) malloc(sizeof(int) * max_tr);
	rows_pos = (int*) malloc(sizeof(int) * (max_tr + 1));
	for (i = 0; i < max_tr; i++) {
		rows_pos[i] = 0;
		row_heights[i] = 0;
	}

	col = 0;
	row = -1;

	// create cells and get their minimal width
	scan_table(e, scan_cell_cb);

	// free tree memory
	while (e->nb_children > 0) {
		e->nb_children--;
		if (!e->children[e->nb_children]) continue;
		if (e->children[e->nb_children]->type == Xd6XmlTreeSegment_element)
		{
			delete((Xd6XmlTreeElement*) e->children[e->nb_children]);
		} else if (e->children[e->nb_children]->type == 
			Xd6XmlTreeSegment_text)
		{
			delete((Xd6XmlTreeText*)e->children[e->nb_children]);
		} else {
			delete(e->children[e->nb_children]);
		}
	}
	
}

Xd6HtmlTagTable::~Xd6HtmlTagTable()
{
	while (nb_cells > 0) {
		delete(cells[--nb_cells]);
	}
}

void Xd6HtmlTagTable::destroy()
{
}

void Xd6HtmlTagTable::draw(int X, int Y)
{
	int i = 0;
	int dm = flags & DAMAGE_ALL;
	
	X += left;
	Y += top;

	if (dm) {
		fl_color(FL_RED);
		for (i = 0; i < t_border; i++) {
			fl_rect(X + i, Y + i, width - 2 * i, 
				(height - descent) - 2 * i);
		}
	}
	fl_color(FL_BLACK);


	fl_clip(X + t_border, Y + t_border, width - 2 * i, 
			(height - descent) - 2 * i);


	for (i = 0; i < nb_cells; i++) {
		if (cells[i]) {
			cells[i]->flags |= dm;
			if  (cells[i]->selected) {
				if (cells[i]->flags &(DAMAGE_ALL|DAMAGE_CHILD)){
					Xd6HtmlTagTd *t = cells[i];
					Xd6HtmlBlock *b = t->cur_block;
					Xd6HtmlLine *l = t->cur_line;
					Xd6HtmlSegment *s = t->cur_seg;
					char *c = t->cur_chr;
					Xd6HtmlDisplay *fo = t->focus;
					t->select_all();
					t->focus = t;
					t->draw(X, Y);
					t->cur_block = b;
					t->cur_line = l;
					t->cur_seg = s;
					t->cur_chr = c;
					t->sel_block = NULL;
					t->sel_line = NULL;
					t->sel_seg = NULL;
					t->sel_chr = NULL;
					t->focus = fo;
				}
			} else {
				if (cells[i]->flags &(DAMAGE_ALL|DAMAGE_CHILD)){
					cells[i]->draw(X, Y);
				}
			}
			cells[i]->flags &= ~(DAMAGE_ALL|DAMAGE_CHILD);
		}
	}
	
	fl_pop_clip();
}

void Xd6HtmlTagTable::draw_selected(int X, int Y)
{
	for(int i = 0; i < nb_cells;i++) {
		if (cells[i]) cells[i]->selected = 1;
	}
	draw(X, Y);
	for(int i = 0; i < nb_cells;i++) {
		if (cells[i]) cells[i]->selected = 0;
	}

}

#ifdef WIN32
void Xd6HtmlTagTable::print(Xd6HtmlPrint *p, int X, int Y)
{
	int i, y;
	double ratio = p->ratio;
	Xd6HtmlFrame *frm = p->frm;
	X += left;
	Y += top;
	y = Y - p->frm->page_height * p->page_nb;
	if (t_border) {
		int co = fl_color();
		HDC f = fl_gc;
		fl_gc = p->hDC;
		fl_clip(frm->page_margin_left, frm->page_margin_top, 
				frm->page_width, frm->page_height);
		fl_line_style(FL_SOLID, (int)(t_border * ratio));
		fl_line((int)((frm->page_margin_left + X) * ratio),
				(int)((frm->page_margin_top + y) * ratio),
				(int)((frm->page_margin_left + X + width) * ratio),
				(int)((frm->page_margin_top + y) * ratio));
		fl_line((int)((frm->page_margin_left + X) * ratio),
				(int)((frm->page_margin_top + y) * ratio),
				(int)((frm->page_margin_left + X) * ratio),
				(int)((frm->page_margin_top + y + (height - descent)) * ratio));
		fl_line((int)((frm->page_margin_left + X + width) * ratio),
				(int)((frm->page_margin_top + y) * ratio),
				(int)((frm->page_margin_left + X + width) * ratio),
				(int)((frm->page_margin_top + y + height - descent) * ratio));
		fl_pop_clip();
		fl_gc = f;
		fl_line_style(FL_SOLID, 1);
	}
	while (1) {
		for (i = 0; i < nb_cells; i++) {
			if (cells[i]) {
				cells[i]->print(p, X, Y);
			}
		}
		y = Y - p->frm->page_height * p->page_nb;
		if (t_border) {
			int co = fl_color();
			HDC f = fl_gc;
			fl_gc = p->hDC;
			fl_clip(frm->page_margin_left, frm->page_margin_top, 
				frm->page_width, frm->page_height);
			fl_line_style(FL_SOLID, (int)(t_border * ratio));
			fl_line((int)((frm->page_margin_left + X) * ratio),
				(int)((frm->page_margin_top + y + (height - descent)) * ratio),
				(int)((frm->page_margin_left + X + width) * ratio),
				(int)((frm->page_margin_top + y + (height - descent)) * ratio));
			fl_line((int)((frm->page_margin_left + X) * ratio),
				(int)((frm->page_margin_top + y) * ratio),
				(int)((frm->page_margin_left + X) * ratio),
				(int)((frm->page_margin_top + y + (height - descent)) * ratio));
			fl_line((int)((frm->page_margin_left + X + width) * ratio),
				(int)((frm->page_margin_top + y) * ratio),
				(int)((frm->page_margin_left + X + width) * ratio),
				(int)((frm->page_margin_top + y + height - descent) * ratio));
			fl_pop_clip();
			fl_gc = f;
			fl_line_style(FL_SOLID, 1);	
		}
		if (Y + height - p->frm->page_height * (p->page_nb + 1) > 0) {
			p->new_page();
		} else {
			break;
		}
	}
}
#else
void Xd6HtmlTagTable::print(Xd6HtmlPrint *p, int X, int Y)
{
	int i, y;

	X += left;
	Y += top;
	y = Y - p->frm->page_height * p->page_nb;
	if (t_border) {
		fprintf(p->tfp, "%d %d %d %d r\n",
			p->frm->page_margin_left + X,
			p->frm->page_height + p->frm->page_margin_bottom - y,
			width, -t_border);
		fprintf(p->tfp, "%d %d %d %d r\n",
			p->frm->page_margin_left + X,
			p->frm->page_height + p->frm->page_margin_bottom - y,
			t_border, -(height - descent));
		fprintf(p->tfp, "%d %d %d %d r\n",
			p->frm->page_margin_left + X + width - t_border,
			p->frm->page_height + p->frm->page_margin_bottom - y,
			t_border, -(height - descent));
	}

	while (1) {
		for (i = 0; i < nb_cells; i++) {
			if (cells[i]) {
				cells[i]->print(p, X, Y);
			}
		}
		y = Y - p->frm->page_height * p->page_nb;
		if (t_border) {
		  fprintf(p->tfp, "%d %d %d %d r\n",
			p->frm->page_margin_left + X,
			p->frm->page_height + p->frm->page_margin_bottom - y
			- (height - descent) + t_border,
			width, -t_border);
		  fprintf(p->tfp, "%d %d %d %d r\n",
			p->frm->page_margin_left + X,
			p->frm->page_height + p->frm->page_margin_bottom - y 
			- (height - descent) + t_border,
			t_border, (height - descent) - t_border);
		  fprintf(p->tfp, "%d %d %d %d r\n",
			p->frm->page_margin_left + X + width - t_border,
			p->frm->page_height + p->frm->page_margin_bottom - y
			- (height - descent) + t_border,
			t_border, (height - descent) - t_border);
		}
		if (Y + height - p->frm->page_height * (p->page_nb + 1) > 0) {
			p->new_page();
		} else {
			break;
		}
	}
}
#endif

void Xd6HtmlTagTable::measure(void)
{
	int i, j, k;
	int x, y, h, mh;
	width = 0;
	height = 0;
	h = 0;
	x = 0;
	descent = 10;
	if (nb_cells < 1) return;

	// set all cols width to 0
	for (i = 0; i < max_td; i++) {
		cols_pos[i] = 0;
	}

	for (i = 0; i < nb_cells; i++) {
		if (cells[i]) {
			cells[i]->measure();
		}
	}

	j = 0;
	// get the columns widths
	for (i = 0; i < nb_cells; i++) {
	
		if (i % max_td == 0) {
			// next row
			j = 0;
		}

		if (cells[i]) {
			int w, k;
			w = 0;
			for (k = 0; k < cells[i]->colspan; k++) {
				// set already known col width
				if (col_widths[j + k] > 0) {
					cols_pos[j + k] = col_widths[j + k];
				} 
				w += cols_pos[j + k];
			}
	
			if (cells[i]->width > w) {
				// dispatch cell width between columns
				int add;
				add = (cells[i]->width - w) / cells[i]->colspan;
				for (k = 0; k < cells[i]->colspan; k++) {
					cols_pos[j + k] += add;
				}
				cols_pos[j] += (cells[i]->width - w) % 
					cells[i]->colspan;
			}
		}
		j++;
	}
	k = 0;
	for (i = 0; i < max_td; i++) {
		k += cols_pos[i];
	}
	k += 2 * t_border;
	if (k < t_width) {
		int add;
		add = (t_width - k - 2 * t_border) / (max_td); 
		for (i = 0; i < max_td; i++) {
			cols_pos[i] += add;
		}
		cols_pos[0] += (t_width - k - 2 * t_border) % (max_td);
	}

	// set cells widths
	x = t_border + t_cellspacing;
	mh = 0;
	k = -1;
	y = t_border;
	for (i = 0; i < nb_cells; i++) {
		if (i % max_td == 0) {
			// next row
			j = 0;	// column
			k++;    // row
			x = t_border + t_cellspacing;
			y += mh + t_cellspacing; 
			rows_pos[k] = y;
			if (row_heights[k] > 0) {
				mh = row_heights[k];
			} else {
				mh = 0;	// row height
			}
		}
		if (cells[i]) {
			int w, ii, ch;
			w = 0;
			for (ii = 0; ii < cells[i]->colspan; ii++) {
				w += cols_pos[j + ii];
				if (ii > 0) {
					w += t_cellspacing;
				}
			}
			cells[i]->resize(w, 1);
			cells[i]->left = x;
			cells[i]->max_height = cells[i]->height + 
				2 * cells[i]->border;
			cells[i]->max_width = w;
			
			ch = 0;
			for (ii = 1; ii < cells[i]->rowspan; ii++) {
				if (row_heights[k + ii] > 0) {
					ch += row_heights[k + ii]; 
				}
			}
			if (cells[i]->height - ch > mh) {
				mh = cells[i]->height - ch;
			}
		}
		x += cols_pos[j] + t_cellspacing;  
		j++;
	}
	k++;
	width = x + t_border;
	y += mh + t_cellspacing;
	rows_pos[k] = y;
	height = y  + t_border + descent;
	
	j = -1;
	for (i = 0; i < nb_cells; i++) {
		if (i % max_td == 0) {
			j++;
		}
		if (cells[i]) {
			cells[i]->top = rows_pos[j];
			cells[i]->max_height = rows_pos[j + cells[i]->rowspan] 
				- rows_pos[j] - t_cellspacing;
			if (cells[i]->valign == VALIGN_MIDDLE) {
				cells[i]->valign_pad = (cells[i]->max_height -
					2 * (cells[i]->border +
						cells[i]->cellpadding) -
					cells[i]->height) / 2;
			} else if (cells[i]->valign == VALIGN_BOTTOM) {
				cells[i]->valign_pad = cells[i]->max_height -
					cells[i]->border - 
					cells[i]->cellpadding -
					cells[i]->height;
			}
		}
	}
	j++;
}

void Xd6HtmlTagTable::scan_table(Xd6XmlTreeElement *elem, TableScanCallback cb) 
{
	int i;

	cb(this, elem);

	for (i = 0; i < elem->nb_children; i++) {
		Xd6XmlTreeElement *e;
		switch(elem->children[i]->type) {
		case Xd6XmlTreeSegment_element:
			e = (Xd6XmlTreeElement*)elem->children[i];
			scan_table(e, cb);
			break;
		default:
			break;
		}
	}
}


void Xd6HtmlTagTable::scan_cell_cb(Xd6HtmlTagTable *s, Xd6XmlTreeElement *e)
{
	if (e->display == DISPLAY_TABLE_COL) {
		int i, w;
		i = s->get_length(e->get_attr_value("colspan"), 100);
		if (i < 1) i = 1;
		w = s->get_length(e->get_attr_value("width"), 100);
		s->col_widths[s->col] = w % i;
		w = w / i;
		s->col_widths[s->col] += w;
		i--;
		s->col++;
		while(i > 0) {
			s->col_widths[s->col++] = w;
			i--;
		}
	} else if (e->display == DISPLAY_TABLE_ROW) {
		s->col = 0;
		s->row++;
		s->row_heights[s->row] = s->get_length(
			e->get_attr_value("height"), 100);
	} else if (e->display == DISPLAY_TABLE_CELL) {
		int j, c;
		Xd6HtmlTagTd *ce;
		while (s->cols_pos[s->col] > s->row) {
			s->col++;
		}

		c = s->col + (s->row * s->max_td);
		ce = s->cells[c] = new Xd6HtmlTagTd(c, e, s);
	
		ce->page_width = 2;
		ce->cellpadding = s->t_cellpadding;
		if (ce->border < 0) ce->border = s->t_border;
		ce->scroll = 0;
		ce->wysiwyg = 0;
		ce->tree2block(e);
		if (ce->nb_blocks == 1) {
			Xd6HtmlBlock *b = ce->blocks[0];
			if (b->nb_lines == 0) {
				b->add_segment((char*)malloc(1), 0,
					&Xd6XmlStl::def);
			}
		}
		//s->cells[c]->measure();

		j = ce->colspan;
		while (j > 0) {
			j--;
			s->cols_pos[s->col] += ce->rowspan;
			s->col++;
		}
	}
}

void Xd6HtmlTagTable::scan_td_cb(Xd6HtmlTagTable *s, Xd6XmlTreeElement *e)
{
	if (e->display == DISPLAY_TABLE_COL) {
		int i;
		i = s->get_length(e->get_attr_value("colspan"), 100);
		if (i < 1) i = 1;
		s->col += i;
	} else if (e->display == DISPLAY_TABLE_ROW) {
		if (s->col > s->max_td) s->max_td = s->col;
		s->col = 0;	
		s->row++;
	} else if (e->display == DISPLAY_TABLE_CELL) {
		int i;
		i = s->get_length(e->get_attr_value("colspan"), 100);
		if (i < 1) i = 1;
		s->col += i;
		i = s->get_length(e->get_attr_value("rowspan"), 100);
		if (i < 1) i = 1;
		if (i + s->row - 1  > s->max_tr) {
			s->max_tr = i + s->row - 1;
		}
	}
}

void Xd6HtmlTagTable::break_line(int h, int ph, int fh)
{
	int i;
	int j;
	int mh;
	int y;
	int poff;
	int page;

	if (nb_cells < 1) return;

	page = h / ph;
	poff = h % ph;
	mh = 0;
	y = t_border;
	j = -1;
	for (i = 0; i < nb_cells; i++) {
		if (i % max_td == 0) {
			j++;
			y += mh + t_cellspacing;
			rows_pos[j] = y;
			mh = row_heights[j];
		}
		if (cells[i]) {
			cells[i]->top = y;
			if (poff + cells[i]->top  + cells[i]->valign_pad 
				+ cells[i]->height >= ph - fh) 
			{
				cells[i]->break_line(h, ph, fh);
			}
			if (cells[i]->height > mh) mh = cells[i]->height;
		}
	}
	j++;
	y +=  mh + t_cellspacing;
	rows_pos[j] = y;
	height = y  + t_border + descent;

	j = -1;
	for (i = 0; i < nb_cells; i++) {
		if (i % max_td == 0) {
			j++;
		}
		if (cells[i]) {
			cells[i]->max_height = rows_pos[j + cells[i]->rowspan] 
				- rows_pos[j] - t_cellspacing;
		}
	}
}

enum {
	UPL = 1,
	UPR,
	DNL,
	DNR
};

int Xd6HtmlTagTable::handle(int e, int x, int y)
{
	int dir = 0;
	int fx = 0, fy = 0, ft = 0, fl = 0, fb = 0, fr = 0, re = 0;
	int cx = 0, cy = -1;
	int i = 0, ret = 0;
	int t = 0, b = 0, l = 0, r = 0;
	int done = 0;
	int spanx = 1, spany = 1;
	Xd6HtmlTagTd *ce;
	Xd6HtmlTagTd *hot;
	Xd6HtmlTagTd *ohot;
	x += left;
	y += top;
	
	for (i = 0; i < nb_cells; i++) {
		cx++;
		if (i % max_td == 0) {
			cx = 0;
			cy++;
		}
		if (cells[i]) {	
			int w, h;
			if (cells[i]->focus) {
				fx = cx;
				fy = cy;
				ft = y + cells[i]->top;
				fb = y + cells[i]->top + cells[i]->max_height;
				fl = x + cells[i]->left;
				fr = x + cells[i]->left + cells[i]->width;
				spanx = cells[i]->colspan;
				spany = cells[i]->rowspan;
			}
			if (e == FL_HIDE_CURSOR) {
				char s = cells[i]->selected;
				cells[i]->selected = 0;
				if (cells[i]->selected != s) {
					cells[i]->damage(DAMAGE_ALL);
				}
			}
			if ((e != FL_PUSH && cells[i]->focus) || e == FL_PUSH) {
				w = cells[i]->max_width;
				h = cells[i]->height;
				int ww =  cells[i]->width;
				int r = cells[i]->handle(e, x, y);
				if (r) ret = 1;
				if (cells[i]->max_width != w || 
					cells[i]->height != h)
				{
					need_resize = 1;
				}
				if (ww != cells[i]->width) {
					cells[i]->damage(DAMAGE_ALL);
				}
			} else {
				cells[i]->focus = NULL;
			}
		}
	}
	if (ret && e == FL_DRAG) {
		for (i = 0; i < nb_cells; i++) {
			if (cells[i]) {	
				char s = cells[i]->selected;
				cells[i]->selected = 0;
				if (cells[i]->selected != s) {
					cells[i]->damage(DAMAGE_ALL);
				}
			}
		}
	}

	if (ret) return ret;
	cy = -1;
	ohot = hot = NULL;
	for (i = 0; i < nb_cells; i++) {
		if (!done && hot)  {
			t = y + hot->top;
			b = y + hot->top + hot->max_height;
			l = x + hot->left;
			r = x + hot->left + hot->max_width;
			switch (dir) {
			case DNL:
				parentf->hotspot(l, b); done = 1; break;
			case UPL:
				parentf->hotspot(l, t); done = 1; break;
			}
		}
		cx++;
		if (i % max_td == 0) {
			cx = 0;
			cy++;
			if (!done && hot)  {
				if (dir == UPR) {
					parentf->hotspot(r, t); 
					done = 1; 
				}
			}
			ohot = NULL;
		} else {
			if (dir == DNL && ohot) {
				hot = ohot;
			}
			ohot = hot;
		}
		
		if (cells[i]) {	
			ce = cells[i];
			t = y + cells[i]->top;
			b = y + cells[i]->top + cells[i]->max_height;
			l = x + cells[i]->left;
			r = x + cells[i]->left + cells[i]->max_width;
			if (e == FL_DRAG) {
				char s = cells[i]->selected;
				if (b > Fl::e_y && r > Fl::e_x && 
					cx <= fx && cy <= fy) 
				{
					hot = ce;
					dir = UPL;
					cells[i]->selected = 1;
				} else if (t < Fl::e_y && l < Fl::e_x && 
					cx >= fx && cy >= fy) 
				{
					dir = DNR;
					hot = ce;
					cells[i]->selected = 1;
				} else if (b > Fl::e_y && l < Fl::e_x && 
					cx >= fx && cy <= fy) 
				{
					dir = UPL;
					hot = ce;
					cells[i]->selected = 1;
				} else if (t < Fl::e_y && r > Fl::e_x && 
					cx <= fx && cy >= fy) 
				{
					dir = DNL;
					hot = ce;
					cells[i]->selected = 1;
				} else {
					cells[i]->selected = 0;

				}
				if (cells[i]->selected != s) {
					cells[i]->damage(DAMAGE_ALL);
				}
				ret = 1;
			} else if (e == FL_KEYBOARD && 
				(Fl::event_state() & FL_SHIFT)) 
			{

			} else if (e == FL_KEYBOARD) {
				switch(Fl::event_key()) {
				case FL_Left:
					ce->focus = NULL;
					re = send_fake_push(ce, x, y, cx, cy, 
						fx, fy, -1, 0);
					if (re) {
						hot = ce;
						dir = UPL;
						ret = 1;
					}
					break;
				case FL_Up:
					ce->focus = NULL;
					re = send_fake_push(ce, x, y, cx, cy, 
						fx, fy, 0, -1);
					if (re) {
						hot = ce;
						dir = UPL;
						ret = 1;
					}
					break;
				case FL_Right:
					ce->focus = NULL;
					re = send_fake_push(ce, x, y, cx, cy, 
						fx, fy, spanx, 0);
					if (re) {
						hot = ce;
						dir = UPR;
						ret = 1;
					}
					break;
				case FL_Down:
					ce->focus = NULL;
					re = send_fake_push(ce, x, y, cx, cy, 
						fx, fy, 0, spany);
					if (re) {
						hot = ce;
						dir = DNL;
						ret = 1;
					}
					break;
				}
			}		
		}
	}
	if (dir == DNL && ohot) {
		hot = ohot;
	}
	if (!done && hot)  {
		t = y + hot->top;
		b = y + hot->top + hot->max_height;
		l = x + hot->left;
		r = x + hot->left + hot->max_width;
		switch (dir) {
		case DNL:
			parentf->hotspot(l, b); done = 1; break;
		case DNR:
			parentf->hotspot(r, b); done = 1; break;
		case UPL:
			parentf->hotspot(l, t); done = 1; break;
		case UPR:
			parentf->hotspot(r, t); done = 1; break;
		}
	}
	if (ret) return ret;
	cy = -1;
	for (i = 0; i < nb_cells; i++) {
		cx++;
		if (i % max_td == 0) {
			cx = 0;
			cy++;
		}
		if (cells[i]) {	
			if (e == FL_DRAG) {

			} else if (e == FL_KEYBOARD && 
				(Fl::event_state() & FL_SHIFT)) 
			{

			} else if (e == FL_KEYBOARD) {
				Xd6HtmlTagTd *ce = cells[i];
				switch(Fl::event_key()) {
				case FL_Left:
				case FL_Right:
			//	case FL_Down:
			//	case FL_Up:
					r =  send_fake_push(ce, x, y, 
						cx, cy, fx, fy, 0, 0);
					if (r) return 1;
				}
			}
		}
	}

	return ret;
}

int Xd6HtmlTagTable::send_fake_push(Xd6HtmlTagTd *ce, int x, int y, 
	int cx, int cy, int fx, int fy, int dx, int dy)
{
	int rs = ce->rowspan - 1;
	int cs = ce->colspan - 1;
	if (!(cx <= fx + dx && cx + cs >= fx + dx && 
		cy <= fy + dy && cy + rs >= fy + dy))
	{
		return 0;
	}

	Fl::e_x = ce->left + x + ce->cellpadding + ce->border + 1;
	Fl::e_y = ce->top + y + ce->cellpadding + ce->border+ce->valign_pad + 1;	

	return ce->handle(FL_PUSH, x, y);
}

void Xd6HtmlTagTable::change_style(Xd6XmlStl *n)
{
	int i = 0;
	for (i = 0; i < nb_cells; i++) {
		if (cells[i] && cells[i]->focus) {	
			int w = cells[i]->max_width;
			int h = cells[i]->height;
			int ww =  cells[i]->width;
			cells[i]->change_style(n);	
			if (cells[i]->max_width != w || 
				cells[i]->height != h)
			{
				need_resize = 1;
			}
			if (ww != cells[i]->width) {
				damage(DAMAGE_ALL);
			}
			return;
		}
	}
}

void Xd6HtmlTagTable::insert_frame(Xd6HtmlFrame *f)
{
	int i = 0;
	for (i = 0; i < nb_cells; i++) {
		if (cells[i] && cells[i]->focus) {	
			cells[i]->insert_frame(f);	
			return;
		}
	}
}

Xd6HtmlFrame *Xd6HtmlTagTable::get_cursor_frame()
{
	int i = 0;
	for (i = 0; i < nb_cells; i++) {
		if (cells[i] && cells[i]->focus) {	
			return cells[i]->get_cursor_frame();	
		}
	}
	return NULL;
}

void Xd6HtmlTagTable::insert_segment(Xd6HtmlSegment *s)
{
	int i = 0;
	for (i = 0; i < nb_cells; i++) {
		if (cells[i] && cells[i]->focus) {	
			cells[i]->insert_segment(s);	
			return;
		}
	}
}

void scan_to_html_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s,
	char *c, int len , void *d);

void Xd6HtmlTagTable::scan_selection(ScanCallback cb, void *data)
{
	int i = 0;
	int multi = 0;
	FILE *fp = NULL;;
	int cx = 0, cy = 0;
	int fx = 1000000, lx = -1, fy = 1000000, ly = -1;

	if (cb == scan_to_html_cb) {
		cy = -1;
		fp = ((Xd6HtmlFrame*)data)->fp;
		multi = -1;
		for (i = 0; i < nb_cells; i++) {
			cx++;
			if (i % max_td == 0) {
				cx = 0;
				cy++;
			}
			if (cells[i] && cells[i]->selected) {
				if (cx < fx) fx = cx;
				if (cx > lx) lx = cx;
				if (cy < fy) fy = cy;
				if (cy > ly) ly = cy;
				multi++;
			}
		}
		if (multi < 0) multi = 0;
	}
	if (multi) {
		fprintf(fp, "<table\n>");
	}
	cy = -1;
	for (i = 0; i < nb_cells; i++) {
		cx++;
		if (i % max_td == 0) {
			cx = 0;
			cy++;
			if (multi) {
				if ((cy - 1) >= fy && (cy - 1) <= ly) {
					fprintf(fp, "</tr\n>");
				}
				if (cy >= fy && cy <= ly) {
					fprintf(fp, "<tr\n>");
				}
			}
		}
		if (cells[i] && (!multi || cells[i]->selected)) {
			cells[i]->fp = fp;	
			if (multi) {
				fprintf(fp, "<td colspan=\"%d\""
					" rowspan=\"%d\"",
						cells[i]->colspan,
						cells[i]->rowspan);
				if (cells[i]->xfmla) {
					char *buf;
					int l;
					l = strlen(cells[i]->xfmla);
					buf = (char*)malloc(l * 6 + 1);
					utf2xml(cells[i]->xfmla, l, buf);
					fprintf(fp,"\nx:fmla=\"%s\"", buf);
					free(buf);
				}
				if (cells[i]->xnum) {
					char *buf;
					int l;
					l = strlen(cells[i]->xnum);
					buf = (char*)malloc(l * 6 + 1);
					utf2xml(cells[i]->xnum, l, buf);
					fprintf(fp,"\nx:num=\"%s\"", buf);
					free(buf);
				}
				fprintf(fp, "\n>");
				Xd6HtmlTagTd *t = cells[i];
				Xd6HtmlBlock *b = t->cur_block;
				Xd6HtmlLine *l = t->cur_line;
				Xd6HtmlSegment *s = t->cur_seg;
				char *c = t->cur_chr;
				Xd6HtmlDisplay *fo = t->focus;
				t->select_all();
				t->focus = t;
				cells[i]->scan_selection(cb, cells[i]);	
				t->cur_block = b;
				t->cur_line = l;
				t->cur_seg = s;
				t->cur_chr = c;
				t->sel_block = NULL;
				t->sel_line = NULL;
				t->sel_seg = NULL;
				t->sel_chr = NULL;
				t->focus = fo;
				fprintf(fp, "</td\n>");
			} else {
				cells[i]->scan_selection(cb, cells[i]);	
			}
		}
	}
	if (multi) {
		fprintf(fp, "</tr\n>");
		fprintf(fp, "</table\n>");
	}
}

void Xd6HtmlTagTable::cut()
{
	int i = 0;
	for (i = 0; i < nb_cells; i++) {
		if (cells[i] && cells[i]->focus) {	
			cells[i]->cut();	
			return;
		}
	}
}

void Xd6HtmlTagTable::take_focus(int t)
{
	int i = 0;
	Xd6HtmlTagTd *ce = NULL;;
	for (i = 0; i < nb_cells; i++) {
		if (cells[i]) {	
			if (!t && !ce) ce = cells[i];
			if (t) ce = cells[i];
		}
	}
	if (!ce) return;
	ce->focus = ce;
}

void Xd6HtmlTagTable::to_html(FILE *fp)
{
	int i, j;
	fprintf(fp, "<table width=\"%d\" border=\"%d\" cellspacing=\"%d\""
		" cellpadding=\"%d\" x:str >\n", 
		width, t_border, t_cellspacing, t_cellpadding);

	for (i = 0; i < max_td; i++) {
		fprintf(fp, "<col width=\"%d\" />\n", cols_pos[i]);
	}
	j = -1;
	for (i = 0; i < nb_cells; i++) {
		if (i % max_td == 0) {
			j++;
			if (j > 0) fprintf(fp, "</tr>\n");
			fprintf(fp, "<tr height=\"%d\" >\n",
				rows_pos[j + 1] - rows_pos[j]);
		}
		if (cells[i]) {	
			cells[i]->to_html(fp);
		}
	}

	fprintf(fp, "</tr>\n</table>\n");
}

void Xd6HtmlTagTable::to_rtf(FILE *fp)
{
	int i,j = 0, x = 0, ii = 0;

	for (i = 0; i < nb_cells; i++) {
		if (i % max_td == 0) {
			int jj = 0;
			j++;
			if (j > 1) fprintf(fp, "\\row\n");
			fprintf(fp, "\\trowd ");
			x = 0;
			for (ii = i; (ii % max_td) != 0  || i == ii; ii++) {
				x += cols_pos[jj] * 20;
				if (((ii + 1) % max_td) == 0 || cells[ii + 1]) {
					fprintf(fp, "\\cellx%d\n", x);
				}
				jj++;
			}
		}
		if (cells[i]) {	
			fprintf(fp, "\\intbl ");
			cells[i]->to_rtf(fp);
			fprintf(fp, "\\cell ");
		}
	}
	fprintf(fp, "\\row\\pard\n");
}

