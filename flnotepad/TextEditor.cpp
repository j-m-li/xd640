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



#include "TextEditor.h"
#include "xd640/Xd6HtmlPrint.h"
#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl.h>
#include "FL/Fl_Check_Button.H"
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
	Xd6XmlStl st; 
	Xd6XmlStl *sstyle; 
	Xd6XmlStl *dstyle; 
	Xd6XmlStl *bstyle; 
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

	st.copy(&st.def);
	st.font_size = FONT_SIZE_3;
	st.preformated = 1;
	st.font = FONT_MONOSPACE;
	st.is_inline = 0;
	st.is_block = 1;
	st.display = 0;
	bstyle = st.def.get_style(&st);
	st.is_inline = 1;
	st.is_block = 0;
	sstyle = st.def.get_style(&st);
	st.display = 1;
	dstyle = st.def.get_style(&st);

	printf("%p %p %p\n", bstyle, sstyle, dstyle);

	frame->add_block();
	frame->blocks[0]->add_segment((char*)malloc(128), 0, sstyle);
	b = frame->blocks[0];
	b->stl = bstyle;
	s = b->segs[b->nb_segs - 1];
	t = s->text;
	while (((c = fgetc(fp)) != EOF)) {
		if (c == '\n') {
			s->text = (char*) realloc(s->text, s->len + 1);
			frame->add_block();
			b = frame->blocks[frame->nb_blocks - 1];
			b->stl = bstyle;
			b->add_segment((char*)malloc(128), 0, sstyle);
			s = b->segs[b->nb_segs - 1];
			t = s->text;
		} else if (c == '\t') {
			s->text = (char*) realloc(s->text, s->len + 1);
			b->add_segment(strdup(""), 0, dstyle);
			s = b->segs[b->nb_segs - 1];
			delete(s);
			s = new Xd6HtmlDisplay(b->nb_segs - 1, strdup("\t"), 1,
				dstyle);
			((Xd6HtmlDisplay*)s)->display = DISPLAY_TAB;
			b->segs[b->nb_segs - 1] = s;
			b->add_segment((char*)malloc(128), 0, sstyle);
			s = b->segs[b->nb_segs - 1];
			t = s->text;
		} else {
			if (s->len > 125) {
				b->add_segment((char*)malloc(128), 0, sstyle);
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
	Xd6XmlStl st; 
	frame->cur_chr = NULL;
	frame->sel_chr = NULL;
        while (frame->nb_blocks > 0) {
                delete(frame->blocks[--frame->nb_blocks]);
        }

	st.copy(&st.def);
	st.font_size = FONT_SIZE_3;
	st.preformated = 1;
	st.font = FONT_MONOSPACE;
	st.is_block = 0;
	st.is_inline = 1;

        frame->add_block();
        frame->blocks[0]->add_segment((char*)malloc(1), 0, st.get_style(&st));
        frame->measure();
        redraw();
}

/*
 * "$Id: $"
 */

