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


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <libintl.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Group.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include "xd640/Xd6DefaultFonts.h"
#include "xd640/Xd6XmlUtils.h"
#include "PppGui.h"
#include <sys/time.h>

#define _(String) gettext((String))
#define PPP_DEVICE "ppp0"

PppGraph::PppGraph(int X,int Y,int W,int H) : Fl_Widget(X,Y,W,H)
{
	int i;

	for (i = 0; i < 128; i++) {
		out_packs[i] = 0;
		in_packs[i] = 0;
	}
	current = 0;
	max = 5;
}

void PppGraph::packet(int in, int out)
{
	in_packs[current] = in;
	out_packs[current] = out;
	if (max < in) max = in;
	if (max < out) max = out;

	current++;
	if (current > 127) current = 0;
	damage(FL_DAMAGE_ALL);
}

void PppGraph::draw(void)
{
	int i;
	float uw, uh;
	int bottom;
	int lx, ly, cx, cy;
	
	fl_clip(x(), y(), w(), h());

	fl_color(FL_WHITE);
	fl_rectf(x(), y(), w(), h() + 1);
	
	uh = (h() - 1) / (float)max;
	uw = w() / 128.0;
	bottom = y() + h() - 1;

	fl_color(FL_BLUE);
	lx = 0;
	ly = 0;
	for (i = 0; i < 128; i++) {
		cx = x() + (int)(uw * i);
		cy = bottom - (int)(uh * in_packs[i]);
		if (i != current) fl_line(lx, ly, cx, cy);
		lx = cx;
		ly = cy;
	}

	fl_color(FL_GREEN);
	lx = 0;
	ly = 0;
	for (i = 0; i < 128; i++) {
		cx = x() + (int)(uw * i);
		cy = bottom - (int)(uh * out_packs[i]);
		if (i != current) fl_line(lx, ly, cx, cy);
		lx = cx;
		ly = cy;
	}

	fl_color(FL_BLACK);
	fl_line(x() + uw * current, y(), x() + uw * current, y() + h());

	fl_pop_clip();
}

PppLeds::PppLeds(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H)
{
	in = 0;
	out = 0;
}

void PppLeds::packet(int inp, int outp)
{
	in = inp;
	out = outp;
	damage(FL_DAMAGE_ALL);
}

void PppLeds::draw()
{
	fl_color(FL_INACTIVE_COLOR);
	fl_rectf(x(), y(), w(), h());

	if (in > 0) {
		fl_color(FL_BLUE);
	} else {
		fl_color(FL_GRAY);
	}
	fl_pie(x() + 2, y() + h() / 2 - 10, 20, 20, 0, 360);
	
	fl_color(FL_BLACK);
	fl_circle(x() + 12, y() + h() / 2, 10);

	if (out > 0) {
		fl_color(FL_GREEN);
	} else {
		fl_color(FL_GRAY);
	}
	fl_pie(x() + w() - 22, y() + h() / 2 - 10, 20, 20, 0, 360);
	fl_color(FL_BLACK);
	fl_circle(x() + w() - 12, y() + h() / 2, 10);
	fl_rect(x() + 1, y() + h() / 2 - 10, w() - 2, 22);
}

PppIcon::PppIcon(int W, int H) : Fl_Window(W, H)
{
	leds = new PppLeds(0, 0, w(), h());
	border(0);
	//Fl_Window::show();
}


void PppIcon::show()
{
	Fl_Window::show();
}


int PppIcon::handle(int event)
{
	printf("hhhh %d %p\n", event, fl_xevent);
	if (fl_xevent) printf("ee %d %d\n", event, fl_xevent->type);
	if (!event && fl_xevent && fl_xevent->type == ReparentNotify) {
		XUnmapWindow(fl_display, fl_xid(this));
	}
	if (event == FL_SHOW || event == FL_HIDE) {
		if (fl_xevent && (fl_xevent->type == MapNotify ||
			fl_xevent->type == UnmapNotify)) 
		{
			return 1;
		}
		//XUnmapWindow(fl_display, fl_xid(this));
		Fl::flush();
		Fl::check();
		return 1;
	}

	return Fl_Window::handle(event);
}

Fl_Menu_Item *PppGui::prov_menu = NULL;

