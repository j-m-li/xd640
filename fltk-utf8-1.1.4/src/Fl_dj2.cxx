//
// "$Id: Fl_x.cxx,v 1.24.2.24.2.23 2002/08/09 03:17:30 easysw Exp $"
//
// X specific code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2002 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//


#  define CONSOLIDATE_MOTION 1
/**** Define this if your keyboard lacks a backspace key... ****/
/* #define BACKSPACE_HACK 1 */

#  include <config.h>
#  include <FL/Fl.H>
#  include <FL/x.H>
#  include <FL/Fl_Window.H>
#  include <FL/fl_utf8.H>
#  include <stdio.h>
#  include <stdlib.h>
#  include "flstring.h"
#  include <unistd.h>
#  include <sys/time.h>

extern GrColor *fl_egacolors;
#define BLACK        GrAllocColor(0,0,0)
#define RED        GrAllocColor(255,0,0)
#define BLUE         GrAllocColor(0,0,200);
#define CYAN         GrAllocColor(0,128,128)
#define LIGHTGRAY    GrAllocColor(200,200,200)
#define DARKGRAY     GrAllocColor(128,128,128)
#define LIGHTBLUE    GrAllocColor(0,0,255)
#define WHITE        GrAllocColor(255,255,255)


////////////////////////////////////////////////////////////////
// interface to poll/select call:

#  if USE_POLL

#    include <poll.h>
static pollfd *pollfds = 0;

#  else
#    if HAVE_SYS_SELECT_H
#      include <sys/select.h>
#    endif /* HAVE_SYS_SELECT_H */

// The following #define is only needed for HP-UX 9.x and earlier:
//#define select(a,b,c,d,e) select((a),(int *)(b),(int *)(c),(int *)(d),(e))
static fd_set fdsets[3];
static int maxfd;
#    define POLLIN 1
#    define POLLOUT 4
#    define POLLERR 8

#  endif /* USE_POLL */
/* JHC 09/19/00 - Added this array to simplify grabbing keystrokes */
/* Because we care about so many different modifiers, its almost easier */
/* to have an array instead of a million if statements */


static int nfds = 0;
static int fd_array_size = 0;
struct FD {
#  if !USE_POLL
  int fd;
  short events;
#  endif
  void (*cb)(int, void*);
  void* arg;
};

static FD *fd = 0;

void Fl::add_fd(int n, int events, void (*cb)(int, void*), void *v) {
  remove_fd(n,events);
  int i = nfds++;
  if (i >= fd_array_size) {
    FD *temp;
    fd_array_size = 2*fd_array_size+1;

    if (!fd) temp = (FD*)malloc(fd_array_size*sizeof(FD));
    else temp = (FD*)realloc(fd, fd_array_size*sizeof(FD));

    if (!temp) return;
    fd = temp;

#  if USE_POLL
    pollfd *tpoll;

    if (!pollfds) tpoll = (pollfd*)malloc(fd_array_size*sizeof(pollfd));
    else tpoll = (pollfd*)realloc(pollfds, fd_array_size*sizeof(pollfd));

    if (!tpoll) return;
    pollfds = tpoll;
#  endif
  }
  fd[i].cb = cb;
  fd[i].arg = v;
#  if USE_POLL
  pollfds[i].fd = n;
  pollfds[i].events = events;
#  else
  fd[i].fd = n;
  fd[i].events = events;
  if (events & POLLIN) FD_SET(n, &fdsets[0]);
  if (events & POLLOUT) FD_SET(n, &fdsets[1]);
  if (events & POLLERR) FD_SET(n, &fdsets[2]);
  if (n > maxfd) maxfd = n;
#  endif
}

void fl_reset_spot(void)
{
}

void fl_set_spot(int font, int size, int x, int y, int w, int h)
{
}

int fl_nb_windows = -1;
_Window **fl_windows = NULL;
Window fl_focus = 0;
int fl_nb_events = 0;
int fl_malloc_events = 0;
XEvent *fl_events = NULL;
int fl_events_stack = 0;
static int need_refresh = 0;
static int need_refocus = 0;

void fl_push_event(XEvent *e) 
{
	XEvent *v;
	fl_nb_events++;
	fl_events_stack++;
	if (fl_events_stack >= fl_malloc_events) {
		fl_malloc_events += fl_malloc_events + 16; 
		fl_events = (XEvent*)realloc(fl_events, 
			sizeof(XEvent) * fl_malloc_events);
	}
	v = fl_events + fl_events_stack - 1;
	v->type = e->type;
	v->target = e->target;
	v->x = e->x;
	v->y = e->y;
	v->w = e->w;
	v->h = e->h;
	v->flags = e->flags;
	v->buttons = e->buttons;
	v->key = e->key;
	v->kbstat = e->kbstat;
}

void fl_pop_event(XEvent *v)
{
	XEvent *e;
	v->type = 0;
	fl_nb_events--;
	if (fl_nb_events < 0) return;
	e = fl_events + fl_events_stack - fl_nb_events - 1;
	v->type = e->type;
	v->target = e->target;
	v->x = e->x;
	v->y = e->y;
	v->w = e->w;
	v->h = e->h;
	v->flags = e->flags;
	v->buttons = e->buttons;
	v->key = e->key;
	v->kbstat = e->kbstat;
	if (fl_nb_events < 1) {
		fl_events_stack = 0;	
	}
}

