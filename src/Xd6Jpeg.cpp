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

#include "Xd6Jpeg.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


Xd6Jpeg::Xd6Jpeg() : Xd6Png() 
{
}

Xd6Jpeg::~Xd6Jpeg() {
}

const char *Xd6Jpeg::mime()
{
        return "image/jpeg";
}

int Xd6Jpeg::load(const char *name)
{
	if (file) free(file);
        file = strdup(name ? name : "");
	printf("load jpeg %s\n", file);
	png = new Fl_JPEG_Image(file);
	data = (unsigned char*)png->data();
	w = png->w();
	h = png->h();
	d = png->d();
	return 1;
}

void Xd6Jpeg::draw(int XP, int YP, int WP, int HP, int cx, int cy) {
        if (!png || !png->data()) return;
        ((Fl_JPEG_Image*)png)->draw(XP, YP, WP, HP, cx, cy);
}

