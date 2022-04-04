/*
 * "$Id:  $"
 *
 * X11 UTF-8 text drawing functions.
 *
 *      Copyright (c) 2000-2002 by O'ksi'D.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * O'KSI'D BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of the O'ksi'D shall
 * not be used in advertising or otherwise to promote the sale, use or other
 * dealings in this Software without prior written authorization from O'ksi'D.
 *
 *  Author: Jean-Marc Lienher ( http://oksid.ch )
 */


#include "Xutf8.h"
#include <X11/Xlib.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if !HAVE_X11_UTF8

/* External auto generated functions : */
#include "ucs2fontmap.c"
/* 
 * extern int ucs2fontmap(char *s, unsigned int ucs, int enc);
 * extern int encoding_number(const char *enc);
 * extern const char *encoding_name(int num);
 */

/*********************************************************************/
/** extract a list of font from the base font name list             **/
/*********************************************************************/
static int 
get_font_list(
	const char	*base_font_name_list, 
	char 		***flist)
{
	const char *ptr;
	const char *p;
	int nb;
	int nb_name;
	
	ptr = base_font_name_list;
	p = NULL;
	nb = 0;
	nb_name = 1;

	while (*ptr) {
		if (*ptr == ',') nb_name++;
		ptr++;
	}

	*flist = (char **) malloc(sizeof(char*) * nb_name);
	ptr = base_font_name_list;

	while (*ptr) {
		int l = 0, i = 0;
	
		while(isspace(*ptr)) ptr++;
		p = ptr;
		while (*ptr && *ptr != ',') { ptr++; l++; }
		if (l > 2) {
			(*flist)[nb] = (char*) malloc(l + 2);
			while (p != ptr) { ((*flist)[nb])[i] = *p; i++; p++; }
			(*flist)[nb][i] = '\0';
			nb++;
		}
		if (*ptr) ptr++;
	}
	if (nb < 1) {
		free(*flist);
		*flist = (char**)NULL;
	}
	return nb;	
}

/*********************************************************************/
/** get the font name used as encoding for "fontspecific" encoding  **/
/** (mainly used for adobe-symbol and adobe-zapfdingbats)	    **/
/*********************************************************************/
static int 
font_spec_enc(
	char *font)
{
	int ret;
	char *enc;
	char *end;

	enc = font;
	while (*enc != '-') enc++; 
	enc++;
	while (*enc != '-') enc++;
	enc++;
	end = enc;
	while (*end != '-') end++;
	*end = '\0';
	
	ret = encoding_number(enc);
	*end = '-';

	return ret;
}


/*********************************************************************/
/** get the sub range of a iso10646-1 font			    **/
/*********************************************************************/
static void
get_range(
	const char	*enc,
	int 		*min,
	int		*max)
{
	const char *ptr = enc;
	const char *ptr1;

	if (!enc) return;

	while (*ptr && *ptr != '-') ptr++;
	if (!*ptr) return;
	while (*ptr && *ptr != '[') ptr++;
	if (!*ptr) return;
	*min = 0xFFFF;
	*max = 0;
	while (*ptr && *ptr != ']') {
		int val;
		ptr++;
		ptr1 = ptr;
		while (*ptr && *ptr != ']' && *ptr != ' ' && *ptr != '_') ptr++;
		val = strtol(ptr1, NULL, 0);
		if (val < *min) *min = val;
		if (val > *max) *max = val;
	}	
}

/*********************************************************************/
/** get the internal encoding number of each fonts 		    **/
/*********************************************************************/
static int *
get_encodings(
	char	**font_name_list, 
	int 	*ranges,
	int 	nb_font)
{
	int *font_encoding_list;
	int i;
	i = 0;

	font_encoding_list = (int *) malloc(sizeof(int) * nb_font);
	while (i < nb_font) {
		char *ptr;
		int ec;
		ptr = font_name_list[i];
		ec = 0;
		font_encoding_list[i] = -1;
		ranges[i * 2] = 0;
		ranges[i * 2 + 1] = 0xFFFF;
		
		if (ptr && strstr(ptr, "fontspecific")) {
			font_encoding_list[i] = font_spec_enc(ptr);
			ptr = NULL;
		}
		while (ptr && *ptr) {
			if (*ptr == '-') {
				ec++;
				if (ec == 13) {
					font_encoding_list[i] = 
						encoding_number(ptr + 1);
					if (font_encoding_list[i] == 0) {
						get_range(ptr + 1, 
							ranges + i * 2,
							ranges + i * 2 + 1);
					}
					break;
				}
			}	
			ptr++;
		}
		if (font_encoding_list[i] < 0) font_encoding_list[i] = 1;
		i++;
	}
	return font_encoding_list;
}

