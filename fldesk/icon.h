/******************************************************************************* *   $Id: icons.h,v 1.1 2000/08/05 19:11:21 nickasil Exp $
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

#ifndef icon_h
#define icon_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Button.H>
#include "xd640/Xd6IconWindow.h"
#include "xd640/Xd6ConfigFile.h"

enum {
	TYPE_None = 0,
	TYPE_FSDevice,
	TYPE_Link,
	TYPE_MimeType,
	TYPE_Application,
	TYPE_Trash,
};

class Icon : public VirtualWindow {
public:
	Xd6IconWindow *ic1;
	Xd6IconWindow *ic2;
	Xd6ConfigFile *cfg;
	Xd6ConfigFileSection *sec;
	Xd6ConfigFileSection *pos;
	const char *pos_file;
	char *launch_name;
	Fl_Menu_Item *items;
	char *url;
	char *data;
	Fl_Box *box;
	Fl_Font font;
	int size;
	int type;
	static Fl_Menu_ *menu;
	static char *buffer;
	char dragging;
	char is_drop;
	char displayed;

	Icon(Xd6ConfigFile *cfg, Xd6ConfigFileSection *sec, Xd6ConfigFileSection *pos,
		const char *pos_file_name, const char *launch_name);
	~Icon(void);
	int handle(int);
	void draw(void);
	void create(void);
	Xd6IconWindow *make_icon(const char *name);
	void show(void);
	int is_mounted(void);
	int is_empty_trash(void);
	void make_menu(void);
	static void open_cb(Fl_Widget* w, void* d);
	static void exec_cb(Fl_Widget* w, void* d);
	static void mount_cb(Fl_Widget* w, void* d);
	static void umount_cb(Fl_Widget* w, void* d);
	static void edit_icon_cb(Fl_Widget* w, void* d);
	static void remove_icon_cb(Fl_Widget* w, void* d);
	static void empty_cb(Fl_Widget* w, void* d);
	int dnd_release(void);
	int dnd_drag(void);
};

#endif
