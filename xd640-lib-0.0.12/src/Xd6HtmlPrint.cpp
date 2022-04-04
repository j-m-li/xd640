/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2001  O'ksi'D
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

#include <FL/Fl.h>
#include "Xd6HtmlPrint.h"
#include "Xd6Std.h"
#include "Xd6CheckButton.h"
#include <FL/x.H>
#include <FL/fl_utf8.h>
#include "Xd6HtmlDisplay.h"
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Output.H>
#include <FL/fl_ask.h>

#define _(String) gettext((String))

extern int fl_font_;
extern int fl_size_;

Xd6HtmlPsFont::Xd6HtmlPsFont()
{
	int i;
	font = 0;
	for (i = 0; i < 0x10000; i++) {
		glyphs[i] = 0;
	}
}

Xd6HtmlPsFont::~Xd6HtmlPsFont()
{
	
}

void Xd6HtmlPsFont::create_glyph(char *s, int l, int ucs, FILE* fp)
{
	int size;
	int width;
	int height;
	XImage *i = NULL;
	static Pixmap p = 0;
	static int pw = 0;
	static int ph = 0;
	int pixel = 0;
	int line = 0;

	if (ucs > 0xFFFF || ucs < 0 || glyphs[ucs]) return;

	fprintf(fp, "/f%d_%d {\n", font, ucs);
	size = fl_size_;
	fl_font(fl_font_, 96);
	width = (int) fl_width(s, l);
	glyphs[ucs] = width;
	if (width < 1) {
		/* Non spacing glyph */
		width = (int) fl_width((unsigned)ucs);
		glyphs[ucs] = - width;
	}
	width *= 2;
	height = 96 + fl_descent();
	Fl::first_window()->make_current();
	if (width > 0) {
		if (pw < width || ph < height || !p) {
			if (p) fl_delete_offscreen(p);
			p = fl_create_offscreen(width, height);
			ph = height;
			pw = width;
		}
		fl_begin_offscreen(p);
		fl_color(FL_WHITE);
		fl_rectf(0, 0, pw, ph);
		fl_color(FL_BLACK);
		if (glyphs[ucs] < 1) {
			/* non spacing glyph */
			fl_draw(s, l, width / 2, 96);
		} else {
			fl_draw(s, l, 0, 96);
		}
		i = XGetImage(fl_display, fl_window, 0, 0, 
			width, height, 1, XYPixmap);
		fl_end_offscreen();
	}
	if (i) {
		unsigned char c;
		unsigned char *data = (unsigned char*) i->data;
		int h = i->height;
		int w = i->xoffset + i->width;
		int bl = i->bytes_per_line;
		int xoff = i->xoffset;
		for (line = 0; line < h; line++) {
			int l = line * bl;
			unsigned char lc = 2;
			int p = xoff;
			for (pixel = xoff; pixel < w; pixel++) {
				c = data[l + (pixel >> 3)];
				if (c & (1 << (pixel & 7))) {
					c = 1;
				} else {
					c = 0;
				}
				if (c != lc) {
					if (lc == 0) {
						fprintf(fp, 
							"%g %g %g 0.125 r\n",
							p / 8.0, 
							(96 - line) / 8.0, 
							(pixel - p) / 8.0);
					}
					p = pixel;
				}
				lc = c;
			}
		}
		XDestroyImage(i);
	}
	fprintf(fp, "} def\n");
	fl_font(fl_font_, size);
}

Xd6HtmlPrint::Xd6HtmlPrint(Xd6HtmlFrame *f, const char *file) 
{
	int i;

	frm = f;

	for(i = 0; i < 12; i++) {
		fonts[i].font = i;
	}

	snprintf(out_file, 1024, "%s", file);
	snprintf(font_file, 1024, ".%s.fnt", file);
	snprintf(text_file, 1024, ".%s.tmp", file);
	cmd = NULL;
}

Xd6HtmlPrint::~Xd6HtmlPrint()
{
	free(cmd);
	unlink(font_file);
}