/*********************************************************************/
/** find the first font which matches the name and load it.	    **/
/*********************************************************************/
XFontStruct *
find_best_font(
	Display         *dpy,
	char		**name)
{
	char **list;
	int cnt;
	XFontStruct *s;

	list = XListFonts(dpy, *name, 1, &cnt);
	if (cnt && list) {
		free(*name);
		*name = strdup(list[0]);
		s = XLoadQueryFont(dpy, *name);
		XFreeFontNames(list);
		return s;
	}
	return NULL;
}

/*********************************************************************/
/** load all fonts 						    **/
/*********************************************************************/
static void 
load_fonts(
	Display 		*dpy, 
	XUtf8FontStruct	*font_set) 
{
	int i;
	char **list;

	i = 0;
	list = NULL;

	font_set->fonts = (XFontStruct**) 
		malloc(sizeof(XFontStruct*) * font_set->nb_font);
	
	font_set->ranges = (int*) 
		malloc(sizeof(int) *  font_set->nb_font * 2);

	font_set->descent = 0;
	font_set->ascent = 0;
	font_set->fid = 0;

	while (i < font_set->nb_font) {
		XFontStruct *fnt;

		fnt = font_set->fonts[i] = 
			find_best_font(dpy, &(font_set->font_name_list[i]));
		if (fnt) {
			font_set->fid = fnt->fid;
			if (fnt->ascent > font_set->ascent) {
				font_set->ascent = fnt->ascent;
			}
			if (fnt->descent > font_set->descent) {
				font_set->descent = fnt->descent;
			}
		} else {
			free(font_set->font_name_list[i]);
			font_set->font_name_list[i] = NULL;
		}
		i++;
	}

	font_set->encodings = 
		get_encodings(font_set->font_name_list, 
			font_set->ranges, font_set->nb_font);
}
#endif 

/*********************************************************************/
/** Creates an array of XFontStruct acording to the comma separated  **/
/** list of fonts. XLoad all fonts.				    **/
/*********************************************************************/
XUtf8FontStruct *
XCreateUtf8FontStruct (
    	Display        	*dpy,
    	const char   	*base_font_name_list)
{
#if HAVE_X11_UTF8
	int i;
	char **miss;
	char *def;
	int cnt;
	XUtf8FontStruct *f = (XUtf8FontStruct*) malloc(sizeof(XUtf8FontStruct));
	f->nb_font = 0;
	f->font_name_list = NULL;
	f->encodings = NULL;
	f->fonts = NULL;
	f->fid = 0;
	f->ascent = 0;
	f->descent = 0;
	f->ranges = NULL;
	f->fs = XCreateFontSet(dpy, base_font_name_list, &miss, &cnt,&def);
	f->nb_font = XFontsOfFontSet(f->fs, &f->fonts, &f->font_name_list);
	for (i = 0; i < f->nb_font; i++) {
		if (f->fonts[i]->ascent > f->ascent) {
			f->ascent = f->fonts[i]->ascent;
		}
		if (f->fonts[i]->descent > f->descent) {
			f->descent = f->fonts[i]->descent;
		}
	}
	return f;
#else 
	XUtf8FontStruct *font_set;
	
	font_set = (XUtf8FontStruct*) 
		malloc(sizeof(XUtf8FontStruct));

 	if (!font_set) {
		return NULL;
        }

	font_set->nb_font = get_font_list(base_font_name_list, 
					&font_set->font_name_list);

	if (font_set->nb_font < 1) {
		free(font_set);
		return NULL;
	}

	load_fonts(dpy, font_set);

	return font_set;	
#endif
}


