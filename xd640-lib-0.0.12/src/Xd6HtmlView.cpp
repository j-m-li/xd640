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


#include "Xd6Std.h"
#include "Xd6HtmlView.h"
#include "Xd6HtmlPrint.h"
#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl.h>
#include <FL/fl_utf8.h>
#include "Xd6CheckButton.h"
#include "Xd6SpellChoice.h"
#include "Xd6FindDialog.h"
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <ctype.h>

#define _(String) gettext((String))

Xd6HtmlView::Xd6HtmlView(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H)
{
	new_frame(W, H);
	clean_tmp_dir();
}

void Xd6HtmlView::new_frame(int W, int H) 
{
        Xd6ConfigFile *cfg = NULL;
        Xd6ConfigFileSection *sec = NULL;
	Xd6ConfigFileGroup *grp = NULL;
	Xd6ConfigFileItem *itm = NULL;
	const char *val = NULL;

	frame = new Xd6HtmlFrame(0);
	frame->parent_widget = this;
	frame->resize(W, H);
	frame->create_off_screen();
	parser = new Xd6XmlParser();
	printer = new Xd6HtmlPrint(frame, "output.ps");

	paper_size = 0;
	margin_right = 28;
	margin_left = 84;
	margin_bottom = 56;
	margin_top = 56;
	landscape = 0;
	inch = 0;
	
	cfg = new Xd6ConfigFile("xd640", "Utilities");
	sec = cfg->get_xd640_section();
	grp = sec->get_group("page", cfg->locale);
	itm = grp->get_create_item("margin top", cfg->locale);
	val = itm->get_value(); if (val) margin_top = (int) atol(val);
	itm = grp->get_create_item("margin left", cfg->locale);
	val = itm->get_value(); if (val) margin_left = (int) atol(val);
	itm = grp->get_create_item("margin bottom", cfg->locale);
	val = itm->get_value(); if (val) margin_bottom = (int) atol(val);
	itm = grp->get_create_item("margin right", cfg->locale);
	val = itm->get_value(); if (val) margin_right = (int) atol(val);
	itm = grp->get_create_item("landscape", cfg->locale);
	val = itm->get_value(); if (val) landscape = (int) atol(val);
	itm = grp->get_create_item("inch", cfg->locale);
	val = itm->get_value(); if (val) inch = (int) atol(val);
	itm = grp->get_create_item("paper size", cfg->locale);
	val = itm->get_value(); 
	if (val) {
		if (!strcmp("A4", val)) paper_size = 0;
		else if (!strcmp("Letter", val)) paper_size = 1;
		else if (!strcmp("Legal", val)) paper_size = 2;
		else if (!strcmp("Executive", val)) paper_size = 3;
	}
	delete(cfg);
	
	set_frame_sizes();

	word_file = NULL;
	spell_ignore = NULL;
	spell_replace = NULL;
	spell_replace_word = NULL;
	spell_nb_i = 0;
	spell_nb_r = 0;

}


Xd6HtmlView::~Xd6HtmlView()
{
	while (spell_nb_i > 0) {
		free(spell_ignore[--spell_nb_i]);
	}
	while (spell_nb_r > 0) {
		free(spell_replace[--spell_nb_r]);
		free(spell_replace_word[spell_nb_r]);
	}
	free(spell_ignore);
	free(spell_replace);
	free(spell_replace_word);
	delete(frame);
	delete(parser);
	delete(printer);
}

void Xd6HtmlView::clean_tmp_dir()
{
	char *run;
	char *cmd = "cd /proc; /bin/ls -d [0-9]* > /tmp/xd640.pid;"
		" cd %s; /bin/ls -d [0-9]* >> /tmp/xd640.pid;"
		"/bin/rm -r `cat /tmp/xd640.pid | sort | uniq -u` 2>/dev/null;"
		"/bin/rm /tmp/xd640.pid 2>/dev/null&";

	Xd6ConfigFile cfg("xd640", "Utilities");
	run = (char*) malloc(strlen(cmd) + strlen(cfg.user_paths->apps) + 10);
	sprintf(run, cmd, cfg.user_paths->apps);
	system(run);
	free(run);
}