void Xd6HtmlPrint::print(Xd6HtmlFrame *frame)
{
	int i, j;
	char buf[32];
	Fl_Widget *o;
	
	frm = frame;
	for (j = 0; j < 12; j++) {
		for (i = 0; i < 0x10000; i++) {
			fonts[j].glyphs[i] = 0;
		}
	}

	Fl_Window print_dialog(240, 240);
	print_dialog.label(_("Printer Setup"));

	Fl_Input command(5, 25, 230, 20);
	command.value("lpr -Plp -\%1");
	command.label(_("Print command:"));
	command.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);

	Fl_Input name(5, 160, 230, 20);
	name.value("output.ps");
	name.label(_("File name:"));
	name.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);

	Fl_Input from(10, 70, 50, 20);
	from.value("1");
	from.label(_("From page:"));
	from.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);


	snprintf(buf, 32, "%d", frm->height / frm->page_height);	
	Fl_Input to(100, 70, 50, 20);
	to.value(buf);
	to.label(_("To:"));
	to.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);

	Xd6CheckButton to_file(120, 115, 25, 25);
	to_file.label(_("Print to file."));

	Fl_Button ok(65, 205, 80, 25, _("OK"));
	Fl_Button cancel(155, 205, 80, 25, _("Cancel"));

	print_dialog.end();
	print_dialog.set_modal();
	print_dialog.show();
	for (;;) {
		o = Fl::readqueue();
		if (o != &ok && o != &cancel) {
			Fl::wait();
		} else {
			break;
		}
	}
	print_dialog.hide();
	Fl::check();
	Fl::flush();
	if (o == &ok) {
		if (to_file.value()) {
			snprintf(out_file, 1024, "%s", name.value());
		} else {
			snprintf(out_file, 1024, "/tmp/%p.ps", frm);
		}
		snprintf(font_file, 1024, "/tmp/%p.fnt.tmp", frm);
		unlink(font_file);

		snprintf(text_file, 1024, "/tmp/%p.ps.tmp", frm);
		from_p = (int)atol(from.value());
		to_p = (int)atol(to.value());
		frame_to_ps(from_p, to_p);
		p_to_file = 1;
		if (!to_file.value()) {
			char tmp[1024];
			p_to_file = 0;
			if (cmd) free(cmd);
			cmd = strdup(command.value());
			snprintf(tmp, 1024, "%s %s", cmd, out_file);
			system(tmp);
			unlink(out_file);
		}
	}
}

void Xd6HtmlPrint::direct_print(Xd6HtmlFrame *f)
{
	frm = f;
	frame_to_ps(from_p, to_p);
	if (p_to_file) {
		char tmp[2048];
		snprintf(tmp, 1024, "%s %s", cmd, out_file);
		system(tmp);
		unlink(out_file);
	}
}

int Xd6HtmlPrint::char_to_ps(char *text, int len, int X, int Y, int *wi) 
{
	unsigned int ucs;
	int l;
	float sc;
	double W;
	double w;

 	l = fl_utf2ucs((unsigned char *)text, len, &ucs);
	if (l < 1) l = 1;

	sc = fl_size_ / 12.0;
	fonts[fl_font_].create_glyph(text, l, (int) ucs, ffp);
	W = fonts[fl_font_].glyphs[ucs] * fl_size_ / 96.0;
	w = fl_width(text, l);
	if (w < 1) {
		w = W * *wi;
		*wi = 0;
	} else {
		*wi = (int)w;
		w = (w - W) / 2.0;
	}
	if (ucs == ' ') return l;
	fprintf(tfp, "s %g %d t ", frm->page_margin_left + X + w, 
		frm->page_height + frm->page_margin_bottom - Y);
       	if (sc != 1) fprintf(tfp, "%g %g h ", sc, sc);
       	fprintf(tfp, "f%d_%d x\n", fl_font_, ucs);
	
	return l;
}

void Xd6HtmlPrint::seg_to_ps(Xd6HtmlSegment *s, int X, int Y)
{
	int i = 0;

	
	if (s->style & DISPLAY) {
		((Xd6HtmlDisplay*)s)->print(this, X, Y); 
		return;
	}
	
	Y -= frm->page_height * page_nb;
	X += s->left;
	Y += s->top + s->height - s->descent;

	s->set_font();
	if (s->style & UNDERLINE) {
		fprintf(tfp, "%d %g %d %g r\n",  
			frm->page_margin_left + X,
			frm->page_height + frm->page_margin_bottom - 
				Y - fl_size_ / 24.0, 
			s->width, -(fl_size_ / 12.0));
	}

	if (s->style & RTL_DIRECTION) {
		i = s->len;
		while (i > 0) {
			int l;
			i--;
			l = fl_utflen((unsigned char*)s->text + i, s->len - i);	
			if (l > 0) {
				int w = 0;
				l = char_to_ps(s->text + i, s->len - i, 
					X , Y, &w);
				X += w;
			}
		}
	} else {
		while (i < s->len) {
			int l;
			int w = 1;
			l = char_to_ps(s->text + i, s->len - i, X , Y, &w);
			X += w;
			i += l;
		}
	}
}

void Xd6HtmlPrint::ps_footer(int p)
{
	char buf[1024];
	int X;
	int len;
	int i;

	fl_font(FL_HELVETICA, 12);
	snprintf(buf, 1024, "%d / %d", p, frm->height / frm->page_height);
	X = frm->page_width / 2	- (int)fl_width(buf) / 2;
	len = strlen(buf);
	i = 0;
	while (i <len) {
		int l;
		int w = 1;
		l = char_to_ps(buf + i, len - i, X , frm->page_height + 15, &w);
		X += w;
		i += l;
	}
}

