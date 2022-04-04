/******************************************************************************* *   $Id: proxy.cpp,v 1.1 2000/08/05 19:11:21 nickasil Exp $
 *
 *   This file is part of the Xd640 project.
 *
 *                 Copyright (c) 2000-2002  O'ksi'D
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
 *   Author : Jean-Marc Lienher (http://www.oksid.ch)
 *
 ******************************************************************************/

#include "proxy.h"
#include <FL/Fl.H>
#include <FL/x.H>
#include <stdio.h>

Atom XA_XdndProxy;
extern Window fl_dnd_source_window;

void Proxy::init()
{
	Window my_win;
	Window ProxyTarget;

	show();
	position(10000, 100);
	Fl::flush();
	Fl::check();
	
	XUnmapWindow(fl_display, fl_xid(this));

	Fl::selection(*this, " ", 1);
        my_win = fl_xid(this);

	XA_XdndProxy = XInternAtom(fl_display, "XdndProxy", 0);
        ProxyTarget = RootWindow(fl_display, fl_screen);
        XChangeProperty(fl_display, my_win, XA_XdndProxy,
                XA_WINDOW, 32, PropModeReplace,
                (const unsigned char *) &my_win, 1);
        XChangeProperty(fl_display, ProxyTarget, XA_XdndProxy,
                XA_WINDOW, 32, PropModeReplace,
                (const unsigned char *) &my_win, 1);

}


int Proxy::handle(int e)
{
        switch(e) {
        case FL_DND_ENTER:
        case FL_DND_DRAG:
		Fl::belowmouse_ = this;
	case FL_DND_LEAVE:
		return 1;
        case FL_DND_RELEASE:
		Fl::belowmouse_ = this;
		is_drop = 1;
                return 1;
	case FL_DROP:
		drop_func();
		return 1;
	}
	return Fl_Window::handle(e);
}

void Proxy::drop_func()
{
	if (!fl_find(fl_dnd_source_window)) {
		do_callback();
	}
}

