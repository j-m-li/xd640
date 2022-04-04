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


#include "Xd6HtmlBrowser.h"
#include <stdio.h>
#include <stdlib.h>


Xd6HtmlBrowser::Xd6HtmlBrowser(int X, int Y, int W, int H) : Fl_Group(X, Y, W, H)
{
	tool = new Xd6HtmlNavigation(this, X, Y, W, 25);
	tool->end();
	view = new Xd6HtmlView(X, Y + 25, W, H - 25);
	view->frame->editor = 0;
	view->frame->wysiwyg = 0;
	
}


Xd6HtmlBrowser::~Xd6HtmlBrowser()
{
}

void Xd6HtmlBrowser::load(const char *f)
{
	view->load(f);
}

void Xd6HtmlBrowser::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	tool->resize(X, Y, W, 25);
	view->resize(X, Y + 25, W, H - 25);
}

int Xd6HtmlBrowser::handle(int e)
{
	int ret;

	ret = Fl_Group::handle(e);
	
	return ret;
}
/*
 * "$Id: $"
 */

