/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000,2001  O'ksi'D
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
#ifndef Xd6HtmlBrowser_h
#define Xd6HtmlBrowser_h

#include "xd640/Xd6HtmlView.h"
#include "Xd6HtmlNavigation.h"
#include <FL/Fl_Group.h>


class Xd6HtmlBrowser : public Fl_Group {
public:
	Xd6HtmlView *view;
	Xd6HtmlNavigation *tool;

	Xd6HtmlBrowser(int X, int Y, int W, int H);
	~Xd6HtmlBrowser(void);
	void load(const char *f);
	void resize(int X, int Y, int W, int H); 
	int handle(int e);
};

#endif

/*
 * "$Id: $"
 */
