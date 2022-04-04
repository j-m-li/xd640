/******************************************************************************* *   $Id: move.cpp,v 1.1 2000/08/05 19:11:22 nickasil Exp $
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
#include "move.h"
#include <libintl.h>

#define _(String) gettext((String))

int Move::setup(char **lst, int nb, char *path, int mode) 
{
	list = lst;
	nb_item = nb;
	ask = mode;
	actual = 0;

        directory = path;
	begin();
	label(lst[0]);
	/*******************************************************************/
	Fl_Group *grp = new Fl_Group(5, 20, 390, 80);
	file = new Fl_Output(10, 20, 380, 20, "");
	file->color(FL_GRAY);
	file->value(lst[0]);
	grp->end();
	Fl_Group::current()->resizable(grp);
	/*******************************************************************/

	Fl_Button *cancel = new Fl_Button(240, 70, 80, 25, _("Cancel"));
	cancel->callback(cb_cancel, this);
	end();
	return 1;
}

int Move::move_files()
{
	int ret = 0;
	struct stat st;
	static char buf[2048];

	if (!list[actual]) return -1;
	snprintf(buf, 2048, "%s%s", directory, strrchr(list[actual], '/'));
	file->value(buf);
	label(list[actual]);

	ret = lstat(buf, &st);

	if (ret != -1) {
		fl_alert(_("Destination file exists.\n"
			"Please, delete it before."));
		return -1;
	}
	
	ret = rename(list[actual], buf);
	if (ret == -1) {
		if (errno == EXDEV) {
			fl_alert(_("Cannot move files between diffrent "
				"filesystems.\nPlease use copy and delete."));
		} else {
			fl_alert("rename: %s", strerror(errno));
		}
		return -1;
	}
	actual++;
	return 1;
}

void Move::cb_move(void *data)
{
	Move *self = (Move *)data;
	if (self->actual < self->nb_item) {
		if (self->move_files() == -1) {
			self->actual++; 
		}
		Fl::add_timeout(0.01, cb_move, data);
	} else {
		self->cb_cancel(NULL, data);
	}
}

int  Move::exec(const Fl_Window* modal_for)
{
        show();
        Fl::add_timeout(0.4, cb_move, (void *) this);
        return Fl::run();
}

void Move::cb_cancel(Fl_Widget *, void *data)
{
	Move *self = (Move *)data;
	Fl::remove_timeout(cb_move, data);
	self->hide();
}

