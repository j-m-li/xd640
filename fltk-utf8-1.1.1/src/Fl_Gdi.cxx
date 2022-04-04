//
// "$Id:  Exp $"
//
// WIN32 GDI printing device for the Fast Light Tool Kit (FLTK).
//
// Copyright (c) 2002  O'ksi'D
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
// Please report all bugs and problems to "oksid@bluewin.ch".
//

#include <stdio.h>
#include <math.h>
#include <FL/Fl_Gdi.H>
#include <FL/Fl.H>
#include <windows.h>
#include <commdlg.h>

Fl_Gdi::~Fl_Gdi()
{
	if (gc) {
		EndPage(gc);
		EndDoc(gc);
		DeleteDC(gc);
	}
}
void Fl_Gdi::fit (int x, int y, int w, int h, int align)
{
	int width = GetDeviceCaps(gc, HORZRES) - L1 - R;
	int height = GetDeviceCaps(gc, VERTRES) - T1 - B;

	L -= x * s1;
	T -= y * s1;

	double ws = width / w;
	double hs = height / h;
	if (ws < hs) {
		s = ws;
	} else {
		s = hs;
	}

	w *= s;
	h *= s;
	int dy = (height - h) / 2;
	int dx = (width - w) / 2;
	if(align & 3)
		if(align & FL_ALIGN_TOP)
			dy=0;
		else
			dy *=2;
	if(align & 12)
		if(align & FL_ALIGN_LEFT)
			dx=0;
		else
			dx *=2;

	T += dy;
	L += dx;

}

void Fl_Gdi::page(int lm, int tm, int rm, int bm, int orientation, int format)
{
	static DOCINFO DocInfo = { sizeof(DOCINFO), "FLTK Doc", NULL,0 }; 
	static PRINTDLG pd;
	
	page_++;
	if (page_ > 1) {
		EndPage(gc);
		StartPage(gc);
		SetTextAlign(gc, TA_BASELINE|TA_LEFT);
    		SetBkMode(gc, TRANSPARENT);
		return;
	}
	ZeroMemory(&pd, sizeof(pd));
	pd.lStructSize = sizeof(pd);
	pd.hwndOwner   = 0;
	pd.hDevMode    = NULL;     
	pd.hDevNames   = NULL;     
	pd.Flags       = PD_RETURNDC | PD_HIDEPRINTTOFILE | PD_PAGENUMS; 
	pd.nCopies     = 1;
	pd.nFromPage   = 0x1; 
	pd.nToPage     = 1; 
	pd.nMinPage    = 1; 
	pd.nMaxPage    = 1; 
	if (format != PS_ASK) {
			pd.Flags |= PD_RETURNDEFAULT;
	}

	if (PrintDlg(&pd)!=TRUE || !pd.hDC) 
		return;
	    

	gc = pd.hDC;
	s1 = s = GetDeviceCaps(gc, VERTRES) / ((GetDeviceCaps(gc, VERTSIZE) / 12.7)) / 72;
	StartDoc(gc, &DocInfo);
	StartPage(gc);	
	SetTextAlign(gc, TA_BASELINE|TA_LEFT);
    	SetBkMode(gc, TRANSPARENT);
	L = L1 = s * lm;
	T = T1 = s * tm;;
	R = s * rm;
	B = s * bm;
	
};




//////////////////////////////// Primitives: Colors  ////////////////////////////////////////////
void Fl_Gdi::draw_image(const uchar *data, int x, int y, int w, int h, int D, int LD)
{
	HDC dc1 = CreateCompatibleDC(gc);
    HBITMAP id = CreateCompatibleBitmap(dc1, w, h);
	SelectObject(dc1, id);
 	o = fl_gc;
	fl_gc = dc1;
	fltk.draw_image(data, 0, 0, w, h, D, LD);
	StretchBlt(gc, x*s+L, y*s+T, w*s, h*s, dc1, 0, 0, w, h, SRCCOPY);	
	fl_gc = o;
	DeleteDC(dc1);
	DeleteObject(id);
}

void Fl_Gdi::draw_image_mono(const uchar *data, int x, int y, int w, int h, int D, int LD)
{
	HDC dc1 = CreateCompatibleDC(gc);
    HBITMAP id = CreateCompatibleBitmap(dc1, w, h);
	SelectObject(dc1, id);
 	o = fl_gc;
	fl_gc = dc1;
	fltk.draw_image_mono(data, 0, 0, w, h, D, LD);
	StretchBlt(gc, x*s+L, y*s+T, w*s, h*s, dc1, 0, 0, w, h, SRCCOPY);	
	fl_gc = o;
	DeleteDC(dc1);
	DeleteObject(id);
}

void Fl_Gdi::draw_image(Fl_Draw_Image_Cb call, void *data, int x, int y, int w, int h, int D)
{
	HDC dc1 = CreateCompatibleDC(gc);
    HBITMAP id = CreateCompatibleBitmap(dc1, w, h);
	SelectObject(dc1, id);
 	o = fl_gc;
	fl_gc = dc1;
	fltk.draw_image(call, data, 0, 0, w, h, D);
	StretchBlt(gc, x*s+L, y*s+T, w*s, h*s, dc1, 0, 0, w, h, SRCCOPY);	
	fl_gc = o;
	DeleteDC(dc1);
	DeleteObject(id);
}

void Fl_Gdi::draw_image_mono(Fl_Draw_Image_Cb call, void *data, int x, int y, int w, int h, int D)
{
	HDC dc1 = CreateCompatibleDC(gc);
	HBITMAP id = CreateCompatibleBitmap(dc1, w, h);
	SelectObject(dc1, id);
 	o = fl_gc;
	fl_gc = dc1;
	fltk.draw_image(call, data, 0, 0, w, h, D);
	StretchBlt(gc, x*s+L, y*s+T, w*s, h*s, dc1, 0, 0, w, h, SRCCOPY);	
	fl_gc = o;
	DeleteDC(dc1);
	DeleteObject(id);
}


