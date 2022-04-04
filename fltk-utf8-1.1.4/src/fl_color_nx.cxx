//
// "$Id: fl_color.cxx,v 1.12.2.5.2.7 2002/05/16 02:16:17 easysw Exp $"
//
// Color functions for the Fast Light Tool Kit (FLTK).
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

// Implementation of Fl_Fltk::color(i), Fl_Fltk::color(r,g,b).

// Also code to look at the X visual and figure out the best way to turn
// a color into a pixel value.

// SGI compiler seems to have problems with unsigned char arguments
// being used to index arrays.  So I always copy them to an integer
// before use.


#  include <FL/Fl.H>
#  include <FL/x.H>
#  include <FL/fl_draw.H>

#  include "Fl_XColor.H"



////////////////////////////////////////////////////////////////
// figure_out_visual() calculates masks & shifts for generating
// pixels in true-color visuals:

uchar fl_redmask, fl_greenmask, fl_bluemask;
int fl_redshift, fl_greenshift, fl_blueshift, fl_extrashift;
static uchar beenhere = 0;

static void figure_out_visual() {
  beenhere = 1;
  // get the bit masks into a more useful form:
  int i,j,m;
  j = 64;
  i = 8;	
  fl_redshift = j-8;
  fl_redmask = (j-i >= 8) ? 0xFF : 0xFF-(255>>(j-i));

  fl_greenshift = j-8;
  fl_greenmask = (j-i >= 8) ? 0xFF : 0xFF-(255>>(j-i));

  fl_blueshift = j-8;
  fl_bluemask = (j-i >= 8) ? 0xFF : 0xFF-(255>>(j-i));

  i = fl_redshift;
  if (fl_greenshift < i) i = fl_greenshift;
  if (fl_blueshift < i) i = fl_blueshift;
  if (i < 0) {
    fl_extrashift = -i;
    fl_redshift -= i; fl_greenshift -= i; fl_blueshift -= i;
  } else
    fl_extrashift = 0;

}

#if MSDOS
static unsigned long fl_cmap[256] = {
#else
static unsigned int fl_cmap[256] = {
#endif
#include "fl_cmap.h" // this is a file produced by "cmap.cxx":
};

#  if HAVE_OVERLAY
Fl_XColor fl_xmap[2][256];
uchar fl_overlay;
Colormap fl_overlay_colormap;
XVisualInfo* fl_overlay_visual;
ulong fl_transparent_pixel;
#  else
Fl_XColor fl_xmap[1][256];
#    define fl_overlay 0
#  endif

////////////////////////////////////////////////////////////////
// Get an rgb color.  This is easy for a truecolor visual.  For
// colormapped it picks the closest color out of the cube in the
// fltk colormap.  However if this color cube entry has been
// requested before, you will get the earlier requested color, and
// even this may be approximated if the X colormap was full.

ulong fl_xpixel(uchar r,uchar g,uchar b) {
  return GR_RGB(r, g, b);
}

void Fl_Fltk::color(uchar r,uchar g,uchar b) {
    GrSetGCForeground(fl_gc,fl_xpixel(r,g,b));//(GR_GC_ID gc, GR_COLOR foreground);
    GrSetGCBackground(fl_gc,GR_RGB(200,200,200));
}

////////////////////////////////////////////////////////////////
// Get a color out of the the fltk colormap.  Again for truecolor
// visuals this is easy.  For colormap this actually tries to allocate
// an X color, and does a least-squares match to find the closest
// color if X cannot allocate that color.

// calculate what color is actually on the screen for a mask:
static inline uchar realcolor(uchar color, uchar mask) {
#  if 0
  // accurate version if the display has linear gamma, but fl_draw_image
  // works better with the simpler version on most screens...
  uchar m = mask;
  uchar result = color&m;
  for (;;) {
    while (m&mask) {m>>=1; color>>=1;}
    if (!m) break;
    mask = m;
    result |= color&m;
  }
  return result;
#  else
  return (color&mask) | (~mask)&(mask>>1);
#  endif
}

ulong fl_xpixel(Fl_Color i) {
  if (i & 0xffffff00) {
    return fl_xpixel((i >> 24) & 255, (i >> 16) & 255, (i >> 8) & 255);
  }
  Fl_XColor &xmap = fl_xmap[0][i];
  if (xmap.mapped) return xmap.pixel;

  uchar r,g,b;
  {unsigned c = fl_cmap[i]; r=uchar(c>>24); g=uchar(c>>16); b=uchar(c>>8);}

    GR_COLOR xcol;
      xmap.pixel = xcol=GR_RGB(r, g, b);
        xmap.mapped = 1;
        xmap.r = r>>8;
        xmap.g = g>>8;
        xmap.b = b>>8;
        return xcol;

}

Fl_Color fl_color_;

void Fl_Fltk::color(Fl_Color i) {
  if (i & 0xffffff00) {
    unsigned rgb = (unsigned)i;
    Fl_Fltk::color((uchar)(rgb >> 24), (uchar)(rgb >> 16), (uchar)(rgb >> 8));
  } else {
    fl_color_ = i;
    XSetForeground(fl_display, fl_gc, fl_xpixel(i));
  }
}

void Fl::free_color(Fl_Color i, int overlay) {
#  if HAVE_OVERLAY
#  else
  if (overlay) return;
#  endif
  if (fl_xmap[overlay][i].mapped) {
#  if USE_COLORMAP
#    if HAVE_OVERLAY
    Colormap colormap = overlay ? fl_overlay_colormap : fl_colormap;
#    else
    Colormap colormap = fl_colormap;
#    endif
    if (fl_xmap[overlay][i].mapped == 1)
      XFreeColors(fl_display, colormap, &(fl_xmap[overlay][i].pixel), 1, 0);
#  endif
    fl_xmap[overlay][i].mapped = 0;
  }
}

void Fl::set_color(Fl_Color i, unsigned c) {
  if (fl_cmap[i] != c) {
    free_color(i,0);
#  if HAVE_OVERLAY
    free_color(i,1);
#  endif
    fl_cmap[i] = c;
  }
}


//
// End of "$Id: fl_color.cxx,v 1.12.2.5.2.7 2002/05/16 02:16:17 easysw Exp $".
//
