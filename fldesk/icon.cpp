/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2002  O'ksi'D
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


#include "icon.h"
#include <libintl.h>
#include <FL/x.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include <FL/filename.H>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <strings.h>
#include <fcntl.h>
#include <ctype.h>
#include "xd640/Xd6System.h"

#define _(String) gettext((String))

Fl_Menu_ *Icon::menu = NULL;
char *Icon::buffer = NULL;
extern Window fl_message_window;

Icon::Icon(Xd6ConfigFile *c, Xd6ConfigFileSection *s, Xd6ConfigFileSection *p,
	const char *f, const char *n) : 
		VirtualWindow(100000, 10000, 32, 32)
{
	displayed = 0;
	is_drop = 0;
	dragging = 0;
	clear_border();
	url = NULL;
	ic1 = NULL;
	ic2 = NULL;
	box = NULL;
	cfg = c;
	sec = s;
	pos = p;
	pos_file = f;
	launch_name = strdup(n);
	type = TYPE_None;
	font = (Fl_Font) 0;
	size = 12;
	data = NULL;
	items = new Fl_Menu_Item[8];
	items->text = NULL;
	if (!menu) {
		menu = new Fl_Menu_Button(0,0,0,0);
		menu->selection_color(137);
	}
}

Icon::~Icon()
{
	delete(ic1);
	delete(ic2);
	delete(sec);
	delete(items);
	free(url);
	free(launch_name);
}

extern const XEvent* fl_xevent;

int Icon::handle(int e) {
        static int bx = 0, by = 0;
        static int px = -10, py = -10;
        static int button1 = 0;
	static char was_dragging = 0;
	const Fl_Menu_Item *m;
        char val[25];
	static char beenhere = 0;

	if (!beenhere) {
		beenhere = 1;
		Fl::selection(*this, " ", 1);
	}
	if (!e) return 0;
        if (e == FL_PUSH) {
		show();
		button1 = (Fl::event_button() == 1 || Fl::event_button() == 2);
	}
        if (button1) {
		Window root;
		Window c; int cx,cy, xx, yy; unsigned int mask;
		switch(e) {
                case FL_DRAG:
			if (px - 5 >= Fl::event_x_root() ||
				px + 5 <= Fl::event_x_root() ||
				py - 5 >= Fl::event_y_root() ||
				py + 5 <= Fl::event_y_root()) 
			{
				px = -11;
				py = -11;
				show();
				Fl::flush();
			} else {
				return 1;
			}
                        // move the window
                        position(Fl::event_x_root() - w() / 2, 
				Fl::event_y_root() + 2);
			if (ic1) {
				Fl::first_window(ic1);
				ic1->position(x() + (w() / 2) - (ic1->w() /2), 
					y() - ic1->h() - 4);
			}
			if (ic2) {
				Fl::first_window(ic2);
				ic2->position(x() + (w() / 2) - (ic2->w() /2),
					y() - ic2->h() - 4);
			}
			if (!dragging) {
				dragging = 1;
				dnd_drag();
				Fl::dnd();
				Fl::event_clicks(0);
				Fl::event_is_click(0);
				was_dragging = 1;
				dragging = 0;
			}
			return 1;
                case FL_PUSH:
			if (px == -10 && py == -10) {
                        	px = Fl::event_x_root(); 
				py = Fl::event_y_root();
				bx = x();
				by = y();
			}
			if (Fl::event_clicks() > 0) {
				Fl::event_clicks(0);
				Fl::event_is_click(0);	
				if (type == TYPE_Application) {
					exec_cb(this, this);
				} else {
					open_cb(this, this);
				}
			}
                        return 1;
                case FL_RELEASE:
			cursor(FL_CURSOR_DEFAULT);
			root = RootWindow(fl_display, fl_screen);
			XQueryPointer(fl_display,
				root,&root,&c,&xx,&yy,&cx,&cy,&mask);
			XLowerWindow(fl_display, fl_xid(this));
			if (ic1) {
				ic1->cursor(FL_CURSOR_DEFAULT);
				XLowerWindow(fl_display, fl_xid(ic1));
			}
			if (ic2) {
				ic2->cursor(FL_CURSOR_DEFAULT);
				XLowerWindow(fl_display, fl_xid(ic2));
			}
			button1 = 0;
			px = -10;
			py = -10;
			if (pos && !c) {
                        	// save window position in position file
                        	snprintf(val, 25, "%d", x());
                        	pos->add_item("X", val, NULL);
                        	snprintf(val, 25, "%d", y());
                        	pos->add_item("Y", val, NULL);
                        	cfg->write_section(pos_file, pos);
				return 1;
			}
			if (!c) return 1;

			// go back to the original position
                       	position(bx, by);
			if (ic1) {
				ic1->position(x() + (w() / 2) - (ic1->w() /2), 
						y() - ic1->h() - 4);
			}
			if (ic2) {
				ic2->position(x() + (w() / 2) - (ic2->w() /2),
						y() - ic2->h() - 4);
			}
                      	return 1;
		default:
			break;
		}
	}

	switch(e) {
	case FL_PUSH:
		menu->parent(this);
		m = items->popup(Fl::event_x_root() - x(), 
			Fl::event_y_root() - y(), 0, 0, menu);
		menu->picked(m);
		return 1;
        case FL_DND_LEAVE:
        case FL_DND_ENTER:
        case FL_DND_DRAG:
		was_dragging = 0;
                return !dragging;
        case FL_DND_RELEASE:
		if (was_dragging) {
			was_dragging = 0;
			return 0;
		}
		is_drop = !dragging;
		return !dragging;
	case FL_DROP:
		if (is_drop)dnd_release();
		is_drop = 0;
		return 1;
	default:
		break;
	}
	was_dragging = 0;
	return 0;
}

