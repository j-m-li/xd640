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

