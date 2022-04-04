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


#include "TextEditor.h"
#include "xd640/Xd6HtmlPrint.h"
#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl.h>
#include "xd640/Xd6CheckButton.h"
#include "xd640/Xd6SpellChoice.h"
#include "xd640/Xd6FindDialog.h"
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_File_Chooser.H>
#include <ctype.h>
#include <libintl.h>

#define _(String) gettext((String))

TextEditor::TextEditor(int X, int Y, int W, int H) : Xd6HtmlView(X, Y, W, H)
{
	frame->wysiwyg = 0;
}


TextEditor::~TextEditor()
{
}

void TextEditor::parse()
{

}

void TextEditor::load(const char *f)
{
	FILE *fp;
	int c;
	int style = FONT_SIZE_3|PREFORMATED|FONT_MONOSPACE|IS_BLOCK;
	Xd6HtmlBlock *b;
	Xd6HtmlSegment *s;
	char *t;
        frame->sel_chr = NULL;
        frame->cur_chr = NULL;
	fp = fopen(f, "r");
	if (!fp) return;
        while (frame->nb_blocks > 0) {
                delete(frame->blocks[--frame->nb_blocks]);
        }

	frame->add_block();
	frame->blocks[0]->add_segment((char*)malloc(128), 0, style);
	b = frame->blocks[0];
	s = b->segs[b->nb_segs - 1];
	t = s->text;
	while ((c = fgetc(fp)) != EOF) {
		if (c == '\n') {
			s->text = (char*) realloc(s->text, s->len + 1);
			frame->add_block();
			b = frame->blocks[frame->nb_blocks - 1];
			b->add_segment((char*)malloc(128), 0, style);
			s = b->segs[b->nb_segs - 1];
			t = s->text;
		} else if (c == '\t') {
			s->text = (char*) realloc(s->text, s->len + 1);
			b->add_segment(strdup(""), 0, 0);
			s = b->segs[b->nb_segs - 1];
			delete(s);
			s = new Xd6HtmlDisplay(b->nb_segs - 1, strdup("\t"), 1,
				style|DISPLAY);
			((Xd6HtmlDisplay*)s)->display = DISPLAY_TAB;
			b->segs[b->nb_segs - 1] = s;
			b->add_segment((char*)malloc(128), 0, style);
			s = b->segs[b->nb_segs - 1];
			t = s->text;
		} else {
			if (s->len > 125) {
				b->add_segment((char*)malloc(128), 0, style);
				s = b->segs[b->nb_segs - 1];
				t = s->text;
			}
			t[s->len++] = (char) c;
		}
	}
        frame->measure();
        redraw();
	fclose(fp);
}

void TextEditor::blank()
{
	frame->cur_chr = NULL;
	frame->sel_chr = NULL;
        while (frame->nb_blocks > 0) {
                delete(frame->blocks[--frame->nb_blocks]);
        }
        frame->add_block();
        frame->blocks[0]->add_segment((char*)malloc(1), 0,
                IS_INLINE|FONT_SIZE_3|FONT_MONOSPACE|PREFORMATED);
        frame->measure();
        redraw();
}

/*
 * "$Id: $"
 */

