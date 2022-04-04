/******************************************************************************
 *   "$Id: $"
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
