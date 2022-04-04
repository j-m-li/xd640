//
// "$Id: Xd6Gif.cxx,v 1.9.2.4 2001/01/22 15:13:40 easysw Exp $"
//
// GIF drawing code for the Fast Light Tool Kit (FLTK).
//
//   Copyright 1997-2000 by Easy Software Products.
//   Image support donated by Matthias Melcher, Copyright 2000.
//   Copyright 1998-2001 by Bill Spitzak and others.
//   Uncompressed Gif image saving inspired by libungif-4.1.0b1  
//     Copyright (c) 1997  Eric S. Raymond
//     Copyright (c) 1989,1990 Gershon Elber
//     Copyright (c) 2001  O'ksi'D
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


#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu_Item.H>

#include "Xd6Gif.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GIF_INTERLACE	0x40
#define GIF_COLORMAP	0x80

#define LZ_MAX_CODE     4095            /* Biggest code possible in 12 bits. */
#define LZ_BITS         12

#define FLUSH_OUTPUT            4096    /* Impossible code, to signal flush. */
#define FIRST_CODE              4097    /* Impossible code, to signal first. */
#define NO_SUCH_CODE            4098    /* Impossible code, to signal empty. */

char Xd6Gif::ColorMap;
char Xd6Gif::ColorRes;
char Xd6Gif::BitsPerPixel;
char Xd6Gif::BackGround;
char Xd6Gif::Transparent;
unsigned long Xd6Gif::PixelCount;
int Xd6Gif::ClearCode;
int Xd6Gif::EOFCode;
int Xd6Gif::RunningCode;
int Xd6Gif::RunningBits;
int Xd6Gif::MaxCode1;
int Xd6Gif::CrntCode;
int Xd6Gif::CrntShiftState;
unsigned long Xd6Gif::CrntShiftDWord;
unsigned char Xd6Gif::Buf[255];

typedef unsigned char	gif_cmap_t[256][3];

static int	gif_read_cmap(FILE *fp, int ncolors, gif_cmap_t cmap);
static int	gif_get_block(FILE *fp, unsigned char *buffer);
static int	gif_get_code (FILE *fp, int code_size, int first_time);
static int	gif_read_lzw(FILE *fp, int first_time, int input_code_size);
static int	gif_read_image(FILE *fp, Xd6Gif *img, gif_cmap_t cmap,
		               int interlace, int transparent);
static int 	load_gif(Xd6Gif *img, FILE  *fp);

static int 	save_gif(Xd6Gif *img, FILE  *fp);
static void 	compress_output(Xd6Gif *img, FILE *fp, int Code);
static void 	buffered_output(Xd6Gif *img, FILE *fp, int c);

void fl_restore_clip(); // in fl_rect.C

