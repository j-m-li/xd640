/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2001  O'ksi'D
 *
 *                      All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *      Neither the name of O'ksi'D nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER 
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
	
	if (e == FL_KEYBOARD && Fl::e_state & FL_CTRL &&  
		Fl::e_length == 1 && Fl::e_text[0] == '\r')
	{
		tool->page_break->do_callback();
		ret = 1;
	}

	if (e == FL_RELEASE || e == FL_KEYBOARD) {
		Xd6HtmlFrame *frm = view->frame->get_cursor_frame();
		if (frm && frm->cur_seg) {
			tool->set_style(frm->cur_seg->stl,
				frm->cur_block->stl);
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

