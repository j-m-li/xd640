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
#ifndef IconCanvas_h
#define IconCanvas_h

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Scroll.H>
#include <dirent.h>
#include <sys/stat.h>
#include "IconGroup.h"

struct file_info {
	const char *real_name;
	struct stat st;
};

class IconCanvas : public Fl_Scroll {
public:
	struct dirent **newlist;
	struct dirent **list;
	struct file_info **newinfo;
	struct file_info **info;
	int newnbf;
	int nbf;
	IconGroup *group;
	IconGroup *oldgroup;
	static int onmover;
        int drag_x;
        int drag_y;
        int dragging;

	IconCanvas(int X, int Y, int W, int H);
	~IconCanvas();
	void rescan(void);
	static void mover(void*);
	void draw_selector(int, int);
	int handle(int);
	void resize(int, int, int, int);
	void update_status(void);
	void draw(void);
};


#endif