void Xd6Gif::draw(int XP, int YP, int WP, int HP, int cx, int cy) {
  // ignore empty or bad pixmap data:
  if (w<0) {
	return;
  }
  if (!w) return;
  // account for current clip region (faster on Irix):
  int X,Y,W,H; fl_clip_box(XP,YP,WP,HP,X,Y,W,H);
  cx += X-XP; cy += Y-YP;
  // clip the box down to the size of image, quit if empty:
  if (cx < 0) {W += cx; X -= cx; cx = 0;}
  if (cx+W > w) W = w-cx;
  if (W <= 0) return;
  if (cy < 0) {H += cy; Y -= cy; cy = 0;}
  if (cy+H > h) H = h-cy;
  if (H <= 0) return;
  if (!id) {
    id = (ulong)fl_create_offscreen(w, h);
    fl_begin_offscreen((Fl_Offscreen)id);
    if (data) fl_draw_image(data, 0, 0, w, h, d, 0);
    if (bitmap) {
#ifdef WIN32 // Matt: mask done
      // this won't work ehen the user changes display mode during run or
      // has two screens with differnet depths
      static uchar hiNibble[16] =
      { 0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
        0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0 };
      static uchar loNibble[16] =
      { 0x00, 0x08, 0x04, 0x0c, 0x02, 0x0a, 0x06, 0x0e,
        0x01, 0x09, 0x05, 0x0d, 0x03, 0x0b, 0x07, 0x0f };
      int np  = GetDeviceCaps(fl_gc, PLANES);   //: was always one on sample machines
      int bpp = GetDeviceCaps(fl_gc, BITSPIXEL);//: 1,4,8,16,24,32 and more odd stuff?
      int Bpr = (bpp*w+7)/8;                    //: bytes per row
      int pad = Bpr&1, w1 = (w+7)/8, shr = ((w-1)&7)+1;
      if (bpp==4) shr = (shr+1)/2;
      uchar *newarray = new uchar[(Bpr+pad)*h], *dst = newarray, *src = bitmap;
      for (int i=0; i<h; i++) {
        //: this is slooow, but we do it only once per pixmap
        for (int j=w1; j>0; j--) {
          uchar b = *src++;
          if (bpp==1) {
            *dst++ = ( hiNibble[b&15] ) | ( loNibble[(b>>4)&15] );
          } else if (bpp==4) {
            for (int k=(j==1)?shr:4; k>0; k--) {
              *dst++ = "\377\360\017\000"[b&3];
              b = b >> 2;
            }
          } else {
            for (int k=(j==1)?shr:8; k>0; k--) {
              if (b&1) {
                *dst++=0;
                if (bpp>8) *dst++=0;
                if (bpp>16) *dst++=0;
                if (bpp>24) *dst++=0;
              } else {
                *dst++=0xff;
                if (bpp>8) *dst++=0xff;
                if (bpp>16) *dst++=0xff;
                if (bpp>24) *dst++=0xff;
              }
              b = b >> 1;
            }
          }
        }
        dst += pad;
      }
      mask = (ulong)CreateBitmap(w, h, np, bpp, newarray);
      delete[] newarray;
#else
      mask = XCreateBitmapFromData(fl_display, fl_window,
                                (const char*)bitmap, (w+7)&-8, h);
#endif
      free(bitmap);
      bitmap = NULL;
    }
    free(data);
    data = NULL;
    fl_end_offscreen();
  }
#ifdef WIN32
  if (mask) {
    HDC new_gc = CreateCompatibleDC(fl_gc);
    SelectObject(new_gc, (void*)mask);
    BitBlt(fl_gc, X, Y, W, H, new_gc, cx, cy, SRCAND);
    SelectObject(new_gc, (void*)id);
    BitBlt(fl_gc, X, Y, W, H, new_gc, cx, cy, SRCPAINT);
    DeleteDC(new_gc);
  } else {
    fl_copy_offscreen(X, Y, W, H, (Fl_Offscreen)id, cx, cy);
  }
#else
  if (mask) {
    // I can't figure out how to combine a mask with existing region,
    // so cut the image down to a clipped rectangle:
    int nx, ny; fl_clip_box(X,Y,W,H,nx,ny,W,H);
    cx += nx-X; X = nx;
    cy += ny-Y; Y = ny;
    // make X use the bitmap as a mask:
    XSetClipMask(fl_display, fl_gc, mask);
    int ox = X-cx; if (ox < 0) ox += w;
    int oy = Y-cy; if (oy < 0) oy += h;
    XSetClipOrigin(fl_display, fl_gc, X-cx, Y-cy);
  }
  fl_copy_offscreen(X, Y, W, H, (Fl_Offscreen)id, cx, cy);
  if (mask) {
    // put the old clip region back
    XSetClipOrigin(fl_display, fl_gc, 0, 0);
    fl_restore_clip();
  }
#endif
}

Xd6Gif::Xd6Gif() {
	w = h = d = 0;
	data = NULL;
	bitmap = NULL;
	mask = 0;
	id = 0;
	file = NULL;
}

