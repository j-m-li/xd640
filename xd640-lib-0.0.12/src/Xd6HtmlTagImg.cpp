/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2001  O'ksi'D
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

#include "Xd6Std.h"
#include <FL/fl_draw.h>
#include "Xd6HtmlTagImg.h"
#include <stdio.h>
#include <stdlib.h>
#include "Xd6Base64.h"

Xd6HtmlTagImg::Xd6HtmlTagImg(int i, Xd6XmlTreeElement *e, Xd6HtmlDisplay *u) : 
	Xd6HtmlDisplay(i, NULL , 0, e ? e->style : 0)
{
	const char *ptr;

	parent = u;
	text = (char *)malloc(1);
	gif = new Xd6Gif();
	source = NULL;
	attr_w = attr_h = attr_border = 0;
	if (e) {
		ptr = e->get_attr_value("width");
		if (ptr) attr_w = (int) atol(ptr);
		ptr = e->get_attr_value("height");
		if (ptr) attr_h = (int) atol(ptr);
		ptr = e->get_attr_value("border");
		if (ptr) attr_border = (int) atol(ptr);
		while (u->display != DISPLAY_FRAME &&
			u->display != DISPLAY_IFRAME &&
			u->display != DISPLAY_TOP_FRAME)
		{
			u = u->parent;
		}
		
		ptr = downloader->request(e->get_attr_value("src"),
			NULL, NULL, (Xd6HtmlFrame*)u, this);
		if (ptr) source = strdup(ptr);
	}
}

Xd6HtmlTagImg::~Xd6HtmlTagImg()
{
	destroy();
}

void Xd6HtmlTagImg::destroy()
{
	free(source);
	delete(gif);
}

void Xd6HtmlTagImg::load(const char *url, const char *file)
{
	gif->load(url);
	//gif->load(file);
}

void Xd6HtmlTagImg::measure() 
{
	width = height = 0;
	if (!gif->data && !gif->id) gif->load(source);

	if (attr_w && attr_h) {
		width = attr_w + 2 * attr_border;
		height = attr_h + 2 * attr_border;
		gif->set_size(attr_w, attr_h);
	} else {
		width = gif->w + 2 * attr_border;
		height = gif->h + 2 * attr_border;
		attr_w = width;
		attr_h = height;
	}
	descent = fl_descent(); 
}

void Xd6HtmlTagImg::draw(int X, int Y)
{
	int i = attr_border;
	X += left;
	Y += top;
	
	fl_color(FL_BLUE);
	for (i--;i > -1; i--) {
		fl_rect(X + i, Y + i, width - 2 * i, height - 2 * i);
	}
	gif->draw(X + attr_border,  Y + attr_border);
}

static void to_hex(FILE *fp, char h) 
{
	char *hex = "0123456789abcdef";
	fprintf(fp, "%c", hex[h]);
}

#ifdef WIN32
void Xd6HtmlTagImg::print(Xd6HtmlPrint *p, int X, int Y)
{
}
#else
void Xd6HtmlTagImg::print(Xd6HtmlPrint *p, int X, int Y)
{
	int i, w = 0, h = 0, d = 3;
	unsigned char *data = NULL;
	if (gif) {
		gif->load(gif->file);
		data = gif->data;
		w = gif->w;
		h = gif->h;
		d = gif->d;
	}
	if (!data) return;

	Y -= p->frm->page_height * p->page_nb;
	X += left + attr_border;
	Y += top + attr_border + height - descent;
	
	
	fprintf(p->tfp, "s %d %d t\n", p->frm->page_margin_left + X,
		p->frm->page_height + p->frm->page_margin_bottom - Y);
	fprintf(p->tfp, "%d %d scale\n", attr_w, attr_h);
	fprintf(p->tfp, "/picstr %d string def\n",  w * d);
	fprintf(p->tfp, "%d %d 8 [%d 0 0 -%d 0 %d]\n", w, h, w, h, h);
	fprintf(p->tfp, "{ currentfile picstr readhexstring pop }\n");
	fprintf(p->tfp, "false 3 colorimage ");
	for (i = 0; i < w * h * d; i++) {
		if ((i % 36) == 0) fprintf(p->tfp, "\n");
		to_hex(p->tfp, (data[i] & 0xF0) >> 4);
		to_hex(p->tfp, (data[i] & 0x0F));
	}
	fprintf(p->tfp, "\nx\n");
	
}
#endif

void Xd6HtmlTagImg::to_html(FILE *fp)
{
	int l;
	char buf[1024];
	char name[1024];
	char out[1024];
	FILE *fi, *fo;

	if (!gif) return;
	sprintf(name, "/tmp/xhtml_%p.gif", this);
	sprintf(out, "/tmp/xhtml_%p.gif.base64", this);
	fprintf(fp, "<img src=\"data:image/gif;base64,\n");
	gif->save(name);
	fi = fopen(name, "r");
	fo = fopen(out, "w");
	Xd6Encode_base64(fi, fo);
	fclose(fi);
	fclose(fo);
	fi = fopen(out, "r");
	l = fread(buf, 1, 1024, fi);
	while (l > 0) {
		fwrite(buf, l, 1, fp);
		l = fread(buf, 1, 1024, fi);
	}
	fclose(fi);
	fprintf(fp, "\" border=\"%d\" width=\"%d\" height=\"%d\" \n/>",
		attr_border, attr_w, attr_h);
	unlink(name);
	unlink(out);
}

