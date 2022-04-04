/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2001  O'ksi'D
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


#include "Xd6Std.h"
#include <FL/fl_draw.h>
#include "Xd6HtmlTagImg.h"
#include "Xd6Png.h"
#include "Xd6Jpeg.h"
#include "Xd6Gif.h"
#include <stdio.h>
#include <stdlib.h>
#include "Xd6Base64.h"

Xd6HtmlTagImg::Xd6HtmlTagImg(int i, Xd6XmlTreeElement *e, Xd6HtmlFrame *u) : 
	Xd6HtmlDisplay(i, NULL , 0, e ? e->stl : NULL)
{
	const char *ptr;

	parent = u;
	text = (char *)malloc(2);
	len = 1;
	gif = new Xd6Png();
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
		
		downloader->request(e->get_attr_value("src"),
			NULL, NULL, (Xd6HtmlFrame*)u, this);
	}
}

Xd6HtmlTagImg::~Xd6HtmlTagImg()
{
	destroy();
}

void Xd6HtmlTagImg::destroy()
{
	if (source) fl_unlink(source);
	free(source);
	delete(gif);
}

void Xd6HtmlTagImg::load(const char *url, const char *file)
{
	char header[6];
	FILE *fp;

//	printf("url %s file %s\n", url, file);

	free(source);
	source = strdup(file);

	fp = fl_fopen(file, "r");
	if (!fp) {
		return;
	}
	int r = fread(header, 1, 6, fp);
	fclose(fp);
	if (r != 6) return;

	delete(gif);

	if (memcmp(header, "GIF87a", 6) == 0 ||
		memcmp(header, "GIF89a", 6) == 0)
	{
		gif = new Xd6Gif();
	} else if (memcmp(header, "\211PNG", 4) == 0) {
		gif = new Xd6Png();
	} else {
		gif = new Xd6Jpeg();
	}
	gif->load(file);

	damage(FL_DAMAGE_ALL);
}

