/*******************************************************************************
 *  "$Id: $"
 *
 *  	Xd6IconWindow for the Fast Light Environement.
 *
 *   		Copyright (c) 2000 O'ksi'D
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

#include "Xd6IconWindow.h"
#include <FL/fl_draw.H>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <FL/x.H>
#include <X11/extensions/shape.h>

extern uchar **fl_mask_bitmap;
extern Atom fl_XdndAware;

static char * default_xpm[] = {
"16 16 3 1",
"       c None",
".      c #000000000000",
"X      c #FFFFFFFF0000",
"  ............  ",
"  .XXXXXXXXXX.. ",
"  .XX.....XXX.. ",
"  .XXXXXXXXXX.. ",
"  .XX.......X.. ",
"  .XXXXXXXXXX.. ",
"  .XX..XXXXXX.. ",
"  .XXXXXXXXXX.. ",
"  .XX.....XXX.. ",
"  .XXXXXXXXXX.. ",
"  .XX....XXXX.. ",
"  .XXXXXXXXXX.. ",
"  .XX......XX.. ",
"  .XXXXXXXXXX.. ",
"  ............. ",
"   ............ "};

/*
 *  read xpm_file an create a shaped window
 */
Xd6IconWindow::Xd6IconWindow(const char *xpm_file, VirtualWindow *sister, 
	int W, int H) : Fl_Window(0, 0, W, H)
{
	struct stat s;
	char **d = NULL;
	int fd;

	pix = 0;
	mask = 0;
	pix_data = NULL;
	flwin = sister;
	win = 0;
	
	if (!stat(xpm_file, &s) && s.st_size > 10) {
		pix_data = (char*) malloc(s.st_size);
		fd = open(xpm_file, O_RDONLY);
		if (fd < 0) return;

		read(fd, pix_data, s.st_size);	
		close(fd);
		d = parse_xpm(pix_data, s.st_size);
	}
	if (d) {
		win = create_window(d, &pix, &mask);
	} else {
		win = create_window(default_xpm, &pix, &mask);
	}

}

Xd6IconWindow::~Xd6IconWindow()
{
	free(pix_data);
	XFreePixmap(fl_display, mask);
	XFreePixmap(fl_display, pix);
}

/*
 *  pass event to the sister window
 */
int Xd6IconWindow::handle(int e)
{
	switch (e) {
	case FL_ENTER:
		fl_cursor(FL_CURSOR_ARROW);
	case 0:
	case FL_SHOW:
	case FL_FOCUS:
	case FL_UNFOCUS:
	case FL_LEAVE:
	case FL_MOVE:
		return Fl_Window::handle(e);
	case FL_HIDE:
		if (shown()) {
			if (visible()) {
				Fl_Widget* p = parent();
				for (;p && p->visible();p = p->parent()) {};
				if (p && p->type() >= FL_WINDOW) return 1;
				XUnmapWindow(fl_display, fl_xid(this));
			}
		}
		return 1;
	default:
		break;
	}
	if (flwin) {
		return flwin->handle(e);
	} else {
		return Fl_Window::handle(e);
	}
}


void Xd6IconWindow::draw()
{
	// draw nothing !  keep the background pixmap.
}

void Xd6IconWindow::show()
{
	return Fl_Window::show();
}

void Xd6IconWindow::hide()
{
	return Fl_Window::hide();
}

/*
 *  parse a buffer which is a copy of an XPM file and return an array
 *  of pointer to the begining of each xpm data strings
 */
char **Xd6IconWindow::parse_xpm(char *d, int l)
{
        char **p = NULL;
        int in_quote = 0;
        int r = 0;
        int i = 0;


        while (i < l) {
                // skip comments
                if (!in_quote && d[i] == '/' && d[i + 1] == '*') {
                        i += 2;
                        while (i < l) {
                                if (d[i] == '*' && d[i + 1] == '/') {
                                        i += 2;
                                        break;
                                }
                                i++;
                        }
                }

                if (!in_quote && d[i] == '"') {
                        // string beginning
                        p = (char**) realloc(p, sizeof(char*) * (r + 1));
                        p[r] = d + i + 1;
                        in_quote = 1;
                } else if (in_quote && d[i] == '"') {
                        // string end
                        d[i] = '\0';
                        in_quote = 0;
                } else if (!in_quote && d[i] == ',') {
                        // end of line
                        r++;
                }
                i++;
        }

        return p;
}

