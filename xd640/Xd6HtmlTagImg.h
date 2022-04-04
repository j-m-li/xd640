/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2001  O'ksi'D
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


#ifndef Xd6HtmlTagImg_h
#define Xd6HtmlTagImg_h

#include "Xd6XmlStyle.h"
#include "Xd6XmlTree.h"
#include "Xd6Png.h"
#include "Xd6HtmlPrint.h"
#include "Xd6HtmlFrame.h"

#define META_SAVE_DC 0x001e
#define META_SET_MAP_MODE 0x0103
#ifndef WIN32
#define META_SETWINDOWORG 0x020b
#define META_SETWINDOWEXT 0x020c
#define META_DIBSTRETCHBLT 0x0b41
#define META_RESTOREDC 0x0127

typedef struct tagMETAHEADER {
	short 	mtType;
	short	mtHeaderSize;
	short	mtVersion;
	int	mtSize;
	short	mtNoObjects;
	int	mtMaxRecord;
	short	mtNoParameters;
} METAHEADER;

typedef struct tagMETARECORD {
	int	rdSize;
	short	rdFunction;
	short	*rdParam;
} METARECORD;

typedef struct tagBITMAPINFOHEADER {
	int	biSize;
	int 	biWidth;
	int 	biHeight;
	short	biPlanes;
	short	biBitCount;
	int 	biCompression;
	int	biSizeImage;
	int	biXPelsPerMeter;
	int	biYPelsPerMeter;
	int	biClrUser;
	int	biClrImportant;
} BITMAPINFOHEADER;

#endif

class Xd6HtmlTagImg : public Xd6HtmlDisplay {
public:
	Xd6Png *gif;
	int attr_w;
	int attr_h;
	int attr_border;
	char *source;

	Xd6HtmlTagImg(int i, Xd6XmlTreeElement *e, Xd6HtmlFrame *u);
	~Xd6HtmlTagImg();

	void measure(void);
	void draw(int X, int Y);
	void destroy(void);
	void print(Xd6HtmlPrint *p, int X, int Y);
	void to_html(FILE *fp);
	void to_rtf(FILE *fp);
	void load(const char *url, const char *file);
};

#endif

/*
 * "$Id: $"
 */