void Xd6HtmlTagImg::measure() 
{
	width = height = 0;
	if (attr_w && attr_h) {
		width = attr_w + 2 * attr_border;
		height = attr_h + 2 * attr_border;
		gif->set_size(attr_w, attr_h);
	} else if (gif->data) {
		width = gif->w + 2 * attr_border;
		height = gif->h + 2 * attr_border;
		attr_w = width;
		attr_h = height;
	} else  {
		width = 32;
		height = 32;
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
	if (!gif->data) {
		fl_color(COLOR_RED);
		fl_rectf(X + attr_border,  Y + attr_border, 
			width - 2 * attr_border, height -  2 * attr_border);
	} else {
		gif->draw(X + attr_border,  Y + attr_border);
	}
}

static void to_hex(FILE *fp, char h) 
{
	char *hex = "0123456789abcdef";
	fprintf(fp, "%c", hex[h]);
}

#if 1 || defined(WIN32) //FIXME!
void Xd6HtmlTagImg::print(Xd6HtmlPrint *p, int X, int Y)
{
}
#else

class My_Ps : public Fl_Fltk
{
public:
	FILE *output;
	My_Ps(FILE *o) : Fl_Fltk() 
	{ 
		type = FL_PS_DEVICE;
		output = o;
	};

	void draw_image(const uchar *data, int x, int y, int iw, int ih, int D=3, int LD=0) 
	{
                const uchar *curdata=data;
                if(!LD) LD=iw*D;
                for (int j=0; j<ih;j++){
                        curdata=data+j*LD;
                        for(int i=0 ; i<iw ; i++) {
                                if (!(i%40))
                                        fprintf(output, "\n");;
				if (D == 4 && curdata[3] < 32) {
					fprintf(output, "FFFFFF");
				} else {
                                	fprintf(output, "%.2x%.2x%.2x", 
						curdata[0], curdata[1], 
						curdata[2]);
				}
                                curdata +=D;
                        }
                }
                fprintf(output, "\n");

	}
	void draw_image(Fl_Draw_Image_Cb call, void *data, int x, int y, int iw, int ih, int D)
	{
		int LD=iw*D;
                uchar *rgbdata=new uchar[LD];
                for (int j=0; j<ih;j++){
                        call(data,0,j,iw,rgbdata);
                        uchar *curdata=rgbdata;
                        for(int i=0 ; i<iw ; i++) {
                                if (!(i%40))
                                        fprintf(output, "\n");
                                fprintf(output, "%.2x%.2x%.2x", 
					curdata[0], curdata[1], curdata[2]);
                                curdata +=D;
                        }
                }
                fprintf(output, "\n");;
                delete[] rgbdata;

	}

};

void Xd6HtmlTagImg::print(Xd6HtmlPrint *p, int X, int Y)
{
	My_Ps *my_ps;
	FILE *none;
	Fl_Device *my_fl;

	int i, w = 0, h = 0, d = 3;
	const unsigned char *data = NULL;
	if (gif) {
		gif->load(source);
		w = gif->w;
		h = gif->h;
		d = gif->d;
		data = gif->data;
	}
	if (!data || (w * h * d) < 1) return;

	Y -= p->frm->page_height * p->page_nb;
	X += left + attr_border;
	Y += top + attr_border + height - descent;
	
	
	fprintf(p->tfp, "s %d %d t\n", p->frm->page_margin_left + X,
		p->frm->page_height + p->frm->page_margin_bottom - Y);
	fprintf(p->tfp, "%d %d scale\n", attr_w, attr_h);
	fprintf(p->tfp, "/picstr %d string def\n",  w * 3);
	fprintf(p->tfp, "%d %d 8 [%d 0 0 -%d 0 %d]\n", w, h, w, h, h);
	fprintf(p->tfp, "{ currentfile picstr readhexstring pop }\n");
	fprintf(p->tfp, "false 3 colorimage ");

	my_ps = new My_Ps(p->tfp);
	my_fl = fl;
	fl = my_ps;
	if (d == 1) {
		gif->png->draw(0,0, w, h, 0, 0);
	} else if (d > 2) {
		fl_draw_image((uchar*)((Fl_RGB_Image*)gif->png)->array, 0, 0, 
			gif->png->w(), gif->png->h(), 
			gif->png->d(), gif->png->ld());
	}
	fl = my_fl;
	delete(my_ps);
	fprintf(p->tfp, "\nx\n");

	gif->set_size(attr_w, attr_h);
}
#endif

void Xd6HtmlTagImg::to_html(FILE *fp)
{
	int l;
	char buf[1024];
	char *out;
	FILE *fi, *fo;

	if (!gif) return;

	fi = fl_fopen(source, "r");
	if (!fi) return;
	out = Xd6ConfigFile::temp();
	fo = fl_fopen(out, "w");
	if (!fo) { fclose(fi); free(out); return; }
	Xd6Encode_base64(fi, fo);
	fclose(fi);
	fclose(fo);

	fi = fopen(out, "r");
	if (!fi) { free(out); return; }
	l = fread(buf, 1, 1024, fi);

	if (l < 6) { fclose(fi); free(out); return; }

	fprintf(fp, "<img src=\"data:%s;base64,\n", gif->mime());

	while (l > 0) {
		fwrite(buf, l, 1, fp);
		l = fread(buf, 1, 1024, fi);
	}
	fclose(fi);
	fprintf(fp, "\" border=\"%d\" width=\"%d\" height=\"%d\" \n/>",
		attr_border, attr_w, attr_h);
	fl_unlink(out);
	free(out);
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

#if 0
class My_Rtf : public Fl_Fltk
{
public:
	FILE *output, *fp;
	My_Rtf(FILE *o) : Fl_Fltk() 
	{ 
		type = FL_PS_DEVICE;
		fp = output = o;
	};

	void draw_image(const uchar *data, int x, int y, int iw, int ih, int D=3, int LD=0) 
	{
                const uchar *curdata=data;
                if(!LD) LD=iw*D;
                for (int j=ih-1; j >= 0;j--){
                        curdata=data+j*LD;
                        for(int i=0 ; i<iw ; i++) {
				int d;
                                if (!((curdata - data)%48))
                                        fprintf(output, "\n");;
				if (D == 4 && curdata[3] < 32) {
					fprintf(output, "ffffff");
				} else {
					d =  curdata[2];
					//d = 0xff;
					fprintf(fp, "%c%c", to_hex(d >> 4), to_hex(d));
					d =  curdata[1];
					//d = 0xff;
					fprintf(fp, "%c%c", to_hex(d >> 4), to_hex(d));
					d = curdata[0];
					//d = 0xff;
					fprintf(fp, "%c%c", to_hex(d >> 4), to_hex(d));
				}
                                curdata +=D;
                        }
                }

	}
	void draw_image(Fl_Draw_Image_Cb call, void *data, int x, int y, int iw, int ih, int D)
	{
		int LD=iw*D;
		printf("jjj %d\n", D);
                uchar *rgbdata=new uchar[LD*ih];
		for (int j = 0; j < ih; j++) {
                        call(data,0,j,iw,rgbdata + LD * j);
		}
		draw_image(rgbdata, x, y, iw, ih, D, LD);
                delete[] rgbdata;

	}
};
#endif 

void Xd6HtmlTagImg::to_rtf(FILE *fp)
{
	//Fl_Device *my_fl, *my_ps;
	int mtSize = 9 + 3 + 4 + 3 + 3 + 5 + 5;
	int blt_size = 0; 
	int size;
	int rest;
	int i = 0;
	int x, y, rw;

	if (!gif) return;
	
	gif->load(source);
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
	print_int(fp, 0x00CC0020); // rop SRCCOPY
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
	print_int(fp, attr_h);
	print_int(fp, attr_w);
	print_short(fp, 0x1);
	print_short(fp, 0x18);
	print_int(fp, 0);
	print_int(fp, 0);
	print_int(fp, 0x0af0);
	print_int(fp, 0x0af0);
	print_int(fp, 0);
	print_int(fp, 0);
	fprintf(fp, "\n");

	printf ("%d %d %d %d\n",  gif->png->w(), gif->png->h(), attr_w, attr_h);
#if 0
	// start of rgb data
	my_ps = new My_Rtf(fp);
	my_fl = fl;
	fl = my_ps;
	if (gif->png->d() == 1) {
		gif->png->draw(0,0, gif->png->w(), gif->png->h(), 0, 0);
	} else if (gif->png->d() > 2) {
		fl_draw_image((uchar*)((Fl_RGB_Image*)gif->png)->array, 0, 0, 
			gif->png->w(), gif->png->h(), 
			gif->png->d(), gif->png->ld());
	}
	fl = my_fl;
	delete(my_ps);
#endif
	if (rest) {
		fprintf(fp, "00");
	}

	// end of META_DIBSTRETCHBLT

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
