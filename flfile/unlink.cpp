/******************************************************************************* *   $Id: unlink.cpp,v 1.1 2000/08/05 19:11:22 nickasil Exp $
 *
 *   This file is part of the FLE project. 
 *
 *                 Copyright (c) 2000-2001  O'ksi'D
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
#include "unlink.h"
#include <libintl.h>

#define _(String) gettext((String))

char **Unlink::list;
int Unlink::nb_item;
int Unlink::actual;
int Unlink::ask;
Unlink *Unlink::self;

int Unlink::setup(char **lst, int nb, int mode) 
{
	list = lst;
	nb_item = nb;
	ask = mode;
	actual = 0;
	self = this;

	begin();
	label(list[0]);

	/*******************************************************************/
	grp = new Fl_Group(5, 5, 390, 95);
	text = new Fl_Box(10, 5, 380, 20, _("Deleting :"));
	text->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
	file = new Fl_Output(10, 30, 380, 20, "");
	file->color(FL_GRAY);
	grp->end();
	Fl_Group::current()->resizable(grp);
	/*******************************************************************/

	cancel = new Fl_Button(240, 70, 80, 25, _("Cancel"));
	cancel->callback(cb_cancel, this);
	end();
	return 1;
}

int Unlink::fn(const  char  *file, const  struct  stat  *sb,
	int  flag,  struct FTW *s)
{
	int ret;

	self->file->label(file);
	Fl::flush();
	Fl::check();

	if (ask) {
		ret = fl_choice(_("Do you really want to DELETE\n"
				"\"%s\" ?"), _("Abort"), 
				_("Yes"), _("Yes to all"), file);

		Fl::flush();
		switch (ret) {
		case 1:
			break;
		case 2:
			ask = 0;
			break;
		default:
			cb_cancel(NULL, self);
		}
	}

	if (flag == FTW_DP) {
		ret = rmdir(file);
		if (ret == -1) {
			ret = fl_choice("rmdir: %s", _("Abort"), _("Continue"), 
				NULL, strerror(errno));
			if (ret != 1) cb_cancel(NULL, self);
		}
	} else {
		ret = unlink(file);
		if (ret == -1) {
			ret = fl_choice("unlink: %s", _("Abort"), 
				_("Continue"), NULL, strerror(errno));
			if (ret != 1) cb_cancel(NULL, self);
		}
	}
	return 0;
}

int Unlink::exec(const Fl_Window* modal_for)
{
	struct stat st;

	show();
	Fl::flush();
	Fl::check();

	usleep(500000);
	while (nb_item > 0) {
		nb_item--;
		
		label(list[nb_item]);
		Fl::flush();
		if (lstat(list[nb_item], &st)) continue;
		if (S_ISDIR(st.st_mode)) {
			nftw(list[nb_item], fn, 20, 
				FTW_CHDIR | FTW_DEPTH | FTW_MOUNT | FTW_PHYS);
		} else if (S_ISREG(st.st_mode)) {
			fn(list[nb_item], &st, FTW_F, NULL);
		} else if (S_ISLNK(st.st_mode)) {
			fn(list[nb_item], &st, FTW_SL, NULL);
		} else {
			// whoow, what are you trying to delete ?
			fn(list[nb_item], &st, 0, NULL);
		}
	}
	return 0;
}

void Unlink::cb_cancel(Fl_Widget *, void *data)
{
	Unlink *self = (Unlink *)data;
	self->hide();
	Fl::flush();
	Fl::check();
	exit(0);
}

