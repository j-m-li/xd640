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
#ifndef Xd6HtmlFrame_h
#define Xd6HtmlFrame_h

#include <FL/Fl_Widget.h>
#include <FL/Fl_Output.h>
#include <FL/x.h>
#include "Xd6HtmlScroll.h"
#include "Xd6HtmlBlock.h"
#include "Xd6XmlTree.h"
#include "Xd6ConfigFile.h"
#include <stdio.h>
#include <string.h>
#include "Xd6HtmlDownload.h"

typedef void (*ScanCallback)(Xd6HtmlBlock*, Xd6HtmlLine *, Xd6HtmlSegment *, 
	char *, int, void *);
typedef int (*Xd6HtmlFrameEngine)(int, Xd6HtmlFrame*, Xd6HtmlBlock*, 
		Xd6HtmlLine*, Xd6HtmlDisplay*, char*);

class Xd6HtmlFrame : public Xd6HtmlScroll {
public:
	Xd6HtmlDisplay *focus;

	Xd6HtmlBlock **blocks;
	int nb_blocks;

	int page_margin_top;
	int page_margin_left;
	int page_margin_right;
	int page_margin_bottom;
	int page_height;
	int page_width;
	int page_land;
	int footer_height;

	Xd6HtmlBlock *sel_block;
	Xd6HtmlLine *sel_line;
	Xd6HtmlSegment *sel_seg;
	char *sel_chr;
	Xd6HtmlBlock *cur_block;
	Xd6HtmlLine *cur_line;
	Xd6HtmlSegment *cur_seg;
	char *cur_chr;

	Xd6HtmlBlock *b1, *b2;
	Xd6HtmlLine *l1, *l2;
	Xd6HtmlSegment *s1, *s2;
	char *c1, *c2;

	int nb_blk;
	int *blk_paddings;
	int *blk_heights;
	int blk_nb_lines;
	Xd6HtmlBlock *blk_current;
	
	char **open_tags;
	char **close_tags;
	int nb_tags;
	
	char *url;
	char *file;

        static int paste_buffer_malloc;
        static int paste_buffer_len;
        static char *paste_buffer;

	FILE *fp;
	int lstyle;
	Xd6HtmlBlock *lb;
	int was_space;
	
	int background;
	int wysiwyg;
	int editor;
	int scroll;
	
	Window off_screen;
	static Fl_Output *status_bar;

	static Xd6HtmlFrameEngine engine;

	static int *tab_stop;
	static int rule_width;

	char *quote;

	char need_paging;
	char modified;

	Xd6HtmlFrame(int i);
	~Xd6HtmlFrame(void);

	void add_block();
	void add_segment(Xd6HtmlBlock *b, char *t, int l, int s);
	Xd6HtmlDisplay* special_elements(Xd6XmlTreeElement *elem, 
		Xd6HtmlDisplay *cp);
	void tree2block(Xd6XmlTreeElement *elem, Xd6HtmlDisplay *cp = NULL);
	void text_tree2block(Xd6XmlTreeElement *elem);
	void measure(void);
	void resize(int W, int H);
	void page_size(int W, int H, int FH);
	int break_line(Xd6HtmlLine *l, int height);
	void create_pages(Xd6HtmlBlock *b = NULL);
	void find_pos(int X, int Y, Xd6HtmlBlock **b, Xd6HtmlLine **l, 
		Xd6HtmlSegment **s, char **c);
	void cut(int fast = 0);
	void set_cut_cursor(char *cursor, Xd6HtmlSegment *s, Xd6HtmlBlock *b);
	static int get_tab(int l, int s); 
	void zoom(int z);

	// Edit
	void merge_blocks(Xd6HtmlBlock *b, Xd6HtmlBlock *b2, char **cursor);
	void save_padding(void);
	void restore_padding(void);
	void insert_block_after(Xd6HtmlBlock *b);
	void split_segment(Xd6HtmlBlock *b, Xd6HtmlSegment *s, char *c);
	void split_block(Xd6HtmlBlock *b, Xd6HtmlSegment *s, char *c);
	void shift_blocks(int nb);
	void change_style(int and_mask, int or_mask);
	void change_block_style(Xd6HtmlBlock *b, int and_mask, int or_mask);
	void change_begin_block_style(Xd6HtmlBlock *b1, Xd6HtmlLine *l1,
		Xd6HtmlSegment *s1, int and_mask, int or_mask);
	void change_end_block_style(Xd6HtmlBlock *b1, Xd6HtmlLine *l1,
		Xd6HtmlSegment *s1, int and_mask, int or_mask);
	void change_middle_block_style(Xd6HtmlBlock *b, Xd6HtmlLine *l1,
		Xd6HtmlSegment *s1, Xd6HtmlLine *l2, Xd6HtmlSegment *s2,
		int and_mask, int or_mask);
	void insert_char(Xd6HtmlSegment *s, char *c, const char *txt, int len);
	void insert_text(const char *txt, int len);
	void insert_segment(Xd6HtmlSegment *s);
	void insert_frame(Xd6HtmlFrame *f);
	Xd6HtmlFrame *get_cursor_frame(void);
	void cursor_to_end(void);
	void cursor_to_begin(void);
	void select_all(void);

	// Export
	static void html_header(const char *name, FILE *fp);
	static void scan_to_text_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s,
        	char *c, int len , void *d);

	void to_text_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s,
        	char *c, int len);
	void to_text(const char *name);
	void to_html_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s,
        	char *c, int len);
	void to_html(const char *name);
	void select_to_html(const char *name);
	void block_to_html(FILE *fp, Xd6HtmlBlock *b, Xd6HtmlBlock *lb);
	void seg_to_html(FILE *fp, Xd6HtmlSegment *s, int style);
	static void text_to_html(FILE *fp, char *txt, int len, int nonbsp = 0);
	void push_tag(FILE *fp, char *otag, char *ctag);
	void pop_tag(FILE *fp, char *ctag);
	void pop_all_tag(FILE *fp);
	void scan_selection(ScanCallback cb, void *data);
	void scan_all(ScanCallback cb, void *data);
	void select_to_buffer(void);
	void sort_selection(Xd6HtmlBlock **b1, Xd6HtmlLine **l1, 
		Xd6HtmlSegment **s1, char **c1, Xd6HtmlBlock **b2, Xd6HtmlLine **l2,
		Xd6HtmlSegment **s2, char **c2);

	
	// Drawings
	void create_off_screen(void);
	void draw(int X, int Y);
	void draw_cursor(int X, int Y);
	void draw_selection(int X, int Y, Xd6HtmlBlock *b, Xd6HtmlLine *l);
	void draw_clipped(int x, int y, int mw, int mh);

	// Handlers
	void move_cursor(int x, int y);
        void move_cursor_left(int r = 0);
        void move_cursor_right(int r = 0);
        void move_cursor_down(void);
        void move_cursor_up(void);
	void go_char_back(void);
	void go_char_forward(void);
        int handle_arrows(void);
	int handle_keys(void);
        int handle(int e, int x, int y);
	int handle_click(void);
	int handle_external(int e, int x, int y);
	virtual void auto_scroll(int X, int Y, int flow_cur = 0);
	void load(void);
	static int handle_click(int e, Xd6HtmlFrame *frame, Xd6HtmlBlock *block,
		Xd6HtmlLine *line, Xd6HtmlDisplay *seg, char *chr);
};

#endif

/*
 * "$Id: $"
 */
