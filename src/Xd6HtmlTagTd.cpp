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


#include "Xd6Std.h"
#include <FL/Fl.h>
#include "Xd6HtmlTagTd.h"
#include "Xd6HtmlPrint.h"
#include <FL/fl_draw.h>
#include "Xd6XmlUtils.h"
#include "Xd6HtmlToRtf.h"

Xd6HtmlTagTd::Xd6HtmlTagTd(int i, Xd6XmlTreeElement *e, Xd6HtmlFrame *p) : 
	Xd6HtmlFrame(i)
{
	const char *a;
	
	display = DISPLAY_TABLE_CELL;

	parent = p;
	focus = 0;
	ostl = stl = e->stl;
	
	stl->para.copy(stl);
	stl->para.display = 0;
	nstl = stl->get_style(&stl->para);

	delete(blocks[0]);
	nb_blocks = 0;
	selected = 0;

	while (p->display != DISPLAY_TOP_FRAME) p = p->parent;
	editor = ((Xd6HtmlFrame*)p)->editor;
	wysiwyg = ((Xd6HtmlFrame*)p)->wysiwyg;
	border = -1;
	if (e->get_attr_value("border")) {
		border = get_length(e->get_attr_value("border"), 100);
	}
	colspan = get_length(e->get_attr_value("colspan"), 100);
	if (colspan < 1) colspan = 1;
	rowspan = get_length(e->get_attr_value("rowspan"), 100);
	if (rowspan < 1) rowspan = 1;
	valign = VALIGN_TOP;
	a = e->get_attr_value("valign");
	if (a) {
		if (!strcasecmp(a, "top")) {
			valign = VALIGN_TOP;
		} else if (!strcasecmp(a, "middle")) {
			valign = VALIGN_MIDDLE;
		} else if (!strcasecmp(a, "bottom")) {
			valign = VALIGN_BOTTOM;
		} else if (!strcasecmp(a, "baseline")) {
			valign = VALIGN_BASELINE;
		}
	}
	xnum = NULL;
	xfmla = NULL;
	a = e->get_attr_value("x:num");
	if (a) xnum = strdup(a);
	a = e->get_attr_value("x:fmla");
	if (a) xfmla = strdup(a);

	scroll = 0;
 	cellpadding = 1;
	real_top = 0;
	real_left = 0;
	valign_pad = 0;
}


Xd6HtmlTagTd::~Xd6HtmlTagTd()
{
	url = NULL;
}

void Xd6HtmlTagTd::destroy()
{
}

void Xd6HtmlTagTd::draw(int X, int Y)
{
	int j, mw, mh;

	stl = nstl;

	mw = max_width;
	mh = max_height;

	max_width -= 1 * (border + cellpadding);
	max_height -= 2 * (border + cellpadding);

	Xd6HtmlFrame::draw(X + border + cellpadding, 
		Y + border + cellpadding + valign_pad);

	stl = ostl;
	max_width = mw;
	max_height = mh;

	X += left;
	Y += top;
	if (flags & DAMAGE_ALL) {
		fl_color(FL_BLACK);
		for (j = 0; j < border; j++) {
			fl_rect(X + j, Y + j, 
				max_width - 2 * j, max_height - 2 * j);
		}
	}
}

