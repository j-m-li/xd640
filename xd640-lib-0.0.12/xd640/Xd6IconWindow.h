/*******************************************************************************
 *  "$Id: $"
 *
 *  	Xd6IconWindow definitions for the Fast Light Environement.
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

#ifndef Xd6IconWindow_h
#define Xd6IconWindow_h

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Pixmap.H>

class VirtualWindow : public Fl_Window {
public:
	VirtualWindow(int x, int y, int w, int h) : Fl_Window(x, y, w, h) {;}
	virtual int handle(int e) { return Fl_Window::handle(e); }
};

class Xd6IconWindow : public Fl_Window {
public:
#ifdef WIN32

#else
	Pixmap pix;
	Pixmap mask;
#endif
	char *pix_data;
	VirtualWindow *flwin;
	Window win;

	Xd6IconWindow(const char *xpm_file, VirtualWindow *window, int W, int H);
	~Xd6IconWindow();
	void show();
	void hide();
	void draw();
	int handle(int e);
	static char **parse_xpm(char *d, int l);
#ifdef WIN32
#else
	Window create_window(char **d, Pixmap *p, Pixmap *m);
#endif
};

void set_wm_icon(char *const*p, Fl_Window *w);

#endif // !Xd6IconWindow_h

/*
 *  End of "$Id:  $".
 */
