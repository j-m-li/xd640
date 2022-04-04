//
// "$Id: scroll.cxx,v 1.4.2.2 2000/06/05 21:21:24 mike Exp $"
//
// UTF-8 test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2000 by Bill Spitzak and others.
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

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <FL/fl_draw.H>
#include <FL/math.h>
#include <FL/fl_utf8.H>
#ifndef _WIN32
#  include <X11/Xlocale.h>  
#  include <X11/Xlib.h>  
#endif


Fl_Scroll* thescroll;

void box_cb(Fl_Widget* o, void*) {
  thescroll->box(((Fl_Button*)o)->value() ? FL_DOWN_FRAME : FL_NO_BOX);
  thescroll->redraw();
}

class My_Input : public Fl_Input
{
public:
	My_Input (int x, int y, int w, int h) : Fl_Input(x, y, w, h) {};
	void draw() {
		if (type() == FL_HIDDEN_INPUT) return;
		 Fl_Boxtype b = box();
		if (damage() & FL_DAMAGE_ALL) draw_box(b, color());
		drawtext(x()+Fl::box_dx(b)+3, y()+Fl::box_dy(b),
			w()-Fl::box_dw(b)-6, h()-Fl::box_dh(b));
	}
	void drawtext(int X, int Y, int W, int H) {
		fl_color(textcolor());
		fl_font(textfont(), textsize());
		fl_rtl_draw(value(), strlen(value()), 
			X + (int)fl_width(value()), Y + fl_height() -fl_descent());
	}
};

int main(int argc, char** argv) {
  Fl_Window window(200 + 5*75,400);
  Fl_Scroll scroll(200,0,5*75,400);
  Fl::set_font((Fl_Font)(FL_FREE_FONT),
#ifdef _WIN32
	" MS Gothic"
#else
	"-*-courier-medium-r-normal--*,"
	"-*-fixed-medium-*-*--*-*-*-*-*-*-iso8859-15,"
	"-*-symbol-*-*-*--*-*-*-*-*-*-adobe-fontspecific,"
	"-*-*dingbats-*-*-*--*-*-*-*-*-*-adobe-fontspecific,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-2,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-3,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-4,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-5,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-6,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-7,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-8,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-9,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-13,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-14,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-koi8-*,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-jisx0208.1983-*,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-jisc6226.1978-*,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-jisx0201.1976-*,"
	"-*-*-*-*-*--*-*-*-*-*-*-ksc5601.1987-*,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-jisx0208.1990-*,"
	"-*-fixed-medium-r-normal--*-*-*-*-*-*-big5*-*,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-iso646.1991-*,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-mulearabic-1,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-muleindian-1,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-muleindian-2,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-mulelao-*,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-tis620.2529-*,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-is13194-*,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-mulearabic-2,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-sisheng_cwnn-*,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-omron_udc_zh-*,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-muleipa-*,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-viscii1.1-*,"
	//"-*-fixed-*-*-*--*-*-*-*-*-*-gost19768.74-*,"
	"-*-unifont-*-*-*--*-*-*-*-*-*-iso10646-1"
#endif	
  );

#ifndef _WIN32
  setlocale(LC_CTYPE, "");
  XSetLocaleModifiers("");
#endif

  //for (int y=1; y<4095; y++) {
  int off = 2;
  
  if (argc > 1) {
	off = (int)strtoul(argv[1], NULL, 0);
	off /= 16;
  }
  argc = 1;
  for (int y=off; y< 0x10000 / 16/*off + 1024*/; y++) {
    int o = 0;
    char bu[25];
    char buf[16*5];
    int i = 16 * y; 
    for (int x=0; x<16; x++) {
	int l;
	l = fl_ucs2utf(i, buf + o);
	if (l < 1) l = 1;
	o += l;
	i++;
    }
    buf[o] = '\0';
    sprintf(bu, "0x%04X", y * 16);
    Fl_Input* b = new Fl_Input(200,(y-off)*25,60,25);
    b->value(strdup(bu));
    b = new Fl_Input(260,(y-off)*25,400,25);
    b->textfont((Fl_Font)(FL_FREE_FONT));
    b->value(strdup(buf));
  }
  scroll.end();
  window.resizable(scroll);


  thescroll = &scroll;

  int l;
  char *latin1 = "ABCabcàèéïâîöüã123";
  char *utf8 = (char*) malloc(strlen(latin1) * 3 + 1);

  Fl_Input i1(5, 5, 190, 25);
  l = fl_latin12utf((const unsigned char*)latin1, strlen(latin1), utf8);
  utf8[l] = '\0';
  i1.value(utf8);

  char *utf8l = (char*) malloc(strlen(utf8) * 3 + 1);
  Fl_Input i2(5, 35, 190, 25);
  l = fl_utf_tolower((const unsigned char*)utf8, l, utf8l);
  utf8l[l] = '\0';
  i2.value(utf8l);

  char *utf8u = (char*) malloc(strlen(utf8l) * 3 + 1);
  Fl_Input i3(5, 65, 190, 25);
  l = fl_utf_toupper((const unsigned char*)utf8l, l, utf8u);
  utf8u[l] = '\0';
  i3.value(utf8u);

  char *ltr_txt = "\\->e\xCC\x82=\xC3\xAA";
  Fl_Input i4(5, 90, 190, 25);
  i4.value(ltr_txt);	
  i4.textfont((Fl_Font)(FL_COURIER));

  unsigned short arabic[] ={/*8238,*/ 1610, 1608, 1606, 1616, 1603, 1608, 1583, 0};
  char abuf[40];
  l = fl_unicode2utf(arabic, 8, abuf);  
  abuf[l] = 0;
  My_Input i5(5, 115, 190, 50);
  i5.textfont((Fl_Font)(FL_FREE_FONT));
  i5.textsize(36);
  i5.value(abuf);

 unsigned short arabic1[] ={/*8238,*/ 0x5c, 1610, 0x20, 1608, 0x20, 1606, 0x20,  1616, 0x20, 1603, 0x20, 1608, 0x20, 1583, 0};
 
  l = fl_unicode2utf(arabic1, 15, abuf);  
  abuf[l] = 0;
  My_Input i6(5, 175, 190, 50);
  i6.textfont((Fl_Font)(FL_FREE_FONT));
  i6.textsize(30);
  i6.value(abuf);
  window.end();
  window.show(argc,argv);
  return Fl::run();
}

//
// End of "$Id: scroll.cxx,v 1.4.2.2 2000/06/05 21:21:24 mike Exp $".
//