#ifdef WIN32
void Xd6HtmlTagTd::print(Xd6HtmlPrint *p, int X, int Y)
{
	int j, k, x, y, yy;
	double ratio = p->ratio;
	Xd6HtmlFrame *frm = p->frm;
	int n = p->page_nb;
	X += left;
	Y += top;
	x = X + border + cellpadding;
	y = Y + border + cellpadding;
	y += valign_pad;
	
	yy = Y - p->frm->page_height * p->page_nb;
	if (border) {
		int co = fl_color();
		HDC f = fl_gc;
		fl_gc = p->hDC;
		fl_clip(frm->page_margin_left, frm->page_margin_top, 
				frm->page_width, frm->page_height);
		fl_line_style(FL_SOLID, (int)(border * ratio));
		fl_line((int)((frm->page_margin_left + X) * ratio),
				(int)((frm->page_margin_top + Y) * ratio),
				(int)((frm->page_margin_left + X + max_width) * ratio),
				(int)((frm->page_margin_top + Y) * ratio));
		fl_line((int)((frm->page_margin_left + X) * ratio),
				(int)((frm->page_margin_top + Y) * ratio),
				(int)((frm->page_margin_left + X) * ratio),
				(int)((frm->page_margin_top + Y + max_height) * ratio));
		fl_line((int)((frm->page_margin_left + X + width) * ratio),
				(int)((frm->page_margin_top + Y) * ratio),
				(int)((frm->page_margin_left + X + width) * ratio),
				(int)((frm->page_margin_top + Y + max_height) * ratio));
		fl_pop_clip();
		fl_gc = f;
		fl_line_style(FL_SOLID, 1);
	}

	for (j = 0; j < nb_blocks; j++) {
		Xd6HtmlBlock *b;
                b = blocks[j];
                for (k = 0; k < b->nb_lines; k++) {
                	Xd6HtmlLine *ln;
                        ln = b->lines[k];
			if ((b->top + y + ln->top) / p->frm->page_height ==
				p->page_nb)
			{ 
                        	p->line_to_ps(ln,  b->left + x, b->top + y);
			}
                }
	}

	yy = Y - p->frm->page_height * p->page_nb;
	int mh = frm->page_height * (p->page_nb - n - 1);
	if (mh < 0) mh = 0;
	if (border) {
		int co = fl_color();
			HDC f = fl_gc;
			fl_gc = p->hDC;
			fl_clip(frm->page_margin_left, frm->page_margin_top, 
				frm->page_width, frm->page_height);
			fl_line_style(FL_SOLID, (int)(border * ratio));
			fl_line((int)((frm->page_margin_left + X) * ratio),
				(int)((frm->page_margin_top + yy + max_height - mh ) * ratio),
				(int)((frm->page_margin_left + X + max_width) * ratio),
				(int)((frm->page_margin_top + yy + max_height - mh) * ratio));
			fl_line((int)((frm->page_margin_left + X) * ratio),
				(int)((frm->page_margin_top + yy) * ratio),
				(int)((frm->page_margin_left + X) * ratio),
				(int)((frm->page_margin_top + yy + max_height - mh) * ratio));
			fl_line((int)((frm->page_margin_left + X + max_width) * ratio),
				(int)((frm->page_margin_top + yy) * ratio),
				(int)((frm->page_margin_left + X + max_width) * ratio),
				(int)((frm->page_margin_top + yy + max_height - mh) * ratio));
			fl_pop_clip();
			fl_gc = f;
			fl_line_style(FL_SOLID, 1);
	}
}
#else
void Xd6HtmlTagTd::print(Xd6HtmlPrint *p, int X, int Y)
{
	int j, k, x, y, yy;

	
	X += left;
	Y += top;
	x = X + border + cellpadding;
	y = Y + border + cellpadding;
	y += valign_pad;
	
	yy = Y - p->frm->page_height * p->page_nb;
	if (border) {
		fprintf(p->tfp, "%d %d %d %d r\n",
                        p->frm->page_margin_left + X,
                        p->frm->page_height + p->frm->page_margin_bottom - yy, 
                        max_width, -border);
		fprintf(p->tfp, "%d %d %d %d r\n",
                        p->frm->page_margin_left + X,
                        p->frm->page_height + p->frm->page_margin_bottom - yy,
                        border, -max_height);
		fprintf(p->tfp, "%d %d %d %d r\n",
                        p->frm->page_margin_left + X + max_width - border,
                        p->frm->page_height + p->frm->page_margin_bottom - yy,
                        border, -max_height);
	}

	for (j = 0; j < nb_blocks; j++) {
		Xd6HtmlBlock *b;
                b = blocks[j];
                for (k = 0; k < b->nb_lines; k++) {
                	Xd6HtmlLine *ln;
                        ln = b->lines[k];
			if ((b->top + y + ln->top) / p->frm->page_height ==
				p->page_nb)
			{ 
                        	p->line_to_ps(b, ln,  b->left + x, b->top + y);
			}
                }
	}

	yy = Y - p->frm->page_height * p->page_nb;
	if (border) {
		fprintf(p->tfp, "%d %d %d %d r\n",
                        p->frm->page_margin_left + X,
                        p->frm->page_height + p->frm->page_margin_bottom - yy
                        - max_height,
                        max_width, border);
		fprintf(p->tfp, "%d %d %d %d r\n",
                        p->frm->page_margin_left + X,
                        p->frm->page_height + p->frm->page_margin_bottom - yy
                        - max_height,
                        border, max_height);
		fprintf(p->tfp, "%d %d %d %d r\n",
                        p->frm->page_margin_left + X + max_width - border,
                        p->frm->page_height + p->frm->page_margin_bottom - yy
                        - max_height,
                        border, max_height);

	}
}
#endif

void Xd6HtmlTagTd::measure(void)
{
	Xd6HtmlFrame::measure();
	width += 2 * (cellpadding + border);
	height += 2 * (cellpadding + border);
}

void Xd6HtmlTagTd::resize(int w, int h) 
{
	Xd6HtmlFrame::resize(w - 2 * (cellpadding + border), 
		h - 2 * (cellpadding + border));
	width += 2 * (cellpadding + border);
	height += 2 * (cellpadding + border);
}

