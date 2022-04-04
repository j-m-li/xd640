
#include <fltk/fl_draw.h>
#include <fltk/x.h>

#ifndef xd6_draw_h
#define xd6_draw_h
inline void fl_polygon(int x, int y, int x1, int y1, int x2, int y2) {
  XPoint p[4];
  p[0].x = x;  p[0].y = y;
  p[1].x = x1; p[1].y = y1;
  p[2].x = x2; p[2].y = y2;
#ifdef WIN32
  SelectObject(fl_gc, fl_brush());
  Polygon(fl_gc, p, 3);
#else
  p[3].x = x;  p[3].y = y;
  XFillPolygon(fl_display, fl_window, fl_gc, p, 3, Convex, 0);
  XDrawLines(fl_display, fl_window, fl_gc, p, 4, 0);
#endif
}

inline void fl_rtl_draw(const char* c, int n, int x, int y)
{
	fl_draw(c, n, x, y);
}

inline void fl_font(int fnt, int size)
{
	switch (fnt) {
	case 0:	
		fl_font(FL_HELVETICA, size); break;
	case 1:
		fl_font(FL_HELVETICA_BOLD, size); break;
	case 2:
		fl_font(FL_HELVETICA_ITALIC, size); break;
	case 3:
		fl_font(FL_HELVETICA_BOLD_ITALIC, size); break;
	case 4:
		fl_font(FL_COURIER, size); break;
	case 5:
		fl_font(FL_COURIER_BOLD, size); break;
	case 6:
		fl_font(FL_COURIER_ITALIC, size); break;
	case 7:
		fl_font(FL_COURIER_BOLD_ITALIC, size); break;
	case 8:
		fl_font(FL_TIMES, size); break;
	case 9:
		fl_font(FL_TIMES_BOLD, size); break;
	case 10:
		fl_font(FL_TIMES_ITALIC, size); break;
	case 11:
		fl_font(FL_TIMES_BOLD_ITALIC, size); break;
	}
}

#endif

