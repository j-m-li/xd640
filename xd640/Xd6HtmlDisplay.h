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


#ifndef Xd6HtmlDisplay_h
#define Xd6HtmlDisplay_h

#include "Xd6HtmlSegment.h"
#include <stdio.h>

#define FL_HIDE_CURSOR 128

class Xd6HtmlPrint;
class Xd6HtmlFrame;
class Xd6HtmlFrame;
class Xd6HtmlView;

class Xd6HtmlDisplay : public Xd6HtmlSegment {
public:
	int display;
        unsigned char damage_;
	Xd6HtmlFrame *parent;
        Xd6HtmlView *parent_widget;

	Xd6HtmlDisplay(int i, char *txt, int l, Xd6XmlStl *style);
	Xd6HtmlDisplay();
	virtual ~Xd6HtmlDisplay();

        unsigned char damage(void) {return damage_; }
        void damage(unsigned char d);

	virtual void measure(void);
	virtual void draw(int X, int Y);
	virtual void draw_selected(int X, int Y);
	virtual void print(Xd6HtmlPrint *p, int X, int Y);
	virtual void to_html(FILE *fp);
	virtual void to_rtf(FILE *fp);
	virtual int handle(int e, int x, int y);
	virtual void destroy(void);
	virtual void break_line(int h, int ph, int fh);
	virtual void load(const char *url, const char *file);
	int event_is_inside(int x, int y);
	static Xd6HtmlDisplay *create(int id, Xd6XmlTreeElement *elem,
				Xd6HtmlFrame *u);
	static void Xd6HtmlDisplay::pre_process(Xd6XmlTreeElement *e, 
				Xd6HtmlFrame *u);
	
	void redraw(void);
	int get_length(const char *str, int ref);
};

#endif

/*
 * "$Id: $"
 */