Xd6Gif::~Xd6Gif() {
  if (id) fl_delete_offscreen((Fl_Offscreen)id);
  if (mask) fl_delete_offscreen((Fl_Offscreen)mask);
  if (data) free(data);
  if (bitmap) free(bitmap);
  if (file) free(file);
}

int Xd6Gif::load(const char *name)
{
	int e;
	FILE *fp;
	fp = fopen(name, "r");
	if (!fp) return -1;
	free(file);
	file = strdup(name);
	e = load_gif(this, fp);
	fclose(fp);
	return e;
}

int Xd6Gif::save(const char *name)
{
	int e;
	FILE *fp;
	fp = fopen(name, "w");
	if (!fp) return -1;
	if (!data) load(file);
	e = save_gif(this, fp);
	fclose(fp);
	return e;
}

void Xd6Gif::set_size(int width, int height)
{
  Xd6Gif *img = this;

  if (!img->data || !img->w || !img->h) return;

  if (width == 0 && height == 0)
  {
    // Use image size...
    width  = img->w;
    height = img->h;
  }
  else if (width == 0)
    // Scale width to height
    width = img->w * height / img->h;
  else if (height == 0)
    // Scale height to width
    height = img->h * width / img->w;

  // Scale the image as needed...
  if (width != img->w && height != img->h)
  {
    unsigned char       *scaled,        // Scaled image data
                        *sptr,          // Source image data pointer
                        *dptr;          // Destination image data pointer
    int                 sy,             // Source coordinates
                        dx, dy,         // Destination coordinates
                        xerr, yerr,     // X & Y errors
                        xmod, ymod,     // X & Y moduli
                        xstep, ystep;   // X & Y step increments


     xmod   = img->w % width;
     xstep  = (img->w / width) * img->d;
     ymod   = img->h % height;
     ystep  = img->h / height;

     if ((scaled = (unsigned char *)malloc(width * height * img->d)) != NULL)
     {
       // Scale the image...
       for (dy = height, sy = 0, yerr = height / 2, dptr = scaled; dy > 0; dy --)
       {
         for (dx = width, xerr = width / 2,
                  sptr = img->data + sy * img->w * img->d;
              dx > 0;
              dx --)
         {
           *dptr++ = sptr[0];
           if (img->d > 1)
           {
             *dptr++ = sptr[1];
             *dptr++ = sptr[2];
           }

           sptr += xstep;
           xerr -= xmod;
           if (xerr <= 0)
           {
             xerr += width;
             sptr += img->d;
           }
         }

         sy   += ystep;
         yerr -= ymod;
         if (yerr <= 0)
         {
           yerr += height;
           sy ++;
         }
       }

       free(img->data);

       img->w    = width;
       img->h    = height;
       img->data = scaled;
     }
  }

}


static int	gif_eof = 0;		// Did we hit EOF?

//
// Local functions...
//

//
// 'load_gif()' - Load a GIF image file...
//

