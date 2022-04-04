/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2003  O'ksi'D
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


#include "IconTree.h"
#include "SmallIcon.h"
#include "callbacks.h"
#include <FL/fl_draw.H>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libintl.h>


#define _(String) gettext((String))

static SmallIcon *small = NULL;
extern Fl_Pixmap *p_exec;
extern Fl_Pixmap *p_folder;

IconTree::IconTree(int X, int Y, int W, int H) : Fl_Tree(X, Y, W, H)
{
    	char *n = strdup("/");
    	char *fn = strdup("file:///");
    	struct dirent **dir = NULL;
    	int nb = fl_filename_list(n, &dir, fl_alphasort);
	if (!small) {
		begin();
		small = new SmallIcon(X - 100, Y -100, 1, 1);
		end();
		remove(small);
        }
	Entry *e = new Entry(n, fn, nb - 2, NULL);
    	root(e);
	
    	color(FL_BACKGROUND_COLOR, FL_SELECTION_COLOR);
	box(FL_FLAT_BOX);
	when(FL_WHEN_NOT_CHANGED);
	callback(item_cb);
	no_scan = 0;
	dragging = -1;
	
}

IconTree::~IconTree()
{

}

void IconTree::item_cb(Fl_Widget* w, void* data)
{

	IconTree *it = (IconTree*) w;
	Entry *e = it->get_entry(it->get_selection());
	if (e->nbd >= 0) {
		StatesValues.newurl = strdup(e->url);
	} else {
		StatesValues.newurl = strdup(e->url);
		char *ptr = StatesValues.newurl + strlen(StatesValues.newurl);
		ptr--;
		while (*ptr != '/') ptr--;
		*ptr = 0;
	}	
	it->no_scan = 1;
	cb_rescan(it, NULL);
	it->no_scan = 0;
}

int IconTree::item_handle(int n, int evt)
{
	static int px = 0, py = 0;
	static int push_ok = 0;
	switch (evt) {
	case FL_PUSH:
		if (Fl::event_x() >= x() - hposition() + 
			20 * levels[n] + widths[n]) 
		{
			return 0;
		}
		push_ok = 1;
		px = Fl::event_x_root();
		py =  Fl::event_y_root();
		if (Fl::event_state() & FL_BUTTON3) {
			Entry *e = get_entry(n);
			const Fl_Menu_Item *m;
			m = e->menu->popup(Fl::event_x(), Fl::event_y());
			if (m) m->do_callback(this);
		} else if ((Fl::event_state() & FL_BUTTON1) && 
			Fl::event_clicks())
		{
			if (item_flags(n) & FLAG_FOLDER) {
				if (item_flags(n) & FLAG_OPEN) {
					item_close(n);
				} else {
					item_open(n);
				}
			} else {
				Entry *e = get_entry(n);
				if (e->pix == p_exec) {
					cb_exec(NULL, NULL);
				} else {
					cb_open(NULL, NULL);
				}
			}
			Fl::event_clicks(0);
		}
			
		return 1;
	case FL_DRAG: {
		static char *buffer = NULL;
                if (px - 5 >= Fl::event_x_root() ||
                        px + 5 <= Fl::event_x_root() ||
                        py - 5 >= Fl::event_y_root() ||
                        py + 5 <= Fl::event_y_root())
                {
                        px = -10;
                        px = -10;
		} else {
			return 1;
		}
		if (dragging >= 0) {
                	small->drag_window->position(Fl::event_x_root() + 2,
                                Fl::event_y_root() + 2);
			return 0;
		}
		if (!push_ok) return 0;
		dragging = n;
                small->drag_window->position(Fl::event_x_root() + 2,
                                Fl::event_y_root() + 2);
                small->drag_window->show();
                free(buffer);
                buffer = get_selected_urls();
                Fl::copy(buffer, strlen(buffer), 0);
                if (Fl::event_state() & FL_BUTTON1) {
                          fl_XdndActionCopy = XdndActionCopy;
                } else {
                          fl_XdndActionCopy = XdndActionAsk;
                }
                Fl::dnd();
                fl_XdndActionCopy = XdndActionCopy;
                small->drag_window->handle(FL_HIDE);
                XUnmapWindow(fl_display, small->drag_window->win);
		item_damage(sel);
		sel = -1;
		dragging = -1;
		push_ok = 0;
		//cb_rescan(NULL, NULL);
	} break;
	case FL_RELEASE:
		push_ok = 0;
		break;
	case FL_DND_DRAG: 
		//if (is_dragging != n && item_flags(n) & FLAG_FOLDER) return 1;
		break;
	case FL_DND_RELEASE:
		//if (is_dragging != n && item_flags(n) & FLAG_FOLDER) return 1;
		break;
	case FL_DROP:
		cb_rescan(NULL, NULL);
		return 1;
	}
	return 0;
}