void fl_clean_layer(void)	
{
	int last = 0;
	for (int layer = 0; layer < 100; layer++) {
		int n = 1;
		int has = 0;
		while (n < fl_nb_windows) {
			if (fl_windows[n] && fl_windows[n]->layer == layer &&
				fl_windows[n]->context) 
			{
				fl_windows[n]->layer = last;
				has = 1;
			}
			n++;
		}
		if (has) last++;
	}

}

int fl_get_top_layer(void)
{
	for (int layer = 0; layer < 100; layer++) {
		int n = 1;
		int has = 0;
		while (n < fl_nb_windows) {
			if (fl_windows[n] && fl_windows[n]->layer == layer &&
				fl_windows[n]->context) 
			{
				has = 1;
			}
			n++;
		}
		if (!has) return layer;
	}
	return 99;
}

void fl_refresh(Window i)
{
	XEvent e;
	e.type = FL_SHOW;
	e.x = 0;
	e.y = 0;
	e.w = 100000;
	e.h = 100000;
	for (int layer = 0; layer < 100; layer++) {
		int n = 1;
		int has = 0;
		while (n < fl_nb_windows) {
			if (fl_windows[n] && fl_windows[n]->layer == layer &&
				fl_windows[n]->context) 
			{
				if (i != n) {
					e.target = n;
					fl_push_event(&e);	
				}
				has = 1;
			}
			n++;
		}
		if (!has) break;
	}
  	GrClearScreen(CYAN);
} 

void fl_unmap_window(Window i)
{
	XEvent e;
	
	if (fl_windows[i]->parent) {
		int n = 1;
		_Window *p = fl_windows[i]->parent;
		while (n < fl_nb_windows) {
			if (fl_windows[n] == p) break;
			n++;
		}
		//fl_windows[i]->parent = fl_windows[0];
		if (n < fl_nb_windows) {
			if (fl_windows[n]->flags & _WIN_IS_WM) {
				fl_unmap_window(n);
			} else {
			}
		}
	}
	if (fl_windows[i]->context) {
		GrDestroyContext(fl_windows[i]->context);
	}	
	fl_windows[i]->context = NULL;
/*	fl_clean_layer();	
	e.target = i;
	e.type = FL_HIDE;
	fl_push_event(&e);
*/
	if (i == fl_focus) need_refocus = 1;
	need_refresh = 1;
}

void fl_map_window(Window i) 
{
	XEvent e;
	int x, y;
	_Window *c = fl_windows[i];
	if (c->x < 0) c->x = 0;
	if (c->y < 0) c->y = 0;
	x = c->x;
	y = c->y;
	if (c->context) {
		GrDestroyContext(c->context);
		c->context = NULL;
	}
	if (c->parent == fl_windows[0] && c->flags & _WIN_BORDER) {
    		int flags = _WIN_IS_WM | c->flags & ~_WIN_BORDER;
		Window win;
		if (c->x < 2) c->x = 2;
		if (c->y < 18) c->y = 18;
    		win = fl_add_window(c->x-2, c->y-18, c->w + 4, c->h + 20, 
			c->title, flags, 0);
    		fl_map_window(win);
		c->parent = fl_windows[win];
		x = 2;
		y = 18;
	} else if (c->flags & _WIN_BORDER && c->parent) {
		Window win = fl_get_parent_window(i);
		if (c->x < 2) c->x = 2;
		if (c->y < 18) c->y = 18;
		c->parent->x = c->x-2;
		c->parent->y = c->y-18;
		c->parent->w = c->w +4;
		c->parent->h = c->h + 20;
		fl_map_window(win);
		x = 2;
		y = 18;
	}
	fl_clean_layer();
	c->layer = fl_get_top_layer();
	c->context = GrCreateSubContext(x, y, c->x + c->w - 1,
			c->y + c->h - 1, c->parent->context, NULL); 
	e.target = i;
	e.x = e.y = 0;
	e.w = c->w;
	e.h = c->h;
	e.type = FL_SHOW;
	fl_push_event(&e);
	if (c->flags & _WIN_BORDER) {
		e.target = fl_focus;
		e.type = FL_UNFOCUS;
		fl_push_event(&e);
		e.target = i;
		e.type = FL_FOCUS;
		fl_push_event(&e);
		fl_focus = i;
		need_refocus = 0;
	}
	need_refresh = 1;
}

void fl_destroy_window(Window i) 
{
	XEvent e;
	int n = 1;
	while (n < fl_nb_windows) {
		if (fl_windows[n]  && 
			fl_windows[n]->parent == fl_windows[i])
		{
			fl_destroy_window(n);
		}
		n++;
	}
	if (fl_windows[i]->context) GrDestroyContext(fl_windows[i]->context);
	if (fl_windows[i]->title) free(fl_windows[i]->title);
	fl_windows[i]->context = NULL;
	fl_windows[i]->title = NULL;
	free(fl_windows[i]);
	fl_windows[i] = NULL;	
	fl_clean_layer();  	
/*	e.target = i;
	e.type = FL_HIDE;
	fl_push_event(&e);
*/
	if (i == fl_focus) need_refocus = 1;
	need_refresh = 1;
}