int					// O - 0 = success, -1 = fail
load_gif(Xd6Gif *img,		// I - Image pointer
        	   FILE      *fp)	// I - File to load from
{
  unsigned char	buf[1024];		// Input buffer
  gif_cmap_t	cmap;			// Colormap
  int		ncolors,		// Bits per pixel
		transparent;		// Transparent color index


  // Read the header
  fread(buf, 1, 13, fp);
  if (!(memcmp(buf, "GIF87a", 6) == 0 ||
	memcmp(buf, "GIF89a", 6) == 0))
  {
	return -1;
  }

  img->w  = (buf[7] << 8) | buf[6];
  img->h  = (buf[9] << 8) | buf[8];
  ncolors = 2 << (buf[10] & 0x07);

  if (buf[10] & GIF_COLORMAP)
    if (!gif_read_cmap(fp, ncolors, cmap))
      return (0);

  transparent = -1;

  while (1)
  {
    switch (getc(fp))
    {
      case ';' :	// End of image
          return (0);	// Early end of file

      case '!' :	// Extension record
          buf[0] = getc(fp);
          if (buf[0] == 0xf9)	// Graphic Control Extension
          {
            gif_get_block(fp, buf);
            if (buf[0] & 1)	// Get transparent color index
              transparent = buf[3];
          }

          while (gif_get_block(fp, buf) != 0);
          break;

      case ',' :	// Image data
          fread(buf, 9, 1, fp);

          if (buf[8] & GIF_COLORMAP)
          {
            ncolors = 2 << (buf[8] & 0x07);

	    if (!gif_read_cmap(fp, ncolors, cmap))
	      return (0);
	  }

          img->w    = (buf[5] << 8) | buf[4];
          img->h    = (buf[7] << 8) | buf[6];
          img->d    = 3;

          img->data = (unsigned char *)malloc(img->w * img->h * img->d);
	  // FIXME
	  transparent = -1;
	  if (transparent >= 0 && !(buf[8] & GIF_INTERLACE)) {
	  	img->bitmap = (unsigned char *)
			malloc(((img->w / 8) + 1) * img->h);
	  } else {
		img->bitmap = NULL;
	  }
          if (img->data == NULL)
            return (0);
	  return (gif_read_image(fp, img, cmap, buf[8] & GIF_INTERLACE, transparent));
    }
  }
}



//
// 'gif_read_cmap()' - Read the colormap from a GIF file...
//

static int				// O - -1 = error, 0 = success
gif_read_cmap(FILE       *fp,		// I - File to read from
  	      int        ncolors,	// I - Number of colors
	      gif_cmap_t cmap)		// O - Colormap
{
  // Read the colormap...
  if (fread(cmap, 3, ncolors, fp) < (size_t)ncolors)
    return (0);

  return (1);
}


//
// 'gif_get_block()' - Read a GIF data block...
//

static int				// O - Number characters read
gif_get_block(FILE  *fp,		// I - File to read from
	      unsigned char *buf)	// I - Input buffer
{
  int	count;				// Number of character to read


  // Read the count byte followed by the data from the file...
  if ((count = getc(fp)) == EOF)
  {
    gif_eof = 1;
    return (-1);
  }
  else if (count == 0)
    gif_eof = 1;
  else if (fread(buf, 1, count, fp) < (size_t)count)
  {
    gif_eof = 1;
    return (-1);
  }
  else
    gif_eof = 0;

  return (count);
}


//
// 'gif_get_code()' - Get a LZW code from the file...
//

static int				// O - LZW code
gif_get_code(FILE *fp,			// I - File to read from
	     int  code_size,		// I - Size of code in bits
	     int  first_time)		// I - 1 = first time, 0 = not first time
{
  unsigned		i, j,		// Looping vars
			ret;		// Return value
  int			count;		// Number of bytes read
  static unsigned char	buf[280];	// Input buffer
  static unsigned	curbit,		// Current bit
			lastbit,	// Last bit in buffer
			done,		// Done with this buffer?
			last_byte;	// Last byte in buffer
  static unsigned	bits[8] =	// Bit masks for codes
			{
			  0x01, 0x02, 0x04, 0x08,
			  0x10, 0x20, 0x40, 0x80
			};


  if (first_time)
  {
    // Just initialize the input buffer...
    curbit  = 0;
    lastbit = 0;
    done    = 0;

    return (0);
  }


  if ((curbit + code_size) >= lastbit)
  {
    // Don't have enough bits to hold the code...
    if (done)
      return (-1);	// Sorry, no more...

    // Move last two bytes to front of buffer...
    if (last_byte > 1)
    {
      buf[0]    = buf[last_byte - 2];
      buf[1]    = buf[last_byte - 1];
      last_byte = 2;
    }
    else if (last_byte == 1)
    {
      buf[0]    = buf[last_byte - 1];
      last_byte = 1;
    }

    // Read in another buffer...
    if ((count = gif_get_block (fp, buf + last_byte)) <= 0)
    {
      // Whoops, no more data!
      done = 1;
      return (-1);
    }

    // Update buffer state...
    curbit    = (curbit - lastbit) + 8 * last_byte;
    last_byte += count;
    lastbit   = last_byte * 8;
  }

  ret = 0;
  for (ret = 0, i = curbit + code_size - 1, j = code_size;
       j > 0;
       i --, j --)
    ret = (ret << 1) | ((buf[i / 8] & bits[i & 7]) != 0);

  curbit += code_size;

  return ret;
}