static int my_strcmp(const char *s1, const char *s2)
{
	int i = 0;
	while (s1[i] == s2[i] && s1[i]) i++;
	return i;
}

void IconTree::rescan()
{
	int nb = item_max();
	int i = 0;
	char *leave = StatesValues.url;
	int deep = 0;
	int roll = -1;
	if (no_scan || !leave) return;

	while (i < nb) {
		Entry *e = get_entry(i);
		int d = my_strcmp(leave, e->url + 8);
		item_clear_flags(i, FLAG_SELECTED);
		if (d > deep) {
			deep = d;
			roll = i;			
		}
		i++;
	}
	if (roll >= 0) {
		item_close(roll);
		item_open(roll);
	}
	Fl::check();
}

void IconTree::item_draw(int n,int X, int Y, int CX, int CY, int CW, int CH)
{
    	Entry *e = get_entry(n);
    	fl_font(0, 14);
    	fl_color((item_flags(n) & FLAG_SELECTED) ? 
		FL_SELECTION_COLOR : FL_BACKGROUND_COLOR);
    	fl_rectf(X + 2 , Y, widths[n], heights[n]);
    	fl_color((item_flags(n) & FLAG_SELECTED) ? FL_WHITE : FL_BLACK);
    	fl_draw(e->name, X + 2 + 16 , Y + fl_height() - fl_descent());
	if (e->pix) e->pix->draw(X, Y);
	else p_folder->draw(X, Y);
    	if (sel == n) {
		fl_color(FL_WHITE);
      		dot_rect(X + 2, Y, widths[n], heights[n]);
    	}
}

void IconTree::item_measure(int n)
{
    Entry *e = get_entry(n);
    fl_font(0, 14);
    heights[n] = fl_height(); 
    widths[n] = (int) fl_width(e->name) + 16;

}

int IconTree::item_nb_children(int n) 
{
    Entry *e = get_entry(n);
    char *b1;
    if (e->nbd >= 0 /*&& e->dirent ==  NULL*/) {
      if (e->dirent) {
	while (e->nbd > 0) free(e->dirent[--e->nbd]);
	free(e->dirent);
      }
      dirent **dir = NULL;
      b1 = item_get_path(n, 0);
      e->nbd = fl_filename_list(b1, &dir, fl_alphasort) - 2;
      e->dirent = dir;
    }
    if (e->dirent && e->nbd >= 0) {
      return e->nbd;
    }
    return -1;

}

int IconTree::item_has_children(int n)
{
    Entry *e = get_entry(n);
    return e->nbd;
}

char *IconTree::item_get_path(int n, int c) {
    Entry *e = get_entry(n);
    char *par[1024];
    static char b1[1024];
    Entry *p = e;
    int i = 0;
    while (p) {
      par[i++] = p->name;
      p = p->parent;
    }
    b1[0] = 0;
    while (i > 0) {
      strcat(b1, par[--i]);
#ifndef WIN32
      strcat(b1, "/");
#endif
    }
    if (c >= 1) strcat(b1, e->dirent[c-1]->d_name);
    return b1;
}

void *IconTree::item_get_child(int n, int c)
{
    Entry *e = get_entry(n);
    Entry *m;
    char buf[1100];
    char *b1;
    char is_dir = -1;
    struct file_info fi;

    b1 = item_get_path(n, c+2);
    fl_stat(b1, &(fi.st));
    if ((fi.st.st_mode & S_IFMT) == S_IFDIR) {
      is_dir = 0;
    }
    snprintf(buf, 1100, "file://%s", b1);
    m = new Entry(strdup(e->dirent[c+1]->d_name), strdup(buf), is_dir, e);
    fi.real_name = e->name;
    small->set_data(&fi);
    m->menu = small->menu();
    m->pix = small->pix;
    //if (!strcmp(m->name, ".") || !strcmp(m->name, "..")) m->nbd = -1;
    return m;
}

void IconTree::item_free(int n)
{
    Entry *e = get_entry(n);
    delete(e);
}