Window fl_get_window()
{
	Window n = 1;
	while (n < fl_nb_windows) {
		if (!fl_windows[n]) break;
		n++;
	}
	if (n >= fl_nb_windows) {
		fl_windows = (_Window**)
			realloc(fl_windows, sizeof(_Window*) * 
				fl_nb_windows * 2);
		memset(fl_windows + fl_nb_windows, 0, 
			sizeof(_Window*) * fl_nb_windows);
		fl_nb_windows *= 2;
		//if (fl_nb_windows > 10) exit(0);
	}
	fl_windows[n] = (_Window*)malloc(sizeof(_Window));
	memset(fl_windows[n], 0, sizeof(_Window));
	return n;
}

void fl_set_window(Window wi, int x, int y, int w, int h, 
	const char *title, int flags)
{
	_Window *c = fl_windows[wi];
	c->x = x;
	c->y = y;
	c->w = w;
	c->h = h;
	if (c->title) free(c->title);
	if (title) c->title = strdup(title);
	else c->title = strdup("");
	c->flags = flags;
}

Window fl_add_window(int x, int y, int w, int h, const char *title, int flags,
	Window parent)
{
	Window wi;
	wi = fl_get_window(); 
	if (flags & _WIN_OFF_SCREEN) {
		char far **memo;
		int i = GrNumPlanes();
		GrContext *c;
		memo = (char far**) malloc(sizeof(char *far) * 4);
		memo[0] = memo[1] = memo[2] = memo[3] = 0;
		while (i > 0) {
			i--;
			memo[i] = (char far*) malloc(GrPlaneSize(w,h));
		}
		c = GrCreateContext(w,h,memo, NULL);
		c->gc_memflags |= 2; // GrDestroyContext musts free the memory ! 
		wi = fl_get_window();
		_Window *q = fl_windows[wi];
		q->context = c;
		q->flags = flags;
		return wi;
	}
	fl_set_window(wi, x, y, w, h, title, flags); 
	fl_windows[wi]->parent = fl_windows[parent];
	return wi;
}



GrContext *fl_get_context(Window i)
{
	return fl_windows[i]->context;
}


void Fl::add_fd(int n, void (*cb)(int, void*), void* v) {
  Fl::add_fd(n, POLLIN, cb, v);
}

void Fl::remove_fd(int n, int events) {
  int i,j;
  for (i=j=0; i<nfds; i++) {
#  if USE_POLL
    if (pollfds[i].fd == n) {
      int e = pollfds[i].events & ~events;
      if (!e) continue; // if no events left, delete this fd
      pollfds[j].events = e;
    }
#  else
    if (fd[i].fd == n) {
      int e = fd[i].events & ~events;
      if (!e) continue; // if no events left, delete this fd
      fd[i].events = e;
    }
#  endif
    // move it down in the array if necessary:
    if (j<i) {
      fd[j] = fd[i];
#  if USE_POLL
      pollfds[j] = pollfds[i];
#  endif
    }
    j++;
  }
  nfds = j;
#  if !USE_POLL
  if (events & POLLIN) FD_CLR(n, &fdsets[0]);
  if (events & POLLOUT) FD_CLR(n, &fdsets[1]);
  if (events & POLLERR) FD_CLR(n, &fdsets[2]);
  if (n == maxfd) maxfd--;
#  endif
}

void Fl::remove_fd(int n) {
  remove_fd(n, -1);
}
extern  GrFont          GrFont_PC8x14;

Window fl_get_parent_window(Window i) 
{
	int n = 1;
	_Window *d = fl_windows[i];
	while (n < fl_nb_windows) {
		_Window *c = fl_windows[n];
		if (d && d->parent == c) {
			return n;
		}
		n++;
	}
	return 0;
}

Window fl_get_child_window(Window i) 
{
	int n = 1;
	_Window *c = fl_windows[i];
	while (n < fl_nb_windows) {
		_Window *d = fl_windows[n];
		if (d && d->parent == c) {
			return n;
		}
		n++;
	}
	return 0;
}

void fl_re_focus(void) 
{
	int i = 1;
	int layer = -1;
	XEvent e;
	while (i < fl_nb_windows) {
		_Window *c = fl_windows[i];
		if (c && c->context && c->layer >= layer) {
			if (c->flags & _WIN_IS_WM) {
			} else {
				fl_focus = i;
			}
			layer = c->layer;
		}
		i++;
	}
	layer = fl_get_top_layer();
	Window w = fl_get_parent_window(fl_focus);
	if (w) fl_windows[w]->layer = layer++;
	fl_windows[fl_focus]->layer = layer; 
	fl_clean_layer();
	e.target = fl_focus;
	e.type = FL_FOCUS;
	fl_push_event(&e);
	
}

Window fl_push_to_focus_window(int x ,int y)
{
	int i = 1;
	XEvent e;
	Window win = fl_focus;
	int layer = -1;
	while (i < fl_nb_windows) {
		_Window *c = fl_windows[i];
		if (c && c->context && c->layer >= layer &&
			x >= c->x && y >= c->y && x <= c->x + c->w &&
			y <= c->y + c->h) 
		{
			win = i;
			layer = c->layer;
		}  
		i++;
	}
	if (win != fl_focus) {
		int l = fl_get_top_layer();
		if (fl_get_parent_window(win) != fl_focus &&
			fl_get_child_window(win) != fl_focus) 
		{
			need_refresh = 1;
		}
		Window wm = fl_get_parent_window(win);
		if (wm) {
			fl_windows[wm]->layer = l;
			l++;
		}
		fl_windows[win]->layer = l++;
		wm = fl_get_child_window(win);
		if (wm) {
			fl_windows[wm]->layer = l;
		}
		fl_clean_layer();
		e.target = fl_focus;
		e.type = FL_UNFOCUS;
		fl_push_event(&e);
		fl_focus = win;
		e.target = fl_focus;
		e.type = FL_FOCUS;
		fl_push_event(&e);
	}
	return fl_focus; 
}