void Icon::draw()
{
	VirtualWindow::draw();
}


void Icon::create()
{
	int width;
	Xd6ConfigFileItem *itm;
	const char *val;
	char buf[1024];
	const char *locale;
	int X, Y;
	
	type = 0;

	fl_open_display();

	if (cfg) {
		locale = cfg->locale;
	} else {
		locale = "C";
	}
	itm = NULL; if (sec) itm = sec->get_item("Name", locale);
	val = NULL; if (itm) val = itm->get_value();

	if (!val) val = _(">Unknown<");
	
	fl_font(font, size);
	snprintf(buf, 40, "%s", val);
	width = (int) fl_width(buf) + 16;

	resize(x(), y(), width, size + 8);

	box = new Fl_Box(0, 0, w(), h(), strdup(buf));
	box->box(FL_UP_BOX);

	itm = NULL; if (sec) itm = sec->get_item("Icon", locale);
	val = NULL; if (itm) val = itm->get_value();
	if (!val) val = "exec";
	
	ic1 = make_icon(val);

	itm = NULL; if (sec) itm = sec->get_item("UnmountIcon", locale);
	val = NULL; if (itm) val = itm->get_value();
	if (!val) {
		itm = NULL; 
		if (sec) itm = sec->get_item("EmptyIcon", locale);
		val = NULL; 
		if (itm) {
			val = itm->get_value();
			type = TYPE_Trash;
		}
	}
	if (val) {
		ic2 = make_icon(val);
	}

	itm = NULL; if (sec) itm = sec->get_item("Type", locale);
	val = NULL; if (itm) val = itm->get_value();

	if (val) {
		if (!strcmp("FSDevice", val)) {
			type = TYPE_FSDevice;
		} else if (!strcmp("Link", val)) {
			type = TYPE_Link;
		} else if (!strcmp("MimeType", val)) {
			type = TYPE_MimeType;
		} else if (!strcmp("Application", val)) {
			type = TYPE_Application;
		}
	}
	switch(type) {
	case TYPE_FSDevice:
		itm = NULL; 
		if (sec) itm = sec->get_item("MountPoint", locale);
		val = NULL; if (itm) val = itm->get_value();
		if (val && cfg) {
			snprintf(buf, 1024, "file://%s", val);
			url = strdup(buf);
		}
		break;
	case TYPE_Link:
		itm = NULL; if (sec) itm = sec->get_item("URL", locale);
		val = NULL; if (itm) val = itm->get_value();
		if (val) {
			url = strdup(val);
		}
		break;
	case TYPE_MimeType:
		break;
	case TYPE_Application:
		break;

	// directories
	case TYPE_Trash:
	case TYPE_None:
		if (cfg) {
			snprintf(buf, 1024, "file://%s", launch_name);
			*(buf + strlen(buf) - 7) = '\0';
			url = strdup(buf);
		}
		break;
	default:
		break;
	}
	make_menu();
	
	X = 500;
	Y = 60;
	itm = NULL; if (pos) itm = pos->get_item("X", locale);
	val = NULL; if (itm) val = itm->get_value();
	if (val) X = atol(val);
	itm = NULL; if (pos) itm = pos->get_item("Y", locale);
	val = NULL; if (itm) val = itm->get_value();
	if (val) Y = atol(val);
	show();
	position(X, Y);
	if (ic1) {
		ic1->position(x() + (w() / 2) - (ic1->w() /2), 
				y() - ic1->h() - 4);
	}
	if (ic2) {
		ic2->position(x() + (w() / 2) - (ic2->w() /2),
				y() - ic2->h() - 4);
	}
	Fl::flush();
}

