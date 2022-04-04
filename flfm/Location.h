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
#ifndef Location_h
#define Location_h

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Pixmap.H>

class Location : public Fl_Group {
public:
	char *url;
	Fl_Input *loc;
	Fl_Button *back;
	Fl_Button *up;
	Fl_Button *nfm;
	Fl_Pixmap *p1;
	Fl_Pixmap *p2;
	Fl_Pixmap *p3;

	Location(int X, int Y, int W, int H);
	~Location();
	int handle(int e);
	void new_url(const char *);
	void value(const char *val);
};


#endif
