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
#include <FL/Fl_Widget.h>
#include <FL/fl_draw.h>
#include <FL/fl_ask.h>
#include <FL/fl_utf8.h>
#include "Xd6HtmlFrame.h"
#include "Xd6XmlParser.h"
#include "Xd6HtmlTagTable.h"
#include "Xd6HtmlTagA.h"
#include "Xd6XmlUtils.h"
#include <sys/stat.h>
#include <FL/Fl_Window.h>

#define _(String) gettext((String))

void Xd6HtmlFrame::push_tag(FILE *fp, char *otag, char *ctag)
{
	open_tags = (char**) realloc(open_tags, sizeof(char*) * (nb_tags + 1));
	open_tags[nb_tags] = strdup(otag);
	close_tags = (char**)realloc(close_tags, sizeof(char*) * (nb_tags + 1));
	close_tags[nb_tags] = strdup(ctag);
	fprintf(fp, "%s", otag);
	nb_tags++;
}

void Xd6HtmlFrame::pop_tag(FILE *fp, char *ctag)
{
	char **ot = NULL;
	char **ct = NULL;
	int nt = 0;

	while (nb_tags > 0) {
		nb_tags--;
		if (strstr(close_tags[nb_tags], "font")) {
			char *s, *s1;
			s1 = strdup(close_tags[nb_tags]);
			s = strstr(s1, "_");
			s--;
			*s = '>';
			s++;
			*s = '\0';
			fprintf(fp, "%s", s1);
			free(s1);
		} else {
			fprintf(fp, "%s", close_tags[nb_tags]);
		}
		if (!strcmp(ctag, close_tags[nb_tags])) {
			free(close_tags[nb_tags]);
			free(open_tags[nb_tags]);
			break;	
		}
		
		ot = (char**)realloc(ot, sizeof(char*) * (nt + 1));
		ct = (char**)realloc(ct, sizeof(char*) * (nt + 1));
		ot[nt] = open_tags[nb_tags];
		open_tags[nb_tags] = NULL;
		ct[nt] = close_tags[nb_tags];
		close_tags[nb_tags] = NULL;
		nt++;
	}
	
	while (nt > 0) {
		nt--;
		push_tag(fp, ot[nt], ct[nt]);
		free(ct[nt]);
		free(ot[nt]);
	}
	free(ct);
	free(ot);
}

void Xd6HtmlFrame::pop_all_tag(FILE *fp)
{
	while (nb_tags > 0) {
		nb_tags--;
		if (strstr(close_tags[nb_tags], "font")) {
			char *s;
			s = strstr(close_tags[nb_tags], "_");
			s--;
			*s = '>';
			s++;
			*s = '\0';
		}
		fprintf(fp, "%s", close_tags[nb_tags]);
		free(close_tags[nb_tags]);
		free(open_tags[nb_tags]);
	}
}

void scan_to_html_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s,
        char *c, int len , void *d)
{
	((Xd6HtmlFrame *)d)->to_html_cb(b, l, s, c , len);
}

void Xd6HtmlFrame::html_header(const char *name, FILE *fp)
{
	fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
	fprintf(fp, "<html ");
	fprintf(fp, "xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n");
	fprintf(fp, "xmlns:s=\"http://www.w3.org/TR/SVG\"\n");
	fprintf(fp, "xmlns:m=\"http://www.w3.org/TR/MathMl2\"\n");
	fprintf(fp, "xmlns=\"http://www.w3.org/TR/REC-html40\" >\n");
	fprintf(fp, "<head>\n");
	fprintf(fp, "<title>%s</title>\n", name);
	fprintf(fp, "<meta content=\"text/html; charset=utf-8\"");
	fprintf(fp, " http-equiv=\"Content-Type\" />\n");
	fprintf(fp, "<meta content=\"O'ksi'D Xd6Html Widget\"");
	fprintf(fp, " name=\"GENERATOR\" />\n");
	fprintf(fp, "<style type=\"text/css\" ><!-- \n");
	fprintf(fp, "div { direction: ltr; unicode-bidi: bidi-override ;\n");
	fprintf(fp, "      font-family: Arial }\n");
	fprintf(fp, "div[align=justify] { text-align: justify}\n");
	fprintf(fp, "li { direction: ltr; unicode-bidi: bidi-override ;\n");
	fprintf(fp, "      font-family: Arial  list-style-type: none}\n");
	fprintf(fp, "li[disc] { list-style-type: disc;}\n");
	fprintf(fp, "li[decimal] { list-style-type: decimal;}\n");
	fprintf(fp, "ul[nomargin] { margin-top: 0; margin-bottom: 0;}\n");
	fprintf(fp, "ol[nomargin] { margin-top: 0; margin-bottom: 0;}\n");
	fprintf(fp, "// --></style>\n");
	fprintf(fp, "</head>\n");
	
	fprintf(fp, "<body ");
	if (tab_stop) {
		int i = 0;
		fprintf(fp, "style=\"tab-stops:");
		while (tab_stop[i]) {
			fprintf(fp, "%dpt ", tab_stop[i]);
			i++;
		}
		fprintf(fp, "\"");
	}
	fprintf(fp, " >\n");
}