void Xd6HtmlView::set_frame_sizes()
{
	frame->page_margin_left = margin_left;
	frame->page_margin_right = margin_right;
	frame->page_margin_bottom = margin_bottom;
	frame->page_margin_top = margin_top;
	if (paper_size == 0) {		// A4
		frame->page_width = 595;
		frame->page_height = 842;
	} else if (paper_size == 1) { 	// Letter
		frame->page_width = 612;
		frame->page_height = 792;
	} else if (paper_size == 2) {	// Legal
		frame->page_width = 612;
		frame->page_height = 1008;
	} else {			// Executive
		frame->page_width = 540;
		frame->page_height = 720;
	}
	if (landscape) {
		int w;
		frame->page_land = 1;
		w = frame->page_width;
		frame->page_width = frame->page_height;
		frame->page_height = w;
	} else {
		frame->page_land = 0;
	}

	frame->page_width -= margin_left + margin_right;
	frame->page_height -= margin_top + margin_bottom;
	frame->resize(w(), h());

}

void Xd6HtmlView::set_sizes()
{
	char buf[32];
        Xd6ConfigFile *cfg = NULL;
        Xd6ConfigFileSection *sec = NULL;
	Xd6ConfigFileGroup *grp = NULL;
	
	set_frame_sizes();
	cfg = new Xd6ConfigFile("xd640", "Utilities");
	sec = cfg->get_xd640_section();
	grp = sec->add_group("page", cfg->locale);

	snprintf(buf, 32, "%d", margin_top);
	grp->add_item("margin top", buf, cfg->locale);
	snprintf(buf, 32, "%d", margin_left);
	grp->add_item("margin left", buf, cfg->locale);
	snprintf(buf, 32, "%d", margin_right);
	grp->add_item("margin right", buf, cfg->locale);
	snprintf(buf, 32, "%d", margin_bottom);
	grp->add_item("margin bottom", buf, cfg->locale);

	if (paper_size == 0) {		// A4
		snprintf(buf, 32, "A4");
	} else if (paper_size == 1) { 	// Letter
		snprintf(buf, 32, "Letter");
	} else if (paper_size == 2) {	// Legal
		snprintf(buf, 32, "Legal");
	} else {			// Executive
		snprintf(buf, 32, "Executive");
	}
	grp->add_item("paper size", buf, cfg->locale);

	snprintf(buf, 32, "%d", landscape);
	grp->add_item("landscape", buf, cfg->locale);

	snprintf(buf, 32, "%d", inch);
	grp->add_item("inch", buf, cfg->locale);

	cfg->write_xd640_section(sec);
	delete(cfg);
}