static const int XEventMask =
ExposureMask|StructureNotifyMask
|KeyPressMask|KeyReleaseMask|KeymapStateMask|FocusChangeMask
|ButtonPressMask|ButtonReleaseMask
|EnterWindowMask|LeaveWindowMask
|PointerMotionMask;

/*
 *  create a shaped X-Window using the pixmap contained in data
 */
Window Xd6IconWindow::create_window(char **data, Pixmap *pix, 
	Pixmap *mask)
{
	Window _sw;
	Window win = 0;
	Window root;
	int pw, ph;
	unsigned char *bitmap = 0;
        unsigned long valuemask;
        XSetWindowAttributes attributes;
        long prop[5] = {0, 1, 1, 0, 0};
        Atom _motif_wm_hints;

	fl_open_display();

	root = RootWindow(fl_display, fl_screen);

        valuemask = CWColormap |CWEventMask | CWOverrideRedirect | CWSaveUnder;
        attributes.override_redirect = 1;
        attributes.save_under = 1;
        attributes.event_mask = XEventMask;
        attributes.colormap = fl_colormap;

	win = XCreateWindow(fl_display, root,
                -48, -48, w(), h(), 0, fl_visual->depth, InputOutput,
                fl_visual->visual, valuemask, &attributes);
	
	if (!win) return 0;

	Fl_X::set_xid(this, win);

	make_current();

        prop[0] |= 2; // MWM_HINTS_DECORATIONS
        prop[2] = 0; // no decorations

        _motif_wm_hints = XInternAtom(fl_display,"_MOTIF_WM_HINTS",0);

        XChangeProperty(fl_display, win , _motif_wm_hints, _motif_wm_hints,
                  32, 0, (unsigned char *)prop, 5);

	fl_measure_pixmap(data, pw, ph);
	if (pw < 1 || ph < 1) return win;
	*pix = fl_create_offscreen(pw, ph);
	_sw = fl_window;
	fl_window = *pix;
	fl_mask_bitmap = &bitmap;
	fl_draw_pixmap(data, 0, 0, FL_BLACK);
	fl_mask_bitmap = 0;
	if (bitmap) {
		*mask = XCreateBitmapFromData(fl_display, fl_window,
				(const char*)bitmap, (pw+7)&-8, ph);
        	XShapeCombineMask(fl_display, win, ShapeBounding, 0, 0, 
			*mask, ShapeSet);
	} 
	fl_end_offscreen();

        XSetWindowBackgroundPixmap(fl_display, win, *pix);

	// D'n'D
	int version = 4;
	XChangeProperty(fl_display, fl_window, fl_XdndAware,
		 XA_ATOM, sizeof(int)*8, 0, (unsigned char*)&version, 1);
	return win;
}

void set_wm_icon(char *const*data, Fl_Window *win)
{
	Pixmap pix;
	Pixmap pix_mask;
	XWMHints *hints;
	int w, h;
	unsigned char *bitmap = 0;

	win->make_current();
	fl_measure_pixmap(data, w, h);
	if (w < 1 || h < 1) return;

        hints = XGetWMHints(fl_display, fl_xid(win));
	pix = fl_create_offscreen(w, h);
	fl_begin_offscreen(pix);
	fl_mask_bitmap = &bitmap;
	fl_draw_pixmap(data, 0, 0, FL_BLACK);
	fl_mask_bitmap = 0;
	if (bitmap) {
		pix_mask = XCreateBitmapFromData(fl_display, fl_window,
				(const char*)bitmap, (w+7)&-8, h);
        	hints->icon_mask = pix_mask;
	}
	fl_end_offscreen();
        hints->icon_pixmap = pix;
        hints->flags = IconPixmapHint | IconMaskHint; // Xd6IconWindowHint
        XSetWMHints(fl_display, fl_xid(win), hints);
}
