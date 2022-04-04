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


#ifndef Xd6HtmlScroll_h
#define Xd6HtmlScroll_h

#include <FL/x.h>
#include "Xd6HtmlDisplay.h"
#include "Xd6HtmlBlock.h"
#include "Xd6XmlTree.h"
#include "Xd6ConfigFile.h"
#include <stdio.h>
#include <string.h>

class Xd6HtmlView;

enum Xd6HtmlScrollFlags {
        CLICK_OVER_VSCROLL = 0x10,
        CLICK_OVER_HSCROLL = 0x20,
        CLICK_OVER_SCROLL = 0x40,
};

//#define FL_DAMAGE_BLOCK 0x10
//#define FL_DAMAGE_ARROWS 0x20


class Xd6HtmlScroll : public Xd6HtmlDisplay {
public:
	int flag;
	int max_width;
	int max_height;
	int vscroll;
	int hscroll;
	int ovscroll;
	int ohscroll;

	Xd6HtmlScroll(int i);
	~Xd6HtmlScroll(void);

	void draw_hscroll(int X, int Y);
	void draw_vscroll(int X, int Y);
	int handle_push_hscroll(int ox, int x);
	int handle_push_vscroll(int oy, int y);
        int handle_drag_hscroll(int ox, int ex);
        int handle_drag_vscroll(int oy, int ey);
	int handle_scroll(int e, int ex, int ey, int x, int y);

	void go_bottom(void);
	void go_top(void);
};

#endif

/*
 * "$Id: $"
 */