void Xd6HtmlView::page_setup()
{
	char buf[32];
	Fl_Widget *o;
	Fl_Menu_Item *mnu = new Fl_Menu_Item[5];
	double fact;
	
	if (inch) {
		fact = 1.0 / 72;
	} else {
		fact = 2.54 / 72;
	}
	
	memset(mnu, 0, 5 * sizeof(Fl_Menu_Item));
	mnu->add(_("A4 (210 x 297 mm)"), 0, NULL);
	mnu->add(_("Letter (8.5 x 11 in.)"), 0, NULL);
	mnu->add(_("Legal (8.5 x 14 in.)"), 0, NULL);
	mnu->add(_("Executive (7.5 x 10 in.)"), 0 , NULL);

	Fl_Window page_dialog(240, 240);
	page_dialog.label(_("Page Setup"));

	Fl_Choice paper(5, 25, 200, 25);
	paper.label(_("Paper Size:"));
	paper.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
	paper.menu(mnu);
	paper.value(paper_size);

	Fl_Input left(10, 75, 70, 20);
	snprintf(buf, 32, "%.3g", margin_left * fact);
	left.label(_("Margin left:"));
	left.value(buf);
	left.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);

	Fl_Input right(100, 75, 70, 20);
	snprintf(buf, 32, "%.3g", margin_right * fact);
	right.label(_("Margin right:"));
	right.value(buf);
	right.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);

	Fl_Input top(10, 120, 70, 20);
	snprintf(buf, 32, "%.3g", margin_top * fact);
	top.label(_("Margin top:"));
	top.value(buf);
	top.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);

	Fl_Input bottom(100, 120, 70, 20);
	snprintf(buf, 32, "%.3g", margin_bottom * fact);
	bottom.label(_("Margin bottom:"));
	bottom.value(buf);
	bottom.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);

	Xd6CheckButton land(15, 150, 25, 25);
	land.label(_("Landscape"));
	land.value(landscape);

	Xd6CheckButton metric(155, 150, 25, 25);
	metric.label(_("Inch"));
	metric.value(inch);

	Fl_Button ok(65, 205, 80, 25, _("OK"));
	Fl_Button cancel(155, 205, 80, 25, _("Cancel"));
	

	page_dialog.end();
	page_dialog.set_modal();
	page_dialog.show();
	for (;;) {
		o = Fl::readqueue();
		if (o == &metric) {
			if (metric.value()) {
				fact = 1.0 / 72;
			} else {
				fact = 2.54 / 72;
			} 
			snprintf(buf, 32, "%.3g", margin_left * fact);
			left.value(buf);
			snprintf(buf, 32, "%.3g", margin_right * fact);
			right.value(buf);
			snprintf(buf, 32, "%.3g", margin_top * fact);
			top.value(buf);
			snprintf(buf, 32, "%.3g", margin_bottom * fact);
			bottom.value(buf);
			page_dialog.redraw();
		} else if (o != &ok && o != &cancel) {
			Fl::wait();
		} else {
			break;
		}
	}
	page_dialog.hide();
	if (o == &ok) {
		margin_right = (int) (atof(right.value()) / fact);
		margin_left = (int) (atof(left.value()) / fact);
		margin_top = (int) (atof(top.value()) / fact);
		margin_bottom = (int) (atof(bottom.value()) / fact);
		paper_size = paper.value();
		landscape = land.value();
		inch = metric.value();
		set_sizes();
	}
	delete(mnu);
	paper.menu(NULL);
}

void Xd6HtmlView::print()
{
	delete(printer);
	printer = new Xd6HtmlPrint(frame, "output.ps");
	printer->print(frame);
}

void Xd6HtmlView::draw()
{
	frame->draw(x(), y());
}

void Xd6HtmlView::resize(int X, int Y, int W, int H) 
{
	frame->resize(W, H);
	Fl_Widget::resize(X, Y, W, H);
	redraw();
}

int Xd6HtmlView::handle(int e)
{
	int ret;

	ret = frame->handle(e, x(), y());
	if (e == FL_ENTER) {
		take_focus();
	}
	if (e == FL_FOCUS) {
		return 1;
	}
	if ((e == FL_KEYBOARD && Fl::e_length == 0) || e == FL_RELEASE) {
		static int is_here = 0;
		if (is_here) return 0;
		is_here = 1;
		parent()->handle(FL_RELEASE);
		is_here = 0;
		if (Fl::event_button() == 2) {
			Fl::event_is_click(0);
			Fl::paste(*this);
		}
	}

	if (ret) {
		return ret;
	}
	return Fl_Widget::handle(e);
}

void Xd6HtmlView::load(const char *n)
{
	Xd6XmlStyle::clean_tab();
	parser->parse_file(n);
	frame->sel_chr = NULL;
	frame->cur_chr = NULL;
	while (frame->nb_blocks > 0) {
		frame->nb_blocks = frame->nb_blocks - 1;
		delete(frame->blocks[frame->nb_blocks]);
	}
	frame->tab_stop = Xd6XmlStyle::get_tabs();
	Xd6HtmlFrame::rule_width = frame->page_width;
	frame->url = strdup(n);
	frame->tree2block(parser->tree->root);
	if (!frame->nb_blocks) {
		frame->add_block();
	}
	if (frame->nb_blocks == 1 && frame->blocks[0]->nb_segs == 0) {
		frame->blocks[0]->add_segment(strdup(""), 0, 
			FONT_SIZE_3|IS_INLINE);

	}
	frame->measure();
	delete(parser);
	parser = new Xd6XmlParser();
	redraw();
}