static char to_hex(int i)
{
	char *he = "0123456789abcdef";
	return he[i & 0x0F];
}

static void print_short(FILE *fp, int i) 
{
	fprintf(fp, "%c%c%c%c", to_hex(i >> 4), to_hex(i), 
		to_hex(i >> 12), to_hex(i >> 8));
}

static void print_int(FILE *fp, int i) 
{
	fprintf(fp, "%c%c%c%c%c%c%c%c", to_hex(i >> 4), to_hex(i), 
		to_hex(i >> 12), to_hex(i >> 8), 
		to_hex(i >> 20), to_hex(i >> 16), 
		to_hex(i >> 28), to_hex(i >> 24));
}

void Xd6HtmlTagImg::to_rtf(FILE *fp)
{
	int mtSize = 9 + 3 + 4 + 3 + 3 + 5 + 5;
	int blt_size = 0; 
	int size;
	int rest;
	int i = 0;
	int x, y, rw;
	if (!gif) return;
	
	gif->load(gif->file);
	if (!gif->data) return;
	gif->set_size(attr_w, attr_h);
	fprintf(fp, "{\\pict\n\\wmetafile8\\picw%d\\pich%d"
		"\\picwgoal%d\\pichgoal%d\n", 
		attr_w * 20, attr_h * 20, attr_w * 20, attr_h * 20);
	fprintf(fp, "010009000003");
	size = attr_w * attr_h * 3;
	rest = size % 2;
	blt_size = size / 2 + 13 + 20 + rest;
	mtSize += blt_size;
	print_int(fp, mtSize);
	print_short(fp, 0);
	print_int(fp, blt_size);
	print_short(fp, 0);
	fprintf(fp, "\n");

	// end of meta header
	print_int(fp, 0x3);
	print_short(fp, META_SAVE_DC);	

	print_int(fp, 0x4);
	print_short(fp, META_SET_MAP_MODE);
	print_short(fp, 0x0008);

	print_int(fp, 0x5);
	print_short(fp, META_SETWINDOWORG);
	print_int(fp, 0);

	print_int(fp, 0x5);
	print_short(fp, META_SETWINDOWEXT);
	print_short(fp, attr_h);
	print_short(fp, attr_w);
		

	fprintf(fp, "\n");
	print_int(fp, blt_size);
	print_short(fp, META_DIBSTRETCHBLT);
	print_int(fp, 0x00CC0020); // rop
	print_short(fp, attr_h); // ySrc
	print_short(fp, attr_w); // xSrc
	print_short(fp, 0); // Src height
	print_short(fp, 0); // Src width
	print_short(fp, attr_h); // dest height
	print_short(fp, attr_w); // dest width
	print_short(fp, 0); // yDest
	print_short(fp, 0); // xDest
	fprintf(fp, "\n");

	// start of dib
	print_int(fp, 0x28);
	print_int(fp, attr_w);
	print_int(fp, attr_h);
	print_short(fp, 0x1);
	print_short(fp, 0x18);
	print_int(fp, 0);
	print_int(fp, 0);
	print_int(fp, 0x0af0);
	print_int(fp, 0x0af0);
	print_int(fp, 0);
	print_int(fp, 0);
	fprintf(fp, "\n");

	// start of rgb data
	y = attr_h;
	i = 0;
	rw = attr_w * 3;
	while (y > 0) {
		y--;
		int o = y * rw;
		x = 0;
		while (x < rw) {
			int d = gif->data[o + x + 2];
			fprintf(fp, "%c%c", to_hex(d >> 4), to_hex(d));
			if (i != 0 && !(i % 40)) fprintf(fp, "\n"); i++;
			d = gif->data[o + x + 1];
			fprintf(fp, "%c%c", to_hex(d >> 4), to_hex(d));
			if (i != 0 && !(i % 40)) fprintf(fp, "\n"); i++;
			d = gif->data[o + x];
			fprintf(fp, "%c%c", to_hex(d >> 4), to_hex(d));
			if (i != 0 && !(i % 40)) fprintf(fp, "\n"); i++;
			x += 3;
		}
	}
	// end of META_DIBSTRETCHBLT

	if (rest) {
		fprintf(fp, "00");
	}
	fprintf(fp, "\n");
	print_int(fp, 0x4);
	print_short(fp, META_RESTOREDC);
	print_short(fp, 0xffff);
	
	print_int(fp, 0x3);
	print_short(fp, 0x0);

	fprintf(fp, "\n}");
}

/*
 * "$Id: $"
 */