/*****************************************************************************/
/** draw a Right To Left UTF-8 string using multiple fonts as needed.	    **/
/*****************************************************************************/
void 
XUtf8DrawRtlString(
	Display 	*display, 
	Drawable 	d,
        XUtf8FontStruct 	*font_set, 
	GC 		gc, 
	int 		x, 
	int 		y, 
	const char	*string,
        int 		num_bytes)
{
#if HAVE_X11_UTF8
	//FIXME
#else
	int 		*encodings; /* encodings array */
	XFontStruct 	**fonts; /* fonts array */
	XChar2b 	buf[128]; /* drawing buffer */
	XChar2b		*ptr;	/* pointer to the drawing buffer */
	int 		fnum; /* index of the current font in the fonts array*/
	int 		i; /* current byte in the XChar2b buffer */
	int 		first; /* first valid font index */
	int 		last_fnum; /* font index of the previous char */
	int 		nb_font;  /* quantity of fonts in the font array */
	char 		glyph[2]; /* byte1 and byte1 value of the UTF-8 char */
	int		*ranges; /* sub range of iso10646 */

	nb_font = font_set->nb_font;

	if (nb_font < 1) {
		/* there is no font in the font_set :-( */
		return;
	}
	
	ranges = font_set->ranges;
	fonts = font_set->fonts;
	encodings = font_set->encodings;
	i = 0;
	fnum = 0;
	ptr = buf + 128;
	
	while(fnum < nb_font && !fonts[fnum]) fnum++;
	if (fnum >= nb_font) {
		/* there is no valid font for the X server */
		return;
	}

	first = fnum;
	last_fnum = fnum;

	while (num_bytes > 0) {
		int 		ulen; /* byte length of the UTF-8 char */
		unsigned int 	ucs;  /* Unicode value of the UTF-8 char */
		unsigned int	no_spc; /* Spacing char equivalent of a 
					   non-spacing char */

		if (i > 120) {
			/*** draw the buffer **/
			XSetFont(display, gc, fonts[fnum]->fid);
			x -= XTextWidth16(fonts[fnum], ptr, i);
			XDrawString16(display, d, gc, x, y, ptr, i);
			i = 0;
			ptr = buf + 128;
		}

		ulen = XFastConvertUtf8ToUcs((unsigned char*)string, 
				num_bytes, &ucs); 

		if (ulen < 1) ulen = 1; 

		no_spc = XUtf8IsNonSpacing(ucs);
		if (no_spc) ucs = no_spc; 

		/* 
		 * find the first encoding which can be used to 	
		 * draw the glyph 				
		 */
		fnum = first;
		while (fnum < nb_font) {
			if (fonts[fnum] && 
				ucs2fontmap(glyph, ucs, encodings[fnum]) >= 0) 
			{
				if (encodings[fnum] != 0 || 
					(ucs >= ranges[fnum * 2] &&
					ucs <= ranges[fnum * 2 + 1]))
				{
					break;
				}
			}
			fnum++;
		}
		if (fnum == nb_font) {
			/** the char is not valid in all encodings ->
			  * draw it using the first font :-( **/
			fnum = first;
			ucs2fontmap(glyph, '?', encodings[fnum]);
		}

		if (last_fnum != fnum || no_spc) {
			XSetFont(display, gc, fonts[last_fnum]->fid);
			x -= XTextWidth16(fonts[last_fnum], ptr, i);
			XDrawString16(display, d, gc, x, y, ptr, i);
			i = 0;
			ptr = buf + 127;
			(*ptr).byte1 = glyph[0];
			(*ptr).byte2 = glyph[1];
			if (no_spc) {
				x += XTextWidth16(fonts[fnum], ptr, 1);
			}
		} else {
			ptr--;
			(*ptr).byte1 = glyph[0];
			(*ptr).byte2 = glyph[1];
		}
		last_fnum = fnum;
		i++;
		string += ulen;
		num_bytes -= ulen;
	}

	if (i < 1) return;

	XSetFont(display, gc, fonts[fnum]->fid);
	x -= XTextWidth16(fonts[last_fnum], ptr, i);
	XDrawString16(display, d, gc, x, y, ptr, i);
#endif
}


