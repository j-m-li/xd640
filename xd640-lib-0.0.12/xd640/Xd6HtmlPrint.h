/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000  O'ksi'D
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
#ifndef Xd6HtmlPrint_h
#define Xd6HtmlPrint_h

#include "Xd6HtmlFrame.h"
#include "Xd6ConfigFile.h"
#include "Xd6Std.h"
#include <FL/fl_draw.h>
#include <FL/Fl_Window.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

class Xd6HtmlPsFont {
public:
	char glyphs[0x10000];
	int font;

	Xd6HtmlPsFont();
	~Xd6HtmlPsFont();
	void create_glyph(char *s, int l, int ucs, FILE* fp);
};

class Xd6HtmlPrint {
public:
	Xd6HtmlFrame *frm;
	char out_file[1024];
#ifdef WIN32
	HDC hDC;
	double ratio;
#else
	Xd6HtmlPsFont fonts[12];
	char font_file[1024];
	char text_file[1024];
	
	FILE *ffp;
	FILE *tfp;
#endif
	int page_nb;
	int from_p;
	int to_p;
	int p_to_file;
	char *cmd;

	Xd6HtmlPrint(Xd6HtmlFrame *f, const char *file);
	~Xd6HtmlPrint();
	void print(Xd6HtmlFrame *f);
	void direct_print(Xd6HtmlFrame *f);
	void frame_to_ps(int f, int t);
	void block_to_ps(Xd6HtmlBlock *b);
	void line_to_ps(Xd6HtmlLine *l, int X, int Y);
	void seg_to_ps(Xd6HtmlSegment *s, int X, int Y);
	int char_to_ps(char *text, int len, int X, int Y, int *wi);
	void ps_footer(int p);
	void new_page(void);
};


#endif

/*
 * "$Id: $"
 */