void Xd6HtmlTagTd::break_line(int h, int ph, int fh)
{
	int i, j, p, op;


	h += top + cellpadding + border + valign_pad;

	op = (h + fh) / ph;
	height = 0;
	for (i = 0; i < nb_blocks; i++) {
		Xd6HtmlBlock *b;
		b = blocks[i];
		b->height = 0;
		b->top = height;
		for (j = 0; j < b->nb_lines; j++) {
			int pad;
			Xd6HtmlLine *l;
			l = b->lines[j];
			p = (h + l->height + fh) / ph;
			if (p > op) {
				if (!Xd6HtmlFrame::break_line(l, h)) {
					pad = ph - (h % ph);
					h += pad;
					b->height += pad;
				}
			}
			if (l->height < 1 && l->nb_segs == 0) b->height += 1;
			op = p;
			l->top = b->height;
			b->height += l->height;
			h += l->height;
		}
		height += b->height;

	}
	
	p = (h + cellpadding + border + fh) / ph;
	if (p > op) {
		height += ph - (h % ph);
	}
	height += 2 * (cellpadding + border);
}

int Xd6HtmlTagTd::handle(int e, int X, int Y) 
{
	int ret = 0;
	X += cellpadding + border;
	Y += cellpadding + border + valign_pad;

	if ((e == FL_PUSH && event_is_inside(X, Y))) {
		focus = this;
		ret = Xd6HtmlFrame::handle(e, X, Y);
	} else if (e != FL_PUSH) {
		char *sl, *cl;
		sl = sel_chr;
		cl = cur_chr;
		ret = Xd6HtmlFrame::handle(e, X, Y);
		if (e == FL_DRAG && !event_is_inside(X, Y)) {
			ret = 0;
		} else if (e == FL_KEYBOARD && sl == sel_chr &&
			cl == cur_chr && (
			Fl::event_key() == FL_Left ||
			Fl::event_key() == FL_Up ||
			Fl::event_key() == FL_Right ||
			Fl::event_key() == FL_Down))
		{
			ret = 0;
		}
	} else {
		focus = NULL;
		ret = Xd6HtmlFrame::handle(FL_UNFOCUS, X, Y);
	}
	vscroll = 0;
	hscroll = 0;
	return ret;
}

int Xd6HtmlTagTd::event_is_inside(int x, int y)
{
        x += left;
        y += top - valign_pad;

        if (Fl::event_x() > x   && Fl::event_y() > y &&
                Fl::event_x() <= x + max_width - border &&
                 Fl::event_y() <= y + max_height - border)
        {
                return 1;
        }
        return 0;

}

static void scan_to_html_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s,
        char *c, int len , void *d)
{
        ((Xd6HtmlTagTd *)d)->to_html_cb(b, l, s, c , len);
}

void Xd6HtmlTagTd::to_html(FILE *f)
{
	fp = f;
	
	fprintf(fp, "<td ");
	fprintf(fp, "border=\"%d\" ", border);
	if (colspan > 1) fprintf(fp, "colspan=\"%d\" ", colspan);
	if (rowspan > 1) fprintf(fp, "rowspan=\"%d\" ", rowspan);
	if (xnum) fprintf(fp, "x:num=\"%s\" ", xnum);
	if (valign == VALIGN_MIDDLE) {
		fprintf(fp, "valign=\"middle\" ");
	} else if (valign == VALIGN_BOTTOM) {
		fprintf(fp, "valign=\"bottom\" ");
	}
	if (stl->text_align == TEXT_ALIGN_CENTER) {
		 fprintf(fp, "align=\"center\" ");
	} else if (stl->text_align == TEXT_ALIGN_RIGHT) {
		 fprintf(fp, "align=\"right\" ");
	} else if (stl->text_align == TEXT_ALIGN_JUSTIFY) {
		 fprintf(fp, "align=\"justify\" ");
	}
	if (stl->rtl_direction) {
		fprintf(fp, "dir=\"rtl\" ");
	}
	if (xfmla) {
		char *buf;
		int l;
		l = strlen(xfmla);
		buf = (char*)malloc(l * 6 + 1);
		utf2xml(xfmla, l, buf);
		fprintf(fp,"\nx:fmla=\"%s\"", buf);
		free(buf);
	}

	fprintf(fp," \n>");
	
	lstyle = &Xd6XmlStl::def;
	lb = NULL;
	pop_all_tag(fp);
	scan_all(scan_to_html_cb, this);
	if (nb_blocks > 0 && blocks[0]->nb_segs > 0) fprintf(fp, "</div\n>");
	fprintf(fp, "</td\n>");

}

void Xd6HtmlTagTd::to_rtf(FILE *f)
{
	Xd6HtmlToRtf r(this);
	r.fp = f;
	r.lb = NULL;
	scan_all(r.scan_to_rtf_cb, &r);
}
