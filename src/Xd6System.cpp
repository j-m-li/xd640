/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2001  O'ksi'D
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
#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Pixmap.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include "Xd6IconWindow.h"
#include <X11/extensions/shape.h>
#include "Xd6System.h"

#include "xpm/img1.xpm"
#include "xpm/img2.xpm"
#include "xpm/img3.xpm"
#include "xpm/img4.xpm"

#define _(String) gettext((String))

static Window win[4] = {0, 0, 0, 0};
static int c_win = 0;
static int count = 0;
static Window create = 0;
static int x_pos, y_pos;
static Window create_win (char **name);

static int run_cmd(const char *cmd)
{
        int pid = fork();
	count = 0;
	create = 0;
        if (pid == -1) {
                fl_alert(_("Cannot create new process !"));
                Fl::flush();
                return -1;
        }
        if (pid == 0) {
		FILE *fp;
		fp = popen(cmd, "w");
                _exit(pclose(fp));
        }
        return pid;
}

static void hide_all()
{
	XUnmapWindow(fl_display, win[0]);
	XUnmapWindow(fl_display, win[1]);
	XUnmapWindow(fl_display, win[2]);
	XUnmapWindow(fl_display, win[3]);
}

void Xd6SystemHide()
{
	hide_all();
}

static void cb_timeout(void *data)
{
	int pid = *((int*) &data);
	int ret;
	int status;
	XWindowAttributes a;

	ret = waitpid(pid, &status, WNOHANG);
	c_win++;
	count++;
	if (c_win > 3) c_win = 0;
	hide_all();
	
	if (create > 0) {
		a.map_state = 0;
		if (XGetWindowAttributes(fl_display, create, &a)) {
			count = 10000;
			create = 0;
			return;
		}
		if (a.map_state == IsViewable) {
			count = 10000;
			create = 0;
			return;
		}
	}
	if (ret == pid) {
		//fl_alert(_("Error: cannot execute command."));
		count = 10000;
	} else if (ret == -1) {
		//Fl::remove_timeout((Fl_Timeout_Handler)cb_timeout, data);
		//count = 10000;
		Fl::add_timeout(0.2, (Fl_Timeout_Handler)cb_timeout, data);
		XMapWindow(fl_display, win[c_win]);
	} else {
		Fl::add_timeout(0.2, (Fl_Timeout_Handler)cb_timeout, data);
		XMapWindow(fl_display, win[c_win]);
	}
	//Fl::flush();
	//Fl::check();
}

int Xd6System(const char *cmd)
{
	int pid = 0;
	
	if (!win[0]) {
		fl_open_display();
		win[0] = create_win(img1_xpm);
		win[1] = create_win(img2_xpm);
		win[2] = create_win(img3_xpm);
		win[3] = create_win(img4_xpm);
	}
	Window root;
	Window c; int cx,cy; unsigned int mmask;
	root = RootWindow(fl_display, fl_screen);
	XQueryPointer(fl_display,root,&root,&c,&x_pos,&y_pos,&cx,&cy,&mmask);
	x_pos -= 32;
	y_pos -= 32;
	XMoveWindow(fl_display, win[0], x_pos, y_pos);
	XMoveWindow(fl_display, win[1], x_pos, y_pos);
	XMoveWindow(fl_display, win[2], x_pos, y_pos);
	XMoveWindow(fl_display, win[3], x_pos, y_pos);
	Fl::flush();
	Fl::check();
	pid = run_cmd(cmd);
	if (pid > 0) {
		Fl::add_timeout(0.1, (Fl_Timeout_Handler)cb_timeout,(void*)pid);
	} else {
		return -1;
	}
	for (;;) {
		if (count > 50) {
			Fl::remove_timeout((Fl_Timeout_Handler)cb_timeout, 
				(void*)pid);
			break;
		}
		Fl::wait();
		if (fl_xevent->type == CreateNotify && count > 1) {
			//create = ((XCreateWindowEvent *)fl_xevent)->window;
			Fl::remove_timeout((Fl_Timeout_Handler)cb_timeout, 
				(void*)pid);
			break;
			
		}
	}
	hide_all();
	return 0;
}

extern uchar **fl_mask_bitmap;

static Window create_win (char **data)
{
        unsigned long valuemask;
        XSetWindowAttributes attributes;
        Window win;
        Pixmap pix;
        Pixmap mask;
        int pw, ph;
        unsigned char *bitmap = 0;
        long prop[5] = {0, 1, 1, 0, 0};
        Atom _motif_wm_hints;
	Window root;
	
	root = RootWindow(fl_display, fl_screen);
        valuemask = CWColormap |CWEventMask | CWOverrideRedirect | CWSaveUnder;
        attributes.override_redirect = 1;
        attributes.save_under = 1;
        attributes.event_mask = VisibilityChangeMask;
        attributes.colormap = fl_colormap;
        win = XCreateWindow(fl_display, root,
                0, 0, 64, 64, 0, fl_visual->depth, InputOutput,
                fl_visual->visual, valuemask, &attributes);

        prop[0] |= 2; // MWM_HINTS_DECORATIONS
        prop[2] = 0; // no decorations
        _motif_wm_hints = XInternAtom(fl_display,"_MOTIF_WM_HINTS",0);
        XChangeProperty(fl_display, win , _motif_wm_hints, _motif_wm_hints,
                  32, 0, (unsigned char *)prop, 5);


        if (!fl_gc) fl_gc = XCreateGC(fl_display, win, 0, 0);
        fl_window = win;
        fl_measure_pixmap(data, pw, ph);
        if (pw < 1 || ph < 1) return win;
        pix = fl_create_offscreen(pw, ph);
        fl_begin_offscreen(pix);
        fl_mask_bitmap = &bitmap;
        fl_draw_pixmap(data, 0, 0, FL_BLACK);
        fl_mask_bitmap = 0;
        fl_end_offscreen();
        XSetWindowBackgroundPixmap(fl_display, win, pix);
        if (bitmap) {
                mask = XCreateBitmapFromData(fl_display, fl_window,
                                (const char*)bitmap, (pw+7)&-8, ph);
                XShapeCombineMask(fl_display, win, ShapeBounding, 0, 0,
                        mask, ShapeSet);
                XFreePixmap(fl_display, mask);
        }
        return win;
}


/*
 * "$Id: $"
 */
