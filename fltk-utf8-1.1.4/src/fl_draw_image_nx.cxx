//
// "$Id: fl_draw_image.cxx,v 1.5.2.6.2.5 2002/08/09 03:17:30 easysw Exp $"
//
// Image drawing routines for the Fast Light Tool Kit (FLTK).
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

// I hope a simple and portable method of drawing color and monochrome
// images.  To keep this simple, only a single storage type is
// supported: 8 bit unsigned data, byte order RGB, and pixels are
// stored packed into rows with the origin at the top-left.  It is
// possible to alter the size of pixels with the "delta" argument, to
// add alpha or other information per pixel.  It is also possible to
// change the origin and direction of the image data by messing with
// the "delta" and "linedelta", making them negative, though this may
// defeat some of the shortcuts in translating the image for X.


// A list of assumptions made about the X display:

// bits_per_pixel must be one of 8, 16, 24, 32.

// scanline_pad must be a power of 2 and greater or equal to 8.

// PsuedoColor visuals must have 8 bits_per_pixel (although the depth
// may be less than 8).  This is the only limitation that affects any
// modern X displays, you can't use 12 or 16 bit colormaps.

// The mask bits in TrueColor visuals for each color are
// contiguous and have at least one bit of each color.  This
// is not checked for.

// For 24 and 32 bit visuals there must be at least 8 bits of each color.

////////////////////////////////////////////////////////////////

#  include <FL/Fl.H>
#  include <FL/fl_draw.H>
#  include <FL/x.H>
#  include "Fl_XColor.H"
#  include "flstring.h"
#  if MSDOS
typedef  unsigned long U32;
#  endif
#  define MAXBUFFER 0xFFFF // 64k - 1

static void innards(const uchar *buf, int X, int Y, int W, int H,
		    int delta, int linedelta, int mono,
		    Fl_Draw_Image_Cb cb, void* userdata)
{
 if (linedelta == 0) linedelta = W*delta;

  if (buf)
  {
    bool flip_h = (    delta < 0); if (flip_h)     delta = -    delta;
    bool flip_v = (linedelta < 0); if (flip_v) linedelta = -linedelta;
    if (delta >= 1)
    {
      GR_WINDOW_ID id = fl_window;
      GR_GC_ID     gc = GrNewGC();
      U32          prev_fg = (U32) -1;
      GR_POINT     plist[1000];
      int          pcnt = 0;

      for ( int y = 0; y < H; ++y )
      {
        const uchar * pixel = buf;
        buf += linedelta;
        for ( int x = 0; x < W; ++x )
        {
          U32 fg;

	  // calculate the foreground color
	  if (delta >= 3) // presume it's RGB...
            fg = GR_RGB(pixel[0],pixel[1],pixel[2]);
          else // presume it's greyscale
            fg = GR_RGB(pixel[0],pixel[0],pixel[0]);

	  // if the foreground color changed, dump any queued pixels remaining
	  // for the old color, then change the color
          if (fg != prev_fg) {
            if (pcnt) {
              GrPoints(id,gc,pcnt,plist);
              pcnt = 0;
            }
            GrSetGCForeground(gc,fg);
            prev_fg = fg;
          }

          // queue the next pixel value
          plist[pcnt].x = X+(flip_h?(W-x-1):x);
          plist[pcnt].y = Y+(flip_v?(H-y-1):y);
          pcnt++;

          // if we've reached the max. number of queued pixels, dump them
          if (pcnt == sizeof(plist)/sizeof(*plist)) {
            GrPoints(id,gc,pcnt,plist);
            pcnt = 0;
          }

          pixel += delta;
        }
      }

      // dump any remaining queued points
      if (pcnt)
        GrPoints(id,gc,pcnt,plist);

      GrDestroyGC(gc);
    }
    else
    {
      Fl::fatal(fl_cannot_do_delta,delta);
    }
  } 
  else
  {
    int linesize = ((linedelta>0) ? linedelta : -linedelta);
    int blocking = H;
    static uchar * buffer = 0;
    static long buffer_size = 0;
    {
      int size = linesize*H;
      if (size > MAXBUFFER) 
      {
        size = MAXBUFFER;
        blocking = MAXBUFFER/linesize;
      }
      if (size > buffer_size) 
      {
        delete[] buffer;
        buffer_size = size;
        buffer = new uchar[size];
      }
    }
    uchar * linebuf = new uchar[linesize];
    for (int j=0; j<H; ) 
    {
      uchar *to = buffer;
      int k;
      for (k = 0; j<H && k<blocking; k++, j++) 
      {
        cb(userdata, X, Y+j, W, linebuf);
        memcpy(to,linebuf,linesize);
        to += linesize;
      }
      innards(buffer,X,Y+j-k,W,blocking,delta,linedelta,0,0,0);
    }
    delete [] linebuf;
  }
}

void Fl_Fltk::draw_image(const uchar* buf, int x, int y, int w, int h, int d, int l){
  innards(buf,x,y,w,h,d,l,(d<3&&d>-3),0,0);
}
void Fl_Fltk::draw_image(Fl_Draw_Image_Cb cb, void* data,
		   int x, int y, int w, int h,int d) {
  innards(0,x,y,w,h,d,0,(d<3&&d>-3),cb,data);
}
void Fl_Fltk::draw_image_mono(const uchar* buf, int x, int y, int w, int h, int d, int l){
  innards(buf,x,y,w,h,d,l,1,0,0);
}
void Fl_Fltk::draw_image_mono(Fl_Draw_Image_Cb cb, void* data,
		   int x, int y, int w, int h,int d) {
  innards(0,x,y,w,h,d,0,1,cb,data);
}

void Fl_Fltk::rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b) {
 // this may need speeded up sometime, as in the above code .. jsk
    fl_color(r,g,b);
    fl_rectf(x,y,w,h);
}


//
// End of "$Id: fl_draw_image.cxx,v 1.5.2.6.2.5 2002/08/09 03:17:30 easysw Exp $".
//
