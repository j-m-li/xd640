/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2001  O'ksi'D
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

