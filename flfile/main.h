/******************************************************************************
 *   "$Id: main.h,v 1.1 2000/08/05 19:11:22 nickasil Exp $"
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
#ifndef main_h
#define main_h
#include "properties.h"
#include "unlink.h"
#include "link.h"
#include "copy.h"
#include "move.h"
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Image.H>
#include <FL/fl_draw.h>
#include <FL/Fl_Menu_Button.H>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <FL/x.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <errno.h>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Image.H>
#include <FL/fl_draw.h>
#include <FL/fl_ask.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <FL/x.H>

enum {
	NONE,
	EXEC,
	DELETE,
	OPEN,
	COPY,
	LINK,
	MOVE,
	PROP,
};

/*** main.cpp ***/
extern int action;
extern int verbose;
extern char *target;
extern char host[256];
extern int nburl;
extern char **urls;

extern char *directory;
extern struct dirent **namelist;
extern int nb_dirent;

void execute(const char *, char **, int);
const char *get_mime_exec(const char *);
char *url2filename(char *url);

/*** ********* ***/
#endif

