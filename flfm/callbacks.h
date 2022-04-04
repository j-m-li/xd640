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


#ifndef callbacks_h
#define callbacks_h

#include <FL/Fl_Widget.H>
#include "gui.h"
#include "xd640/Xd6ConfigFile.h"

struct states_struct {
	char sort_size;
	char sort_type;
	char sort_name;
	char view_icon;
	char view_tree;
	char view_detail;
	char show_hide;
	char *url;
	char *newurl;
	char *status;
	char *history[10];
};

extern struct states_struct StatesValues;
extern GUI *gui;
extern Xd6ConfigFile *cfg;
extern Xd6ConfigFileSection *cfg_sec;

char *get_selected_urls();

void cb_rescan(Fl_Widget*, void*);
void cb_newdir(Fl_Widget*, void*);
void cb_pref(Fl_Widget*, void*);
void cb_exit(Fl_Widget*, void*);
void cb_sort(Fl_Widget*, void*);
void cb_about(Fl_Widget*, void*);
void cb_help(Fl_Widget*, void*);

void cb_open(Fl_Widget*, void*);
void cb_exec(Fl_Widget*, void*);
void cb_open_width(Fl_Widget*, void*);
void cb_open_dir(Fl_Widget*, void*);
void cb_change_dir(Fl_Widget*, void*);
void cb_delete(Fl_Widget*, void*);
void cb_properties(Fl_Widget*, void*);

void cb_move_link_copy(Fl_Widget*, void*);

void cb_back(Fl_Widget*, void*);
void cb_up(Fl_Widget*, void*);
void cb_new_fm(Fl_Widget*, void*);
void cb_loc_input(Fl_Widget*, void*);

void cb_directory(void*);

#endif
