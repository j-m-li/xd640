/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2002  O'ksi'D
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
#include <FL/fl_utf8.h>
#include <FL/fl_ask.h>
#include "Xd6HtmlToRtf.h"
#include "Xd6XmlParser.h"
#include "Xd6HtmlTagTable.h"
#include "Xd6HtmlTagA.h"
#include "Xd6XmlUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define _(String) gettext((String))

Xd6HtmlToRtf::Xd6HtmlToRtf(Xd6HtmlFrame *f)
{
	open_tags = NULL;
	nb_tags = 0;
	close_tags = NULL;
	frame = f;
}

void Xd6HtmlToRtf::push_tag(FILE *fp, char *otag, char *ctag)
{
	open_tags = (char**) realloc(open_tags, sizeof(char*) * (nb_tags + 1));
	open_tags[nb_tags] = strdup(otag);
	close_tags = (char**)realloc(close_tags, sizeof(char*) * (nb_tags + 1));
	close_tags[nb_tags] = strdup(ctag);
	fprintf(fp, "%s", otag);
	nb_tags++;
}

void Xd6HtmlToRtf::pop_tag(FILE *fp, char *ctag)
{
	char **ot = NULL;
	char **ct = NULL;
	int nt = 0;

	while (nb_tags > 0) {
		nb_tags--;
		fprintf(fp, "%s", close_tags[nb_tags]);
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

void Xd6HtmlToRtf::pop_all_tags(FILE *fp)
{
	while (nb_tags > 0) {
		nb_tags--;
		fprintf(fp, "%s", close_tags[nb_tags]);
	}
}


void Xd6HtmlToRtf::scan_to_rtf_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, 
	Xd6HtmlSegment *s, char *c, int len , void *d)
{
	((Xd6HtmlToRtf *)d)->to_rtf_cb(b, l, s, c , len);
}

void Xd6HtmlToRtf::header(FILE *fp)
{
	fprintf(fp, "{\\rtf1\\ansi\\deff0 \\uc0 {\\fonttbl");
	fprintf(fp, "\\f0\\fswiss Arial;");
	fprintf(fp, "\\f1\\fmodern Courier New;");
	fprintf(fp, "\\f2\\froman Times New Roman;");
	fprintf(fp, "}\n");
}

void Xd6HtmlToRtf::to_rtf(const char *name)
{
	fp = fopen(name, "w");
	
	if (!fp) {
		fl_alert(_("Cannot open file..."));
		return;
	}

	header(fp);	

	lb = NULL;
	lstyle = &Xd6XmlStl::def;
	frame->scan_all(scan_to_rtf_cb, this);
	fprintf(fp, "}\n");
	fclose(fp);
}


void Xd6HtmlToRtf::to_rtf_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s,
	char *c, int len)
{
	if (b != lb) {
		if (b->stl->page_break && b->lines[0] != l) {
			if (lb) fprintf(fp, "\\par");
			fprintf(fp, "\\page\n");
			lb = NULL;
			return;
		}
		lstyle = &Xd6XmlStl::def;
		block_to_rtf(fp, b, lb);
	}
	seg_to_rtf(fp, s, lstyle);

	lb = b;
	lstyle = s->stl;

	
	if (!(s->stl->display)) {
		text_to_rtf(fp, c, len);
	}
}

void Xd6HtmlToRtf::block_to_rtf(FILE *fp, Xd6HtmlBlock *b, Xd6HtmlBlock*lb)
{
	tab_nb = 0;
	if (lb) {
		pop_all_tags(fp);
		fprintf(fp, "\\par\n");
	}
	fprintf(fp, "\\f0\\fs24 ");
	if (b->stl->page_break) {
		fprintf(fp, "\\page\n");
	}
	
	if (frame->tab_stop) {
		int i;
		i = 0;
		while (frame->tab_stop[i]) {	
			fprintf(fp, "\\tx%d ", frame->tab_stop[i] * 20);
			i++;
		}
	}
	if (b->stl->text_align ==  TEXT_ALIGN_CENTER) {
		fprintf(fp, "\\qc ");
	} else if (b->stl->text_align == TEXT_ALIGN_RIGHT) {
		fprintf(fp, "\\qr ");
	} else if (b->stl->text_align == TEXT_ALIGN_JUSTIFY) {
		fprintf(fp, "\\qj ");
	} else {
		fprintf(fp, "\\ql ");
	}
	if (b->stl->rtl_direction) {
		fprintf(fp, "\\rtlmark ");
	} else {
		fprintf(fp, "\\ltrmark ");
	}
}

void Xd6HtmlToRtf::seg_to_rtf(FILE *fp, Xd6HtmlSegment *s, Xd6XmlStl *style)
{
	if (s->stl->display) {
		((Xd6HtmlDisplay*)s)->to_rtf(fp);
		if (((Xd6HtmlDisplay*)s)->display == DISPLAY_TAB) {
			if (s->stl->rtl_direction) {
				fprintf(fp, "\\tqr");
			}		
			fprintf(fp, "\\tab ");
		}
		return;
	}
	if ((style->font_bold) != (s->stl->font_bold)) {
		if (style->font_bold) {
			pop_tag(fp, "\\b0 ");
		} else {
			push_tag(fp, "\\b ", "\\b0 ");
		}
	}

	if ((style->font_italic) != (s->stl->font_italic)) {
		if (style->font_italic) {
			pop_tag(fp, "\\i0 ");
		} else {
			push_tag(fp, "\\i ", "\\i0 ");
		}
	}

	if ((style->underline) != (s->stl->underline)) {
		if (style->underline) {
			pop_tag(fp, "\\u0 ");
		} else {
			push_tag(fp, "\\ul ", "\\ul0 ");
		}
	}
	if ((style->font) != (s->stl->font)) {
		
		if ((s->stl->font) == FONT_SERIF) {
			fprintf(fp, "\\f2 ");
		} else if ((s->stl->font) == FONT_MONOSPACE) {
			fprintf(fp, "\\f1 ");
		} else {
			fprintf(fp, "\\f0");
		}	
	}
	if ((style->font_size) != (s->stl->font_size)) {
		int si;
		si = s->stl->font_size;
		fprintf(fp, "\\fs%d ", si * 2);
	}
		
	if ((style->rtl_direction) != (s->stl->rtl_direction)) {
		if (s->stl->rtl_direction) {
			fprintf(fp, "\\rtlmark "); 
		} else {
			fprintf(fp, "\\ltrmark ");
		}
	}
}

void Xd6HtmlToRtf::text_to_rtf(FILE *fp, char *txt, int len, int nonbsp)
{
	int i;
	for (i = 0; i < len;) {
		if ((unsigned)txt[i] < 128) {
			if (txt[i] == '\\') {
				fprintf(fp, "\\\\");
			} else if (txt[i] == '{') {
				fprintf(fp, "\\{");
			} else if (txt[i] == '}') {
				fprintf(fp, "\\}");
			} else {
				fprintf(fp, "%c", txt[i]);
			}
			i++;
		} else {
			int l;
			unsigned int ucs;
			short u;
			l = fl_utf2ucs((unsigned char*)txt + i, len - i, &ucs); 
			if (l < 1) {
				l = 1;
				ucs = txt[i];
			}
			u = (short)(ucs & 0xFFFF);
			fprintf(fp, "\\u%d ", u);
			i += l;
		}
	}
}
