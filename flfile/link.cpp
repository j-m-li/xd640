/******************************************************************************* *   $Id: link.cpp,v 1.1 2000/08/05 19:11:22 nickasil Exp $
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
 *   Author : Jean-Marc Lienher <nickasil@linuxave.net>
 *
 ******************************************************************************/
#include "link.h"
#include <libintl.h>

#define _(String) gettext((String))

char **Link::list;
int Link::nb_item;
int Link::actual;
int Link::ask;
Link *Link::self;
char *Link::directory;
int Link::dirlen;

int Link::setup(char **lst, int nb, char *path, int mode) 
{
	list = lst;
	nb_item = nb;
	ask = mode;
	directory = path;
	dirlen = strlen(directory);
	self = this;
	begin();
	label(list[0]);

	/*******************************************************************/
	Fl_Group *grp = new Fl_Group(5, 20, 390, 80);
	file = new Fl_Output(10, 20, 380, 20, "");
	file->color(FL_GRAY);
	file->value(directory);
	grp->end();
	Fl_Group::current()->resizable(grp);
	/*******************************************************************/

	Fl_Button *cancel = new Fl_Button(240, 70, 80, 25, _("Cancel"));
	cancel->callback(cb_cancel, this);
	end();
	return 1;
}

int Link::fn(const  char  *file, const  struct  stat  *sb,
        int  flag,  struct FTW *s)
{
        int ret;
	char buf[1024];
	const char *ptr;


	ptr = file + strlen(file);
	while (*ptr != '/' && ptr > file) ptr--;
	ptr++;
	snprintf(buf, 1024, "%s%s", directory, ptr);
        self->file->label(buf);
	self->label(file);
        Fl::flush();
        Fl::check();

        if (ask) {
                ret = fl_ask(_("Do you really want to LINK\n"
                                "\"%s\" to \n\"%s\" ?"), file, buf);
                Fl::flush();
                if (ret != 1) return -1;
        }

	ret = symlink(file, buf);
	if (ret == -1) {
		fl_alert("symlink: %s", strerror(errno));
		return -1;
	}
	return 0;
}


int  Link::exec(const Fl_Window* modal_for)
{
	struct stat st;

	show();
	Fl::flush();
	Fl::check();
	usleep(500000);

	while (nb_item > 0) {
		nb_item--;
		if (lstat(list[nb_item], &st)) continue;
		fn(list[nb_item], &st, FTW_F, NULL);
	}
        return 0;
}

void Link::cb_cancel(Fl_Widget *, void *data)
{
	Link *self = (Link *)data;
	self->hide();
	exit(0);
}

