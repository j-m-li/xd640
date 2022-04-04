#ifndef fl_device_H
#define fl_device_H
#include <FL/Enumerations.H>
#include <FL/Fl_Group.H>

#ifdef WIN32
#include <windows.h>
extern FL_EXPORT HDC fl_gc;
#endif

extern FL_EXPORT Fl_Color fl_color_;
extern FL_EXPORT int fl_size_;
extern FL_EXPORT int fl_font_;

typedef void (*Fl_Draw_Image_Cb)(void* ,int,int,int,uchar*);


class Fl_Device{
public:
	int type;
#ifdef WIN32
	HDC gc, o;
#endif
	double s;
	int L;
	int T;

	virtual FL_EXPORT void color(Fl_Color c)=0;
	virtual FL_EXPORT void color(uchar r, uchar g, uchar b)=0;
	virtual FL_EXPORT Fl_Color color()=0;
	
	virtual FL_EXPORT void clip(double x, double y, double w, double h)=0;
	virtual FL_EXPORT void clip(int x, int y, int w, int h)=0;

	virtual FL_EXPORT void push_no_clip()=0;
	virtual FL_EXPORT void pop_clip()=0;

	virtual FL_EXPORT int not_clipped(double x, double y, double w, double h)=0;
	virtual FL_EXPORT int not_clipped(int x, int y, int w, int h)=0;
	
	virtual FL_EXPORT int clip_box(double _x, double _y, double _w, double _h, double& x, double& y, double& w, double& h)=0;
	virtual FL_EXPORT int clip_box(int _x, int _y, int _w, int _h, int& x, int& y, int& w, int& h)=0;
	virtual FL_EXPORT void point(int x, int y)=0;

	virtual FL_EXPORT void line_style(int style, int width, char* dashes)=0;

	virtual FL_EXPORT void rect(int x, int y, int w, int h)=0;
	virtual FL_EXPORT void rectf(int x, int y, int w, int h)=0;

	virtual FL_EXPORT void line(int x1, int y1, int x2, int y2)=0;
	virtual FL_EXPORT void line(int x1, int y1, int x2, int y2, int x3, int y3)=0;

	virtual FL_EXPORT void loop(int x1, int y1, int x2 ,int y2, int x3, int y3)=0;
	virtual FL_EXPORT void loop(int x1, int y1, int x2 ,int y2, int x3, int y3, int x4, int y4)=0;

	virtual FL_EXPORT void polygon(int x1, int y1, int x2 ,int y2, int x3, int y3)=0;
	virtual FL_EXPORT void polygon(int x1 ,int y1, int x2, int y2, int x3, int y3, int x4, int y4)=0;

	virtual FL_EXPORT void xyline(int x, int y, int x1)=0;
	virtual FL_EXPORT void xyline(int x, int y, int x1, int y2)=0;
	virtual FL_EXPORT void xyline(int x, int y, int x1, int y2, int x3)=0;

	virtual FL_EXPORT void yxline(int x, int y, int y1)=0;
	virtual FL_EXPORT void yxline(int x, int y, int y1, int x2)=0;
	virtual FL_EXPORT void yxline(int x, int y, int y1, int x2, int y3)=0;

	virtual FL_EXPORT void arc(int x, int y, int w, int h, double a1, double a2)=0;
	virtual FL_EXPORT void pie(int x, int y, int w, int h, double a1, double a2)=0;

	virtual FL_EXPORT void begin_points()=0;
	virtual FL_EXPORT void begin_line()=0;
	virtual FL_EXPORT void begin_loop()=0;
	virtual FL_EXPORT void begin_polygon()=0;
	virtual FL_EXPORT void vertex(double x, double y)=0;
	virtual FL_EXPORT void curve(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3)=0;
	virtual FL_EXPORT void arc(double x, double y, double r, double start, double a)=0;
	virtual FL_EXPORT void circle(double x, double y, double r)=0;
	virtual FL_EXPORT void end_points()=0;
	virtual FL_EXPORT void end_line()=0;
	virtual FL_EXPORT void end_loop()=0;
	virtual FL_EXPORT void end_polygon()=0;
	virtual FL_EXPORT void begin_complex_polygon()=0;
	virtual FL_EXPORT void gap()=0;
	virtual FL_EXPORT void end_complex_polygon()=0;
	virtual FL_EXPORT void transformed_vertex(double x, double y)=0;
	virtual FL_EXPORT void translate(double x, double y)=0;

	virtual FL_EXPORT void font(int face, int size)=0;
	virtual FL_EXPORT int font()=0;
	virtual FL_EXPORT int size()=0;
	virtual FL_EXPORT int height()=0;
	virtual FL_EXPORT double width(const char* s)=0;
	virtual FL_EXPORT double width(const char* s, int n)=0;
	virtual FL_EXPORT double width(unsigned int c)=0;
	virtual FL_EXPORT int descent()=0;
	virtual FL_EXPORT void draw(const char* s, int x, int y)=0;
	virtual FL_EXPORT void draw(const char* s, int n, int x, int y)=0;
	virtual FL_EXPORT void rtl_draw(const char* s, int n, int x, int y)=0;
	virtual FL_EXPORT void transformed_draw(const char* s, int n, double x, double y)=0;
	virtual FL_EXPORT void transformed_draw(const char* s, double x, double y)=0;

	virtual FL_EXPORT void draw_image(const uchar*, int,int,int,int, int delta=3, int ldelta=0)=0;
	virtual FL_EXPORT void draw_image_mono(const uchar*, int,int,int,int, int delta=1, int ld=0)=0;
	virtual FL_EXPORT void draw_image(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=3)=0;
	virtual FL_EXPORT void draw_image_mono(Fl_Draw_Image_Cb, void*, int,int,int,int, int delta=1)=0;
	virtual FL_EXPORT void rectf(int x, int y, int w, int h, uchar r, uchar g, uchar b)=0;
	virtual FL_EXPORT ~Fl_Device(){};



};
extern FL_EXPORT Fl_Device *fl;

class Fl_PrintingGroup:public Fl_Group
{
public:
	void FL_EXPORT print(Fl_Device *d);
	
	FL_EXPORT Fl_PrintingGroup(int x,int y,int w,int h, const char * label=0):Fl_Group(x,y,w,h,label){};
};





#endif