//
// 'gif_read_lzw()' - Read a byte from the LZW stream...
//

static int				// I - Byte from stream
gif_read_lzw(FILE *fp,			// I - File to read from
	     int  first_time,		// I - 1 = first time, 0 = not first time
 	     int  input_code_size)	// I - Code size in bits
{
  int		i,			// Looping var
		code,			// Current code
		incode;			// Input code
  static short	fresh = 0,		// 1 = empty buffers
		code_size,		// Current code size
		set_code_size,		// Initial code size set
		max_code,		// Maximum code used
		max_code_size,		// Maximum code size
		firstcode,		// First code read
		oldcode,		// Last code read
		clear_code,		// Clear code for LZW input
		end_code,		// End code for LZW input
		table[2][4096],		// String table
		stack[8192],		// Output stack
		*sp;			// Current stack pointer


  if (first_time)
  {
    // Setup LZW state...
    set_code_size = input_code_size;
    code_size     = set_code_size + 1;
    clear_code    = 1 << set_code_size;
    end_code      = clear_code + 1;
    max_code_size = 2 * clear_code;
    max_code      = clear_code + 2;

    // Initialize input buffers...
    gif_get_code(fp, 0, 1);

    // Wipe the decompressor table...
    fresh = 1;

    for (i = 0; i < clear_code; i ++)
    {
      table[0][i] = 0;
      table[1][i] = i;
    }

    for (; i < 4096; i ++)
      table[0][i] = table[1][0] = 0;

    sp = stack;

    return (0);
  }
  else if (fresh)
  {
    fresh = 0;

    do
      firstcode = oldcode = gif_get_code(fp, code_size, 0);
    while (firstcode == clear_code);

    return (firstcode);
  }

  if (sp > stack)
    return (*--sp);

  while ((code = gif_get_code (fp, code_size, 0)) >= 0)
  {
    if (code == clear_code)
    {
      for (i = 0; i < clear_code; i ++)
      {
	table[0][i] = 0;
	table[1][i] = i;
      }

      for (; i < 4096; i ++)
	table[0][i] = table[1][i] = 0;

      code_size     = set_code_size + 1;
      max_code_size = 2 * clear_code;
      max_code      = clear_code + 2;

      sp = stack;

      firstcode = oldcode = gif_get_code(fp, code_size, 0);

      return (firstcode);
    }
    else if (code == end_code)
    {
      unsigned char	buf[260];


      if (!gif_eof)
        while (gif_get_block(fp, buf) > 0);

      return (-2);
    }

    incode = code;

    if (code >= max_code)
    {
      *sp++ = firstcode;
      code  = oldcode;
    }

    while (code >= clear_code)
    {
      *sp++ = table[1][code];
      if (code == table[0][code])
	return (255);

      code = table[0][code];
    }

    *sp++ = firstcode = table[1][code];
    code  = max_code;

    if (code < 4096)
    {
      table[0][code] = oldcode;
      table[1][code] = firstcode;
      max_code ++;

      if (max_code >= max_code_size && max_code_size < 4096)
      {
	max_code_size *= 2;
	code_size ++;
      }
    }

    oldcode = incode;

    if (sp > stack)
      return (*--sp);
  }

  return (code);
}


