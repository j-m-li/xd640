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
#include "Xd6HtmlPrint.h"
#include <FL/fl_utf8.H>

Xd6HtmlPrint::Xd6HtmlPrint(Xd6HtmlFrame *f, const char *file)
{
	snprintf(out_file, 1024, "%s", file);
}

Xd6HtmlPrint::~Xd6HtmlPrint()
{
}

void Xd6HtmlPrint::print(Xd6HtmlFrame *f)
{
	PRINTDLG pd;
	
	static DOCINFO DocInfo = { sizeof(DOCINFO), out_file, NULL,0 }; // declare DocInfo for use and set the name of the print job as 'Name Of Document'

	frm = f;
	// Initialize PRINTDLG
	ZeroMemory(&pd, sizeof(pd));
	pd.lStructSize = sizeof(pd);
	pd.hwndOwner   = 0;
	pd.hDevMode    = NULL;     // Don't forget to free or store hDevMode
	pd.hDevNames   = NULL;     // Don't forget to free or store hDevNames
	pd.Flags       = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC | PD_HIDEPRINTTOFILE | PD_PAGENUMS; 
	pd.nCopies     = 1;
	pd.nFromPage   = 0x1; 
	pd.nToPage     = f->height / f->page_height; 
	pd.nMinPage    = 1; 
	pd.nMaxPage    = f->height / f->page_height; 


	if (PrintDlg(&pd)!=TRUE) 
		return;
	hDC = pd.hDC;
	from_p = pd.nFromPage;
	to_p = pd.nToPage;
	StartDoc(pd.hDC, &DocInfo);
	frame_to_ps(from_p, to_p);
	EndDoc(hDC);
	DeleteDC(hDC);
}

void Xd6HtmlPrint::direct_print(Xd6HtmlFrame *f)
{
	PRINTDLG pd;
	
	static DOCINFO DocInfo = { sizeof(DOCINFO), out_file, NULL,0 }; // declare DocInfo for use and set the name of the print job as 'Name Of Document'

	frm = f;
	// Initialize PRINTDLG
	ZeroMemory(&pd, sizeof(pd));
	pd.lStructSize = sizeof(pd);
	pd.hwndOwner   = 0;
	pd.hDevMode    = NULL;     // Don't forget to free or store hDevMode
	pd.hDevNames   = NULL;     // Don't forget to free or store hDevNames
	pd.Flags       = PD_RETURNDEFAULT | PD_RETURNDC; 
	pd.nCopies     = 1;
	pd.nFromPage   = 1; 
	pd.nToPage     = f->height / f->page_height; 
	pd.nMinPage    = 1; 
	pd.nMaxPage    = f->height / f->page_height; 


	if (PrintDlg(&pd)!=TRUE) 
		return;
	hDC = pd.hDC;
	from_p = pd.nFromPage;
	to_p = pd.nToPage;
	StartDoc(hDC, &DocInfo); 
	frame_to_ps(from_p, to_p);
	EndDoc(hDC);
	DeleteDC(hDC);
}

void Xd6HtmlPrint::frame_to_ps(int f, int t)
{
	from_p = f;
	to_p = t;
	int h = GetDeviceCaps(hDC, VERTRES);
	int w = GetDeviceCaps(hDC, HORZRES);
	double ph = frm->page_height +
				frm->page_margin_top + 
				frm->page_margin_bottom;
	double pw = frm->page_width +
				frm->page_margin_left +
				frm->page_margin_right;
	double dh = h / ph;
	double dw = w / pw;
	ratio = dh;
	if (dw < dh) ratio = dw;
	page_nb = 0;
	StartPage(hDC);
	for (int i = 0; i < frm->nb_blocks; i++) {
		block_to_ps(frm->blocks[i]);	
	}
	page_nb++;
	if (page_nb >= from_p && page_nb <= to_p) {
		ps_footer(page_nb);
		EndPage(hDC); 
	}
}

void Xd6HtmlPrint::block_to_ps(Xd6HtmlBlock *b)

{
	int i;
	for (i = 0; i < b->nb_lines; i++) {
		line_to_ps(b->lines[i], b->left, b->top);
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
	int XX = X;
	s->set_font();
	s->set_color();

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
	if (s->style & UNDERLINE) {
		int co = fl_color();
		HDC f = fl_gc;
		fl_gc = hDC;
		fl_line_style(FL_SOLID, (int)(fl_size() * ratio / 12));
		fl_line((int)((frm->page_margin_left + XX) * ratio),
				(int)((Y + frm->page_margin_top + 1) * ratio),
				(int)((frm->page_margin_left + XX + s->width) * ratio),
				(int)((Y + frm->page_margin_top + 1) * ratio));
		fl_gc = f;
		fl_line_style(FL_SOLID, 1);
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

int Xd6HtmlPrint::char_to_ps(char *text, int len, int X, int Y, int *wi)
{
	unsigned int ucs;
	int l;
	double W;
	double w;
	SIZE s;
 	l = fl_utf2ucs((unsigned char *)text, len, &ucs);
	if (l < 1) l = 1;
	HDC f;
	int co = fl_color();
	w = fl_width(text, l);
	int fo = fl_font();
	f = fl_gc;
	fl_gc = hDC;
	int si = fl_size();
	
	fl_font(FL_HELVETICA, 6);
	fl_font(fo, (int)(si * ratio));
	fl_color(co);

	unsigned short u = ucs;
	fl_draw(NULL, 0, 0, 0); // select object
	GetTextExtentPoint32W(hDC, &u, 1, &s);
	W = s.cx / ratio;
	
	if (w < 1) {
		w = W * *wi;
		*wi = 0;
	} else {
		*wi = (int)w;
		w = (w - W) / 2.0;
	}

	fl_draw(text, l, (int)((frm->page_margin_left + X + w) * ratio), 
		(int)((frm->page_margin_top + Y) * ratio - s.cy + fl_descent())); 
	fl_gc = f;
	fl_font(fo, si);
	return l;
}



void Xd6HtmlPrint::new_page(void)
{
	page_nb++;
	if (page_nb >= from_p && page_nb <= to_p) {
		ps_footer(page_nb);
	}
	if (page_nb >= from_p && page_nb + 1 <= to_p) {	
		EndPage(hDC); 
		StartPage(hDC);
	}
}




/*
 * "$Id: $"
 */
