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
#include "FL/Fl_Check_Button.H"
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
#ifndef __USE_XOPEN_EXTENDED
#define __USE_XOPEN_EXTENDED
#endif
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

	Fl_Check_Button *file_user_read;
	Fl_Check_Button *file_user_write;
	Fl_Check_Button *file_user_exec;
	Fl_Check_Button *file_uid;
	Fl_Check_Button *file_group_read;
	Fl_Check_Button *file_group_write;
	Fl_Check_Button *file_group_exec;
	Fl_Check_Button *file_gid;
	Fl_Check_Button *file_other_read;
	Fl_Check_Button *file_other_write;
	Fl_Check_Button *file_other_exec;
	Fl_Check_Button *file_sticky;
	Fl_Input *file_owner;
	Fl_Input *file_group;

	char *get_sizes(char **lst, int nb);
	static int fn(const  char  *file, const  struct  stat  *sb,
        	int  flag,  struct FTW *s);
	static void cb_cancel(Fl_Widget *, void *);
	static void cb_ok(Fl_Widget *, void *);

};

#endif
