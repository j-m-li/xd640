//
// "$Id: fl_font_win32.cxx,v 1.9.2.2 2000/06/05 21:21:08 mike Exp $"
//
// WIN32 font selection routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2002 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <FL/fl_utf8.H>
#include "Fl_Font.H"
#include <stdio.h>

extern  GrFont          GrFont_PC8x14;

Fl_FontSize::Fl_FontSize(const char* name, int size) {
	char buf[100];
	snprintf(buf, 100, name, size);
	fid = GrLoadFont(buf);
	if (!fid) {
		fid = &GrFont_PC8x14;
	}
	minsize = maxsize = size;
}

Fl_FontSize* fl_fontsize;

Fl_FontSize::~Fl_FontSize() {
  if (this == fl_fontsize) fl_fontsize = 0;
}

////////////////////////////////////////////////////////////////

// WARNING: if you add to this table, you must redefine FL_FREE_FONT
// in Enumerations.H & recompile!!
static Fl_Fontdesc built_in_table[] = {
{"helv%d.fnt"},
{"helv%db.fnt"},
{"helv%di.fnt"},
{"helv%dbi.fnt"},
{"cour%d.fnt"},
{"cour%db.fnt"},
{"cour%di.fnt"},
{"cour%dbi.fnt"},
{"tms%d.fnt"},
{"tms%db.fnt"},
{"tms%di.fnt"},
{"tms%dbi.fnt"},
{"symb%d.fnt"},
{" Terminal"},
{"BTerminal"},
{" Wingdings"},
};

Fl_Fontdesc* fl_fonts = built_in_table;

static Fl_FontSize* find(int fnum, int size) {
  
  Fl_Fontdesc* s = fl_fonts+fnum;
  if (!s->name) s = fl_fonts; // use 0 if fnum undefined
  Fl_FontSize* f;
  for (f = s->first; f; f = f->next)
    if (f->minsize <= size && f->maxsize >= size) return f;
  f = new Fl_FontSize(s->name, size);
  f->next = s->first;
  s->first = f;
  return f;
}

////////////////////////////////////////////////////////////////
// Public interface:

FL_EXPORT int fl_font_ = 0;
FL_EXPORT int fl_size_ = 0;
GrTextOption fl_font_opt;

void Fl_Fltk::font(int fnum, int size) {
  if (fnum == fl_font_ && size == fl_size_) return;
  fl_font_ = fnum; fl_size_ = size;
  fl_fontsize = find(fnum, size);
  fl_font_opt.txo_font = fl_fontsize->fid;
  fl_font_opt.txo_fgcolor.v = fl_gc->lno_color;   
  fl_font_opt.txo_bgcolor.v = GrNOCOLOR; 
  fl_font_opt.txo_chrtype = GR_WORD_TEXT; // FIXME_DJGPP
//  fl_font_opt.txo_chrtype = GR_BYTE_TEXT;
  fl_font_opt.txo_direct = GR_TEXT_RIGHT;
  fl_font_opt.txo_xalign = GR_ALIGN_LEFT;
  fl_font_opt.txo_yalign = GR_ALIGN_BASELINE;
 
}

int Fl_Fltk::height() {
	return fl_size_;
}

int Fl_Fltk::descent() {
//FIXME_DJGPP
	return 0;
}


// Unicode string buffer
static unsigned short *wstr = NULL;
static int wstr_len	= 0;

double Fl_Fltk::width(const char* c, int n) {
  int i = 0;
  double w = 0;
  while (i < n) {
    unsigned int ucs;
    int l = fl_utf2ucs((const unsigned char*)c + i, n - i, &ucs);
    if (l < 1) l = 1; 
    i += l;
    if (!fl_nonspacing(ucs)) {
 	w+= GrFontCharWidth(fl_font_opt.txo_font, (int)ucs);
    }
  }
  return  w;
}

double Fl_Fltk::width(unsigned int c) { 
  return (double) GrFontCharWidth(fl_font_opt.txo_font, (int)c);
}

void Fl_Fltk::draw(const char* str, int n, int x, int y) {
  int i = 0;
  int lx = 0;
  fl_font_opt.txo_fgcolor.v = fl_gc->lno_color;   
  while (i < n) {
    unsigned int u;
    unsigned short ucs;
    int l = fl_utf2ucs((const unsigned char*)str + i, n - i, &u);
    if (fl_nonspacing(u)) {
	x -= lx;
    } else {
        lx = (int) Fl_Fltk::width(u);
    }
    ucs = u; 
    if (l < 1) l = 1;
    i += l;
    GrDrawChar((int)u, x, y, &fl_font_opt);
    x += lx;
  }
}

void Fl_Fltk::rtl_draw(const char* c, int n, int x, int y) {
  int wn;
  int i = 0;
  int lx = 0;
  if (n > wstr_len) {
    wstr = (unsigned short*) realloc(wstr, sizeof(short) * n);
    wstr_len = n;
  }
  wn = fl_utf2unicode((const unsigned char *)c, n, wstr);
  fl_font_opt.txo_fgcolor.v = fl_gc->lno_color;   
  while (i < wn) {
    lx = (int) Fl_Fltk::width(wstr[i]);
    x -= lx;
//FIXME_DJGPP
  //  TextOutW(fl_gc, x, y, (WCHAR*)wstr + i, 1);
	if (fl_nonspacing(wstr[i])) {
		x += lx;
	}
    i++;
  }
}

//
// End of "$Id: fl_font_win32.cxx,v 1.9.2.2 2000/06/05 21:21:08 mike Exp $".
//