Xd6IconWindow *Icon::make_icon(const char *file) 
{
	char name[1024];
	struct stat s;
	Xd6IconWindow *ic;

	name[0] = '\0';

	if (cfg) {
        	// search XPM file
        	snprintf(name, 1024, "%s/%s.xpm",
                	cfg->user_paths->icons_medium, file);
        	if (stat(name, &s)) {
                	snprintf(name, 1024, "%s/apps/%s.xpm",
                        	cfg->user_paths->icons_medium, file);
        	}
        	if (stat(name, &s)) {
                	snprintf(name, 1024, "%s/%s.xpm",
                        	cfg->global_paths->icons_medium, file);
        	}
        	if (stat(name, &s)) {
                	snprintf(name, 1024, "%s/apps/%s.xpm",
                        	cfg->global_paths->icons_medium, file);
        	}
        	if (stat(name, &s)) {
                	snprintf(name, 1024, "%s/apps/exec.xpm",
                        	cfg->global_paths->icons_medium);
        	}
               	if (stat(name, &s)) return NULL;
        	if (s.st_size < 10) return NULL;
	}
	ic = new Xd6IconWindow(name, this, 32, 32);
	return ic;
}

void Icon::show()
{
	if (!displayed) {
		displayed = 1;
		Fl::grab(this);
		Fl_Window::show();
		Fl::grab(0);
		if (ic1) {
			ic1->show();
			Fl::flush();
			Fl::check();
		} 
		if (ic2) {
			ic2->show();
			Fl::flush();
			Fl::check();
		}
	}
	if (type == TYPE_FSDevice) {
		if (is_mounted()) {
			if (ic2) {
				ic2->handle(FL_HIDE);
			}
			if (ic1) ic1->show();
			items[0].flags = FL_MENU_INACTIVE;
			items[1].flags = 0;
		} else {
			if (ic1) {
				ic1->handle(FL_HIDE);
			}
			if (ic2) ic2->show();
			items[1].flags = FL_MENU_INACTIVE;
			items[0].flags = 0;
		}
	} else if (type == TYPE_Trash) {
		if (is_empty_trash()) {
			if (ic1) {
				ic1->handle(FL_HIDE);
			}
			if (ic2) ic2->show();
		} else {
			if (ic2) {
				ic2->handle(FL_HIDE);
			}
			if (ic1) ic1->show();
		}
	} else {
		if (ic2) {
			ic2->handle(FL_HIDE);
		}
		if (ic1) ic1->show();
	}
	VirtualWindow::show();
}

int Icon::is_mounted()
{
	Xd6ConfigFileItem *itm;
	const char *val;

	if (!cfg || !sec) return 0;
	
	itm = NULL; itm = sec->get_item("Dev", NULL);
	val = NULL; if (itm) val = itm->get_value();
	if (!val) {
		itm = NULL; itm = sec->get_item("MountPoint", NULL);
		val = NULL; if (itm) val = itm->get_value();
	}
	if (val) {
		int fp;
		char *buf;
		struct stat s;
		char *ptr;

		if (stat("/etc/mtab", &s)) return 0;
		buf = (char*) malloc(s.st_size);
		fp = open("/etc/mtab", O_RDONLY);
		read(fp, buf, s.st_size);
		close(fp);

		buf[s.st_size - 1] = '\0';
		ptr = strstr(buf, val);	
		if (ptr) {
			ptr += strlen(val);
			if (isspace(*ptr)) {
				free(buf);
				return 1;
			}
		}
		free(buf);
	}
	return 0;
}

