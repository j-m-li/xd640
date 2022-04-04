/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2001  O'ksi'D
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


#include <FL/Fl.h>
#include "Xd6HtmlPrint.h"
#include "Xd6Std.h"
#include "FL/Fl_Check_Button.H"
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
/*
	FT_Set_Pixel_Sizes(face, 0, 16);
*/

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

	fprintf(fp, "/p%d_%d {\n", font, ucs);
	fprintf(fp, "ucache\n -300 -300 300 300 setbbox\n");

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
						int x1, x2, y1, y2;
						x1 = p;
						x2 = pixel;
						y1 = 96 - line;
						y2 = y1 -1;

/*
 0 setbox
 1 moveto
 3 lineto
 32 < n <= 255 = repeat next n - 32 times
 5 curveto
 10 closepath
 11 ucache

token
	135: 16 bit int low order first
struct token {
	char tk; // 135
	char rep;  128 + 32
	char len1; signed number of elements (low)
	char len2;	(high)
	short array[]; // elements
	//... data
};
*/
						fprintf(fp,"%d %d moveto\n", x1, y1);
						fprintf(fp,"%d %d lineto\n", x2, y1);
						fprintf(fp,"%d %d lineto\n", x2, y2);
						fprintf(fp,"%d %d lineto\n", x1, y2);
						fprintf(fp,"closepath\n");
					}
					p = pixel;
				}
				lc = c;
			}
		}
		XDestroyImage(i);
	}
	fprintf(fp, "} cvlit def\n");
	fprintf(fp, "/f%d_%d {\n", font, ucs);
	fprintf(fp, ".125 .125 h p%d_%d ufill\n", font, ucs);
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
	command.value("lpr -#1");
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

	Fl_Check_Button to_file(120, 115, 25, 25);
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
	if (ucs > 0xFFFF) ucs = '?';

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
	if (ucs == ' ') {
		fprintf(tfp, "%%%%spc\n");
		return l;
	}
	fprintf(tfp, "s %g %d t ", frm->page_margin_left + X + w, 
		frm->page_height + frm->page_margin_bottom - Y);
       	if (sc != 1) fprintf(tfp, "%g %g h ", sc, sc);
       	fprintf(tfp, "f%d_%d x\n", fl_font_, ucs);
	
	return l;
}

void Xd6HtmlPrint::seg_to_ps(Xd6HtmlSegment *s, int X, int Y)
{
	int i = 0;

	
	if (s->stl->display) {
		((Xd6HtmlDisplay*)s)->print(this, X, Y); 
		return;
	}
	
	Y -= frm->page_height * page_nb;
	X += s->left;
	Y += s->top + s->height - s->descent;

	s->set_font();
	if (s->stl->double_under) {
		fprintf(tfp, "%d %g %d %g r\n",  
			frm->page_margin_left + X,
			frm->page_height + frm->page_margin_bottom - 
				Y - fl_size_ / 6.0, 
			s->width, -(fl_size_ / 12.0));
		fprintf(tfp, "%d %g %d %g r\n",  
			frm->page_margin_left + X,
			frm->page_height + frm->page_margin_bottom - 
				Y - fl_size_ / 24.0, 
			s->width, -(fl_size_ / 12.0));
	}
	if (s->stl->underline) {
		fprintf(tfp, "%d %g %d %g r %%%%underline %d\n",  
			frm->page_margin_left + X,
			frm->page_height + frm->page_margin_bottom - 
				Y - fl_size_ / 24.0, 
			s->width, -(fl_size_ / 12.0),
			fl_utflen((unsigned char*)s->text, strlen(s->text)));
	}
	if (s->stl->strike) {
		fprintf(tfp, "%d %g %d %g r\n",  
			frm->page_margin_left + X,
			frm->page_height + frm->page_margin_bottom - 
				Y + fl_size_ / 3.0, 
			s->width, -(fl_size_ / 12.0));
	}

	if (s->stl->rtl_direction) {
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

void Xd6HtmlPrint::line_to_ps(Xd6HtmlBlock *b, Xd6HtmlLine *l, int X, int Y) 
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

	if (l->id == 0) {
		int ar = b->stl->text_align == TEXT_ALIGN_RIGHT;

		if (b->stl->list == LIST_NUMBER) {
			char buf[100];
			int X, Y, len;
			fl_font(FONT_SANS_SERIF, FONT_SIZE_3);
			snprintf(buf, 100, "%d.", b->list_id);
			Y = b->top + l->top - (frm->page_height * page_nb) + 
				+ l->height - l->descent;
			len = strlen(buf);

			if (ar) {
				i = len;
				X =  b->left + b->width + 5;
				while (i > 0) {
					int l;
					i--;
					l = fl_utflen((unsigned char*)
						buf + i, len - i);
					if (l > 0) {
						int w = 0;
						l = char_to_ps(buf + i, len - i,
							X , Y, &w);
						X += w;
					}
				}
			} else {
				X =  b->left  - (int)fl_width(buf) - 5;
				i = 0;
				while (i <len) {
					int l;
					int w = 1;
					l = char_to_ps(buf + i, 
						len - i, X , Y, &w);
					X += w;
					i += l;
				}
			}
		} else if (b->stl->list == LIST_DISC) {
			int Y = - (b->top + l->top + l->height - l->descent - 
				(frm->page_height * page_nb)) + 
				frm->page_height + frm->page_margin_bottom;

			if (b->stl->blockquote & 0x1) {
				int x2;
				x2 = b->left - 9 + frm->page_margin_left;
				if (ar) x2 += b->width + 18;
				fprintf(tfp, "gsave 0 0 0 setrgbcolor ");
				fprintf(tfp, "1 setlinewidth newpath\n");
				fprintf(tfp, "%d %d 2 0 360 arc\n", x2, Y + 4);
				fprintf(tfp, "closepath stroke grestore\n");
			} else {	
				int x2;
				x2 = b->left - 9 + frm->page_margin_left;
				if (ar) x2 += b->width + 18;
				fprintf(tfp, "gsave 0 0 0 setrgbcolor ");
				fprintf(tfp, "1 setlinewidth newpath\n");
				fprintf(tfp, "%d %d moveto\n", x2, Y + 6);
				fprintf(tfp, "%d %d lineto\n", x2 + 5, Y + 6);
				fprintf(tfp, "%d %d lineto\n", x2 + 5, Y  + 1);
				fprintf(tfp, "%d %d lineto\n", x2, Y + 1);
				fprintf(tfp, "closepath stroke grestore\n");
			}
		}
	}

	for (i = 0; i < l->nb_segs; i++) {
		seg_to_ps(l->segs[i], X, Y);
	}
}

void Xd6HtmlPrint::block_to_ps(Xd6HtmlBlock *b)
{
	int i;
	for (i = 0; i < b->nb_lines; i++) {
		line_to_ps(b, b->lines[i], b->left, b->top);
	} 
	fprintf(tfp, "%%%%newline\n");
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
        fprintf(ofp, "/i { lineto } def\n");
        fprintf(ofp, "/b { curveto } def\n");
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
