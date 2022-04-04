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
typedef int (*Xd6HtmlFrameTextEngine)(int, Xd6HtmlFrame*, Xd6HtmlBlock*, 
		Xd6HtmlLine*, Xd6HtmlSegment*, char*);

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
	Xd6XmlStl *lstyle;
	Xd6HtmlBlock *lb;
	int was_space;
	
	int background;
	int wysiwyg;
	int editor;
	int scroll;
	
	Window off_screen;
	static Fl_Output *status_bar;

	static Xd6HtmlFrameEngine engine;
	static Xd6HtmlFrameTextEngine text_engine;

	static int *tab_stop;
	static int rule_width;

	char *quote;

	char need_paging;
	char modified;

	int hot_x, hot_y;
	char is_hot;

	Xd6HtmlFrame(int i);
	~Xd6HtmlFrame(void);

	static int def_text_engine(int e, Xd6HtmlFrame *frame, 
		Xd6HtmlBlock *block,
        	Xd6HtmlLine *line, Xd6HtmlSegment *seg, char *text);
	void add_block();
	void add_segment(Xd6HtmlBlock *b, char *t, int l, Xd6XmlStl *s);
	Xd6HtmlDisplay* special_elements(Xd6XmlTreeElement *elem, 
		Xd6HtmlFrame *cp);
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
	static int get_tab(int l, Xd6XmlStl *s); 
	void zoom(int z);

	// Edit
	void check_parent(void);
	void clean_block(Xd6HtmlBlock *b, Xd6HtmlSegment **s, char **c);
	void merge_segments(Xd6HtmlBlock *b, Xd6HtmlSegment *s1, 
		Xd6HtmlSegment *s2);
	void merge_blocks(Xd6HtmlBlock *b, Xd6HtmlBlock *b2, char **cursor);
	void save_padding(void);
	void restore_padding(void);
	void insert_block_after(Xd6HtmlBlock *b);
	void split_segment(Xd6HtmlBlock *b, Xd6HtmlSegment *s, char *c);
	void split_block(Xd6HtmlBlock *b, Xd6HtmlSegment *s, char *c);
	void shift_blocks(int nb);
	void change_style(Xd6XmlStl *n);
	void change_block_style(Xd6HtmlBlock *b, Xd6XmlStl *n);
	void change_begin_block_style(Xd6HtmlBlock *b1, Xd6HtmlLine *l1,
		Xd6HtmlSegment *s1, Xd6XmlStl *n);
	void change_end_block_style(Xd6HtmlBlock *b1, Xd6HtmlLine *l1,
		Xd6HtmlSegment *s1, Xd6XmlStl *n);
	void change_middle_block_style(Xd6HtmlBlock *b, Xd6HtmlLine *l1,
		Xd6HtmlSegment *s1, Xd6HtmlLine *l2, Xd6HtmlSegment *s2,
		Xd6XmlStl *n);
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
	void seg_to_html(FILE *fp, Xd6HtmlSegment *s, Xd6XmlStl *style);
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
	void go_previous_seg(void);
	void go_next_seg(void);
	void go_char_back(void);
	void go_char_forward(void);
        int handle_arrows(void);
	int handle_keys(void);
        int handle(int e, int x, int y);
	int handle_click(void);
	int handle_external(int e, int x, int y);
	virtual void auto_scroll(int X, int Y, int flow_cur = 0);
	void hotspot(int x, int y);
	int check_hotspot(int x, int y);
	void load(void);
	static int handle_click(int e, Xd6HtmlFrame *frame, Xd6HtmlBlock *block,
		Xd6HtmlLine *line, Xd6HtmlDisplay *seg, char *chr);
};

#endif

/*
 * "$Id: $"
 */