void Xd6HtmlView::blank()
{
	frame->cur_chr = NULL;
	while (frame->nb_blocks > 0) {
		delete(frame->blocks[--frame->nb_blocks]);
	}
	frame->add_block();
	frame->blocks[0]->add_segment((char*)malloc(1), 0,
		IS_INLINE|FONT_SIZE_3|FONT_SANS_SERIF);
	frame->measure();
	redraw();
}

static void scan_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s, char *, 
	int, void *d)
{
	((Xd6HtmlView*)d)->spell_cb(s, l, b);
}

void Xd6HtmlView::spell_cb(Xd6HtmlSegment *s, Xd6HtmlLine *line, Xd6HtmlBlock *blk)
{
	int l;
	char buf[1024];
	int ok = 0;
	char *text;
	unsigned char b;
	unsigned int ucs;
	int i;

	if (cancel_spelling) return;
	if (!word_file) return;
	if (s->len < 2) return;
	l = s->len;

	while (s->text[l - 1] == ' ' || s->text[l - 1] == '\t' ||
		s->text[l - 1] == '.' ||
		s->text[l - 1] == ',' || s->text[l - 1] == ':' ||
		s->text[l - 1] == ';' || s->text[l - 1] == '"' ||
		s->text[l - 1] == '\'' || s->text[l - 1] == '(') 
	{
		l--; 
	}
	if (s->text[l - 2] == '\xC2' && s->text[l - 1] == '\xA0') {
		l -= 2;
	}
	if (l < 2) return;
	text = s->text;
	i = 0;
	while (i < l) {
		if (text[i] == '\'') break;
		i++;
	}
	if (i < l) {
		if (i < l / 2) {
			text += i + 1;
			l -= i + 1;
		} else {
			l = i;
		}
	}
	while (isdigit(*text) || *text == '"' || *text == '\'') {
		text++;
		l--;
	}
	if (l < 2) return;

	/* ignore ? */
	i = 0;
	while (i < spell_nb_i) {
		if ((int)strlen(spell_ignore[i]) == l) {
			int j = 0;
			while (j < l) {
				if (spell_ignore[i][j] != text[j]) break;
				j++;
			}
			if (j == l) return;
		}
		i++;
	}

	/* always replace ? */
	i = 0;
	while (i < spell_nb_r) {
		if ((int)strlen(spell_replace[i]) == l) {
			int j = 0;
			while (j < l) {
				if (spell_replace[i][j] != text[j]) break;
				j++;
			}
			if (j == l) {
				replace_by(spell_replace_word[i],
					blk, line, s, text, l);
				return;
			}
		}
		i++;
	}

	/* is the word in the dictionary file ? */
	rewind(word_file);
	fl_utf2ucs((unsigned char *)text, l, &ucs);
	fl_ucs2utf(fl_tolower(ucs), buf);
	b = (unsigned char)buf[0];
	buf[0] = 0;
	while (!feof(word_file) && (unsigned char)buf[0] < b) {
		fseek(word_file, 20000, SEEK_CUR);
		fgets(buf, 1024, word_file);
		fgets(buf, 1024, word_file);
	}
	fseek(word_file, -20000, SEEK_CUR);
	if (ftell(word_file) > 0) fgets(buf, 1024, word_file);
	while (!ok && fgets(buf, 1024, word_file)) {
		if (!fl_utf_strncasecmp(buf, text, l) && 
			(buf[l] == '\n' || buf[l] == '\0')) 
		{
			ok = 1;
		}
		if ((unsigned char)buf[0] > b) ok = -1;
	}

	/* is the word in the personal dictionary file ? */
	if (ok != 1 && pword_file) {
		rewind(pword_file);
		while (ok != 1 && fgets(buf, 1024, pword_file)) {
			if (!fl_utf_strncasecmp(buf, text, l) &&
				(buf[l] == '\n' || buf[l] == '\0'))
			{
				ok = 1;
			}
		}
	}

	if (ok != 1) {
		/* it isn't in the dictionary */
		spell_choice(blk, line, s, text, l);
	}
}

