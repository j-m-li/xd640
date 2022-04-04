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

#include "gui.h"

void cb_new(Fl_Widget*, void*);
void cb_open(Fl_Widget*, void*);
void cb_save(Fl_Widget*, void*);
void cb_save_as(Fl_Widget*, void*);
void cb_print(Fl_Widget*, void*);
void cb_pref(Fl_Widget*, void*);
void cb_exit(Fl_Widget*, void*);

void cb_cut(Fl_Widget*, void*);
void cb_copy(Fl_Widget*, void*);
void cb_paste(Fl_Widget*, void*);
void cb_find(Fl_Widget*, void*);
//void cb_zoom(Fl_Widget*, void*);

void cb_picture(Fl_Widget*, void*);
void cb_spell(Fl_Widget*, void*);
void cb_keyboard(Fl_Widget*, void*);

void cb_about(Fl_Widget*, void*);
void cb_help(Fl_Widget*, void*);

#endif

/*
 *  End of : "$Id: $"
 */

