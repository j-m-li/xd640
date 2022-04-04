/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2002  O'ksi'D
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
	frame->scan_all(scan_to_rtf_cb, this);
	fprintf(fp, "}\n");
	fclose(fp);
}


void Xd6HtmlToRtf::to_rtf_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s,
	char *c, int len)
{
	if (b != lb) {
		if (b->style & PAGE_BREAK && b->lines[0] != l) {
			if (lb) fprintf(fp, "\\par");
			fprintf(fp, "\\page\n");
			lb = NULL;
			return;
		}
		lstyle = IS_INLINE|FONT_SIZE_3|FONT_SANS_SERIF;
		block_to_rtf(fp, b, lb);
	}
	seg_to_rtf(fp, s, lstyle);

	lb = b;
	lstyle = s->style;

	
	if (!(s->style & DISPLAY)) {
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
	if (b->style & PAGE_BREAK) {
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
	if (b->style & TEXT_ALIGN_CENTER) {
		fprintf(fp, "\\qc ");
	} else if (b->style & TEXT_ALIGN_RIGHT) {
		fprintf(fp, "\\qr ");
	} else {
		fprintf(fp, "\\ql ");
	}
	if (b->style & RTL_DIRECTION) {
		fprintf(fp, "\\rtlmark ");
	} else {
		fprintf(fp, "\\ltrmark ");
	}
}

void Xd6HtmlToRtf::seg_to_rtf(FILE *fp, Xd6HtmlSegment *s, int style)
{
	if (s->style & DISPLAY) {
		((Xd6HtmlDisplay*)s)->to_rtf(fp);
		if (((Xd6HtmlDisplay*)s)->display == DISPLAY_TAB) {
			if (s->style & RTL_DIRECTION) {
				fprintf(fp, "\\tqr");
			}		
			fprintf(fp, "\\tab ");
		}
		return;
	}
	if ((style & FONT_BOLD) != (s->style & FONT_BOLD)) {
		if (style & FONT_BOLD) {
			pop_tag(fp, "\\b0 ");
		} else {
			push_tag(fp, "\\b ", "\\b0 ");
		}
	}

	if ((style & FONT_ITALIC) != (s->style & FONT_ITALIC)) {
		if (style & FONT_ITALIC) {
			pop_tag(fp, "\\i0 ");
		} else {
			push_tag(fp, "\\i ", "\\i0 ");
		}
	}

	if ((style & UNDERLINE) != (s->style & UNDERLINE)) {
		if (style & UNDERLINE) {
			pop_tag(fp, "\\u0 ");
		} else {
			push_tag(fp, "\\ul ", "\\ul0 ");
		}
	}
	if ((style & FONTS) != (s->style & FONTS)) {
		
		if ((s->style & FONTS) == FONT_SERIF) {
			fprintf(fp, "\\f2 ");
		} else if ((s->style & FONTS) == FONT_MONOSPACE) {
			fprintf(fp, "\\f1 ");
		} else {
			fprintf(fp, "\\f0");
		}	
	}
	if ((style & FONT_SIZES) != (s->style & FONT_SIZES)) {
		int si;
		switch(s->style & FONT_SIZES) {
		case FONT_SIZE_1:
			si = 8; break;
		case FONT_SIZE_2:
			si = 10; break;
		case FONT_SIZE_3:
			si = 12; break;
		case FONT_SIZE_4:
			si = 14; break;
		case FONT_SIZE_5:
			si = 18; break;
		case FONT_SIZE_6:
			si = 24; break;
		case FONT_SIZE_7:
			si = 36; break;
		default:
			si = 12;
		}
		fprintf(fp, "\\fs%d ", si * 2);
	}
		
	if ((style & RTL_DIRECTION) != (s->style & RTL_DIRECTION)) {
		if (s->style & RTL_DIRECTION) {
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
			l = fl_utf2ucs((unsigned char*)txt + i, len - i, &ucs); 
			if (l < 1) {
				l = 1;
				ucs = txt[i];
			}
			fprintf(fp, "\\u%d ", (signed short)ucs);
			i += l;
		}
	}
}
