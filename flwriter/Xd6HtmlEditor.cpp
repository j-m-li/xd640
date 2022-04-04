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


#include "xd640/Xd6Std.h"
#include "Xd6HtmlEditor.h"
#include "Mailing.h"
#include <stdio.h>
#include <stdlib.h>


Xd6HtmlEditor::Xd6HtmlEditor(int X, int Y, int W, int H) : Fl_Group(X, Y, W, H)
{
	file = NULL;
	tool = new Xd6HtmlToolbar(this, X, Y, W, 25);
	tool->end();
	view = new Xd6HtmlView(X, Y + 25, W, H - 25);
	page_width = -1;
}


Xd6HtmlEditor::~Xd6HtmlEditor()
{
	free(file);
}

void Xd6HtmlEditor::load(const char *f)
{
	view->load(f);
}

void Xd6HtmlEditor::resize(int X, int Y, int W, int H)
{
	Fl_Group::resize(X, Y, W, H);
	tool->resize(x(), y(), w(), 25);
	view->resize(x(), y() + 25, w(), h() - 25);
}

int Xd6HtmlEditor::handle(int e)
{
	int ret;

	ret = Fl_Group::handle(e);
	
	if (e == FL_RELEASE) {
		if (view->frame->cur_seg) {
			tool->set_style(view->frame->cur_seg->style,
				view->frame->cur_block->style);
		}
	}
	return ret;
}

void Xd6HtmlEditor::mailing()
{
	Mailing *m;

	m = new Mailing(this);
	m->go();

	m->hide();
	delete(m);
}

/*
 * "$Id: $"
 */

