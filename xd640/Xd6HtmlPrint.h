/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000  O'ksi'D
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
	void line_to_ps(Xd6HtmlBlock *b, Xd6HtmlLine *l, int X, int Y);
	void seg_to_ps(Xd6HtmlSegment *s, int X, int Y);
	int char_to_ps(char *text, int len, int X, int Y, int *wi);
	void ps_footer(int p);
	void new_page(void);
};


#endif

/*
 * "$Id: $"
 */
