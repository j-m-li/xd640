//
// "$Id: fl_font_x.cxx,v 1.10.2.3 2002/06/08 13:07:19 easysw Exp $"
//
// Standard X11 font selection code for the Fast Light Tool Kit (FLTK).
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
/* Define a class here for the font cache */

#define WIDTH_CACHE_SIZE 20
#include <config.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include "Fl_Font.H"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
  GR_FONT_ID fontid;
  GR_FONT_INFO fi;
  int age;
  unsigned char widths[256];
} width_cache_struct;

class font_cache
{
private:
  width_cache_struct* cache[WIDTH_CACHE_SIZE];

public:
  width_cache_struct* check_cache(GR_FONT_ID curfont)
  {
    /* For the moment, we index the cache with the font ID */
    int n;
    
    for(n = 0; n < WIDTH_CACHE_SIZE; n++)
      {
      if (cache[n])
	{
	  if (cache[n]->fontid == curfont)
	    {
	      cache[n]->age = 0;
	      return(cache[n]);
	    }
	  
	  cache[n]->age++;
	}
      }
 
    return( (width_cache_struct *) 0);
  }
  
  void add_cache(GR_FONT_ID curfont, unsigned char *widths, GR_FONT_INFO * fi )
  {
    int n;
    int oldest = -1;
    

    for(n = 0; n < WIDTH_CACHE_SIZE; n++)
      {
	if (!cache[n])
	  {
	    cache[n] = (width_cache_struct *) malloc (sizeof(width_cache_struct));
	    
	    if (!cache[n])
	      return;
	    
	    cache[n]->fontid = curfont;
	    cache[n]->age = 0;
	    
	    bcopy(widths, cache[n]->widths, 256);
	    memcpy(&cache[n]->fi,fi,sizeof(GR_FONT_INFO));
	  return;
	  }
	else
	  {
	    if (oldest != -1)
	      {
		if (cache[n]-> age > cache[oldest]->age)
		  oldest = n;
	      }
	    else
	      oldest = n;
	  }
      }

    if (oldest == -1)
      return;
    
    bzero(cache[oldest], sizeof(width_cache_struct));
    
    cache[oldest]->fontid = curfont;
    cache[oldest]->age = 0;
    
    bcopy(widths, cache[oldest]->widths, 256);
    
  return;
  }

  font_cache()
  {
    int n;

    for(n = 0; n < WIDTH_CACHE_SIZE; n++)
      cache[n] = (width_cache_struct *) 0;
  }

  ~font_cache()
  {
    int n;

    for(n = 0; n < WIDTH_CACHE_SIZE; n++)
      if (cache[n])
	free(cache[n]);
  }
};

font_cache width_cache;


static int my_size = 0;

Fl_FontSize::Fl_FontSize(const char* name) {
  isTTF = false;
  
  font = GrCreateFont((GR_CHAR *)name, my_size, NULL);

  // need to determine whether the font is TrueType
  // GrGetFontInfo() needs a way to indicate that
  // so this is a bit of a hack; are all TTF fonts in .ttf files?
  if(font) {
    if( strstr(name, ".ttf")) {
	isTTF = true;
    }
  } else { 
//        Fl::warning("bad font: %s, using a builtin", name);
//	font = GrCreateFont("HZKFONT", 16, NULL);
	font = GrCreateFont((GR_CHAR *)"fonts/simfang.ttf", 16, NULL);
	if (!font)
		font = GrCreateFont((GR_CHAR *)GR_FONT_GUI_VAR, 0, NULL);
  }

}

Fl_FontSize* fl_fontsize;

// attempts to resize the font in microwindows
void Fl_FontSize::Resize(int size)  {
	GrSetFontSize(font, size);
}

Fl_FontSize::~Fl_FontSize() {
  if(font)	
    GrDestroyFont(font);
}

////////////////////////////////////////////////////////////////

// WARNING: if you add to this table, you must redefine FL_FREE_FONT
// in Enumerations.H & recompile!!
static Fl_Fontdesc built_in_table[] = {
  
  {"/usr/local/microwin/fonts/arial.ttf"},
  {"/usr/local/microwin/fonts/arialb.ttf"},
  {"/usr/local/microwin/fonts/ariali.ttf"},
  {"/usr/local/microwin/fonts/arialz.ttf"},
  {"/usr/local/microwin/fonts/cour.ttf"},
  {"/usr/local/microwin/fonts/courb.ttf"},
  {"/usr/local/microwin/fonts/couri.ttf"},
  {"/usr/local/microwin/fonts/courz.ttf"},
  {"/usr/local/microwin/fonts/times.ttf"},
  {"/usr/local/microwin/fonts/timesb.ttf"},
  {"/usr/local/microwin/fonts/timesi.ttf"},
  {"/usr/local/microwin/fonts/timesz.ttf"},
  {"/usr/local/microwin/fonts/impact.ttf"},
  {"/usr/local/microwin/fonts/comic.ttf"},
  {"/usr/local/microwin/fonts/comicbd.ttf"}
};

