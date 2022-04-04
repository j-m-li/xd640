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