int fl_pending_event(void) 
{
	static GrMouseEvent me;
	static XEvent e;
	static int x, y;
	e.type = 0;
	e.target = fl_focus;
	GrSetContext(NULL);
	GrMouseGetEvent(GR_M_EVENT|GR_M_POLL ,&me);
	e.x = x;
	e.y = y;
	e.h = 0;
	e.w = 0;
	e.buttons = me.buttons;
	e.key = me.key;
	e.kbstat = me.kbstat;
	e.flags = me.flags;
	if (me.flags & GR_M_MOTION) {
		e.type = FL_MOVE;
		x = me.x;
		y = me.y;
	}
	if (e.type) fl_push_event(&e);
	e.type = 0;
	if (me.flags & GR_M_KEYPRESS) {
		e.type = FL_KEYBOARD;
	}
	if (e.type) fl_push_event(&e);
	e.type = 0;
	if (me.flags & GR_M_BUTTON_DOWN) {
		e.type = FL_PUSH;
		e.target = fl_push_to_focus_window(x, y);
	}
	if (e.type) fl_push_event(&e);
	e.type = 0;
	e.target = fl_focus;
	if (me.flags & GR_M_BUTTON_UP) {
		e.type = FL_RELEASE;
	}
	if (e.type) fl_push_event(&e);
	e.type = 0;
  
	if (fl_nb_events < 1) {
		if (need_refocus) {
			fl_re_focus();
			need_refocus = 0;
		}
		if (need_refresh) {
			need_refresh = 0;
			fl_refresh(0);
		}
	}
	return fl_nb_events;
}


#  if CONSOLIDATE_MOTION
static Fl_Window* send_motion;
extern Fl_Window* fl_xmousewin;
#  endif
static void do_queued_events() {
    XEvent event;
    while (fl_pending_event()) {
	fl_pop_event(&event);
        fl_handle(event);
    }
  // we send FL_LEAVE only if the mouse did not enter some other window:
  if (!fl_xmousewin) Fl::handle(FL_LEAVE, 0);
#  if CONSOLIDATE_MOTION
  else if (send_motion == fl_xmousewin) {
    send_motion = 0;
    Fl::handle(FL_MOVE, fl_xmousewin);
  }
#  endif
}

// these pointers are set by the Fl::lock() function:
static void nothing() {}
void (*fl_lock_function)() = nothing;
void (*fl_unlock_function)() = nothing;

// This is never called with time_to_wait < 0.0:
// It should return negative on error, 0 if nothing happens before
// timeout, and >0 if any callbacks were done.
int fl_wait(double time_to_wait) {
  // OpenGL and other broken libraries call XEventsQueued
  // unnecessarily and thus cause the file descriptor to not be ready,
  // so we must check for already-read events:
  if (fl_display && fl_pending_event()) {do_queued_events(); return 1;}

#  if !USE_POLL
  fd_set fdt[3];
  fdt[0] = fdsets[0];
  fdt[1] = fdsets[1];
  fdt[2] = fdsets[2];
#  endif
  int n;

  fl_unlock_function();
  time_to_wait = 0.0002;
  if (time_to_wait < 2147483.648) {
#  if USE_POLL
    n = ::poll(pollfds, nfds, int(time_to_wait*1000 + .5));
#  else
    timeval t;
    t.tv_sec = int(time_to_wait);
    t.tv_usec = int(1000000 * (time_to_wait-t.tv_sec));
    n = ::select(maxfd+1,&fdt[0],&fdt[1],&fdt[2],&t);
#  endif
  } else {
#  if USE_POLL
    n = ::poll(pollfds, nfds, -1);
#  else
    n = ::select(maxfd+1,&fdt[0],&fdt[1],&fdt[2],0);
#  endif
  }

  fl_lock_function();

  if (n > 0) {
    for (int i=0; i<nfds; i++) {
#  if USE_POLL
      if (pollfds[i].revents) fd[i].cb(pollfds[i].fd, fd[i].arg);
#  else
      int f = fd[i].fd;
      short revents = 0;
      if (FD_ISSET(f,&fdt[0])) revents |= POLLIN;
      if (FD_ISSET(f,&fdt[1])) revents |= POLLOUT;
      if (FD_ISSET(f,&fdt[2])) revents |= POLLERR;
      if (fd[i].events & revents) fd[i].cb(f, fd[i].arg);
#  endif
    }
  }
  return n;
}

// fl_ready() is just like fl_wait(0.0) except no callbacks are done:
int fl_ready() {
  return 1;
#  if USE_POLL
  return ::poll(pollfds, nfds, 0);
#  else
  timeval t;
  t.tv_sec = 0;
  t.tv_usec = 0;
  fd_set fdt[3];
  fdt[0] = fdsets[0];
  fdt[1] = fdsets[1];
  fdt[2] = fdsets[2];
  return ::select(maxfd+1,&fdt[0],&fdt[1],&fdt[2],&t);
#  endif
}

