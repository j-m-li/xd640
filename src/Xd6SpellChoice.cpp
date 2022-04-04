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



#include "Xd6SpellChoice.h"
#include "Xd6Std.h"
#include <FL/Fl_Button.h>
#include <FL/fl_utf8.h>
#include <string.h>

#define _(String) gettext((String))

int Xd6SpellChoice::status = 0;

Xd6SpellChoice::Xd6SpellChoice(const char *w, FILE *dict) : Fl_Window(240, 265)
{
	word = w;
	fp = dict;
	label(_("Spelling"));

	sb_guesses = new Fl_Select_Browser(5, 20, 230, 110, _("Guesses:"));
	sb_guesses->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
	sb_guesses->callback(cb_g);
	sb_guesses->has_scrollbar(Fl_Browser::BOTH_ALWAYS);
	find_guesses();
	sb_guesses->position(0);

	i_value = new Fl_Input(5, 135, 230, 20);
	i_value->value(w);

	status = 0;
	b_ignore = new Fl_Button(120, 165, 110, 25, _("Ignore"));
//	b_always_ignore = new Fl_Button(120, 190, 110, 25, _("Always Ignore"));
	b_replace = new Fl_Button(5, 165, 110, 25, _("Replace"));
//	b_always_replace =  new Fl_Button(5, 190, 110, 25, _("Always Replace"));
	b_find_guesses = new Fl_Button(5, 215, 110, 25, _("Guess"));
	b_cancel = new Fl_Button(120, 215, 110, 25, _("Cancel"));
	b_personal = new Fl_Button(5, 240, 110, 25, _("Add to dict."));

	b_ignore->callback(cb_i);
	//b_always_ignore->callback(cb_ai);
	b_replace->callback(cb_r);
	//b_always_replace->callback(cb_ar);
	b_find_guesses->callback(cb_fg);
	b_cancel->callback(cb_c);
	b_personal->callback(cb_p);

	set_modal();
	show();
}	


Xd6SpellChoice::~Xd6SpellChoice()
{
}


void Xd6SpellChoice::cb_i(Fl_Widget *w, void *d)
{
	Xd6SpellChoice *self;
	self = (Xd6SpellChoice*)w->parent();
	self->hide();
	self->status = 1;
}

void Xd6SpellChoice::cb_ai(Fl_Widget *w, void *d)
{
	Xd6SpellChoice *self;
	self = (Xd6SpellChoice*)w->parent();
	self->hide();
	self->status = 2;
}

void Xd6SpellChoice::cb_r(Fl_Widget *w, void *d)
{
	Xd6SpellChoice *self;
	self = (Xd6SpellChoice*)w->parent();
	self->hide();
	self->status = 3;
}

void Xd6SpellChoice::cb_ar(Fl_Widget *w, void *d)
{
	Xd6SpellChoice *self;
	self = (Xd6SpellChoice*)w->parent();
	self->hide();
	self->status = 4;
}

void Xd6SpellChoice::cb_c(Fl_Widget *w, void *d)
{
	Xd6SpellChoice *self;
	self = (Xd6SpellChoice*)w->parent();
	self->hide();
}

void Xd6SpellChoice::cb_g(Fl_Widget *w, void *d)
{
	Fl_Select_Browser *g = (Fl_Select_Browser*) w;
	Xd6SpellChoice *self;
	self = (Xd6SpellChoice*)w->parent();
	
	self->i_value->value(g->text(g->value()));
}

void Xd6SpellChoice::cb_fg(Fl_Widget *w, void *d)
{
	Xd6SpellChoice *self;
	self = (Xd6SpellChoice*)w->parent();
	
	self->word = self->i_value->value();
	self->sb_guesses->clear();
	self->find_guesses();
	self->redraw();
}

void Xd6SpellChoice::cb_p(Fl_Widget *w, void *d)
{
	Xd6SpellChoice *self;
	self = (Xd6SpellChoice*)w->parent();
	self->hide();
	self->status = 5;
}

void Xd6SpellChoice::find_guesses()
{
	int l;
	char buf[1024];
	if (!fp) return;
	rewind(fp);
	l = strlen(word);
	while (fgets(buf, 1024, fp)) {
		int i = 0;
		int sw = 0;
		int bl = strlen(buf) - 1;
		buf[bl] = '\0';
		
		if (bl == l && !fl_utf_strncasecmp(buf, word, l)) {
			sb_guesses->clear();
			sb_guesses->add(buf);
			return;
		}

		if (bl + 3 < l || bl - 3 > l) continue;
		buf[bl] = 0;
		while (i < l) {
			int ll;
			char *ptr = buf;
			int j = 0;

			ll = fl_utflen((unsigned char*)word, l - i);
			if (ll < 1) ll = 1;
			while (*ptr) {
				if (j == i) break;
				if (word[j] == *ptr) {
					j++;
					if (j == bl && !sw)  {
						sw = 1;
						sb_guesses->add(buf);
						break;
					}
				} else {
					ptr = buf + bl;
					break;
				}
				ptr++;
			}
			i += ll;
			if (i < l) {
				if (strstr(ptr, word + i)) {
					sb_guesses->add(buf);
				}
			}
		}
	}
}

/*
 * "$Id: $"
 */

