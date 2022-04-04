/******************************************************************************
 *   "$Id: $"
 *
 *   This file is part of the FLE project. 
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
#ifndef IconGroup_h
#define IconGroup_h


class IconCanvas;

class IconGroup : public Fl_Group {
public:
	IconCanvas *canvas;
	int sx, sy, sw, sh;
	int lx, ly;
	int px, py , pw, ph;
	int draw_sel;

	IconGroup(int X, int Y, int W, int H, IconCanvas *C);
	~IconGroup();
	int handle(int e);
	void draw(void);
	void show_selector(int x, int y, int w, int h);
	void hide_selector(void);
	void overlay_rect(void);
};


class NormalGroup : public IconGroup {
public:
	NormalGroup(int X, int Y, int W, int H, IconCanvas *C);
	~NormalGroup();
};

class DetailGroup : public IconGroup {
public:
	DetailGroup(int X, int Y, int W, int H, IconCanvas *C);
	~DetailGroup();
};

#endif
