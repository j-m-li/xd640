/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000,2001  O'ksi'D
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 *   Author : Jean-Marc Lienher ( http://oksid.ch )
 *
 ******************************************************************************/
#ifndef Xd6HtmlTagImg_h
#define Xd6HtmlTagImg_h

#include "Xd6XmlStyle.h"
#include "Xd6XmlTree.h"
#include "Xd6Gif.h"
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
	Xd6Gif *gif;
	int attr_w;
	int attr_h;
	int attr_border;
	char *source;

	Xd6HtmlTagImg(int i, Xd6XmlTreeElement *e, Xd6HtmlDisplay *u);
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
