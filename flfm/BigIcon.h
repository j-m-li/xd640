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
#ifndef BigIcon_h
#define BigIcon_h

#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Pixmap.H>
#include "xd640/Xd6IconWindow.h"

struct file_info;
void reset_selection(void);

class BigIcon : public Fl_Menu_Button {
public:
	static Xd6IconWindow *drag_window;
	static Fl_Menu_Item *m_folder;
	static Fl_Menu_Item *m_exec;
	static Fl_Menu_Item *m_unknown;
	static Fl_Menu_Item *m_special;
	Fl_Pixmap *pix;
	const char *real_name;
	int selected;
	int width;
	int height;

	BigIcon(int X, int Y, int W, int H);
	~BigIcon();
	void set_data(struct file_info *fi);
	virtual int handle(int e);
	virtual void draw(void);
	virtual int is_inside(void);
	static void create_menus(void);
	static void create_drag_window(void);
};

#endif