////////////////////////////////////////////////////////////////

Display *fl_display;
Window fl_message_window = 0;
int fl_screen;
XVisualInfo *fl_visual;
Colormap fl_colormap;
static int update = 0;
static void fd_callback(int,void *) {
  do_queued_events();
}


void fl_open_display() {
  if (fl_display) return;
  fl_display = "open";
  GrSetMode( GR_default_graphics );
  //fl_egacolors = GrAllocEgaColors();
  GrClearScreen(GrAllocColor(0, 128, 128));
  if (fl_nb_windows == -1) {
		fl_windows = (_Window**)malloc(sizeof(_Window*) * 2);
		fl_nb_windows = 2;
		memset(fl_windows, 0, sizeof(_Window*) * 2);
		fl_windows[0] = (_Window*)malloc(sizeof(_Window));
		memset(fl_windows[0], 0, sizeof(_Window));
		fl_windows[0]->context = NULL;//GrCreateSubContext( 0, 0,
			//GrMaxX(), GrMaxY(), NULL,NULL );
		fl_windows[0]->w = GrMaxX();  
		fl_windows[0]->h = GrMaxY();  
  }
  if (GrMouseDetect()) {
    GrMouseEventMode(1);
    GrMouseInit();
    GrMouseSetColors(GrAllocColor(255,0,0),GrAllocColor(255,255,255));
    GrMouseDisplayCursor();
  }
}


void fl_close_display() {
}

int Fl::h() {
  fl_open_display ();

  return 9;
}

int Fl::w() {
  fl_open_display ();
  return 0;
}

void
Fl::get_mouse (int &x, int &y)
{
  x = Fl::e_x;
  y = Fl::e_y;
}
////////////////////////////////////////////////////////////////
// Code used for paste and DnD into the program:

Fl_Widget *fl_selection_requestor;
char *fl_selection_buffer[2];
int fl_selection_length[2];
int fl_selection_buffer_length[2];
char fl_i_own_selection[2];

// Call this when a "paste" operation happens:
void Fl::paste(Fl_Widget &receiver, int clipboard) {
  if (fl_i_own_selection[clipboard]) {
    // We already have it, do it quickly without window server.
    // Notice that the text is clobbered if set_selection is
    // called in response to FL_PASTE!
    Fl::e_text = fl_selection_buffer[clipboard];
    Fl::e_length = fl_selection_length[clipboard];
    receiver.handle(FL_PASTE);
    return;
  }
}


////////////////////////////////////////////////////////////////
// Code for copying to clipboard and DnD out of the program:

void Fl::copy(const char *stuff, int len, int clipboard) {
  if (!stuff || len<0) return;
  if (len+1 > fl_selection_buffer_length[clipboard]) {
    delete[] fl_selection_buffer[clipboard];
    fl_selection_buffer[clipboard] = new char[len+100];
    fl_selection_buffer_length[clipboard] = len+100;
  }
  memcpy(fl_selection_buffer[clipboard], stuff, len);
  fl_selection_buffer[clipboard][len] = 0; // needed for direct paste
  fl_selection_length[clipboard] = len;
  fl_i_own_selection[clipboard] = 1;
}

////////////////////////////////////////////////////////////////

XEvent* fl_xevent; // the current x event
ulong fl_event_time; // the last timestamp from an x event

char fl_key_vector[32]; // used by Fl::get_key()

// Record event mouse position and state from an XEvent:

static int px, py;
static ulong ptime;
static Window fl_move_window = 0;
static void set_event_xy() {
  int dx = 0, dy = 0;
#  if CONSOLIDATE_MOTION
  send_motion = 0;
#  endif
  if (fl_move_window) {
	dx += fl_windows[fl_move_window]->x;
	dy += fl_windows[fl_move_window]->y;
	fl_move_window = fl_get_parent_window(fl_move_window);
  }
  Fl::e_x_root = fl_xevent->x;
  Fl::e_x = fl_xevent->x - dx;
  Fl::e_y_root = fl_xevent->y;
  Fl::e_y = fl_xevent->y - dy;
  struct timeval t;
  gettimeofday(&t, 0);
  fl_event_time = t.tv_usec / 1000 + t.tv_sec;
// turn off is_click if enough time or mouse movement has passed:
  if (abs(Fl::e_x_root-px)+abs(Fl::e_y_root-py) > 3 ||
      fl_event_time >= ptime+1000)
    Fl::e_is_click = 0;
}

// if this is same event as last && is_click, increment click count:
static inline void checkdouble() {
  if (Fl::e_is_click == Fl::e_keysym)
    Fl::e_clicks++;
  else {
    Fl::e_clicks = 0;
    Fl::e_is_click = Fl::e_keysym;
  }
  px = Fl::e_x_root;
  py = Fl::e_y_root;
  ptime = fl_event_time;
}

static Fl_Window* resize_bug_fix;
extern int (*fl_local_grab)(int); // in Fl.cxx

