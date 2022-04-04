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


#ifndef Xd6HtmlTagTd_h
#define Xd6HtmlTagTd_h

#include "Xd6HtmlFrame.h"

enum {
	VALIGN_TOP 	= 1,
	VALIGN_MIDDLE,
	VALIGN_BOTTOM,
	VALIGN_BASELINE,
};

class Xd6HtmlTagTd : public Xd6HtmlFrame {
public:
	int rowspan;
	int colspan;
	int valign;
	int cellpadding;
	int border;
	int real_top;
	int real_left;
	int valign_pad;
	char *xnum;
	char *xfmla;
	char selected;

	Xd6XmlStl *ostl;
	Xd6XmlStl *nstl;

	Xd6HtmlTagTd(int id, Xd6XmlTreeElement *elem, Xd6HtmlFrame *p);
	~Xd6HtmlTagTd();
	
	
	void draw(int X, int Y);
	void destroy(void);
	void measure(void);
	int handle(int e, int x, int y);
	void resize(int w, int h);
	void break_line(int h, int ph, int fh);
	void print(Xd6HtmlPrint *p, int X, int Y);
	int event_is_inside(int x, int y);
	void to_html(FILE *fp);
	void to_rtf(FILE *fp);
};

#endif

/*
 * "$Id: $"
 */
