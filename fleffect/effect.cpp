/******************************************************************************
 *   "$Id: launch.cpp,v 1.1 2000/08/05 19:11:21 nickasil Exp $"
 *
 *   This file is part of the FLE project.
 *
 *                 Copyright (c) 2000  O'ksi'D
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

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Pixmap.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "xd640/Xd6IconWindow.h"
#include <X11/extensions/shape.h>

extern char **environ;

static int loops = 15;

static pid_t pid = 0;

static Window create_win (char **name);
static Window root;
static int x_pos = 0, y_pos = 0;
extern int fl_wait(double time_to_wait);

#include "img1.xpm"
#include "img2.xpm"
#include "img3.xpm"

static int run_cmd(char *cmd, char **argv)
{
	pid = fork();
	if (pid == -1) {
		fl_alert("Cannot create new process !");
		Fl::flush();
		return -1;
	}
	if (pid == 0) {
		usleep(200000);
		execvp(cmd, argv);
		_exit(-126);
	}
	return 1;	
}

int main (int argc, char **argv)
{
	int status = 0;
	int index = 0;
  	Window win[20], old;
	int nb = 0; 
	int nb_pix = 3;
  	Window c; int cx,cy; unsigned int mmask;
	
	Fl::args(argc, argv, index);

	fl_open_display();

  	root = RootWindow(fl_display, fl_screen);
  	XQueryPointer(fl_display,root,&root,&c,&x_pos,&y_pos,&cx,&cy,&mmask);
	x_pos -= 32;
	y_pos -= 32;

	win[0] = create_win(img1_xpm);
	win[1] = create_win(img2_xpm);
	win[2] = create_win(img3_xpm);
	Fl::flush();

	XMapWindow(fl_display, win[0]);
	Fl::flush();

	if (index < argc) {
		int i = argc - index + 1;
		char **arg = (char **)malloc(i * sizeof(char*));
		i--;
		arg[i] = NULL;
		while (i) {
			i--;
			arg[i] = argv[i + index];
		}
		arg[0] = strrchr(argv[index], '/');
		if (!arg[0]) {
			arg[0] = argv[index];
		} else {
			arg[0]++;
		}
		status = run_cmd(argv[index], arg);
		free(arg);
		if (status == -1) return -1;
	}


	while (--loops) {

		old = win[nb];
		if (++nb == nb_pix) nb=0;
		XUnmapWindow(fl_display, old);
		XMapWindow(fl_display, win[nb]);
		Fl::flush();
		Fl::check();
		usleep(150000);
		if (waitpid(pid, &status, WNOHANG) == pid) {
			waitpid(pid, &status, WUNTRACED); /* anti zombie code */
			XUnmapWindow(fl_display, win[nb]);
			Fl::flush();
			Fl::check();
			if (WIFEXITED(status) && 
				(char)WEXITSTATUS(status) == -126) 
			{
				fl_alert("Cannot execute \"%s\"", argv[index]);
				status = -1;
			}
			loops = 1;
		}
	}
	XUnmapWindow(fl_display, win[nb]);
	XDestroyWindow(fl_display, win[0]);
	XDestroyWindow(fl_display, win[1]);
	XDestroyWindow(fl_display, win[2]);
	Fl::flush();
    	return status;
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

  	valuemask = CWColormap |CWEventMask | CWOverrideRedirect | CWSaveUnder;
	attributes.override_redirect = 1;
	attributes.save_under = 1;
   	attributes.event_mask = VisibilityChangeMask;
	attributes.colormap = fl_colormap;

	win = XCreateWindow(fl_display, root,
		x_pos, y_pos, 64, 64, 0, fl_visual->depth, InputOutput, 
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

//
// "$Id: $"
//