////////////////////////////////////////////////////////////////
int
fl_handle (const XEvent & xevent)
{
  int i,x, y,w,h;
  fl_xevent = (XEvent *) & xevent;
  Window xid = xevent.target;	//fl_window;

  int button = 0;
  int event = xevent.type;
  Fl_Window *window = fl_find (xid);
  x = xevent.x;
  y = xevent.y;
  w = xevent.w;
  h = xevent.h;


  if (window)

    switch (xevent.type) {
    case FL_CLOSE:
      event = FL_CLOSE;
      if (Fl::handle (event, window))
	exit(0);
      else
	exit(-1);
      break;
    case FL_SHOW:
      update = 1;
      Fl_X::i (window)->wait_for_expose = 0;
      window->damage (FL_DAMAGE_EXPOSE, x, y, w, h);
      Fl::flush();
      //Fl_X::i (window)->wait_for_expose = 1;
      break;
    case FL_RELEASE:	//tanghao
      if (xevent.flags & GR_M_LEFT_UP) {
	button = FL_Button + 0x01;
	Fl::e_state &= ~FL_BUTTON1;
      } else if (xevent.flags & GR_M_RIGHT_UP) {
	button = FL_Button + 0x02;
	Fl::e_state &= ~FL_BUTTON2;
      } else {
	Fl::e_state &= ~FL_BUTTON3;
	button = FL_Button + 0x03;
      }
      Fl::e_keysym = button;
      set_event_xy ();
      break;

    case FL_PUSH:
      if (xevent.flags & GR_M_LEFT_DOWN) {
	button = FL_Button + 0x01;
	Fl::e_state |= FL_BUTTON1;
      } else if (xevent.flags & GR_M_RIGHT_DOWN) {
	button = FL_Button + 0x02;
	Fl::e_state |= FL_BUTTON2;
      } else {
	button = FL_Button + 0x03;
	Fl::e_state |= FL_BUTTON3;
      }

      Fl::e_keysym = button;
      set_event_xy ();
      checkdouble ();
      break;

    case FL_MOVE:
      fl_move_window = xid;
      set_event_xy ();

#if CONSOLIDATE_MOTION
      send_motion = fl_xmousewin = window;
      return 0;
#else
      event = FL_MOVE;
      break;
#endif

    case FL_KEYBOARD:
      {
	unsigned short keycode = xevent.key;
	static char buffer[21];
	int len = 0;

	buffer[len++] = keycode;
	buffer[len] = 0;


	Fl::e_state = 0;

	  Fl::e_keysym = (keycode & 0x00FF);
	Fl::e_text = buffer;
	Fl::e_length = len;

	Fl::e_is_click = 0;
	break;
      }
    case FL_ENTER:
      set_event_xy (); // FIXME_DJGPP
      break;
    case FL_LEAVE:
      set_event_xy ();
      break;
    } else {
	static int push = 0;
	static int ox, oy;
	_Window *w = fl_windows[xid];
	Window child = fl_get_child_window(xid);
	if (!(w && w->context &&  w->flags & _WIN_IS_WM && fl_display)) {
		return 0; 
	}
	if (xevent.type == FL_SHOW) {
		GrFBoxColors co;
		GrTextOption to;
  		to.txo_font = &GrFont_PC8x14;   
  		to.txo_fgcolor.v = WHITE;   
  		to.txo_bgcolor.v = GrNOCOLOR; 
  		to.txo_chrtype = GR_BYTE_TEXT;
  		to.txo_direct = GR_TEXT_RIGHT;
  		to.txo_xalign = GR_ALIGN_LEFT;
  		to.txo_yalign = GR_ALIGN_BASELINE;
			
		co.fbx_intcolor = LIGHTGRAY; 
		co.fbx_topcolor = WHITE; 
		co.fbx_rightcolor = BLACK; 
		co.fbx_bottomcolor = BLACK; 
		co.fbx_leftcolor = WHITE; 
		GrSetContext(w->context);
		// borders
		GrHLine(0,w->w-1,0, WHITE);
		GrHLine(0,w->w-1,1, LIGHTGRAY);
		GrHLine(0,w->w-1,w->h-1, BLACK);
		GrHLine(0,w->w-1,w->h-2, LIGHTGRAY);
		GrVLine(0, 0, w->h-1, WHITE);
		GrVLine(1, 1, w->h-2, LIGHTGRAY);
		GrVLine(w->w-1, 0, w->h-1, BLACK);
		GrVLine(w->w-2, 1, w->h-2, LIGHTGRAY);
		
		// title bar
		if (fl_focus == child || fl_focus == xid) {
			GrFilledBox(2,2,w->w -3, 17, LIGHTBLUE);
		} else {
			GrFilledBox(2,2,w->w -3, 17, DARKGRAY);
		}
		GrFramedBox(4,4, 15, 15, 1, &co);
		GrHLine(6,13,9, BLACK);
		GrHLine(6,13,10, BLACK);
		GrDrawString(w->title, strlen(w->title), 18, 14, &to);
		if (w->flags & _WIN_CLOSE) {
		  GrFramedBox(w->w-16,4, w->w-5, 15, 1, &co);
		  GrLine(w->w-13, 6, w->w-7, 13, BLACK); 
		  GrLine(w->w-13, 13, w->w-7, 6, BLACK); 
		  GrLine(w->w-14, 6, w->w-8, 13, BLACK); 
		  GrLine(w->w-14, 13, w->w-8, 6, BLACK); 
		}
		if (w->flags & _WIN_RESIZE) {
		  GrFramedBox(w->w-31,4, w->w-20, 15, 1, &co);
		  GrHLine(w->w-29, w->w-22,6, BLACK);
		  GrHLine(w->w-29, w->w-22,7, BLACK);
		  GrHLine(w->w-29, w->w-22,13, BLACK);
		  GrVLine(w->w-29, 8, 12, BLACK);
		  GrVLine(w->w-22, 8, 12, BLACK);
		}
		if (w->flags & _WIN_ICONIFY) {
		  GrFramedBox(w->w-46,4, w->w-35, 15, 1, &co);
		  GrHLine(w->w-44, w->w-40,12, BLACK);
		  GrHLine(w->w-44, w->w-40,13, BLACK);
		}
		GrSetContext( NULL ); 
	} else if (xevent.type == FL_PUSH) {
		x = xevent.x - w->x;
		y = xevent.y - w->y;
		ox = xevent.x;
		oy = xevent.y;
		if (y >= 4 && y <= 15) {
			if (w->flags & _WIN_CLOSE && 
				x >= w->w-16 && x <= w->w-5) 
			{
				push = 1; // close
			} else if (x >= 4 && x <= 15) {
				push = 2;// menu
			} else if (w->flags & _WIN_RESIZE && 
				x >= w->w-31 && x <= w->w-20) 
			{
				push = 3;// expand
			} else if (w->flags & _WIN_ICONIFY && 
				x >= w->w-44 && x <= w->w-40) 
			{
				push = 4; // iconify
			} else {
				GrMouseEraseCursor();
				GrMouseSetCursorMode(3, w->x -ox,
					w->y - oy, w->x -ox +w->w,
					w->y -oy + w->h, RED);
				GrMouseDisplayCursor();
				push = 5;// title
			}
		}
	} else if (xevent.type == FL_MOVE) {
		if (push == 5) {
				
		}
	} else if (xevent.type == FL_RELEASE) {
		XEvent e;
		_Window *win = fl_windows[child];
		x = xevent.x - w->x;
		y = xevent.y - w->y;
		GrMouseEraseCursor();
		GrMouseSetCursorMode(0); 
		GrMouseDisplayCursor();
		if (push == 1 && x >= w->w-16 && y >= 4 && 
			x <= w->w-5 && y <= 15) 
		{
			// close
			push = 0;
			e.type = FL_CLOSE;
			e.target = child;
			fl_push_event(&e);
			fl_destroy_window(xid);
			return 1;
		}
		if (push == 5) {
			push = 0;
			win->x += xevent.x - ox;
			win->y += xevent.y - oy;
			fl_unmap_window(child);
			fl_clean_layer();
			fl_map_window(child);
		}
		push = 0;
		fl_focus = child;
		e.type = FL_FOCUS;
		e.target = fl_focus;
		fl_push_event(&e);
	}
	return 1;
    }
  return Fl::handle (event, window);
}
////////////////////////////////////////////////////////////////

