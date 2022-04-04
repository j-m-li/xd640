/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000-2002  O'ksi'D
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
#ifndef Xd6HtmlEditor_h
#define Xd6HtmlEditor_h
#include <FL/Fl_Group.h>
#include "xd640/Xd6HtmlView.h"
#include "Xd6HtmlToolbar.h"



class Xd6HtmlEditor : public Fl_Group {
void end() {Fl_Group::end();};
public:
	Xd6HtmlView *view;
	Xd6HtmlToolbar *tool;
	int page_width;
	char *file;

	Xd6HtmlEditor(int X, int Y, int W, int H);
	~Xd6HtmlEditor(void);
	void load(const char *f);
	void resize(int X, int Y, int W, int H); 
	int handle(int e);
	void mailing(void);
	
};

#endif

/*
 * "$Id: $"
 */