static int sel_no_hide(const struct dirent *d)
{
        if (d->d_name[0] == '.') return 0;
        return 1;
}

static void cb_exit(Fl_Widget *,void *)
{
	exit(0);
}

PppGui::PppGui(int W, int H) : Fl_Double_Window(W, H)
{
	int nb_provider;
	struct dirent **list;

	label(_("flppp: pppd dialup interface"));
	iconlabel(_("flppp"));
	icon_window = NULL;

	graph = new PppGraph(5, 5, W - 10, 64);
	providers = new Fl_Choice(W / 2 - 80, 80, 160, 25);
	status = new Fl_Multiline_Output(5, 112, W - 10, 65);
	pon = new Fl_Button(5, 190, W / 2 - 10, 40);
	poff = new Fl_Button(W / 2 + 5, 190, W / 2 - 10, 40);

	status->value(_("Disconnected."));
	pon->label(_("Connect"));
	pon->callback(cb_pon, this);
	poff->label(_("Disconnect"));
	poff->callback(cb_poff, this);

	poff->deactivate();


	nb_provider = scandir("/etc/ppp/peers", &list, sel_no_hide, alphasort);
	if (nb_provider < 1) {
		status->value(_("pppd is not yet configured, please run"
			" pppconfig before flppp."));
		return;
	}
	prov_menu = new Fl_Menu_Item[nb_provider + 1];
	prov_menu[nb_provider].text = NULL;

	while (nb_provider > 0) {
		nb_provider--;
		prov_menu[nb_provider].text = strdup(list[nb_provider]->d_name);
		prov_menu[nb_provider].shortcut_ = 0;
		prov_menu[nb_provider].callback_ = NULL;
		prov_menu[nb_provider].user_data_ = NULL;
		prov_menu[nb_provider].flags = 0;
		prov_menu[nb_provider].labeltype_ = 0;
		prov_menu[nb_provider].labelcolor_ = 0;
		prov_menu[nb_provider].labelfont_ = 0;
		prov_menu[nb_provider].labelsize_ = 0;
		free(list[nb_provider]);
	}
	free(list);
	providers->menu(prov_menu);
	end();
	
//	icon_window = new PppIcon(48, 48);
	callback(cb_exit);
}


void PppGui::cb_pon(Fl_Widget *btn, void *data) 
{
	PppGui *self = (PppGui*) data;
	char buf[1024];
	struct stat s;
	
	if (!self->providers->mvalue()) return;
	self->poff->activate();
	self->pon->deactivate();

	if (stat("/var/log/messages", &s)) {
		self->log_offset = 0;
	} else {
		self->log_offset = s.st_size;
	}
	self->log_len = 0;
	Fl::add_timeout(1, cb_update, self);

	snprintf(buf, 1024, "/usr/sbin/pppd call %s &", 
		self->providers->mvalue()->label());
	system(buf);

	snprintf(buf, 1024, "netscape -remote 'raise()' || netscape &");
	system(buf);

	self->connect_time = 0;
}

void PppGui::cb_poff(Fl_Widget *btn, void *data)
{
	PppGui *self = (PppGui*) data;
	char buf[1024];

	Fl::remove_timeout(cb_update, self);

	if (!self->providers->mvalue()) return;
	self->poff->deactivate();
	self->pon->activate();
	snprintf(buf, 1024, "kill -TERM `/sbin/pidof pppd` 2>/dev/null"); 
	system(buf);
	snprintf(buf, 1024, "kill -TERM `/bin/pidof pppd` 2>/dev/null"); 
	system(buf);
	self->status->value(_("Disconnected."));
}

void PppGui::cb_update(void *data)
{
	PppGui *self = (PppGui*) data;

	if (!self->is_connected()) {
		Fl::add_timeout(1, cb_update, self);
		self->update_message();
	} else {
		self->statistic();
		Fl::add_timeout(.5, cb_update, self);
	}
}