void Xd6HtmlView::spell()
{
	char buf[1024];
	char *f;
	Xd6ConfigFile *cfg;

	cancel_spelling = 0;
	snprintf(buf, 1024, _("/usr/share/dict/french.utf8"));
	f = fl_file_chooser(_("Dictionary : "), "*.utf8", buf);
	if (!f) {
		fl_alert(_("No dictionary selected"));
		return;
	}
	cfg = new Xd6ConfigFile("xd640", "Utilities");
	snprintf(buf, 1024, "%s/.xd640/dict/personal.utf8", cfg->home_dir);
	delete(cfg);
	pword_file = fopen(buf, "r");

	if (!frame->sel_chr) {
		int i = 0;
		int ii = frame->nb_blocks;
		frame->cur_block = frame->blocks[0];
		frame->cur_line = frame->cur_block->lines[0];
		frame->cur_seg = frame->cur_line->segs[0];
		frame->cur_chr = frame->cur_seg->text;
		
		while (i == 0 && ii > 0) {
			ii--;
			frame->sel_block = frame->blocks[ii];
			frame->sel_line = frame->sel_block->lines[
					frame->sel_block->nb_lines - 1];
			i = frame->sel_line->nb_segs;	
		}
		if (i < 1) return;
		frame->sel_seg = frame->sel_line->segs[
					frame->sel_line->nb_segs - 1];
		frame->sel_chr = frame->sel_seg->text + frame->sel_seg->len;
	}
	
	Fl::flush();

	word_file = fopen(f, "r");
	if (!word_file) return;
	frame->scan_selection(scan_cb, this);
	frame->sel_chr = frame->cur_chr = NULL;
	frame->cur_seg = frame->sel_seg = NULL;
	fclose(word_file);
	fclose(pword_file);
	frame->measure();
	frame->create_pages();
	redraw();
}

void Xd6HtmlView::add_to_dict(const char *w)
{
	char buf[2048];
	Xd6ConfigFile *cfg;

	cfg = new Xd6ConfigFile("xd640", "Utilities");

	if (pword_file) {
		fclose(pword_file);
	} else {
		snprintf(buf, 2048, "mkdir ~/.xd640/dict/");
		system(buf);
	}
	snprintf(buf, 2048, "%s/.xd640/dict/personal.utf8", cfg->home_dir);
	pword_file = fopen(buf, "a");
	fprintf(pword_file, "%s\n", w);
	fclose(pword_file);
	snprintf(buf, 2048, "cd  ~/.xd640/dict/; "
		"sort personal.utf8 |uniq > p; /bin/rm personal.utf8; "
		"/bin/mv p personal.utf8");
	system(buf);
	snprintf(buf, 2048, "%s/.xd640/dict/personal.utf8", cfg->home_dir);
	pword_file = fopen(buf, "r");
	delete(cfg);
}

void Xd6HtmlView::spell_choice(Xd6HtmlBlock *b, Xd6HtmlLine *l, 
	Xd6HtmlSegment *s, char *text, int len)
{
	int i = 0;
	char *word;

	word = (char*) malloc(len + 1);
	while(i < len) {
		word[i] = text[i];
		i++;
	}
	word[i] = 0;

	frame->cur_block = frame->sel_block = b;
	frame->cur_seg = frame->sel_seg = s;
	frame->cur_line = frame->sel_line = l;
	frame->cur_chr = text;
	frame->sel_chr = text + len;
	frame->auto_scroll(1, 1);
	redraw();
	Fl::flush();

	Xd6SpellChoice sp(word, word_file);
	for (;;) {
		Fl::wait();
		if (!sp.shown()) break;
	}
	switch(sp.status) {
	case 2:		//b_always_ignore
		add_spell_ignore(word);
	case 1:		//b_ignore
		break;
	case 4:		//b_always_replace
		add_spell_replace(word, sp.i_value->value());
	case 3:		//b_replace
		replace_by(sp.i_value->value(), b, l, s, text, len);
		break;
	case 5:	
		add_to_dict(sp.i_value->value());
		break;
	default:
		cancel_spelling = 1;
		break;
	}
	free(word);
}

