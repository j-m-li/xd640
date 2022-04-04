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
#ifndef gui_h
#define gui_h

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Double_Window.H>
#include <sys/time.h>

class PppGraph : public Fl_Widget {
public:
	int out_packs[128];
	int in_packs[128];
	int current;
	int max;

	PppGraph(int X, int Y,int W, int H);
	void packet(int in, int out);
	void draw(void);
};

class PppLeds : public Fl_Widget {
public:
	int in;
	int out;

        PppLeds(int X, int Y, int W, int H);
        void draw(void);
	void packet(int inp, int outp);
};

class PppIcon : public Fl_Window {
public:
	PppLeds *leds;
	PppIcon(int W, int H);
	virtual int handle(int event);
	void show(void);
};

class PppGui : public Fl_Double_Window {
public:
	PppGraph *graph;
	Fl_Choice *providers;
	static Fl_Menu_Item *prov_menu;
	Fl_Multiline_Output *status;
	Fl_Button *pon;
	Fl_Button *poff;
	PppIcon *icon_window;
	Fl_Font font;
	int size;
	long log_offset;
	char log_buffer[1024];
	int log_len;
	struct timeval last_time;
	long last_rx;
	long last_tx;
	long connect_time;

	PppGui(int W, int H);
	static void cb_pon(Fl_Widget *btn, void *data);
	static void cb_poff(Fl_Widget *btn, void *data);
	static void cb_update(void *data);
	void update_message(void);
	void statistic(void);
	int is_connected(void);
};


#endif
