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
#ifndef Xd6HtmlTagTable_h
#define Xd6HtmlTagTable_h

#include "Xd6HtmlDisplay.h"
#include "Xd6HtmlTagTd.h"

class Xd6HtmlTagTable;

typedef void (*TableScanCallback)(Xd6HtmlTagTable *, Xd6XmlTreeElement *);

class Xd6HtmlTagTable : public Xd6HtmlDisplay {
public:
	Xd6HtmlTagTable(int id, Xd6XmlTreeElement *elem, Xd6HtmlDisplay *p);
	~Xd6HtmlTagTable();
	
	Xd6HtmlFrame *parentf;
	int t_width;
	int t_border;
	int t_cellspacing;
	int t_cellpadding;

	int col;
	int max_td;
	int row;
	int max_tr;
	
	Xd6HtmlTagTd **cells;
	int nb_cells;
	int *cols_pos;
	int *rows_pos;
	
	int *col_widths; 
	int *row_heights;

	int need_resize;

	void draw(int X, int Y);
	void print(Xd6HtmlPrint *p, int X, int Y);
	void destroy(void);
	void measure(void);
	int handle(int e, int x, int y);
	void create_table(Xd6XmlTreeElement *e);
	void scan_table(Xd6XmlTreeElement *e, TableScanCallback cb);
	static void scan_cell_cb(Xd6HtmlTagTable *self, Xd6XmlTreeElement *e);
	static void scan_td_cb(Xd6HtmlTagTable *self, Xd6XmlTreeElement *e);
	void break_line(int h, int ph, int fh);
	void to_html(FILE *fp);
	void to_rtf(FILE *fp);
	void change_style(int and_mask, int or_mask);
	void insert_frame(Xd6HtmlFrame *f);
	void insert_segment(Xd6HtmlSegment *s);
	Xd6HtmlFrame *get_cursor_frame(void);
	void cut(void);
	void select_to_html(const char *f);
};

#endif

/*
 * "$Id: $"
 */
