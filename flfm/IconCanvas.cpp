/******************************************************************************
 *   "$Id: $"
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

#include "IconCanvas.h"
#include "BigIcon.h"
#include "callbacks.h"
#include <FL/fl_draw.H>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libintl.h>

#define _(String) gettext((String))

int IconCanvas::onmover = 0;

IconCanvas::IconCanvas(int X, int Y, int W, int H) : Fl_Scroll(X, Y, W, H)
{
	newlist = NULL;
	list = NULL;
	newnbf = 0;
	nbf = 0;
	group = NULL;
	newinfo = NULL;
	info = NULL;
	oldgroup = NULL;
        drag_x = 0;
        drag_y = 0;
        dragging = 0;
	type(BOTH_ALWAYS);
	box(FL_FLAT_BOX);
}

IconCanvas::~IconCanvas()
{
	while (nbf > 0) {
		nbf--;
		free(list[nbf]);
		free(info[nbf]);
	}
	free(list);
	free(info);
	delete(oldgroup);
}

static int sel_all(const struct dirent *d)
{
	if (d->d_name[0] == '.' && (d->d_name[1] == '\0' ||
		(d->d_name[1] == '.' && d->d_name[2] == '\0')))
	{
		return 0;
	}
	return 1;
}

static int sel_no_hide(const struct dirent *d)
{
	if (d->d_name[0] == '.') return 0;
	return 1;
}

static int sizesort(const void *d1, const void *d2)
{
	struct file_info *f1 = *((struct file_info**) d1);
	struct file_info *f2 = *((struct file_info**) d2);
	
	if (S_ISDIR(f1->st.st_mode) && S_ISDIR(f2->st.st_mode)) {
		return 0;
	} else if (S_ISDIR(f1->st.st_mode)) {
		return -(0x7fffffff);
	} else if (S_ISDIR(f2->st.st_mode)) {
		return 0x7fffffff;
	}
	return (f2->st.st_size - f1->st.st_size);
}

static int typesort(const void *d1, const void *d2)
{
	struct file_info *f1 = *((struct file_info**) d1);
	struct file_info *f2 = *((struct file_info**) d2);
	if (S_ISDIR(f1->st.st_mode) && S_ISDIR(f2->st.st_mode)) {
		return 0;
	} else if (S_ISDIR(f1->st.st_mode)) {
		return -(0x7fffffff);
	} else if (S_ISDIR(f2->st.st_mode)) {
		return 0x7fffffff;
	}
	return ((f2->st.st_mode & S_IFMT) - (f1->st.st_mode & S_IFMT));
}

void IconCanvas::rescan()
{
	int i;
	if (!StatesValues.url) return;

	if (StatesValues.show_hide) {	
		newnbf = scandir(StatesValues.url, &newlist, 
				sel_all, alphasort);
	} else {
		newnbf = scandir(StatesValues.url, &newlist, 
				sel_no_hide, alphasort);
	}

	// prevent drop timeout callback
	//gui->dnd()->init();

	newinfo = (struct file_info **) 
			malloc(sizeof(struct file_info *) * newnbf);
	i = 0;
	while (i < newnbf) {
		newinfo[i] = (struct file_info *) 
				malloc(sizeof(struct file_info));
		newinfo[i]->real_name = newlist[i]->d_name;
		if (stat(newinfo[i]->real_name, &(newinfo[i]->st))) {
			newinfo[i]->st.st_mode = 0;
			newinfo[i]->st.st_size = 0;
		}
		i++;
	}
	if (StatesValues.sort_type) {
		qsort(newinfo, newnbf, sizeof(struct file_info*), typesort);
	} else if (StatesValues.sort_size) {
		qsort(newinfo, newnbf, sizeof(struct file_info*), sizesort);
	}

	remove(group);
	delete(oldgroup);
	oldgroup = group;

	while (nbf > 0) {
		nbf--;
		free(list[nbf]);
		free(info[nbf]);
	}
	free(list);
	free(info);

	nbf = newnbf;
	list = newlist;
	info = newinfo;

	begin();
	if (StatesValues.view_detail) {
		group = new DetailGroup(x(), y(), w(), h(), this);	
	} else {
		group = new NormalGroup(x(), y(), w(), h(), this);	
	}
	end();
	resizable(NULL);

	update_status();
	Fl::flush();
	Fl::check();
}

void IconCanvas::mover(void* data)
{
        IconCanvas *w = (IconCanvas *) data;
        Fl::remove_timeout(mover);
        onmover = 0;
        w->handle(FL_DRAG);
}

void IconCanvas::draw_selector(int ev_x, int ev_y)
{
	int redr = 0;
        const int numchildren = group->children();
        BigIcon *b;
        int x=0, y=0, dx=0, dy=0;
	int drag_dx, drag_dy, mx, my;

        drag_dx = ev_x - drag_x;
        drag_dy = ev_y - drag_y;

        if (drag_x < ev_x) {
                x = drag_x;
                dx = drag_dx;
        } else {
                x = ev_x;
                dx = -drag_dx;
        }
        if (drag_y < ev_y) {
                y = drag_y;
                dy = drag_dy;
        } else {
                y = ev_y;
                dy = -drag_dy;
        }

	x -= xposition();
	y -= yposition();
        mx = x + dx;
        my = y + dy;

        for (int i=0; i < numchildren; ++i) {
                int bx, by, bw, bh;
                b = (BigIcon *)group->child(i);
                bx = b->x();
                by = b->y();
                bw = b->w();
                bh = b->h();

                if (bx + bw > x && bx < mx && by + bh > y && by < my) {
			if (!b->selected) {
				b->selected = 1;
				b->damage(FL_DAMAGE_ALL);
				redr = 1;
			}
                } else {
			if (b->selected) {
				b->selected = 0;
				b->damage(FL_DAMAGE_ALL);
				redr = 1;
			}
                }
      }

	group->damage(FL_DAMAGE_CHILD);
	group->show_selector(x, y, dx, dy);
	if (redr) {
		update_status();
		Fl::flush();
	}
	//redraw();
	//Fl::flush();
}

int IconCanvas::handle(int event)
{
        int move = 0;
        int ypos = yposition();
        int xpos = xposition();
      	int ev_x; 
       	int ev_y;
	int mx, my;

        if (!dragging) {
                int ret = 0;
                onmover = 0;
                if (event == FL_PUSH && 
			Fl::event_state() & FL_BUTTON1  &&
			!(Fl::get_key(FL_Control_L) ||
                        Fl::get_key(FL_Control_R)))
                {
                        int i = group->children();
                        while (i) {
                                i--;
                                ((BigIcon*)group->child(i))->selected = 0;
                        }
			update_status();
			group->damage(FL_DAMAGE_CHILD);
                }
                if (event != FL_FOCUS) {
                        ret = Fl_Scroll::handle(event);
                }
                if (!ret && event == FL_PUSH) {
			int i = group->children();
                        Fl::focus(this);
			if (!(Fl::get_key(FL_Control_L) || 
				Fl::get_key(FL_Control_R)))
			{
				while (i) {
					i--;
					((BigIcon*)group->child(i))->
						selected = 0;
				}
				group->redraw();
			}
                        dragging = 1;
                        drag_x = Fl::event_x() + xposition();
                        drag_y = Fl::event_y() + yposition();
                        return 1;
                }
                return ret;
        } else if (event == FL_RELEASE) {
                dragging = 0;
                Fl::remove_timeout(mover);
		group->hide_selector();
                group->redraw();
                //Fl::flush();
                return 1;
        } else if (event != FL_DRAG) {
		if (!(Fl::event_state() & FL_BUTTON1)) {
			dragging = 0;
		}
                return Fl_Scroll::handle(event);
        }

       	if (onmover) {
               	Fl::remove_timeout(mover, this);
		onmover = 0;
	}

	Fl::get_mouse(mx, my);
	//mx = Fl::event_x_root();
	//my = Fl::event_y_root();
	mx -= window()->x();
	my -= window()->y();
      	ev_x = mx + xposition();
       	ev_y = my + yposition();

	if (!(drag_x - 3 >= ev_x || drag_x + 3 <= ev_x || drag_y - 3 >= ev_y ||
		drag_y + 3 <= ev_y))
	{
		return 1;
	}
	if (Fl::has_timeout(mover, this)) return 1;

        if (mx > scrollbar.x() - 16 &&
                xposition() < group->w() - 16 - w())
        {
                xpos += 18;
                move = 1;
        } else if (my > hscrollbar.y() - 16 &&
                yposition() < group->h() - 16 - h())
        {
                ypos += 18;
                move = 1;
        } else if (mx < x() + 16 && xposition() > 15) {
                xpos -= 18;
                move = 1;
        } else if (my < y() + 16 && yposition() > 15) {
                ypos -= 18;
                move = 1;
        } else {
		group->hide_selector();
                draw_selector(ev_x, ev_y);
        }
        if (move) {
                position(xpos, ypos);
                draw_selector(ev_x, ev_y);
		group->hide_selector();
		group->redraw();       
 	}
        if (move && !onmover) {
                Fl::add_timeout(.1, mover, this);
        }
        return 1;
}

void IconCanvas::update_status(void)
{
        const int numchildren = group->children();
	static char buf[80];
	unsigned long dsize = 0;	
	unsigned long ssize = 0;	
	int nbs = 0;

        for (int i=0; i < numchildren; ++i) {
		if (i >= nbf) break;
		if (((BigIcon *)group->child(i))->selected) {
			ssize += info[i]->st.st_size;
			nbs++;
		}
		dsize += info[i]->st.st_size;
	}
	
	if (nbs) {
		snprintf(buf, 80, _(" %d selected files  /  %ld kb"), 
			nbs, ssize / 1024);
	} else {
		snprintf(buf, 80, _(" %d files  /  %ld kb"), 
			numchildren, dsize / 1024);
	}
	gui->stat_bar->value(buf);
}

void IconCanvas::resize(int X, int Y, int W, int H) 
{
	int ww;
	int hh;
	int xp;
	int yp;

	Fl_Scroll::resize(X, Y, W, H);
	if (!group) return;
	ww = W - 16;
	hh = H - 16;
	if (group->w() > ww) ww = group->w();
	if (group->h() > hh) hh = group->h();
	group->resize(group->x(), group->y(), ww, hh);

	yp = scrollbar.value();
	xp = scrollbar.value();

	if (yp + H > hh - 16) {
		yp = hh - H + 16;
	}
	if (xp + W > ww - 16) {
		xp = 0;
	}
	position(xp, yp);
}

void IconCanvas::draw() 
{
	Fl_Scroll::draw();
}

