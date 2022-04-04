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


#include "IconCanvas.h"
#include "IconGroup.h"
#include "BigIcon.h"
#include "SmallIcon.h"
#include "callbacks.h"
#include <FL/fl_draw.H>

#define _(String) gettext((String))


IconGroup::IconGroup(int X, int Y, int W, int H, IconCanvas *C) : 
	Fl_Group(X, Y, W, H)
{
	canvas = C;
	draw_sel = 0;
	sw = sh = sx = sy = 0;
	lx = X;
	ly = Y;
	px = py = ph = pw = 0;
	box(FL_FLAT_BOX);
}

IconGroup::~IconGroup()
{
}

void IconGroup::show_selector(int x, int y, int w, int h)
{
	sx = x;
	sy = y;
	sw = w;
	sh = h;
	draw_sel = 1;
}

void IconGroup::hide_selector()
{
	draw_sel = 0;
	fl_clip(parent()->x(), parent()->y(), parent()->w() - 16, 
		parent()->h() - 16);
	if (pw) overlay_rect(); 
	pw = 0;
	fl_pop_clip();
}

void IconGroup::overlay_rect()
{
  XSetFunction(fl_display, fl_gc, GXxor);
  XSetForeground(fl_display, fl_gc, 0xffffffff);
  XDrawRectangle(fl_display, fl_window, fl_gc, px, py, pw, ph);
  XSetFunction(fl_display, fl_gc, GXcopy);
}

void IconGroup::draw()
{
	if (draw_sel && pw) {
		overlay_rect(); 
		pw = 0;
	}
	lx = x();
	ly = y();
	Fl_Group::draw();
	if (draw_sel) {
		px = sx; py = sy; pw = sw; ph = sh;
		if (pw) overlay_rect();
	}
}

int IconGroup::handle(int e)
{
	return Fl_Group::handle(e);
}

NormalGroup::NormalGroup(int X, int Y, int W, int H, IconCanvas *C) : 
	IconGroup(X, Y, W, H, C)
{
	int i;
	BigIcon *b;
	int xx = x() + 15;
	int yy = y() + 15;
	int mx;
	int ww, hh;
	int lineh = 0;
	
	mx = w() + x() - 30;

	i = 0;
	begin();	
	while (i < canvas->nbf) {
		b = new BigIcon(xx, yy, 32, 32);
		b->labelfont(gui->font);
		b->labelsize(gui->size);
		b->set_data(canvas->info[i]);
		if (b->height > lineh) lineh = b->height;
		if (xx + b->width > mx) {
			yy += lineh + 15;
			lineh = 0;
			xx = x() + 15;
			b->resize(xx, yy, b->w(), b->h());
			xx += b->width + 15;
		} else {
			xx += b->width + 15;
		}
		i++;
	}
	yy += lineh + 15;
	end();	
	resizable(NULL);
	ww = w();
	hh = yy - y() + 80;
	if (ww < parent()->w() - 16) {
		ww = parent()->w() - 16;
	} 
	if (hh < parent()->h() - 16) {
		hh = parent()->h() - 16;
	}
	resize(x(), y(), ww, hh);
	redraw();
}

NormalGroup::~NormalGroup()
{
}

DetailGroup::DetailGroup(int X, int Y, int W, int H, IconCanvas *C) : 
	IconGroup(X, Y, W, H, C)
{
	int i;
	SmallIcon *b;
	int xx = x() + 5;
	int yy = y() + 30;
	int lineh = gui->size + 4;
	int mw;

	fl_font(gui->font, gui->size);
	mw  = (int) fl_width("W") * 150;

	if (lineh < 18) lineh = 18;

	i = 0;
	begin();	
	while (i < canvas->nbf) {
		b = new SmallIcon(xx, yy, mw, lineh);
		b->labelfont(gui->font);
		b->labelsize(gui->size);
		b->set_data(canvas->info[i]);
		yy += lineh + 4;
		i++;
	}
	end();	
	resizable(NULL);
	resize(x(), y(), mw + 50, yy + 20);
	redraw();
}

DetailGroup::~DetailGroup()
{
}