Fl_Fontdesc* fl_fonts = built_in_table;

#define MAXSIZE 32767

// return dash number N, or pointer to ending null if none:
const char* fl_font_word(const char* p, int n) {
  while (*p) {if (*p=='-') {if (!--n) break;} p++;}
  return p;
}

// return a pointer to a number we think is "point size":
char* fl_find_fontsize(char* name) {
  char* c = name;
  // for standard x font names, try after 7th dash:
  if (*c == '-') {
    c = (char*)fl_font_word(c,7);
    if (*c++ && isdigit(*c)) return c;
    return 0; // malformed x font name?
  }
  char* r = 0;
  // find last set of digits:
  for (c++;* c; c++)
    if (isdigit(*c) && !isdigit(*(c-1))) r = c;
  return r;
}

const char* fl_encoding = "iso8859-1";

// return true if this matches fl_encoding:
int fl_correct_encoding(const char* name) {
  if (*name != '-') return 0;
  const char* c = fl_font_word(name,13);
  return (*c++ && !strcmp(c,fl_encoding));
}
// font choosers - if the fonts change in built_in_table, 
// these and the enum in Fl/Enumerations.H will have to change

inline bool IsBold(int fnum) {
	switch(fnum) {
	case FL_HELVETICA_BOLD:
	case FL_HELVETICA_BOLD_ITALIC:
	case FL_COURIER_BOLD:
	case FL_COURIER_BOLD_ITALIC:
		return true;
	default: return false;
	}
}
inline bool IsItalic(int fnum) {
	switch(fnum) {
	case FL_HELVETICA_ITALIC:
	case FL_HELVETICA_BOLD_ITALIC:
	case FL_COURIER_ITALIC:
	case FL_COURIER_BOLD_ITALIC:
		return true;
	default: return false;
	}
}

// locate or create an Fl_FontSize for a given Fl_Fontdesc and size:
static Fl_FontSize* find(int fnum, int size) {

  if (fnum >= 3) fnum = 0;
  Fl_Fontdesc* s = fl_fonts+fnum;
  if (!s || !s->name) s = fl_fonts; // use font 0 if still undefined
  Fl_FontSize* f;
  for (f = s->first; f; f = f->next) {
    if (f->minsize <= size && f->maxsize >= size && !f->isTTF) {
      return f;
    }
  }


  // time to create one
  if (!s->first) {	
    my_size = size;
    s->first = new Fl_FontSize(s->name);
  } else {
    return s->first;

  }
	

  // if this is TTF there will only be a first, so resize it
  if(s->first && s->first->isTTF) {
    s->first->Resize(size);
    return s->first;
  } else if (s->first) {
	return s->first;
  }


  if (!s->xlist) {
      s->first = new Fl_FontSize(GR_FONT_GUI_VAR);//"/Setup/microwin/src/fonts/simfang.ttf"););//"simfang.ttf");
      s->first->minsize = size;
      s->first->maxsize = size;
      return s->first;
    }
  // search for largest <= font size:
  char* name = s->xlist[0]; int ptsize = 0;	// best one found so far
  int matchedlength = 32767;
  char namebuffer[1024];	// holds scalable font name
  int found_encoding = 0;
  int m = s->n; if (m<0) m = -m;
  for (int n=0; n < m; n++) {

  char* thisname = s->xlist[n];
  if (fl_correct_encoding(thisname)) {
    if (!found_encoding) ptsize = 0; // force it to choose this
    found_encoding = 1;
  } else {
    if (found_encoding) continue;
  }
  char* c = fl_find_fontsize(thisname);
  int thissize = c ? atoi(c) : MAXSIZE;
  int thislength = strlen(thisname);
  if (thissize == size && thislength < matchedlength) {
     // exact match, use it:
     name = thisname;
     ptsize = size;
     matchedlength = thislength;
  } else if (!thissize && ptsize!=size) {

      // whoa!  A scalable font!  Use unless exact match found:
      int l = c-thisname;
      memcpy(namebuffer,thisname,l);
#if 1 // this works if you don't want stdio
      if (size>=100) namebuffer[l++] = size/100+'0';
      if (size>=10) namebuffer[l++] = (size/10)%10+'0';
      namebuffer[l++] = (size%10)+'0';
#else
      //for some reason, sprintf fails to return the right value under Solaris.
      l += sprintf(namebuffer+l,"%d",size);
#endif
      while (*c == '0') c++;
      strcpy(namebuffer+l,c);
      name = namebuffer;
      ptsize = size;
  } else if (!ptsize ||	// no fonts yet
	       thissize < ptsize && ptsize > size || // current font too big
	       thissize > ptsize && thissize <= size // current too small
      ) {
      name = thisname; ptsize = thissize;
      matchedlength = thislength;
    }
  }

  if (ptsize != size) { // see if we already found this unscalable font:
    for (f = s->first; f; f = f->next) {
      if (f->minsize <= ptsize && f->maxsize >= ptsize) {
	if (f->minsize > size) f->minsize = size;
	if (f->maxsize < size) f->maxsize = size;
	return f;
      }
    }
  }

  // okay, we definately have some name, make the font:
  f = new Fl_FontSize(name);
  if (ptsize < size) {f->minsize = ptsize; f->maxsize = size;}
  else {f->minsize = size; f->maxsize = ptsize;}
  f->next = s->first;
  s->first = f;
  return f;
}