//
// 'gif_read_image()' - Read a GIF image stream...
//

static int				// I - 0 = success, -1 = failure
gif_read_image(FILE       *fp,		// I - Input file
	       Xd6Gif  *img,		// I - Image pointer
	       gif_cmap_t cmap,		// I - Colormap
	       int      interlace,	// I - Non-zero = interlaced image
	       int transparent)	
{
  unsigned char	code_size,		// Code size
		*temp;			// Current pixel
  int		xpos,			// Current X position
		ypos,			// Current Y position
		pass;			// Current pass
  int		pixel;			// Current pixel
  static int	xpasses[4] = { 8, 8, 4, 2 },
		ypasses[5] = { 0, 4, 2, 1, 999999 };


  xpos      = 0;
  ypos      = 0;
  pass      = 0;
  code_size = getc(fp);

  if (gif_read_lzw(fp, 1, code_size) < 0)
    return (0);

  temp = img->data;

  while ((pixel = gif_read_lzw(fp, 0, code_size)) >= 0)
  {
    temp[0] = cmap[pixel][0];

    if (img->bitmap) {
	if (pixel == transparent) {
		img->bitmap[(xpos / 8) + (ypos * ((img->w + 7) / 8))] &= 
			0x0FE << (xpos % 8);
	} else {
		img->bitmap[(xpos / 8) + (ypos * ((img->w + 7) / 8))] |= 
			0x01 << (xpos % 8);
	}
	
    }
    if (img->d > 1)
    {
      temp[1] = cmap[pixel][1];
      temp[2] = cmap[pixel][2];
    }

    xpos ++;
    temp += img->d;
    if (xpos == img->w)
    {
      xpos = 0;

      if (interlace)
      {
        ypos += xpasses[pass];
        temp += (xpasses[pass] - 1) * img->w * img->d;

        if (ypos >= img->h)
	{
	  pass ++;

          ypos = ypasses[pass];
          temp = img->data + ypos * img->w * img->d;
	}
      }
      else
	ypos ++;
    }

    if (ypos >= img->h)
      break;
  }

  return (1);
}


void buffered_output(Xd6Gif *img, FILE *fp, int c)
{
	if (c == FLUSH_OUTPUT) {
		if (img->Buf[0] != 0) {
			fwrite(img->Buf, img->Buf[0] + 1, 1, fp);
		}
		img->Buf[0] = 0;
		fwrite("\0", 1, 1, fp);
	} else {
		if (img->Buf[0] == 255) {
			fwrite(img->Buf, img->Buf[0] + 1, 1, fp);
			img->Buf[0] = 0;
		}
		img->Buf[++(img->Buf[0])] = c;
	}
}

void compress_output(Xd6Gif *img, FILE *fp, int Code)
{
	if (Code == FLUSH_OUTPUT) {
		while (img->CrntShiftState > 0) {
			buffered_output(img, fp, img->CrntShiftDWord & 0xff);
			img->CrntShiftDWord >>= 8;
			img->CrntShiftState -= 8;	
		}
		img->CrntShiftState = 0;
		buffered_output(img, fp, FLUSH_OUTPUT);
	} else {
		img->CrntShiftDWord |= ((long) Code) << img->CrntShiftState;
		img->CrntShiftState += img->RunningBits;
		while (img->CrntShiftState >= 8) {
			buffered_output(img, fp, img->CrntShiftDWord & 0xff);
			img->CrntShiftDWord >>= 8;
			img->CrntShiftState -= 8;
		}
	}
}