static int sel_no_hide(const struct dirent *d)
{
	if (d->d_name[0] == '.') return 0;
	return 1;
}

int Icon::is_empty_trash()
{
	char *dir;
	int nb;
	struct dirent **namelist = NULL;

	if (!cfg || !sec || !launch_name) return 0;

	dir = strdup(launch_name);
	
	*(dir + strlen(dir) - 7) = '\0';
	nb = scandir(dir, &namelist, sel_no_hide, alphasort);
	
	if (nb < 1) {
		free(dir);
		return 1;
	}
	while (nb > 0) {
		nb--;
		free(namelist[nb]);
	}
	free(namelist);
	return 0;
}

void Icon::open_cb(Fl_Widget* w, void* d)
{
	char buf[2048];
	Icon *ic = (Icon*) d;
	if (!ic || !ic->url) return;
	
	if (*(ic->url + strlen(ic->url) - 1) == '/') {
		snprintf(buf, 2048, "flfm \"%s\" ", ic->url);
	} else {
		snprintf(buf, 2048, "flfile --open \"%s\" ", ic->url);
	}
	Xd6System(buf);
}

void Icon::mount_cb(Fl_Widget* w, void* d)
{
	Icon *ic = (Icon*) d;
	Xd6ConfigFileItem *itm;
	const char *dev;
	const char *dir;
	const char *fst;
	const char *ro;

	if (!ic || !ic->cfg || !ic->sec) return;
	
	itm = NULL; itm = ic->sec->get_item("Dev", NULL);
	dev = NULL; if (itm) dev = itm->get_value();
	itm = NULL; itm = ic->sec->get_item("MountPoint", NULL);
	dir = NULL; if (itm) dir = itm->get_value();
	itm = NULL; itm = ic->sec->get_item("FSType", NULL);
	fst = NULL; if (itm) fst = itm->get_value();
	itm = NULL; itm = ic->sec->get_item("ReadOnly", NULL);
	ro = NULL; if (itm) ro = itm->get_value();

	if (dir || dev) {
		char buf[2048];
		const char *rdo = "";	
		const char *device = "/cdrom";

		if (ro && ro[0] == '0') rdo = "-oro";
	
		if (dir) {
			device = dir;
		} else if (dev) {
			device = dev;
		}
		if (!fst || !strcmp(fst, "Default")) {
			snprintf(buf, 2048, "mount %s ", device);
		}
		system(buf);
	}
	ic->show();
	Fl::flush();
}

void Icon::umount_cb(Fl_Widget* w, void* d)
{
	Icon *ic = (Icon*) d;
	Xd6ConfigFileItem *itm;
	const char *val;

	if (!ic || !ic->cfg || !ic->sec) return;
	
	itm = NULL; itm = ic->sec->get_item("Dev", NULL);
	val = NULL; if (itm) val = itm->get_value();
	if (!val) {
		itm = NULL; itm = ic->sec->get_item("MountPoint", NULL);
		val = NULL; if (itm) val = itm->get_value();
	}

	if (val) {
		char buf[2048];
		snprintf(buf, 2048, "umount %s; eject %s", val, val);
		system(buf);
	}
	ic->show();
	Fl::flush();
}

void Icon::edit_icon_cb(Fl_Widget* w, void* d)
{
	Icon *ic = (Icon*) d;
	char buf[2048];
	if (!ic || !ic->launch_name) return;

	snprintf(buf, 2048, "flnotepad \"%s\"", ic->launch_name);
	system(buf);

	snprintf(buf, 2048, "fldesk &");
	system(buf);
	exit(0);
}

void Icon::remove_icon_cb(Fl_Widget* w, void* d)
{
	Icon *ic = (Icon*) d;
	if (!ic) return;

	if (ic->launch_name) {
		unlink(ic->launch_name);
	}
	ic->hide();
	if (ic->ic1) ic->ic1->hide();
	if (ic->ic2) ic->ic2->hide();
}