void Xd6HtmlFrame::to_html(const char *name)
{
	fp = fopen(name, "w");
	
	if (!fp) {
		fl_alert(_("Cannot open file..."));
		return;
	}

	lstyle = &lstyle->def;

	html_header(name, fp);	

	lb = NULL;
	
	pop_all_tag(fp);
	scan_all(scan_to_html_cb, this);
	fprintf(fp, "</div\n>");
	if (lb) {
		int n = lb->stl->blockquote;
		if (lb->stl->list != LIST_NONE) fprintf(fp, "</li\n>");
		while (n--) fprintf(fp, "</ul\n>");
	}
	fprintf(fp, "</body>\n");
	fprintf(fp, "</html>\n");
	fclose(fp);
}

void Xd6HtmlFrame::scan_to_text_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s,
        char *c, int len , void *d)
{
	((Xd6HtmlFrame *)d)->to_text_cb(b, l, s, c , len);
}

void Xd6HtmlFrame::to_text(const char *name) 
{
	fp = fopen(name, "w");
	
	if (!fp) return;

	lb = NULL;
	lstyle = &lstyle->def;
	fprintf(fp, "%s", quote);
	scan_all(scan_to_text_cb, this);

	fclose(fp);
}

void Xd6HtmlFrame::select_to_html(const char *name)
{
	fp = fopen(name, "w");
	
	if (!fp) return;
	
	fprintf(fp, "<html>\n");
	fprintf(fp, "<head>\n");
	fprintf(fp, "<title>Selection</title>\n");
	fprintf(fp, "</head>\n");
	fprintf(fp, "<body>\n");
	
	lstyle = &lstyle->def;

	lb = NULL;
	scan_selection(scan_to_html_cb, this);
	pop_all_tag(fp);
	fprintf(fp, "</div>\n");

	fprintf(fp, "</body>\n");
	fprintf(fp, "</html>\n");
	fclose(fp);
}

void Xd6HtmlFrame::to_text_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s,
	char *c, int len)
{
	int i;
	if (b->nb_segs < 1) return;
	if (lb && b != lb) {
		fprintf(fp, "\n");
		fprintf(fp, "%s", quote);
	}
	for (i = 0; i < len; i++) {
		fprintf(fp, "%c", c[i]);
	}
	lb = b;
}

void Xd6HtmlFrame::to_html_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s,
	char *c, int len)
{
	if (lstyle->a_link) {
		if ((!(s->stl->a_link))  ||
			(s->stl->display &&
			((Xd6HtmlDisplay*)s)->display == DISPLAY_A_LINK &&
			s->len > 0) || (b != lb))
		{
			pop_all_tag(fp);
			lstyle = &lstyle->def;
		}
	}
	if (b != lb) {
		was_space = 1;
		if (b->stl->page_break && 
			(b->nb_segs == 0 || b->lines[0] != l)) 
		{
			if (lb) fprintf(fp, "</div>");
			fprintf(fp, "<br clear=\"all\" "
				"style=\"page-break-before:always\"\n/>");
			lb = NULL;
			return;
		}
		lstyle = &lstyle->def;
		block_to_html(fp, b, lb);
	}
	seg_to_html(fp, s, lstyle);

	lb = b;
	lstyle = s->stl;

	
	if (!(s->stl->display)) {
		if (len == 0 && b->nb_segs == 1) {
			fprintf(fp, "&nbsp;");
		} else if (len == 1 && c[0] == ' ') {
			if (was_space) {
				fprintf(fp, "&nbsp;");
				was_space = 0;
			} else {
				fprintf(fp, " ");
				was_space = 1;
			}
		} else {
			text_to_html(fp, c, len);
			if (len > 0 && c[len - 1] == ' ') {
				was_space = 1;
			} else {
				was_space = 0;
			}
		}
	}
}