////////////////////////////////////////////////////////////////
// Public interface:

int fl_font_ = 0;
int fl_size_ = 0;
GR_FONT_ID fl_xfont = 0;
static GC font_gc;

void Fl_Fltk::font(int fnum, int size) {
  if (fnum == fl_font_ && size == fl_size_) return;
  fl_font_ = fnum; fl_size_ = size;
  Fl_FontSize* f = find(fnum, size);
  if (f != fl_fontsize) {
    fl_fontsize = f;
    fl_xfont = f->font;
    font_gc = 0;
  }
}

int Fl_Fltk::height() {
	GR_FONT_INFO    fi;
	width_cache_struct *wc = width_cache.check_cache(fl_xfont);
	
	if(!wc) {
	  GrGetFontInfo(fl_xfont, &fi);	  
	  width_cache.add_cache(fl_xfont, fi.widths,&fi);
	  return fi.height;
	} else {
	  return wc->fi.height;
	}
}

int Fl_Fltk::descent() {
 	GR_FONT_INFO    fi;
	width_cache_struct *wc = width_cache.check_cache(fl_xfont);
	
	if(!wc) {
	  GrGetFontInfo(fl_xfont, &fi);	  
	  width_cache.add_cache(fl_xfont, fi.widths,&fi);
	  return fi.height - fi.baseline;
	} else {
	  return wc->fi.height - wc->fi.baseline;
	}
}

double Fl_Fltk::width(const char* c, int n) {
 double w=0;
  GR_FONT_INFO    fi;
  width_cache_struct *wc = width_cache.check_cache(fl_xfont);
  unsigned char *fwidths;
  
  if (!wc)
    {      
      GrGetFontInfo(fl_xfont, &fi);
      width_cache.add_cache(fl_xfont, fi.widths,&fi);
      
      fwidths = fi.widths;
    }
  else
    {
      fwidths = wc->widths;
    }

  for(int i=0;i<n;i++)
    {
      w+=(double)(fwidths[*(c+i)]);	
    }
  return w;

}

double Fl_Fltk::width(unsigned int c) {
   GR_FONT_INFO    fi;
  width_cache_struct *wc = width_cache.check_cache(fl_xfont);
  unsigned char *fwidths;
  
  if (!wc)
    {      
      GrGetFontInfo(fl_xfont, &fi);
      width_cache.add_cache(fl_xfont, fi.widths,&fi);
      
      fwidths = fi.widths;
    }
  else
    {
      fwidths = wc->widths;
    }
	
  return fwidths[c];
}

void Fl_Fltk::draw(const char* c, int n, int x, int y) {
   if (font_gc != fl_gc) {
    if (!fl_xfont)
      {
	fl_font(FL_HELVETICA, 14);
      }
    }
    font_gc = fl_gc;
   GrSetGCFont(fl_gc, fl_xfont);
   
 	//FIXME hack becasue nano-X will not draw a true type font
	// correctly without first setting its background!!!
	GR_GC_INFO info;
	
	GrGetGCInfo(fl_gc, &info);
	if(info.background == MWRGB(255, 255, 255)) 
		GrSetGCBackground(fl_gc, MWRGB(255, 0, 0));
	else
		GrSetGCBackground(fl_gc, MWRGB(255, 255, 255));	
  GrSetGCUseBackground(fl_gc, GR_FALSE);
  GrText(fl_window, fl_gc, x, y, (GR_CHAR *)c, n, GR_TFASCII);
}

void Fl_Fltk::rtl_draw(const char* c, int n, int x, int y) {
   if (font_gc != fl_gc) {
    if (!fl_xfont)
      {
	fl_font(FL_HELVETICA, 14);
      }
    }
    font_gc = fl_gc;
   GrSetGCFont(fl_gc, fl_xfont);
 	//FIXME hack becasue nano-X will not draw a true type font
	// correctly without first setting its background!!!
	GR_GC_INFO info;
	
	GrGetGCInfo(fl_gc, &info);
	if(info.background == MWRGB(255, 255, 255)) 
		GrSetGCBackground(fl_gc, MWRGB(255, 0, 0));
	else
		GrSetGCBackground(fl_gc, MWRGB(255, 255, 255));	
  GrSetGCUseBackground(fl_gc, GR_FALSE);
  GrText(fl_window, fl_gc, x, y, (GR_CHAR *)c, n, GR_TFASCII);

}

//
// End of "$Id: fl_font_x.cxx,v 1.10.2.3 2002/06/08 13:07:19 easysw Exp $".
//
