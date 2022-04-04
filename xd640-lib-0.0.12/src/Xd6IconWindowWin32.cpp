/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2002  O'ksi'D
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

#include "Xd6Std.h"
#include "Xd6IconWindow.h"

Xd6IconWindow::Xd6IconWindow(const char *xpm_file, 
	VirtualWindow *window, int W, int H) : Fl_Window(W, H)
{

}

Xd6IconWindow::~Xd6IconWindow()
{
}

void Xd6IconWindow::show()
{
}

void Xd6IconWindow::hide()
{
}

void Xd6IconWindow::draw()
{
}

int Xd6IconWindow::handle(int e)
{
	return 0;
}

char **Xd6IconWindow::parse_xpm(char *d, int l)
{
	static char *b = "i";
	return &b;
}





void set_wm_icon(char *const*p, Fl_Window *w)
{

}


/*
 * "$Id: $"
 */