void Xd6HtmlFrame::block_to_html(FILE *fp, Xd6HtmlBlock *b, Xd6HtmlBlock*lb)
{
	if (lb) {
		pop_all_tag(fp);
		fprintf(fp, "</div\n>");
		if (lb->stl->list != LIST_NONE) fprintf(fp, "</li\n>");
		if (lb->stl->blockquote > b->stl->blockquote) {
			  fprintf(fp, "</ul\n>");
		}
	}
	if (b->stl->page_break) {
		fprintf(fp, "<br clear=\"all\" "
			"style=\"page-break-before:always\"\n/>");
	}

	if ((lb && lb->stl->blockquote < b->stl->blockquote) ||
		(!lb &&  b->stl->blockquote > 0)) 
	{
		  fprintf(fp, "<ul nomargin\n>");
	}
	if (b->stl->list == LIST_DISC) {
		fprintf(fp, "<li disc\n>");
	} else if (b->stl->list == LIST_NUMBER) {
		fprintf(fp, "<li decimal\n>");
	} else if (b->stl->list != LIST_NONE) {
		fprintf(fp, "<li\n>");
	}

	if (b->stl->text_align == TEXT_ALIGN_CENTER) {
		fprintf(fp, "<div align=\"center\"");
	} else if (b->stl->text_align == TEXT_ALIGN_RIGHT) {
		fprintf(fp, "<div align=\"right\"");
	} else if (b->stl->text_align == TEXT_ALIGN_JUSTIFY) {
		fprintf(fp, "<div align=\"justify\"");
	} else {
		fprintf(fp, "<div");
	}
	if (b->stl->rtl_direction) {
		fprintf(fp, " dir=\"rtl\"\n>");
	} else {
		fprintf(fp, "\n>");
	}
}

void Xd6HtmlFrame::seg_to_html(FILE *fp, Xd6HtmlSegment *s, Xd6XmlStl *cstyle)
{
	if (s->stl->display) {
		if (((Xd6HtmlDisplay*)s)->display == DISPLAY_A_LINK &&
			s->len > 0) 
		{
			Xd6HtmlTagA *a = (Xd6HtmlTagA*)s;
			char buf[2048];
			pop_all_tag(fp);
			if (a->href && a->target) {
				char *t = (char*) malloc(strlen(a->target) * 6);
				char *h = (char*) malloc(strlen(a->href) * 6);
				utf2xml(a->target, strlen(a->target), t);
				utf2xml(a->href, strlen(a->href), h);
				snprintf(buf, 2048, "<a href=\"%s\""
					" target=\"%s\"\n>", h, t);
				free(h);
				free(t);
			} else if (a->href) {
				char *h = (char*) malloc(strlen(a->href) * 6);
				utf2xml(a->href, strlen(a->href), h);
				snprintf(buf, 2048, "<a href=\"%s\"\n>", h);
				free(h);
			} else if (a->name) {
				char *h = (char*) malloc(strlen(a->name) * 6);
				utf2xml(a->name, strlen(a->name), h);
				snprintf(buf, 2048, "<a name=\"%s\"\n>", h);
				free(h);
			}
			push_tag(fp, buf, "</a>");
			cstyle = lstyle = &lstyle->def;	
		}
		((Xd6HtmlDisplay*)s)->to_html(fp);
		if (((Xd6HtmlDisplay*)s)->display == DISPLAY_TAB) {
			fprintf(fp, "<span style=\"mso-tab-count:1\"\n/>");
		}
		return;
	}
	if (cstyle->font_bold != s->stl->font_bold) {
		if (cstyle->font_bold) {
			pop_tag(fp, "</strong>");
		} else {
			push_tag(fp, "<strong\n>", "</strong>");
		}
	}

	if (cstyle->font_italic != s->stl->font_italic) {
		if (cstyle->font_italic) {
			pop_tag(fp, "</em>");
		} else {
			push_tag(fp, "<em\n>", "</em>");
		}
	}

	if (cstyle->underline != s->stl->underline ||
		cstyle->double_under != s->stl->double_under) 
	{
		if (cstyle->underline) {
			pop_tag(fp, "</u>");
		} 
		if (s->stl->underline) {
			if (s->stl->double_under) {
				push_tag(fp, "<u double\n>", "</u>");
			} else {
				push_tag(fp, "<u\n>", "</u>");
			}
		}
	}
	if (cstyle->font != s->stl->font) {
		char buf[1024];
		
		if (s->id > 0) pop_tag(fp, "</font _f >");
		if (s->stl->font == FONT_SERIF) {
			sprintf(buf, "<font face=\"Times New Roman\"\n>");
		} else if (s->stl->font == FONT_MONOSPACE) {
			sprintf(buf, "<font face=\"Courier New\"\n>");
		} else {
			sprintf(buf, "<font face=\"Arial\"\n>");
		}	
		push_tag(fp, buf, "</font _f >");
	}
	if (cstyle->font_size != s->stl->font_size) {
		char buf[1024];
		int si;
		if (s->id > 0) pop_tag(fp, "</font _s >");
		switch(s->stl->font_size) {
		case FONT_SIZE_1:
			si = 1; break;
		case FONT_SIZE_2:
			si = 2; break;
		case FONT_SIZE_3:
			si = 3; break;
		case FONT_SIZE_4:
			si = 4; break;
		case FONT_SIZE_5:
			si = 5; break;
		case FONT_SIZE_6:
			si = 6; break;
		case FONT_SIZE_7:
			si = 7; break;
		default:
			si = 3;
		}
		sprintf(buf, "<font size=\"%d\"\n>", si);
		push_tag(fp, buf, "</font _s >");
	}
		
	if (cstyle->rtl_direction != s->stl->rtl_direction) {
		if (s->stl->rtl_direction) {
			fprintf(fp, "&#%d;", 0x202E); // RLO
		} else {
			fprintf(fp, "&#%d;", 0x202C); // PDF
			was_space = 1;
		}
	}
}

