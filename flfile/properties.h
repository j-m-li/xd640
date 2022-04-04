/******************************************************************************* *   $Id: properties.h,v 1.1 2000/08/05 19:11:22 nickasil Exp $
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
#ifndef properties_h
#define properties_h
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Clock.H>
#include <FL/fl_ask.H>
#include <stdlib.h>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Multiline_Output.H>
#include "xd640/Xd6CheckButton.h"
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#define __USE_XOPEN_EXTENDED
#include <ftw.h>

class Properties : public Fl_Window {
public:
	Properties(int w, int h) : Fl_Window(w, h) { };
	int setup(char **lst, int nb, int = 1);
	struct stat file_status;
	int get_file_stat(void);
        char prefix[1024];
        int prelen;
	char buf[2048];
	int ask;
	int exec(const Fl_Window* = 0); 
	
	char *created;
	char *modified;
	char *accessed;
	char *size;
	char *full_name;
	char *name;
	char *directory;
	char *content;
	char *owner;
	char *group;
	char *type;
	static long tsize;
	Fl_Output *file_full_name;
	Fl_Input *file_name;
	Fl_Output *file_type;
	Fl_Output *file_size;
	Fl_Output *file_created;
	Fl_Output *file_modified;
	Fl_Output *file_accessed;
	Fl_Multiline_Output *file_content;
	Fl_Output *nb_files;
	Fl_Output *total_size;

	Xd6CheckButton *file_user_read;
	Xd6CheckButton *file_user_write;
	Xd6CheckButton *file_user_exec;
	Xd6CheckButton *file_uid;
	Xd6CheckButton *file_group_read;
	Xd6CheckButton *file_group_write;
	Xd6CheckButton *file_group_exec;
	Xd6CheckButton *file_gid;
	Xd6CheckButton *file_other_read;
	Xd6CheckButton *file_other_write;
	Xd6CheckButton *file_other_exec;
	Xd6CheckButton *file_sticky;
	Fl_Input *file_owner;
	Fl_Input *file_group;

	char *get_sizes(char **lst, int nb);
	static int fn(const  char  *file, const  struct  stat  *sb,
        	int  flag,  struct FTW *s);
	static void cb_cancel(Fl_Widget *, void *);
	static void cb_ok(Fl_Widget *, void *);

};

#endif