void PppGui::update_message()
{
	int fd;
	char buf[1000];
	int len;

	fd = open("/var/log/messages", O_RDONLY);
	if (fd < 0) return;
	lseek(fd, log_offset, SEEK_SET);
	len = read(fd, buf, 1000);
	close(fd);
	if (len > 0) {
		int i = 0;
		char *ptr;

		log_offset += len;

		if (log_len + len > 1023) {
			int rest = 1023 - len;
			int off = log_len - rest;
			while (i < rest) {
				log_buffer[i] = log_buffer[i + off];
				i++;
			}
			log_len = rest;
		}
	
		i = 0;
		while (i < len) {
			log_buffer[log_len] = buf[i];
			i++;
			log_len++;
		}

		log_buffer[log_len] = '\0';		
		status->value(log_buffer);

		/*
		 *  Is the connection terminated ?
		 */
		ptr = strstr(log_buffer, ": Exit.\n");
		while (ptr > log_buffer && *ptr != '\n') {
			if (!strncmp(ptr, " pppd[", 6)) {
				cb_poff(NULL, this);
				return;
			}
			ptr--;
		}

		/*
		 *  has the chat connection failed ?
		 */
		ptr = strstr(log_buffer, ": Failed\n");
		while (ptr > log_buffer && *ptr != '\n') {
			if (!strncmp(ptr, " chat[", 6)) {
				cb_poff(NULL, this);
				return;
			}
			ptr--;
		}

	}
}

int PppGui::is_connected()
{
	int fd;
	char buf[1024];
	int len;

	fd = open("/proc/net/route", O_RDONLY);
	if (fd < 0 ) return 0;
	len = read(fd, buf, 1023);
	close(fd);
	if (len < 1) return 0;
	buf[len] = '\0';
	if (!strstr(buf, PPP_DEVICE)) return 0;
	return 1;
}

void PppGui::statistic()
{
	char *ptr;
	int fd;
	char buf[1024];
	long rx = -1;
	long tx = -1;
	float spi = 0;
	float spo = 0;
	int len;

	fd = open("/proc/net/dev", O_RDONLY);
	if (fd < 0 ) return;
	len = read(fd, buf, 1023);
	close(fd);
	if (len < 1) return;
	buf[len] = '\0';
	ptr = strstr(buf, PPP_DEVICE);

	if (ptr) {
		long dt;
		int ct;
		struct timeval tv;

		gettimeofday(&tv, NULL);
		sscanf(ptr, 
			"%*[^:]:%ld %*d %*d %*d %*d %*d %*d %*d %ld", &rx, &tx); 
		
		dt = (tv.tv_sec - last_time.tv_sec) * 1000;
		dt += (tv.tv_usec - last_time.tv_usec) / 1000;
		spi = (rx - last_rx) / dt;
		spi = spi / 1024.0 * 1000.0;
		spo = (tx - last_tx) / dt;
		spo = spo / 1024.0 * 1000.0;

		if (connect_time == 0) connect_time = tv.tv_sec;
		ct = (int)(tv.tv_sec - connect_time);

		snprintf(log_buffer, 1024,
			"Receive:  %ld kB (%.1f kB/s)\n"
			"Transmit: %ld kB (%.1f  kB/s)\n"
			"Duration: %d min %d sec\n",
				rx / 1024, spi, tx / 1024, spo,
				ct / 60, ct % 60);

		last_rx = rx;
		last_tx = tx;
		last_time.tv_sec = tv.tv_sec;
		last_time.tv_usec = tv.tv_usec;
		graph->packet((int)spi, (int)spo);
		icon_window->leds->packet((int)spi, (int)spo);
		
	}
	log_len = 0;
	status->value(log_buffer);
}

#if 0
	FILE *fp;
	char buf[1024];
	int len = 0;
	int rx = -1;
	int tx = -1;

	fp = popen("netstat -i", "r");
	if (fp) {
		len = fread(buf, 1, 1023, fp);
		pclose(fp);
	}
	
	if (len > 10) {
		char *ptr;
		char *foo;

		buf[len] = '\0';
		ptr = strstr(buf, "eth0");
		if (ptr) {
			sscanf(ptr, 
				"%*s %*d %*d %d %*d %*d %*d %d ", &rx, &tx); 
			printf("RX %d TX %d\n %s", rx, tx, ptr);

			log_len = 0;
			status->value(log_buffer);
			return 1;
		}
	}
	return 0;
}
#endif

//
// End of "$Id:  $".
//
