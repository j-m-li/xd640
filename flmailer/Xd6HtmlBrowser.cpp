/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000,2001  O'ksi'D
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


#include "gui.h"
#include "Xd6HtmlBrowser.h"
#include "MailParser.h"
#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libintl.h>
#include <dirent.h>
#include <sys/stat.h>
#include "Xd6XmlUtils.h"

#define _(String) gettext((String))

extern Xd6ConfigFile *cfg;
extern char **environ;

static void l_cb(Fl_Widget *w, void *)
{
	((Xd6HtmlBrowser*)w->parent())->list_cb();
}

Xd6HtmlBrowser::Xd6HtmlBrowser(int X, int Y, int W, int H) : Fl_Group(X, Y, W, H)
{
	list = new Fl_Select_Browser(X, Y + 25, W, 80);
	list->callback(l_cb);
	view = new Xd6HtmlView(X, Y + 105, W, H - 105);
	view->frame->editor = 0;
	view->frame->wysiwyg = 0;
	folders = NULL;
	nb_folder = 0;
	pid = 0;
	upid = 0;
	init();
}


Xd6HtmlBrowser::~Xd6HtmlBrowser()
{
}

void Xd6HtmlBrowser::save_list()
{
	int i;
	int size = list->size();
	char path[1024];
	FILE *index;
	snprintf(path, 1024, "%s/%s/index.txt", 
				cfg->user_paths->apps, 
				current_folder->box->get_create_item(
				"directory", NULL)->get_value());
	index = fopen(path, "w");
	if (!index) return;
	for (i = 1; i <= size; i++) {
		const char *d;
		d = list->text(i);
		if (*d == '\v') {
			fprintf(index, "%s\n", d);

		}
	}
	fclose(index);
}

void Xd6HtmlBrowser::list_cb(void)
{
	int v = list->value();
	char *d;
	if (v < 1) return;
	d = (char*)list->text(v);
	if (!current_folder) return;
	if (d) {
		int save = 0;
		if (*d == '\v' && *(d +1) == 'b') {
			d++;
			*d = ' ';
			save = 1;
		}
		while (*d && *d != '\f') d++; d++;
		while (*d && *d != '\f') d++; d++;
		while (*d && *d != '\f') d++; d++;
		if (*d) d += 3;
		snprintf(path, 1024, "%s/%s/%s", 
				cfg->user_paths->apps, 
				current_folder->box->get_create_item(
				"directory", NULL)->get_value(),
				d);
		view->load(path);
		if (save) {
			Fl::check();
			Fl::flush();
			save_list();
		}
	}
}


void Xd6HtmlBrowser::load(const char *f)
{
	view->load(f);
}


/*
 * "$Id: $"
 */