void Xd6HtmlFrame::text_to_html(FILE *fp, char *txt, int len, int nonbsp)
{
	int i;
	for (i = 0; i < len;) {
		if ((unsigned)txt[i] < 128) {
			if (txt[i] == '"') {
				fprintf(fp, "&quot;");
			} else if (txt[i] == '<') {
				fprintf(fp, "&lt;");
			} else if (txt[i] == '>') {
				fprintf(fp, "&gt;");
			} else if (txt[i] == '&') {
				fprintf(fp, "&amp;");
			} else if (txt[i] == '\'') {
				fprintf(fp, "&#39;");
			} else if (!nonbsp && txt[i] == ' ' && i < len - 1) {
				fprintf(fp, "&nbsp;");
			} else {
				fprintf(fp, "%c", txt[i]);
			}
			i++;
		} else {
			int l;
			unsigned int ucs;
			l = fl_utf2ucs((unsigned char*)txt + i, len - i, &ucs); 
			if (l < 1 || ucs > 0xFFFF) {
				l = 1;
				ucs = txt[i];
			}
			fprintf(fp, "&#%d;", ucs);
			i += l;
		}
	}
}


static void select_to_buffer_scan_cb(Xd6HtmlBlock *b, Xd6HtmlLine *, Xd6HtmlSegment *, 
	char *c, int l, void *d)
{
	int i;
	static Xd6HtmlBlock *blk = NULL;
	Xd6HtmlFrame *self = (Xd6HtmlFrame*)d;

        if (self->paste_buffer_malloc - self->paste_buffer_len < l + 32) {
                self->paste_buffer_malloc += l + 1024;
                self->paste_buffer = (char*) realloc(self->paste_buffer,
                                self->paste_buffer_malloc);
        }
	if (self->paste_buffer_len > 0 && blk != b) {
		self->paste_buffer[self->paste_buffer_len++] = '\n';
	}
	blk = b;
	for(i = 0; i < l; i++) {
		self->paste_buffer[self->paste_buffer_len++] = c[i];
	}
}