void Icon::empty_cb(Fl_Widget* w, void* d)
{
	char buf[2048];
	Icon *ic = (Icon*) d;
	const char *end;
	if (!ic || !ic->url) return;
	
	end = ic->url + strlen(ic->url);
	if (ic->cfg && *(end - 1) == '/') {
		const char *ptr = ic->url;
		ptr += strlen("file://");
		if (ptr < end && strstr(ptr, "/Trash/")) {
			snprintf(buf, 2048, "/bin/rm -r %s*", ptr);
			system(buf);
			ic->show();
			Fl::flush();
		}
	}
}

void Icon::exec_cb(Fl_Widget* w, void* d)
{
	char buf[2048];
	Xd6ConfigFileItem *itm;
	const char *val;
	Icon *ic = (Icon*) d;
	if (!ic || (!ic->url && ic->type != TYPE_Application)) return;

	itm = NULL; itm = ic->sec->get_item("Path", NULL);
	val = NULL; if (itm) val = itm->get_value();

	if (val) {
		fl_filename_expand(buf, val);
		chdir(buf);
	}
	
	if (ic->type == TYPE_Application) {
		const char *exe;
		itm = NULL; itm = ic->sec->get_item("Exec", NULL);
		exe = NULL; if (itm) exe = itm->get_value();
		if (!exe) return;
		snprintf(buf, 2048, "flfile --exec \"%s\" ", exe);
	} else if (ic->url && *(ic->url + strlen(ic->url) - 1) == '/') {
		snprintf(buf, 2048, "flfm \"%s\" ", ic->url);
	} else {
		snprintf(buf, 2048, "flfile --exec \"%s\" ", ic->url);
	}
	Xd6System(buf);
}

void Icon::make_menu()
{
	if (type == TYPE_FSDevice) {
		items->add(_("Mount"), 0, mount_cb, this, 0);
		items->add(_("Unmount"), 0, umount_cb, this, FL_MENU_INACTIVE);
		items->add(_("Open"), 0, open_cb, this, FL_MENU_DIVIDER);
		items->add(_("Edit icon"), 0, edit_icon_cb, this, 0);
		items->add(_("Remove icon"), 0, remove_icon_cb, this, 0);
	} else if (type == TYPE_Application) {
		items->add(_("Execute"), 0, exec_cb, this, FL_MENU_DIVIDER);
		items->add(_("Edit icon"), 0, edit_icon_cb, this, 0);
		items->add(_("Remove icon"), 0, remove_icon_cb, this, 0);
	} else if (type == TYPE_Trash) {
		items->add(_("Open"), 0, open_cb, this, FL_MENU_DIVIDER);
		items->add(_("Empty trash"), 0, empty_cb, this, 0);
	} else {
		items->add(_("Open"), 0, open_cb, this, FL_MENU_DIVIDER);
		items->add(_("Edit icon"), 0, edit_icon_cb, this, 0);
		items->add(_("Remove icon"), 0, remove_icon_cb, this, 0);
	}
	for (int i = 0; i < 8; i++) {
		items[i].labelsize(size);
		items[i].labelfont(font);
	}
}

/*
 *  drop handler
 */
int Icon::dnd_release()
{
	FILE *fp;
	char buf[2048];
		//printf("REC:%s\n", Fl::e_text);
		if (type == TYPE_Trash && url) {
			snprintf(buf, 2048, "flfile --move %s", url);
		} else if (type == TYPE_None && url) {
			snprintf(buf, 2048, "flfile --copy %s", url);
		} else if (type == TYPE_Application) {
			const char *exe;
			Xd6ConfigFileItem *itm;
			itm = NULL; if (sec) itm = sec->get_item("Exec", NULL);
			exe = NULL; if (itm) exe = itm->get_value();
			if (!exe) return 1;
			snprintf(buf, 2048, "flfile --exec %s ", exe);
		} else {
			return 1;
		}
		fp = popen(buf, "w");
		if (fp) fwrite(Fl::e_text, Fl::e_length, 1, fp);
		fclose(fp);
		show();
	return 1;
}

/*
 *  drag handler
 */
int Icon::dnd_drag()
{
		free(buffer);
		buffer = NULL;
		if (url) {
			buffer = strdup(url);
		} else {
			buffer = strdup(" ");
		}
		Fl::selection(*this, buffer, strlen(buffer));
}