int					// O - 0 = success, -1 = fail
save_gif(Xd6Gif *img,			// I - Image pointer
        	   FILE      *fp)	// I - File to save to
{
	int CrntCode = 0;
	unsigned long pixcnt;
	char *comment = "\nThis is an UNCOMPRESSED Gif picture!\n";

	if (!img->data) return -1;

	img->ColorMap = 1;
	img->ColorRes = 3;
	img->BitsPerPixel = 8;
	img->BackGround = 0;
	img->Transparent = 7;

	fprintf(fp, "GIF89a");
	fprintf(fp, "%c%c", img->w & 0xff, (img->w >> 8) & 0xff);
	fprintf(fp, "%c%c", img->h & 0xff, (img->h >> 8) & 0xff);
	fprintf(fp, "%c", (img->ColorMap ? 0x80 : 0x00) | 
		((img->ColorRes - 1) << 4) |
		(img->BitsPerPixel - 1));
	fprintf(fp, "%c", img->BackGround);
	fwrite("\0", 1, 1, fp);

	if (img->ColorMap) {
		unsigned c;
		int i;
		int nbc = 2 << (img->BitsPerPixel - 1);
		for (i = 0; i < nbc; i++) {
			c = Fl::get_color((Fl_Color)i);
			fprintf (fp, "%c%c%c", (c & 0xff000000) >> 24,
                                (c & 0xff0000) >> 16, (c & 0xff00) >> 8);
		}	
	}
	
	fprintf(fp, "!"); // Extension record
	fprintf(fp, "\xf9"); // Graphic Control Extension
	fwrite("\x04\x01\x00\x00", 4, 1, fp);
	fprintf(fp,"%c", img->Transparent); 
	fwrite("\0", 1, 1, fp);
	
	fprintf(fp, "!"); // Extension record
	fprintf(fp, "\xfe"); // Comment extension
	fprintf(fp,"%c%s", strlen(comment), comment);
	fwrite("\0", 1, 1, fp);


	fprintf(fp, ","); // Image data
	fwrite("\0\0", 2, 1, fp); // Left
	fwrite("\0\0", 2, 1, fp); // Top
	fprintf(fp, "%c%c", img->w & 0xff, (img->w >> 8) & 0xff);
	fprintf(fp, "%c%c", img->h & 0xff, (img->h >> 8) & 0xff);
	fwrite("\0", 1, 1, fp); // ColorMap Interlace BitsPerPixel
	fprintf(fp, "%c", img->BitsPerPixel);

	pixcnt = img->PixelCount = (long) img->w * (long) img->h;
	img->Buf[0] = 0;
	img->ClearCode = (1 << img->BitsPerPixel);
	img->EOFCode = img->ClearCode + 1;
	img->RunningCode = 0;
	img->RunningBits = img->BitsPerPixel + 1;
	img->MaxCode1 = 1 << img->RunningBits;
	img->CrntCode = FIRST_CODE;
	img->CrntShiftState = 0;
	img->CrntShiftDWord = 0;

	/* Send Clear to make sure the encoder is initialized. */
	compress_output(img, fp, img->ClearCode);
	while (img->PixelCount > 0) {
		int pix;
		int c;
		pix = (int)(pixcnt - img->PixelCount) * 3;
		c = fl_color_cube(img->data[pix] * FL_NUM_RED / 256, 
			img->data[pix + 1] * FL_NUM_GREEN / 256, 
			img->data[pix + 2] * FL_NUM_BLUE / 256);
		if (img->CrntCode == FIRST_CODE) {
			CrntCode = c & 0xff;	
		} else {
			CrntCode = img->CrntCode;
			compress_output(img, fp, CrntCode);
		}
		img->RunningCode++;
		CrntCode = c & 0xff;
		if (img->RunningCode >= (1 << (img->BitsPerPixel)) - 2) {
			compress_output(img, fp, img->ClearCode);
			img->RunningCode = 0;
		}
		img->PixelCount--;
		img->CrntCode = CrntCode;
	}	

	compress_output(img, fp, CrntCode);
	compress_output(img, fp, img->EOFCode);
	compress_output(img, fp, FLUSH_OUTPUT);
	fprintf(fp, ";"); // End of image
	return 0;
}

