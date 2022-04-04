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
#include "Xd6HtmlTagTable.h"
#include "Xd6HtmlPrint.h"
#include <FL/fl_draw.h>

Xd6HtmlTagTable::Xd6HtmlTagTable(int i, Xd6XmlTreeElement *e, Xd6HtmlDisplay *p) : 
	Xd6HtmlDisplay(i, NULL, 0, e->style)
{
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
		t_border = 1;
	}
	t_cellspacing = get_length(e->get_attr_value("cellspacing"), t_width);
	t_cellpadding = get_length(e->get_attr_value("cellpadding"), t_width);
	if (t_cellspacing < 0) t_cellspacing = 1;
	if (t_cellpadding < 0) t_cellpadding = 1;
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
	int i;
	
	X += left;
	Y += top;

	fl_color(FL_RED);
	for (i = 0; i < t_border; i++) {
		fl_rect(X + i, Y + i, width - 2 * i, 
			(height - descent) - 2 * i);
	}
	fl_color(FL_BLACK);


	fl_clip(X + t_border, Y + t_border, width - 2 * i, 
		(height - descent) - 2 * i);

	for (i = 0; i < nb_cells; i++) {
		if (cells[i]) {
			cells[i]->draw(X, Y);
		}
	}
	
	fl_pop_clip();
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
	if (k < t_width && !parentf->editor) {
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
				if (row_heights[ii] > 0) ch += row_heights[ii]; 
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

		while (s->cols_pos[s->col] > s->row) {
			s->col++;
		}

		c = s->col + (s->row * s->max_td);
		s->cells[c] = new Xd6HtmlTagTd(c, e, s);
	
		s->cells[c]->page_width = 2;
		s->cells[c]->cellpadding = s->t_cellpadding;
		if (s->cells[c]->border < 0) s->cells[c]->border = s->t_border;
		s->cells[c]->scroll = 0;
		s->cells[c]->wysiwyg = 0;
		s->cells[c]->tree2block(e);
		//s->cells[c]->measure();

		j = s->cells[c]->colspan;
		while (j > 0) {
			j--;
			s->cols_pos[s->col] += s->cells[c]->rowspan;
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
			mh = 0;
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

int Xd6HtmlTagTable::handle(int e, int x, int y)
{
	int i, ret = 0;
	x += left;
	y += top;

	for (i = 0; i < nb_cells; i++) {
		if (cells[i]) {	
			int w, h;
			if ((e != FL_PUSH && cells[i]->focus) || e == FL_PUSH) {
				w = cells[i]->width;
				h = cells[i]->height;
				int r = cells[i]->handle(e, x, y);
				if (r) ret = 1;
				if (cells[i]->width != w || 
					cells[i]->height != h)
				{
					need_resize = 1;
				}
			} else {
				cells[i]->focus = NULL;
			}
		}
	}
	return ret;
}

void Xd6HtmlTagTable::change_style(int and_mask, int or_mask)
{
	int i = 0;
	for (i = 0; i < nb_cells; i++) {
		if (cells[i] && cells[i]->focus) {	
			cells[i]->change_style(and_mask, or_mask);	
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


void Xd6HtmlTagTable::select_to_html(const char *f)
{
	int i = 0;
	for (i = 0; i < nb_cells; i++) {
		if (cells[i] && cells[i]->focus) {	
			cells[i]->select_to_html(f);	
			return;
		}
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

