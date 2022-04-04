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


#include "SmallIcon.h"
#include "callbacks.h"
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Item.H>
#include <libintl.h>
#include "xd640/Xd6IconWindow.h"
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>

#define _(String) gettext((String))

static Fl_Pixmap *p_character = NULL;
Fl_Pixmap *p_folder = NULL;
static Fl_Pixmap *p_link = NULL;
static Fl_Pixmap *p_pipe = NULL;
Fl_Pixmap *p_exec = NULL;
static Fl_Pixmap *p_block = NULL;
static Fl_Pixmap *p_socket = NULL;
static Fl_Pixmap *p_unknown = NULL;


#include "xpm/mcharacter.xpm"
#include "xpm/mfolder.xpm"
#include "xpm/mlink.xpm"
#include "xpm/mexec.xpm"
#include "xpm/mblock.xpm"
#include "xpm/mpipe.xpm"
#include "xpm/msocket.xpm"
#include "xpm/munknown.xpm"

SmallIcon::SmallIcon(int X, int Y, int W, int H) : 
	BigIcon(X, Y, W, H)
{
	offset = 0;
	info[0] = '\0';
	info_len = 0;
	width = W;
	height = H;
	pix = NULL;
	real_name = NULL;
	selected = 0;
	align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	selection_color(137);
	type(1);
}

SmallIcon::~SmallIcon()
{
}

static void create_pixmaps()
{
	p_character = new Fl_Pixmap(mcharacter_xpm);
	p_folder = new Fl_Pixmap(mfolder_xpm);
	p_link = new Fl_Pixmap(mlink_xpm);
	p_pipe = new Fl_Pixmap(mpipe_xpm);
	p_exec = new Fl_Pixmap(mexec_xpm);
	p_block = new Fl_Pixmap(mblock_xpm);
	p_socket = new Fl_Pixmap(msocket_xpm);
	p_unknown = new Fl_Pixmap(munknown_xpm);
}


void SmallIcon::set_data(struct file_info *fi)
{
	struct stat st;
	char perm[] = "----------";
	struct passwd *pwp;
	struct group *grp;
	char *ptr;

	real_name = fi->real_name;
	label(fi->real_name);	

	if (!p_link) {
		create_pixmaps();
	}
	if (!m_exec) {
		create_menus();
	}
	if (!drag_window) {
		window()->begin();
		create_drag_window();
		((Fl_Group*)parent())->begin();
	}

	switch(fi->st.st_mode & S_IFMT) {
	case S_IFSOCK:
		menu(m_special);
		pix = p_socket; break;
	case S_IFLNK:
		if (!stat(real_name, &st)) {
			switch(st.st_mode & S_IFMT) {
			case S_IFREG:
				if (st.st_mode & S_IXUSR) {
					menu(m_exec);
					callback(cb_exec);
				} else {
					menu(m_unknown);
					callback(cb_open);
				}
				break;
			case S_IFDIR:
				callback(cb_change_dir);
				menu(m_folder); break;
			default:
				menu(m_special);
			}
		} else {
			menu(m_special);
		}
		pix = p_link; break;
	case S_IFREG:
		if (fi->st.st_mode & S_IXUSR) {
			menu(m_exec);
			pix = p_exec; 
			callback(cb_exec);
		} else {
			menu(m_unknown);
			pix = p_unknown;		
			callback(cb_open);
		}
		break;
	case S_IFBLK:
		menu(m_special);
		pix = p_block; break;
	case S_IFDIR:
		menu(m_folder);
		callback(cb_change_dir);
		pix = p_folder; break;
	case S_IFCHR:
		menu(m_special);
		pix = p_character; break;
	case S_IFIFO:
		menu(m_special);
		pix = p_pipe; break;
	default:
		menu(m_special);
		pix = p_unknown;		
	}

	offset = (int) fl_width(real_name) + 30;
	if (offset < 200) offset = 200;

                if (fi->st.st_mode & S_IRUSR) perm[1] = 'r';
                if (fi->st.st_mode & S_IWUSR) perm[2] = 'w';
                if (fi->st.st_mode & S_IXUSR) perm[3] = 'x';
                if (fi->st.st_mode & S_IRGRP) perm[4] = 'r';
                if (fi->st.st_mode & S_IWGRP) perm[5] = 'w';
                if (fi->st.st_mode & S_IXGRP) perm[6] = 'x';
                if (fi->st.st_mode & S_IROTH) perm[7] = 'r';
                if (fi->st.st_mode & S_IWOTH) perm[8] = 'w';
                if (fi->st.st_mode & S_IXOTH) perm[9] = 'x';
                if (fi->st.st_mode & S_ISUID) {
                        if (fi->st.st_mode & S_IXUSR) {
                                perm[3] = 's';
                        } else {
                                perm[3] = 'S';
                        }
                }
                if (fi->st.st_mode & S_ISGID) {
                        if (fi->st.st_mode & S_IXGRP) {
                                perm[6] = 's';
                        } else {
                                perm[6] = 'S';
                        }
                }
              if (fi->st.st_mode & S_ISVTX) {
                        if (fi->st.st_mode & S_IXOTH) {
                                perm[9] = 's';
                        } else {
                                perm[9] = 'S';
                        }
                }
                if (S_ISLNK(fi->st.st_mode)) {
                        perm[0] = 'l';
                }
                if (S_ISDIR(fi->st.st_mode)) {
                        perm[0] = 'd';
                }
                if (S_ISCHR(fi->st.st_mode)) {
                        perm[0] = 'c';
                }
                if (S_ISBLK(fi->st.st_mode)) {
                        perm[0] = 'b';
                }
                if (S_ISFIFO(fi->st.st_mode)) {
                        perm[0] = 'p';
                }
                if (S_ISSOCK(fi->st.st_mode)) {
                        perm[0] = 's';
                }

	pwp = getpwuid(fi->st.st_uid);
	grp = getgrgid(fi->st.st_gid);
	ptr = ctime(&fi->st.st_ctime);
	if (ptr) *(ptr + strlen(ptr) - 1) = '\0';
	snprintf(info, 256, _("%s    %s / %s  (%s)  %ld bytes"),
			perm, pwp ? pwp->pw_name : "?", 
			grp ? grp->gr_name : "?", 
			ptr ? ptr : "?", fi->st.st_size);
	info_len = strlen(info);
}

int SmallIcon::is_inside()
{
	int ix;

	ix = x();
	if (Fl::event_x() >= x() && Fl::event_x() <= x() + w() &&
		Fl::event_y() >= y() + 3 && Fl::event_y() <= y() + h() - 6)
	{
		return 1;
	}
	return 0;
}

void SmallIcon::draw()
{
	int X = x();
	int W = w();
	Fl_Color col;
	resize(X + 20, y(), w() - 20, h());
	if (selected) {
		col = labelcolor();
		draw_box(FL_FLAT_BOX, selection_color());
		labelcolor(fl_contrast(col, selection_color()));
		draw_label();
		labelcolor(col);
	} else {
		draw_box(FL_FLAT_BOX, color());
		draw_label();
	}
	fl_draw(info, info_len, x() + offset, 
		y() + fl_height() - fl_descent() + 1);
	resize(X, y(), W, h());
	pix->draw(x(), y() + 1);
}


