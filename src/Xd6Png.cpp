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



#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu_Item.H>
#include <config.h>

#include "Xd6Png.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern "C"
{
#if defined(HAVE_LIBPNG) && defined(HAVE_LIBZ)
#  include <zlib.h>
#  ifdef HAVE_PNG_H
#    include <png.h>
#  else
#    include <libpng/png.h>
#  endif // HAVE_PNG_H
#endif // HAVE_LIBPNG ** HAVE_LIBZ
}



void Xd6Png::draw(int XP, int YP, int WP, int HP, int cx, int cy) {
	if (!png || !png->data()) return;
	((Fl_Image*)png)->draw(XP, YP, WP, HP, cx, cy);
}

Xd6Png::Xd6Png() 
{
	file = NULL;
	bit = 0;
	png = NULL;
	data = NULL;
	id = 0;
}

Xd6Png::~Xd6Png() {
  if (png) delete((Fl_PNG_Image*)png);
  if (file) free(file);
}

int Xd6Png::load(const char *name)
{
        if (file) free(file);
	file = strdup(name ? name : "");
        if (png) delete((Fl_PNG_Image*)png);
	png = new Fl_PNG_Image(file);
	data = (unsigned char*)png->data();
	w = png->w();
	h = png->h();
	d = png->d();
	return 1;
}

const char *Xd6Png::mime()
{
	return "image/png";
}

int Xd6Png::save(const char *name)
{
   	FILE *fp;
   	png_structp png_ptr;
   	png_infop info_ptr;

	if (!png) return 0;
	if (bit) return 0;
	fp = fopen(name, "wb");
	if (fp == NULL) return 0;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
			NULL, NULL, NULL);
	info_ptr = png_create_info_struct(png_ptr);
	setjmp(png_jmpbuf(png_ptr));
	png_init_io(png_ptr, fp);

	if (png->d() == 2 ) {
		png_set_IHDR(png_ptr, info_ptr, png->w(), png->h(), 8,
			PNG_COLOR_TYPE_GRAY_ALPHA, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	} else if (png->d() == 1) {
		png_set_IHDR(png_ptr, info_ptr, png->w(), png->h(), 8,
			PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	} else if (png->d() == 4) {
		png_set_IHDR(png_ptr, info_ptr, png->w(), png->h(), 8,
			PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	} else {
		png_set_IHDR(png_ptr, info_ptr, png->w(), png->h(), 8,
			PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	}

	png_write_info(png_ptr, info_ptr);
	
	png_bytep *row_pointers = new png_bytep[png->h()];
	for (int k = 0; k < png->h(); k++) {
		row_pointers[k] = (png_bytep) (((Fl_RGB_Image*)png)->array + 
					k * png->w() * png->d());
	}
	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	delete[] row_pointers;
	fclose(fp);
	return 1;
}

void Xd6Png::set_size(int width, int height)
{
	if (!png) return;
	Fl_Image *npng = png->copy(width, height);
	delete(png);
	png = npng;
	data = (unsigned char*)png->data();
	w = png->w();
	h = png->h();
	d = png->d();
}