void Xd6HtmlView::add_spell_ignore(const char *w)
{
	spell_ignore = (char**) realloc(spell_ignore, 
		(spell_nb_i + 1) * sizeof(char*));
	spell_ignore[spell_nb_i++] = strdup(w);
}

void Xd6HtmlView::add_spell_replace(const char *w, const char *r)
{
	spell_replace = (char**) realloc(spell_replace,
		(spell_nb_r + 1) * sizeof(char*));
	spell_replace[spell_nb_r] = strdup(w);
	spell_replace_word = (char**) realloc(spell_replace_word,
		(spell_nb_r + 1) * sizeof(char*));
	spell_replace_word[spell_nb_r++] = strdup(r);
}

void Xd6HtmlView::replace_by(const char *w, Xd6HtmlBlock *b, Xd6HtmlLine *l,
	Xd6HtmlSegment *s, char *text, int len)
{
	int wl, i;
	int p, dl;

	if (!w) return;
	wl = strlen(w);
	p = text - s->text;
	dl = wl - len;
	s->len = s->len + dl;
	if (wl > len) {
		s->text = (char*) realloc(s->text, sizeof(char) * 
			(s->len + 1));
		i = s->len;
		while (i > 0 && i > p) {
			i--;
			s->text[i] = s->text[i - dl];
		}
	}
	i = p;
	while (i < s->len) {
		if (dl < 0) {
			if (i - p >= wl) {
				s->text[i] = s->text[i - dl];
			} 
		}
		if (i - p < wl) {
			s->text[i] = w[i - p];
		}
		i++;
	}
	s->text[s->len] = 0;
}

static void scan_find_cb( Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s, char *, 
	int, void *d)
{
	((Xd6HtmlView*)d)->find_cb(b, l, s);
}
	
void Xd6HtmlView::find_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s)
{
	
	if (find_dialog->status == -1) return;
	find_dialog->cb_replace = replace_cb;
	find_dialog->finder(b, l, s);
}

void Xd6HtmlView::replace_cb(const char *str, Xd6HtmlFrame *frame)
{
	if (str) {
		int len, i;

		if (frame->cur_seg == frame->sel_seg) {
			len = frame->sel_chr - frame->cur_chr;
		} else {
			frame->cur_seg->len = frame->cur_chr -
				frame->cur_seg->text;
			i = frame->cur_seg->id + 1;
			while (i < frame->sel_seg->id) {
				frame->sel_block->segs[i]->len = 0;
				i++;
			}
			len = frame->sel_seg->len - 
				(frame->sel_chr - frame->sel_seg->text) + 1;
		}
		replace_by(str, frame->sel_block, frame->sel_line,
				frame->sel_seg, frame->sel_chr - len, len);		
	}

}

void Xd6HtmlView::find()
{
	find_dialog = new Xd6FindDialog(frame);
	for (;;) {
		Fl_Widget *o;
		o = Fl::readqueue();
		if (find_dialog->status == 0) {
			Fl::wait();
		} else {
			break;
		}
	}
	if (find_dialog->status == -1) return;
	find_dialog->status = 0;
	frame->scan_all(scan_find_cb, this);
	delete(find_dialog);
	frame->sel_chr = frame->cur_chr = NULL;
	frame->cur_seg = frame->sel_seg = NULL;
	frame->measure();
	frame->create_pages();
	redraw();
}

/*
 * "$Id: $"
 */