/*****************************************************************************/
/** draw an UTF-8 string using multiple fonts as needed.		    **/
/*****************************************************************************/
void 
XUtf8DrawString(
	Display 	*display, 
	Drawable 	d,
        XUtf8FontStruct 	*font_set, 
	GC 		gc, 
	int 		x, 
	int 		y, 
	const char	*string,
        int 		num_bytes)
{
#if HAVE_X11_UTF8
	Xutf8DrawString(display, d, font_set->fs, gc, x, y, string, num_bytes);
#else
	int 		*encodings; /* encodings array */
	XFontStruct 	**fonts; /* fonts array */
	XChar2b 	buf[128]; /* drawing buffer */
	int 		fnum; /* index of the current font in the fonts array*/
	int 		i; /* current byte in the XChar2b buffer */
	int 		first; /* first valid font index */
	int 		last_fnum; /* font index of the previous char */
	int 		nb_font;  /* quantity of fonts in the font array */
	char 		glyph[2]; /* byte1 and byte1 value of the UTF-8 char */
	int		*ranges; /* sub range of iso10646 */

	nb_font = font_set->nb_font;

	if (nb_font < 1) {
		/* there is no font in the font_set :-( */
		return;
	}
	ranges = font_set->ranges;
	fonts = font_set->fonts;
	encodings = font_set->encodings;
	i = 0;
	fnum = 0;
	
	while(fnum < nb_font && !fonts[fnum]) fnum++;
	if (fnum >= nb_font) {
		/* there is no valid font for the X server */
		return;
	}

	first = fnum;
	last_fnum = fnum;

	while (num_bytes > 0) {
		int 		ulen; /* byte length of the UTF-8 char */
		unsigned int 	ucs;  /* Unicode value of the UTF-8 char */
		unsigned int	no_spc; /* Spacing char equivalent of a 
					   non-spacing char */

		if (i > 120) {
			/*** draw the buffer **/
			XSetFont(display, gc, fonts[fnum]->fid);
			XDrawString16(display, d, gc, x, y, buf, i);
			x += XTextWidth16(fonts[fnum], buf, i);
			i = 0;
		}

		ulen = XFastConvertUtf8ToUcs((unsigned char*)string, 
				num_bytes, &ucs); 

		if (ulen < 1) ulen = 1; 

		no_spc = XUtf8IsNonSpacing(ucs);
		if (no_spc) ucs = no_spc; 

		/* 
		 * find the first encoding which can be used to 	
		 * draw the glyph 				
		 */
		fnum = first;
		while (fnum < nb_font) {
			if (fonts[fnum] && 
				ucs2fontmap(glyph, ucs, encodings[fnum]) >= 0) 
			{
				if (encodings[fnum] != 0 || 
					(ucs >= ranges[fnum * 2] &&
					ucs <= ranges[fnum * 2 + 1]))
				{
					break;
				}
			}
			fnum++;
		}
		if (fnum == nb_font) {
			/** the char is not valid in all encodings ->
			  * draw it using the first font :-( **/
			fnum = first;
			ucs2fontmap(glyph, '?', encodings[fnum]);
		}

		if (last_fnum != fnum || no_spc) {
			XSetFont(display, gc, fonts[last_fnum]->fid);
			XDrawString16(display, d, gc, x, y, buf, i);
			x += XTextWidth16(fonts[last_fnum], buf, i);
			i = 0;
			(*buf).byte1 = glyph[0];
			(*buf).byte2 = glyph[1];
			if (no_spc) {
				x -= XTextWidth16(fonts[fnum], buf, 1);
			}
		} else {
			(*(buf + i)).byte1 = glyph[0];
			(*(buf + i)).byte2 = glyph[1];
		}
		last_fnum = fnum;
		i++;
		string += ulen;
		num_bytes -= ulen;
	}

	XSetFont(display, gc, fonts[fnum]->fid);
	XDrawString16(display, d, gc, x, y, buf, i);
#endif
}



