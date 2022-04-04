/******************************************************************************* *   $Id: link.h,v 1.1 2000/08/05 19:11:22 nickasil Exp $
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
 *   Author : Jean-Marc Lienher <nickasil@linuxave.net>
 *
 ******************************************************************************/
#ifndef link_h
#define link_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#define __USE_XOPEN_EXTENDED
#include <ftw.h>

class Link : public Fl_Window {
public:
	static char *directory;
	static int dirlen;
        static char **list;
        static int nb_item;
        static int actual;
        static int ask;
        static Link *self;
        Fl_Output *file;
        Fl_Button *cancel;
        Fl_Group *grp;
        Fl_Box *text;

	Link(int w, int h) : Fl_Window(w, h) { };
	int setup(char **, int, char*, int = 1);
        static void cb_cancel(Fl_Widget *, void *);
        static int fn(const  char  *file, const  struct  stat  *sb,
                        int  flag,  struct FTW *s);
        int exec(const Fl_Window* = 0);
};

#endif
