//
// "$Id: Xd6Gif.H,v 1.6.2.6 2001/04/24 19:35:38 easysw Exp $"
//
// Gif header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2001 by Bill Spitzak and others.
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

#ifndef Xd6Gif_H
#define Xd6Gif_H

#include <FL/Fl.h>

struct Xd6Gif {
  unsigned char *data;
  unsigned char *bitmap;
  int w, h, d; // set by first draw...
  unsigned long id;
  unsigned long mask; // for internal use (mask bitmap)
  char *file;
  static char ColorMap;
  static char ColorRes;
  static char BitsPerPixel;
  static char BackGround;
  static char Transparent;
  static unsigned long PixelCount;
  static int ClearCode;
  static int EOFCode;
  static int RunningCode;
  static int RunningBits;
  static int MaxCode1;
  static int CrntCode;
  static int CrntShiftState;
  static unsigned long CrntShiftDWord;
  static unsigned char Buf[255];
  Xd6Gif();
  ~Xd6Gif();
  void draw(int X, int Y, int W, int H, int cx=0, int cy=0);
  void draw(int X, int Y) {draw(X, Y, w, h, 0, 0);}
  int load(const char *name);
  int save(const char *name);
  void set_size(int w, int h);
};

#endif

//
// End of "$Id: Xd6Gif.H,v 1.6.2.6 2001/04/24 19:35:38 easysw Exp $".
//
