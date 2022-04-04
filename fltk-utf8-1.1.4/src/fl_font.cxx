//
// "$Id: fl_font.cxx,v 1.9.2.4 2000/06/05 21:21:08 mike Exp $"
//
// Font selection code for the Fast Light Tool Kit (FLTK).
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

// Select fonts from the FLTK font table.
#include "flstring.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include <FL/fl_utf8.H>
#include "Fl_Font.H"
  
#include <stdio.h>
#include <stdlib.h>


#ifdef WIN32
#  include "fl_font_win32.cxx"
#elif defined(__MACOS__)
#  include "fl_font_mac.cxx"
#elif NANO_X
#  include "fl_font_nx.cxx"
#elif DJGPP
#  include "fl_font_dj2.cxx"
#elif USE_XFT
#  include "fl_font_xft.cxx"
#else
#  include "fl_font_x.cxx"
#endif // WIN32


double Fl_Fltk::width(const char* c) {
  if (c) return Fl_Fltk::width(c, strlen(c));
  else return 0.0f;
}

void Fl_Fltk::draw(const char* str, int x, int y) {
  Fl_Fltk::draw(str, strlen(str), x, y);
}


//
// End of "$Id: fl_font.cxx,v 1.9.2.4 2000/06/05 21:21:08 mike Exp $".
//
