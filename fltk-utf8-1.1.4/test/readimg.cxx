
//
// "$Id: scroll.cxx,v 1.4.2.2 2000/06/05 21:21:24 mike Exp $"
//
//  fl_read_image test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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
#include <FL/Fl_Box.H>
#include <string.h>
#include <stdio.h>
#include <FL/fl_draw.H>
#include <FL/fl_math.h>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Ps.H>
#ifdef WIN32
#include <FL/Fl_Gdi.H>
#endif
#include <FL/Fl_Group.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Image.H>
#include "porsche.xpm"

unsigned char * fl_read_image(unsigned char *p, int X, int Y, int w, int h,
	int alpha);

Fl_Window *win;

void print(Fl_Widget *, void *wo) {
        win->make_current();
	unsigned char *data = fl_read_image(NULL, 0, 0, win->w(),
		win->h(), 0);
	
        Fl_Window window(win->w(), win->h());
	Fl_PrintingGroup * g = new Fl_PrintingGroup(0,0, win->w(), win->h());
	Fl_RGB_Image img(data, win->w(), win->h(), 3);
	Fl_Box w(0,0, win->w(), win->h());
	w.image(&img);
	g->end();
	window.end();
	window.show();
	Fl::check();
#ifdef WIN32
	Fl_Gdi *doc = new Fl_Gdi();
#else
	const char *f = fl_file_chooser("Save", "*.ps" , "*.ps");
	FILE *o = fl_fopen(f, "w");
	Fl_Ps * doc= new Fl_Ps(o,1,PS_PORTRAIT, PS_A4);
#endif
	doc->page(72,72,72,72, PS_LANDSCAPE, PS_ASK); //margins
	doc->fit(g->x(),g->y(),g->w(),g->h()); //fitting into margins
	g->print(doc);
	delete doc;
#ifndef WIN32
	 fclose(o);
#endif
};



int main(int argc, char** argv) {
  Fl_Window window(5*75,400);
  window.box(FL_NO_BOX);
  win = &window;
 
  Fl_Box box(0,0,5*75,window.h()); // gray area below the scroll
  box.box(FL_FLAT_BOX);
  Fl_Button b(0, 75 + 25,5*75,120,"Pixmap"); 
  Fl_Pixmap *pixmap = new Fl_Pixmap(porsche_xpm); 
  b.image(pixmap);
  

  Fl_Button *b1 = new Fl_Button(5,310, 90, 25, "Print window");
  b1->callback(print,0);

  window.end();
  window.show(argc,argv);
  return Fl::run();
}

//
// End of "$Id: scroll.cxx,v 1.4.2.2 2000/06/05 21:21:24 mike Exp $".
//
