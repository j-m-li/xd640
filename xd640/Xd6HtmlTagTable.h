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


#ifndef Xd6HtmlTagTable_h
#define Xd6HtmlTagTable_h

#include "Xd6HtmlDisplay.h"
#include "Xd6HtmlFrame.h"
#include "Xd6HtmlTagTd.h"

class Xd6HtmlTagTable;

typedef void (*TableScanCallback)(Xd6HtmlTagTable *, Xd6XmlTreeElement *);

class Xd6HtmlTagTable : public Xd6HtmlFrame {
public:
	Xd6HtmlTagTable(int id, Xd6XmlTreeElement *elem, Xd6HtmlFrame *p);
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
	void draw_selected(int X, int Y);
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
	void change_style(Xd6XmlStl *n);
	void insert_frame(Xd6HtmlFrame *f);
	void insert_segment(Xd6HtmlSegment *s);
	Xd6HtmlFrame *get_cursor_frame(void);
	void cut(void);
	void scan_selection(ScanCallback cb, void *data);
	int send_fake_push(Xd6HtmlTagTd *ce, int x, int y,
		int cx, int cy, int fx, int fy, int dx, int dy);
	void take_focus(int t);
};

#endif

/*
 * "$Id: $"
 */