void
Fl_Window::resize (int X, int Y, int W, int H)
{
  int is_a_resize = (W != w () || H != h ());
  int resize_from_program = (this != resize_bug_fix);
  if (!resize_from_program)
    resize_bug_fix = 0;
  if (X != x () || Y != y ())
    set_flag (FL_FORCE_POSITION);
  else if (!is_a_resize)
    return;
  if (is_a_resize) {
    Fl_Group::resize (X, Y, W, H);
    if (shown ()) {
      fl_unmap_window(fl_xid(this));
      fl_map_window(fl_xid(this));
      redraw ();
      //i->wait_for_expose = 1;
    }
  } else {
    x (X);
    y (Y);
  }

  if (resize_from_program && shown ()) {
      int xid = fl_xid(this);
      fl_windows[xid]->x = X;
      fl_windows[xid]->y = Y;
      fl_windows[xid]->w = W;
      fl_windows[xid]->h = H;
      fl_unmap_window(xid);
      fl_map_window(xid);
  }

}
////////////////////////////////////////////////////////////////

// A subclass of Fl_Window may call this to associate an X window it
// creates with the Fl_Window:

void fl_fix_focus(); // in Fl.cxx

Fl_X* Fl_X::set_xid(Fl_Window* win, Window winxid) {
  Fl_X* xp = new Fl_X;
  xp->xid = winxid;
  xp->other_xid = 0;
  xp->setwindow(win);
  xp->next = Fl_X::first;
  xp->region = 0;
  xp->wait_for_expose = 1;
  xp->backbuffer_bad = 1;
  Fl_X::first = xp;
  if (win->modal()) {Fl::modal_ = win; fl_fix_focus();}
  return xp;
}

// More commonly a subclass calls this, because it hides the really
// ugly parts of X and sets all the stuff for a window that is set
// normally.  The global variables like fl_show_iconic are so that
// subclasses of *that* class may change the behavior...

char fl_show_iconic;	// hack for iconize()
int fl_background_pixel = -1; // hack to speed up bg box drawing
int fl_disable_transient_for; // secret method of removing TRANSIENT_FOR



int fl_show_it = 1;