void Xd6HtmlFrame::select_to_buffer()
{
	if (sel_chr) {
		paste_buffer_len = 0;
		scan_selection(select_to_buffer_scan_cb, this);
		if (paste_buffer) {
			paste_buffer[paste_buffer_len] = '\0';
		}
#if HAVE_FLTK_UTF || HAVE_FLTK
		Fl::selection(*Fl::focus(), paste_buffer, paste_buffer_len);
#else
		Fl::copy(paste_buffer, paste_buffer_len);
#endif
	}
}

void Xd6HtmlFrame::sort_selection(Xd6HtmlBlock **b1, Xd6HtmlLine **l1,
                Xd6HtmlSegment **s1, char **c1, Xd6HtmlBlock **b2, Xd6HtmlLine **l2,
                Xd6HtmlSegment **s2, char **c2)
{
	int slid, clid;

	if (!sel_chr || !cur_chr) return;

	slid = sel_line->get_seg_id(sel_seg); 
	clid = cur_line->get_seg_id(cur_seg); 

	if (sel_block->id < cur_block->id ||
		(sel_block->id == cur_block->id && 
		  (sel_line->id < cur_line->id ||
		    (sel_line->id == cur_line->id &&
		      (slid < clid ||
			(slid == clid &&
			  (
			    (sel_chr < cur_chr && 
			      !(sel_seg->stl->rtl_direction)) ||
			    (sel_chr > cur_chr &&
			      (sel_seg->stl->rtl_direction))
		) ) ) ) ) ) )
	{
		*b1 = sel_block;
		*b2 = cur_block;
		*l1 = sel_line;
		*l2 = cur_line;
		*s1 = sel_seg;
		*s2 = cur_seg;
		*c1 = sel_chr;
		*c2 = cur_chr;
	} else {
		*b2 = sel_block;
		*b1 = cur_block;
		*l2 = sel_line;
		*l1 = cur_line;
		*s2 = sel_seg;
		*s1 = cur_seg;
		*c2 = sel_chr;
		*c1 = cur_chr;
	}
}


static void scan_line(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s1, 
	Xd6HtmlSegment *s2, char *c1, char *c2, ScanCallback cb, void *d)
{
	Xd6HtmlSegment **segs;
	int s1id, s2id;
	int i;

	if (s1 == s2) {

		cb(b, l, s1, c1, c2 - c1, d);
		return;
	}
	segs = (Xd6HtmlSegment**) malloc(sizeof(Xd6HtmlSegment*) * b->nb_segs);
	s1id = l->get_seg_id(s1);
	s2id = l->get_seg_id(s2);

	for (i = 0; i < b->nb_segs;i++) {
		segs[i] = NULL;
	}
	for (i = s1id; i <= s2id; i++) {
		segs[l->segs[i]->id] = l->segs[i];
	}
	for (i = 0; i < b->nb_segs;i++) {
		if (s1 == segs[i] && s1->stl->rtl_direction) {
			cb(b, l, s1, s1->text, c1 - s1->text, d);
		} else if (s1 == segs[i]) {
			cb(b, l, s1, c1, s1->len - (c1 - s1->text), d);
		} else if (s2 == segs[i] && s2->stl->rtl_direction) {	
			cb(b, l, s2, c2, s2->len - (c2 - s2->text), d);
		} else if (s2 == segs[i]) {
			cb(b, l, s2, s2->text, c2 - s2->text, d);
		} else if (segs[i]) {
			cb(b, l, segs[i], segs[i]->text, segs[i]->len, d);
		}
	}
	free(segs);
}

void Xd6HtmlFrame::scan_all(ScanCallback cb, void *data)
{
	int ib, il, is, ok;
	Xd6HtmlBlock *b1;
	Xd6HtmlLine *l1;
	Xd6HtmlSegment *s1;
	Xd6HtmlLine l(0);
	Xd6HtmlSegment s(0, NULL, 0, &Xd6XmlStl::def);

	l.add_segment(&s);
	for (ib = 0; ib < nb_blocks; ib++) {
		ok = 0;
		b1 = blocks[ib];
		for (il = 0; il < b1->nb_lines; il++) {
			l1 = b1->lines[il];
			for (is = l1->begin; is <= l1->end; is++) {
				ok = 1;
				s1 = b1->segs[is];
				cb(b1, l1, s1, s1->text, s1->len, data);
			}
		}
		if (!ok) {
			cb(b1, &l, &s, s.text, 0, data);
		}
	}
}