/*****************************************************************************/
/** returns the pixel width of a UTF-8 string				    **/
/*****************************************************************************/
int  
XUtf8TextWidth(
        XUtf8FontStruct 	*font_set, 
	const char 	*string,
        int 		num_bytes)
{
#if HAVE_X11_UTF8
	int ret;
	XRectangle ink;
	XRectangle log;
	ret = Xutf8TextExtents(font_set->fs, string, num_bytes, &ink, &log);
	return log.width;
#else
	int		x;
	int 		*encodings; /* encodings array */
	XFontStruct 	**fonts; /* fonts array */
	XChar2b 	buf[128]; /* drawing buffer */
	int 		fnum; /* index of the current font in the fonts array*/
	int 		i; /* current byte in the XChar2b buffer */
	int 		first; /* first valid font index */
	int 		last_fnum; /* font index of the previous char */
	int 		nb_font;  /* quantity of fonts in the font array */
	char 		glyph[2]; /* byte1 and byte1 value of the UTF-8 char */
	int		*ranges; /* sub range of iso10646 */

	nb_font = font_set->nb_font;
	x = 0;

	if (nb_font < 1) {
		/* there is no font in the font_set :-( */
		return x;
	}

	ranges = font_set->ranges;
	fonts = font_set->fonts;
	encodings = font_set->encodings;
	i = 0;
	fnum = 0;
	
	while(fnum < nb_font && !fonts[fnum]) fnum++;
	if (fnum >= nb_font) {
		/* there is no valid font for the X server */
		return x;
	}

	first = fnum;
	last_fnum = fnum;

	while (num_bytes > 0) {
		int 		ulen; /* byte length of the UTF-8 char */
		unsigned int 	ucs;  /* Unicode value of the UTF-8 char */
		unsigned int	no_spc; /* Spacing char equivalent of a 
					   non-spacing char */

		if (i > 120) {
			/*** measure the buffer **/
			x += XTextWidth16(fonts[fnum], buf, i);
			i = 0;
		}

		ulen = XFastConvertUtf8ToUcs((unsigned char*)string, 
				num_bytes, &ucs); 

		if (ulen < 1) ulen = 1; 

		no_spc = XUtf8IsNonSpacing(ucs);
		if (no_spc) {
			ucs = no_spc;
		}

		/* 
		 * find the first encoding which can be used to 	
		 * draw the glyph 				
		 */
		fnum = first;
		while (fnum < nb_font) {
			if (fonts[fnum] && 
				ucs2fontmap(glyph, ucs, encodings[fnum]) >= 0) 
			{
				if (encodings[fnum] != 0 || 
					(ucs >= ranges[fnum * 2] &&
					ucs <= ranges[fnum * 2 + 1]))
				{
					break;
				}
			}
			fnum++;
		}
		if (fnum == nb_font) {
			/** the char is not valid in all encodings ->
			  * draw it using the first font :-( **/
			fnum = first;
			ucs2fontmap(glyph, '?', encodings[fnum]);
		}

		if (last_fnum != fnum || no_spc) {
			x += XTextWidth16(fonts[last_fnum], buf, i);
			i = 0;
			(*buf).byte1 = glyph[0];
			(*buf).byte2 = glyph[1];
			if (no_spc) {
				/* go back to draw the non-spacing char over
				 *  the previous char */
				x -= XTextWidth16(fonts[fnum], buf, 1);
			}
		} else {
			(*(buf + i)).byte1 = glyph[0];
			(*(buf + i)).byte2 = glyph[1];
		}
		last_fnum = fnum;
		i++;
		string += ulen;
		num_bytes -= ulen;
	}

	x += XTextWidth16(fonts[last_fnum], buf, i);

	return x;
#endif
}