void
Fl_X::make_xid (Fl_Window * w, XVisualInfo * visual, Colormap colormap)
{

  Fl_Group::current (0);	// get rid of very common user bug: forgot end()

  int X = w->x ();
  int Y = w->y ();
  int W = w->w ();
  if (W <= 0)
    W = 1;			// X don't like zero...
  int H = w->h ();
  if (H <= 0)
    H = 1;			// X don't like zero...
  int root;
  root = w->parent ()? fl_xid (w->window ()) : 0;

  if (!Fl::grab ()) {
    // center windows in case window manager does not do anything:
    if (!(w->flags () & Fl_Window::FL_FORCE_POSITION)) {
      w->x (X = (Fl::w () - W) / 2);
      w->y (Y = (Fl::h () - H) / 2);
    }
    // force the window to be on-screen.  Usually the X window manager
    // does this, but a few don't, so we do it here for consistency:
    if (w->border ()) {
      // ensure border is on screen:
      // (assumme extremely minimal dimensions for this border)
      const int top = 20;
      const int left = 1;
      const int right = 1;
      const int bottom = 1;
      if (X + W + right > Fl::w ())
	X = Fl::w () - right - W;
      if (X - left < 0)
	X = left;
      if (Y + H + bottom > Fl::h ())
	Y = Fl::h () - bottom - H;
      if (Y - top < 0)
	Y = top;
    }
    // now insure contents are on-screen (more important than border):
    if (X + W > Fl::w ())
      X = Fl::w () - W;
    if (X < 0)
      X = 0;
    if (Y + H > Fl::h ())
      Y = Fl::h () - H;
    if (Y < 0)
      Y = 0;
  }


  {
    Window wid;
    int flags = _WIN_BORDER|_WIN_ICONIFY|_WIN_RESIZE|_WIN_CLOSE;

    wid = fl_add_window(X, Y, W, H, w->label(), flags, root);
    Fl_X *x = set_xid (w, wid);
    w->set_visible ();
    fl_map_window(x->xid);
    fl_window = x->xid;		
    w->handle (FL_SHOW);
    w->redraw ();
  }
}
////////////////////////////////////////////////////////////////
// Send X window stuff that can be changed over time:

void Fl_X::sendxjunk() {
  if (w->parent() || w->override()) return; // it's not a window manager window!

  if (!w->size_range_set) { // default size_range based on resizable():
    if (w->resizable()) {
      Fl_Widget *o = w->resizable();
      int minw = o->w(); if (minw > 100) minw = 100;
      int minh = o->h(); if (minh > 100) minh = 100;
      w->size_range(w->w() - o->w() + minw, w->h() - o->h() + minh, 0, 0);
    } else {
      w->size_range(w->w(), w->h(), w->w(), w->h());
    }
    return; // because this recursively called here
  }
}

void Fl_Window::size_range_() {
  size_range_set = 1;
  if (shown()) i->sendxjunk();
}

////////////////////////////////////////////////////////////////

// returns pointer to the filename, or null if name ends with '/'
const char *fl_filename_name(const char *name) {
  const char *p,*q;
  if (!name) return (0);
  for (p=q=name; *p;) if (*p++ == '/') q = p;
  return q;
}

void Fl_Window::label(const char *name,const char *iname) {
  Fl_Widget::label(name);
  iconlabel_ = iname;
  if (shown() && !parent()) {
    if (!name) name = "";
    if (!iname) iname = fl_filename_name(name);
    if (fl_windows[fl_xid(this)]->title) {
	free(fl_windows[fl_xid(this)]->title);
    }
    fl_windows[fl_xid(this)]->title = strdup(name);
  }
}

////////////////////////////////////////////////////////////////
// Implement the virtual functions for the base Fl_Window class:

// If the box is a filled rectangle, we can make the redisplay *look*
// faster by using X's background pixel erasing.  We can make it
// actually *be* faster by drawing the frame only, this is done by
// setting fl_boxcheat, which is seen by code in fl_drawbox.cxx:
//
// On XFree86 (and prehaps all X's) this has a problem if the window
// is resized while a save-behind window is atop it.  The previous
// contents are restored to the area, but this assummes the area
// is cleared to background color.  So this is disabled in this version.
// Fl_Window *fl_boxcheat;
static inline int can_boxcheat(uchar b) {return (b==1 || (b&2) && b<=15);}

void Fl_Window::show() {
  image(Fl::scheme_bg_);
  if (Fl::scheme_bg_) {
    labeltype(FL_NORMAL_LABEL);
    align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
  } else {
    labeltype(FL_NO_LABEL);
  }
  if (!shown()) {
    fl_open_display();
    if (can_boxcheat(box())) fl_background_pixel = int(fl_xpixel(color()));
    Fl_X::make_xid(this);
  } else {
    //fl_map_window(i->xid);
  }
}

Window fl_window;
Fl_Window *Fl_Window::current_;
GC fl_gc;

// make X drawing go into this window (called by subclass flush() impl.)
void Fl_Window::make_current() {
  static GrLineOption gc;
  gc.lno_color = RED;
  gc.lno_width = 1;
  gc.lno_pattlen = 0;
  gc.lno_dashpat = NULL;
  fl_window = i->xid;
  fl_gc = &gc;
  current_ = this;
  fl_clip_region(0);
  GrSetContext(fl_windows[fl_xid(this)]->context);
}


//
// End of "$Id: Fl_x.cxx,v 1.24.2.24.2.23 2002/08/09 03:17:30 easysw Exp $".
//