void Xd6HtmlFrame::scan_selection(ScanCallback cb, void *data)
{
	Xd6HtmlSegment *seg;
	Xd6HtmlBlock *b1, *b2;
	Xd6HtmlLine *l1, *l2;
	Xd6HtmlSegment *s1, *s2;
	char *c1, *c2;
	int s1id, s2id;
	int i, j, k;

	if (!sel_chr || !cur_chr) {
		if (focus && focus->stl->display &&
			((Xd6HtmlDisplay*)focus)->display == DISPLAY_TABLE)
		{
			((Xd6HtmlTagTable*)focus)->scan_selection(cb, data);
		}
		return;
	}
	sort_selection(&b1, &l1, &s1, &c1, &b2, &l2, &s2, &c2);
	s1id = l1->get_seg_id(s1);
	s2id = l2->get_seg_id(s2);
	if (b1 == b2) {
		if (l1 == l2) {
			if (s1 == s2) {
				if (s1->stl->rtl_direction) {
					cb(b1, l1, s1, c2, c1 - c2, data);
				} else {
					cb(b1, l1, s1, c1, c2 - c1, data);
				}
				return;
			}
			/***************************************/
			scan_line(b1, l1, s1, s2, c1, c2, cb, data);
			/***************************************/
			return;
		}
		/***************************************/
		if (l1->nb_segs > 0) {
			seg = l1->segs[l1->nb_segs - 1];
			if (seg->stl->rtl_direction) {
				scan_line(b1, l1, s1, seg, c1, 
					seg->text, cb, data);
			} else {		
				scan_line(b1, l1, s1, seg, c1, 
					seg->text + seg->len, cb, data);
			}
		}
		/***************************************/
		for (j = l1->id + 1; j < l2->id; j++) {
			Xd6HtmlLine *l;
			l = b1->lines[j];
			for (i = l->begin; i <= l->end; i++) {
				seg = b1->segs[i];
				cb(b1, l, seg, seg->text, seg->len, data);
			}
		}
		/***************************************/
		if (l2->nb_segs > 0) {
			seg = l2->segs[0];
			if (seg->stl->rtl_direction) {	
				scan_line(b1, l2, seg, s2, seg->text + seg->len,
					c2, cb, data);
			} else {
				scan_line(b1, l2, seg, s2, seg->text, 
					c2, cb, data);
			}
		}
		/***************************************/
		return;
	}

	/***************************************/
	if (l1->nb_segs > 0) {
		seg = l1->segs[l1->nb_segs - 1];
		if (seg->stl->rtl_direction) {	
			scan_line(b1, l1, s1, seg, c1, seg->text, 
				cb, data);
		} else {
			scan_line(b1, l1, s1, seg, c1, seg->text + seg->len, 
				cb, data);
		}
	}
	/***************************************/
	for (j = l1->id + 1; j < b1->nb_lines; j++) {
		Xd6HtmlLine *l;
		l = b1->lines[j];
		for (i = l->begin; i <= l->end; i++) {
			seg = b1->segs[i];
			cb(b1, l, seg, seg->text, seg->len, data);
		}
	}
	/***************************************/
	for (k = b1->id + 1; k < b2->id; k++) {
		Xd6HtmlBlock *b;
		b = blocks[k];
		for (j = 0; j < b->nb_lines; j++) {
			Xd6HtmlLine *l;
			l = b->lines[j];
			for (i = l->begin; i <= l->end; i++) {
				seg = b->segs[i];
				cb(b, l, seg, seg->text, seg->len, data);
			}
		}
	}
	
	/***************************************/
	for (j = 0; j < l2->id; j++) {
		Xd6HtmlLine *l;
		l = b2->lines[j];
		for (i = l->begin; i <= l->end; i++) {
			seg = b2->segs[i];
			cb(b2, l, seg, seg->text, seg->len, data);
		}
	}
	/***************************************/
	if (l2->nb_segs > 0) {
		seg = l2->segs[0];
		if (seg->stl->rtl_direction) {
			scan_line(b2, l2, seg, s2, seg->text + seg->len, 
				c2, cb, data);
		} else {
			scan_line(b2, l2, seg, s2, seg->text, c2, cb, data);
		}
	}
	/***************************************/
}


/*
 * "$Id: $"
 */

