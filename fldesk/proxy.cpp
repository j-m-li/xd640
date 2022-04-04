/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2002  O'ksi'D
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

	position(100, 100);
	Fl::flush();
	Fl::check();
	

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
	XUnmapWindow(fl_display, fl_xid(this));
        switch(e) {
	case FL_SHOW:
		return 1;
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

