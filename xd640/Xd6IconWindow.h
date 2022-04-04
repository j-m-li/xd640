/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000  O'ksi'D
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