void Xd6HtmlPrint::new_page() 
{
	page_nb++;
	
	if (page_nb >= from_p && page_nb <= to_p) {
		fprintf(tfp, "grestore\n");
		ps_footer(page_nb);
	}
	if (page_nb >= from_p && page_nb + 1 <= to_p) {	
		fprintf(tfp, "showpage\n");
		fprintf(tfp, "%%%%Page: %d %d\n", 
			page_nb + 1, page_nb + 1);
		if (frm->page_land) {
			fprintf(tfp, "%d 0 t 90 rotate\n", 
				frm->page_height +
				frm->page_margin_top + 
				frm->page_margin_bottom);
		}
		fprintf(tfp, "gsave\n");
		fprintf(tfp, "newpath\n");
		fprintf(tfp, "%d %d moveto\n", frm->page_margin_left,
			 frm->page_margin_bottom);
		fprintf(tfp, "%d %d rlineto\n", 0, frm->page_height);
		fprintf(tfp, "%d %d rlineto\n", frm->page_width, 0);
		fprintf(tfp, "%d %d rlineto\n", 0, - (frm->page_height));
		fprintf(tfp, "closepath\n");
		fprintf(tfp, "clip\n");
	}


}

void Xd6HtmlPrint::line_to_ps(Xd6HtmlLine *l, int X, int Y) 
{
	int i;

	X += l->left;
	Y += l->top;
	
	if ((Y / frm->page_height) > page_nb) {
		new_page();
	}
	if (!(page_nb + 1 >= from_p && page_nb + 1<= to_p)) {
		return;
	}

	for (i = 0; i < l->nb_segs; i++) {
		seg_to_ps(l->segs[i], X, Y);
	}
}

void Xd6HtmlPrint::block_to_ps(Xd6HtmlBlock *b)
{
	int i;
	for (i = 0; i < b->nb_lines; i++) {
		line_to_ps(b->lines[i], b->left, b->top);
	} 
}

void Xd6HtmlPrint::frame_to_ps(int f, int t)
{
	int i;
	FILE *ofp;
	
	from_p = f;
	to_p = t;
	ffp = fopen(font_file, "a");
	if (!ffp) {
		fl_alert(_("Cannot open output file"));
		return;
	}
	tfp = fopen(text_file, "w");
	if (!tfp) {
		fclose(ffp);
		fl_alert(_("Cannot open output file"));
		return;
	}
	
	page_nb = 0;
	fprintf(tfp, "%%%%Page: %d %d\n", from_p, from_p);
	if (frm->page_land) {
		fprintf(tfp, "%d 0 t 90 rotate\n", frm->page_height +
			frm->page_margin_top + frm->page_margin_bottom);
	}
	fprintf(tfp, "gsave\n");
	fprintf(tfp, "newpath\n");
	fprintf(tfp, "%d %d moveto\n", frm->page_margin_left,
		 frm->page_margin_bottom);
	fprintf(tfp, "%d %d rlineto\n", 0, frm->page_height);
	fprintf(tfp, "%d %d rlineto\n", frm->page_width, 0);
	fprintf(tfp, "%d %d rlineto\n", 0, - (frm->page_height));
	fprintf(tfp, "closepath\n");
	fprintf(tfp, "clip\n");

	for (i = 0; i < frm->nb_blocks; i++) {
		block_to_ps(frm->blocks[i]);	
	}
	page_nb++;
	if (page_nb >= from_p && page_nb <= to_p) {
		fprintf(tfp, "grestore\n");
		ps_footer(page_nb);
	}

	fclose(tfp);
	fclose(ffp);
	tfp = fopen(text_file, "r");
	ffp = fopen(font_file, "r");
	ofp = fopen(out_file, "w");

        fprintf(ofp, "%%!PS-Adobe-2.0\n");
        fprintf(ofp, "%%%%Creator: O'ksi'D Xd6Html Widget\n");
        fprintf(ofp, "%%%%Pages: %d\n", to_p - from_p + 1);

	if (frm->page_land) {
		fprintf(ofp, "%%%%Orientation: Landscape\n");
	} else {
		fprintf(ofp, "%%%%Orientation: Portrait\n");
	}
        fprintf(ofp, "%%%%EndComments\n");
        fprintf(ofp, "/c { setrgbcolor } def\n");
        fprintf(ofp, "/r { rectfill } def\n");
        fprintf(ofp, "/s { gsave } def\n");
        fprintf(ofp, "/x { grestore } def\n");
        fprintf(ofp, "/h { scale } def\n");
        fprintf(ofp, "/t { translate } def\n");
        fprintf(ofp, "/m { moveto } def\n");
        fprintf(ofp, "/l { rlineto } def\n");
        fprintf(ofp, "/p { stroke } def\n");
        fprintf(ofp, "/w { setlinewidth } def\n");

	while(!feof(ffp)) {
		char buf[1024];
		int len;
		len = fread(buf, 1, 1024, ffp);
		fwrite(buf, 1, len, ofp);
	}
	fprintf(ofp, "%%%%EndProlog\n");
	while(!feof(tfp)) {
		char buf[1024];
		int len;
		len = fread(buf, 1, 1024, tfp);
		fwrite(buf, 1, len, ofp);
	}
	fprintf(ofp, "showpage\n");
	fprintf(ofp, "%%%%EOF\n");
	fclose(tfp);
	fclose(ffp);
	fclose(ofp);
	unlink(text_file);
}

/*
 * "$Id: $"
 */
