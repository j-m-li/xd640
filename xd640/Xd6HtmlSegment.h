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


#ifndef Xd6HtmlSegment_h
#define Xd6HtmlSegment_h

#include "Xd6XmlStyle.h"

enum Xd6HtmlSegmentFlags {
	DAMAGE_ALL 	= 1,
	DAMAGE_CHILD	= 2
};

class Xd6XmlStl;

class Xd6HtmlSegment {
public:
	unsigned short id;

	int top;
	int left;
	unsigned short descent;
	int width : 30;
	unsigned int flags : 2;
	int height;
	Xd6XmlStl *stl;

	char *text;
	unsigned short len;
	
	static int *sizes;
	static int sizes1[];
	static int sizes2[];
	static int sizes3[];
	static int sizes4[];

	static int bg_color;
	static int bg_x;
	static int bg_y;
	static int bg_w;
	static int bg_h;

	
	Xd6HtmlSegment(int i, char *txt, int l, Xd6XmlStl *style);
	Xd6HtmlSegment();
	~Xd6HtmlSegment();

	static void color(int c) {bg_color = c;}
	static void rectf(int x, int y, int w, int h) { 
		bg_x = x;
		bg_y = y;
		bg_w = w;
		bg_h = h;
	}

	static void draw_bg();
	void draw_bg(int X, int Y);
	void measure(void);
	void position(int X, int Y); // relative to Xd6HtmlLine pos.
	void find_pos(int x, int X, char **chr);
	void rtl_draw(const char *text, int len, int X, int Y, int W);
	void draw(int X, int Y);
	void draw_all(int X, int Y);
	void draw_end(int X, int Y, char *chr);
	void draw_begin(int X, int Y, char *chr);
	void draw_middle(int X, int Y, char *b, char *e);
	void cut_end(char *chr);
	void cut_begin(char *chr);
	char *cut_middle(char *b, char *e);
	
	void set_font(void);
	void set_color(void);
};

#endif

/*
 * "$Id: $"
 */
