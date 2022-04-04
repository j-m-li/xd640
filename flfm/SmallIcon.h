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
#ifndef SmallIcon_h
#define SmallIcon_h

#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Pixmap.H>
#include "BigIcon.h"


class SmallIcon : public BigIcon {
public:
	char info[256];
	int offset;
	int info_len;

	SmallIcon(int X, int Y, int W, int H);
	~SmallIcon();
	void set_data(struct file_info *fi);
	virtual void draw(void);
	virtual int is_inside(void);
};

#endif