/*****************************************************************************/
/** returns the pixel width of a UCS char				    **/
/*****************************************************************************/
int
XUtf8UcsWidth(
        XUtf8FontStruct  *font_set,
        unsigned int            ucs)
{
#if HAVE_X11_UTF8
	int l;
	char buf[5];
	l = XConvertUcsToUtf8(ucs, buf);
	if (l < 0) l = 0;
	return XUtf8TextWidth(font_set, buf, l);
#else
	int		x;
	int 		*encodings; /* encodings array */
	XFontStruct 	**fonts; /* fonts array */
	XChar2b 	buf[8]; /* drawing buffer */
	int 		fnum; /* index of the current font in the fonts array*/
	int 		i; /* current byte in the XChar2b buffer */
	int 		first; /* first valid font index */
	int 		last_fnum; /* font index of the previous char */
	int 		nb_font;  /* quantity of fonts in the font array */
	char 		glyph[2]; /* byte1 and byte1 value of the UTF-8 char */
	int		*ranges; /* sub range of iso10646 */

	nb_font = font_set->nb_font;
	x = 0;

	if (nb_font < 1) {
		/* there is no font in the font_set :-( */
		return x;
	}

	ranges = font_set->ranges;
	fonts = font_set->fonts;
	encodings = font_set->encodings;
	i = 0;
	fnum = 0;
	
	while(fnum < nb_font && !fonts[fnum]) fnum++;
	if (fnum >= nb_font) {
		/* there is no valid font for the X server */
		return x;
	}

	first = fnum;
	last_fnum = fnum;


	ucs = XUtf8IsNonSpacing(ucs);

	/* 
	 * find the first encoding which can be used to 	
	 * draw the glyph 				
	 */
	fnum = first;
	while (fnum < nb_font) {
		if (fonts[fnum] && 
			ucs2fontmap(glyph, ucs, encodings[fnum]) >= 0) 
		{
			if (encodings[fnum] != 0 || (ucs >= ranges[fnum * 2] &&
				ucs <= ranges[fnum * 2 + 1]))
			{
				break;
			}
		}
		fnum++;
	}
	if (fnum == nb_font) {
		/** the char is not valid in all encodings ->
		  * draw it using the first font :-( **/
		fnum = first;
		ucs2fontmap(glyph, '?', encodings[fnum]);
	}

	(*buf).byte1 = glyph[0];
	(*buf).byte2 = glyph[1];

	x += XTextWidth16(fonts[fnum], buf, 1);

	return x;
#endif
}

/*****************************************************************************/
/** draw an UTF-8 string and clear the background.	 		    **/
/*****************************************************************************/
void
XUtf8DrawImageString(
        Display         *display,
        Drawable        d,
        XUtf8FontStruct         *font_set,
        GC              gc,
        int             x,
        int             y,
        const char      *string,
        int             num_bytes)
{
#if HAVE_X11_UTF8
	Xutf8DrawImageString(display, d, font_set->fs, gc, x, y, 
		string, num_bytes);
#else
	/* FIXME: must be improved ! */
	int w;
	int fill_style;
	unsigned long foreground;
	unsigned long background;
	int function;
	XGCValues xgcv;

	w = XUtf8TextWidth(font_set, string, num_bytes);
	
	XGetGCValues(display, gc, 
		GCFunction|GCForeground|GCBackground|GCFillStyle, &xgcv);
	
	function = xgcv.function;
	fill_style = xgcv.fill_style;
	foreground = xgcv.foreground;
	background = xgcv.background;

	xgcv.function = GXcopy;
	xgcv.foreground = background;
	xgcv.background = foreground;
	xgcv.fill_style = FillSolid;

	XChangeGC(display, gc,
		GCFunction|GCForeground|GCBackground|GCFillStyle, &xgcv);

	XFillRectangle(display, d, gc, x, y - font_set->ascent, 
		w, font_set->ascent + font_set->descent);

	xgcv.function = function;
	xgcv.foreground = foreground;
	xgcv.background = background;
	xgcv.fill_style = fill_style;

	XChangeGC(display, gc,
		GCFunction|GCForeground|GCBackground|GCFillStyle, &xgcv);

	XUtf8DrawString(display, d, font_set, gc, x, y, string, num_bytes);
#endif
}

/*****************************************************************************/
/** free the XFontSet and others things created by XCreateUtf8FontSet       **/
/*****************************************************************************/
void 
XFreeUtf8FontStruct(
	Display 	*dpy, 
	XUtf8FontStruct *font_set)
{
#if HAVE_X11_UTF8
	XFreeFontSet(dpy, font_set->fs);
	free(font_set);
#else
	int i;
	i = 0;
	while (i < font_set->nb_font) {
		if (font_set->fonts[i]) {
			XFreeFont(dpy, font_set->fonts[i]);
			free(font_set->font_name_list[i]);
		}
		i++;
	}
	free(font_set->ranges);
	free(font_set->font_name_list);
	free(font_set->fonts);
	free(font_set->encodings);
	free(font_set);
#endif
}

/*
 *  End of "$Id: $".
 */

