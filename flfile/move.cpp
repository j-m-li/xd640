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

