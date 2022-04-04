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
#  include <nxdraw.h>
#  include <nanowm.h>
#  include <FL/Fl_Window.H>
#  include <FL/fl_utf8.H>
#  include <stdio.h>
#  include <stdlib.h>
#  include "flstring.h"
#  include <unistd.h>

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
#    if MSDOS
#      define fd_set int 
#      define FD_SET(a,b)
#      define FD_CLR(a,b)
#      define FD_ISSET(a,b) 1
struct timeval {
	unsigned long tv_sec, tv_usec;
};
static int select(int nfds, fd_set *r, fd_set *w, fd_set *e, struct timeval *t) 
{
	return 1;
}

#    endif
static fd_set fdsets[3];
static int maxfd;
#    define POLLIN 1
#    define POLLOUT 4
#    define POLLERR 8

#  endif /* USE_POLL */
/* JHC 09/19/00 - Added this array to simplify grabbing keystrokes */
/* Because we care about so many different modifiers, its almost easier */
/* to have an array instead of a million if statements */

/* I think I have everything mapped, but check me for accuracy */

static struct
{
  unsigned short key;
  unsigned short value;
}
keymap_array[] =
{
  {
  MWKEY_ENTER, FL_Enter}
  , {
  MWKEY_BACKSPACE, FL_BackSpace}
  , {
  MWKEY_TAB, FL_Tab}
  , {
  MWKEY_SCROLLOCK, FL_Scroll_Lock}
  , {
	MWKEY_ESCAPE, FL_Escape}
	, {
  MWKEY_HOME, FL_Home}
  , {
  MWKEY_LEFT, FL_Left}
  , {
  MWKEY_UP, FL_Up}
  , {
  MWKEY_RIGHT, FL_Right}
  , {
  MWKEY_DOWN, FL_Down}
  , {
  MWKEY_PAGEUP, FL_Page_Up}
  , {
  MWKEY_PAGEDOWN, FL_Page_Down}
  , {
  MWKEY_END, FL_End}
  ,
    //  { 99 , FL_Insert},
  {
  MWKEY_BACKSPACE, FL_BackSpace}
  , {
  MWKEY_KP_ENTER, FL_KP_Enter}
  , {
  MWKEY_KP7, FL_KP + '7'}
  , {
  MWKEY_KP4, FL_KP + '4'}
  , {
  MWKEY_KP8, FL_KP + '8'}
  , {
  MWKEY_KP6, FL_KP + '6'}
  , {
  MWKEY_KP2, FL_KP + '2'}
  , {
  MWKEY_KP9, FL_KP + '9'}
  , {
  MWKEY_KP3, FL_KP + '3'}
  , {
  MWKEY_KP1, FL_KP + '1'}
  , {
  MWKEY_KP5, FL_KP + '5'}
  , {
  MWKEY_KP0, FL_KP + '0'}
  , {
  MWKEY_KP_PERIOD, FL_KP + '.'}
  , {
  MWKEY_KP_MULTIPLY, FL_KP + '*'}
  , {
  MWKEY_KP_PLUS, FL_KP + '+'}
  , {
  MWKEY_KP_MINUS, FL_KP + '-'}
  , {
  MWKEY_KP_DIVIDE, FL_KP + '/'}
  , {
  MWKEY_F1, FL_F + 1}
  , {
  MWKEY_F2, FL_F + 2}
  , {
  MWKEY_F3, FL_F + 3}
  , {
  MWKEY_F4, FL_F + 4}
  , {
  MWKEY_F5, FL_F + 5}
  , {
  MWKEY_F6, FL_F + 6}
  , {
  MWKEY_F7, FL_F + 7}
  , {
  MWKEY_F8, FL_F + 8}
  , {
  MWKEY_F9, FL_F + 9}
  , {
  MWKEY_F10, FL_F + 10}
  , {
  MWKEY_F11, FL_F + 11}
  , {
  MWKEY_F12, FL_F + 12}
  , {
  MWKEY_RSHIFT, FL_Shift_R}
  , {
  MWKEY_LSHIFT, FL_Shift_L}
  , {
  MWKEY_LCTRL, FL_Control_L}
  , {
  MWKEY_RCTRL, FL_Control_R}
  , {
  MWKEY_CAPSLOCK, FL_Caps_Lock}
  , {
  MWKEY_LMETA, FL_Alt_L}
  , {
  MWKEY_RMETA, FL_Alt_R}
  , {
  MWKEY_DELETE, FL_Delete}
  , {
  0, 0}
};

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

#  if CONSOLIDATE_MOTION
static Fl_Window* send_motion;
extern Fl_Window* fl_xmousewin;
#  endif
static void do_queued_events() {
    XEvent event;
    GrCheckNextEvent(&event);
    if(event.type == GR_EVENT_TYPE_NONE) return;
    fl_handle(event);
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
  if (fl_display) {do_queued_events(); return 1;}

#  if !USE_POLL
  fd_set fdt[3];
  fdt[0] = fdsets[0];
  fdt[1] = fdsets[1];
  fdt[2] = fdsets[2];
#  endif
  int n;

  fl_unlock_function();

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
int update = 0;

static void fd_callback(int,void *) {
  do_queued_events();
}

GR_SCREEN_INFO si;

void fl_open_display() {
  if (fl_display) return;
 
  int d = 0;
  if ((d = GrOpen ()) < 0) {
      printf ("cannot open Nano-X graphics,Please run 'nano-X -p' first.\n");
      exit (1);
  }

  GrSetErrorHandler(NULL);
#if MSDOS
  GrGetScreenInfo(&si);
  GR_EVENT event;
  GR_WM_PROPERTIES props;
  win window;
  	/* add root window*/
	window.wid = GR_ROOT_WINDOW_ID;
	window.pid = GR_ROOT_WINDOW_ID;
	window.type = WINDOW_TYPE_ROOT;
	window.clientid = 1;
	window.sizing = GR_FALSE;
	window.active = 0;
	window.data = NULL;
	add_window(&window);

	GrSelectEvents(GR_ROOT_WINDOW_ID, GR_EVENT_MASK_CHLD_UPDATE);

	/* Set new root window background color*/
	props.flags = GR_WM_FLAGS_BACKGROUND;
	props.background = GrGetSysColor(GR_COLOR_DESKTOP);
	GrSetWMProperties(GR_ROOT_WINDOW_ID, &props);

  printf ("this is in nano-X\n");
#endif
  fl_display = (Display*)d;

  // for nanox,by tanghao
  fl_screen = 0;

  //end nanox
}


void fl_close_display() {
	GrClose();
}

int Fl::h() {
  fl_open_display ();

  GR_SCREEN_INFO si;		/* information about screen */
  GrGetScreenInfo (&si);
  return si.cols;
}

int Fl::w() {
  fl_open_display ();

  GR_SCREEN_INFO si;		/* information about screen */
  GrGetScreenInfo (&si);
  return si.rows;
}

void
Fl::get_mouse (int &x, int &y)
{
  fl_open_display ();
  GR_WINDOW_INFO info;
  GrGetWindowInfo (fl_window, &info);	//(GR_WINDOW_ID wid, GR_WINDOW_INFO *infoptr);
  x = info.x + info.width / 2;
  y = info.y + info.height / 2;
  //end nanox
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

GR_EVENT* fl_xevent; // the current x event
ulong fl_event_time; // the last timestamp from an x event

char fl_key_vector[32]; // used by Fl::get_key()

// Record event mouse position and state from an XEvent:

static int px, py;
static ulong ptime;

static void set_event_xy() {
#  if CONSOLIDATE_MOTION
  send_motion = 0;
#  endif
  Fl::e_x_root = fl_xevent->button.rootx;
  Fl::e_x = fl_xevent->button.x;
  Fl::e_y_root = fl_xevent->button.rooty;
  Fl::e_y = fl_xevent->button.y;
  ulong state = Fl::e_state & 0xff0000;	// keep shift key states
  if (fl_xevent->button.buttons & GR_BUTTON_L)
    state |= FL_BUTTON3;
  if (fl_xevent->button.buttons & GR_BUTTON_M)
    state |= FL_BUTTON2;
  if (fl_xevent->button.buttons & GR_BUTTON_R)
    state |= FL_BUTTON1;
  Fl::e_state = state;  
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
fl_handle (const GR_EVENT & xevent)
{
  int i;

  GR_WINDOW_INFO info;
  fl_xevent = (GR_EVENT *) & xevent;
  Window xid = xevent.general.wid;	//fl_window;

  int button = 0;
  int event = 0;
  Fl_Window *window = fl_find (xid);

  /* #$*#&$ - Some events are not tied to a window */

  if (xevent.type == GR_EVENT_TYPE_FDINPUT) {
    int fdnum = xevent.fdinput.fd;
    
    for(i = 0; i < nfds; i++) {
      if (fd[i].fd == fdnum) { 
	if (fd[i].cb) fd[i].cb(fdnum, fd[i].arg);
	break;
      }
    }

    return Fl::handle (event, window);
  }


  if (window)

    switch (xevent.type) {

    case GR_EVENT_TYPE_CLOSE_REQ:
      event = FL_CLOSE;
      if (Fl::handle (event, window))
	exit(0);
      else
	exit(-1);
      break;

    case GR_EVENT_TYPE_UPDATE:
      update = 1;
      GR_WINDOW_INFO info;
      switch (xevent.update.utype) {
      case GR_UPDATE_MAP:
	//fix_exposure = 1;
	event = FL_SHOW;
	break;
      case GR_UPDATE_SIZE:
	GrGetWindowInfo(xid,&info);
	window->resize(info.x, info.y, xevent.update.width, xevent.update.height);
        Fl_X::i (window)->wait_for_expose = 0;
	break;
      case GR_UPDATE_MOVE:
	GrGetWindowInfo(xid,&info);
	window->resize(xevent.update.x, xevent.update.y, info.width, info.height);
	break;
      default:
	break;
      }
      break;
    case GR_EVENT_TYPE_EXPOSURE:

      Fl_X::i (window)->wait_for_expose = 0;

      //if ( !fix_exposure )
      	window->damage (FL_DAMAGE_EXPOSE, xevent.exposure.x,
		      xevent.exposure.y, xevent.exposure.width,
		      xevent.exposure.height);
      //fix_exposure = 0;

      
/*      if (Fl::first_window ()->non_modal ()
	  && window != Fl::first_window ())
	Fl::first_window ()->show ();
*/
      break;

    case GR_EVENT_TYPE_BUTTON_UP:	//tanghao
      if (xevent.button.changebuttons & 0x04) {
	button = FL_Button + 0x01;
      } else if (xevent.button.changebuttons & 0x02)
	button = FL_Button + 0x02;
      else
	button = FL_Button + 0x03;

      Fl::e_keysym = button;	//tanghao have problem +
      set_event_xy ();
      Fl::e_state &= ~(0x01 << (xevent.button.buttons - 1));	//tanghao have problem
      event = FL_RELEASE;
      break;

    case GR_EVENT_TYPE_BUTTON_DOWN:	//tanghao
      if (xevent.button.changebuttons & 0x04) {
	button = FL_Button + 0x01;
      } else if (xevent.button.changebuttons & 0x02) {
	button = FL_Button + 0x02;
      } else {
	button = FL_Button + 0x03;
      }

      Fl::e_keysym = button;
      set_event_xy ();
      checkdouble ();
      Fl::e_state |= (0x01 << (xevent.button.buttons - 1));
      event = FL_PUSH;
      break;

    case GR_EVENT_TYPE_MOUSE_POSITION:	//tanghao
      fl_window = xevent.mouse.wid;
      set_event_xy ();

#if CONSOLIDATE_MOTION
      send_motion = fl_xmousewin = window;
      return 0;
#else
      event = FL_MOVE;
      break;
#endif

    case GR_EVENT_TYPE_FOCUS_IN:
      event = FL_FOCUS;
      break;

    case GR_EVENT_TYPE_FOCUS_OUT:
      event = FL_UNFOCUS;
      break;

    case GR_EVENT_TYPE_KEY_UP:
      {
	int keycode = xevent.keystroke.ch;
	fl_key_vector[keycode / 8] &= ~(1 << (keycode % 8));
	set_event_xy ();
	break;
      }

    case GR_EVENT_TYPE_KEY_DOWN:
      {
	unsigned short keycode = xevent.keystroke.ch;
	static char buffer[21];
	int len = 0;

	buffer[len++] = keycode;
	buffer[len] = 0;

	/* Modifiers, passed from Nano-X */

	Fl::e_state = 0;

	 if ( (keycode == MWKEY_LCTRL) || (keycode == MWKEY_RCTRL) )
	   break;
	 else if ( (keycode == MWKEY_LALT) || (keycode == MWKEY_RALT ) )
	   break;
	 else if (keycode == MWKEY_LSHIFT || (keycode == MWKEY_RSHIFT) )
	   break;
	 
	 if (xevent.keystroke.modifiers & MWKMOD_CTRL)
	   Fl::e_state |= FL_CTRL;
	 if (xevent.keystroke.modifiers & MWKMOD_SHIFT)
	   Fl::e_state |= FL_SHIFT;
	 if (xevent.keystroke.modifiers & MWKMOD_CAPS)
	   Fl::e_state |= FL_CAPS_LOCK;
	 if (xevent.keystroke.modifiers & MWKMOD_NUM)
	   Fl::e_state |= FL_NUM_LOCK;
	 if (xevent.keystroke.modifiers & (MWKMOD_ALT|MWKMOD_META))
	   Fl::e_state |= FL_META;
	 
	 /* This goes through the new keymap_array, and
	   handles those keys that are defined.  Otherwise,
	   we just drop out and set the keysem to the raw value */

	int i = 0;

	while (keymap_array[i].value) {
	  if (keycode == keymap_array[i].key) {
	    Fl::e_keysym = keymap_array[i].value;
	    break;
	  }

	  i++;
	}

	if (keymap_array[i].value == 0) {
	  Fl::e_keysym = (keycode & 0x00FF);
	}
#ifdef OLDOLDOLD

	if (keycode == '\r')
	  Fl::e_keysym = (int) 65293;	//tanghao Enter
	else if (keycode == '\b')
	  Fl::e_keysym = (int) 65288;	//tanghao backspace
	else if (keycode == 82)
	  Fl::e_keysym = (int) 65362;	//tanghao up
	else if (keycode == 84)
	  Fl::e_keysym = (int) 65364;	//tanghao down
	else if (keycode == 81)
	  Fl::e_keysym = (int) 65361;	//tanghao left
	else if (keycode == 83)
	  Fl::e_keysym = (int) 65363;	//tanghao right
	else if (keycode == 227)
	  Fl::e_keysym = (int) FL_Control_L;	// left ctrl
	else if (keycode == 225)
	  Fl::e_keysym = (int) FL_Alt_L;	// left alt
	else if (keycode == 233)
	  Fl::e_keysym = (int) FL_Shift_L;	// left shift
	else
	  Fl::e_keysym = (int) (keycode & 0x00FF);	//tanghao
#endif

	Fl::e_text = buffer;
	Fl::e_length = len;

	Fl::e_is_click = 0;

	event = FL_KEYBOARD;
	break;
      }
    case GR_EVENT_TYPE_MOUSE_ENTER:
      set_event_xy ();
      //    Fl::e_state = xevent.xcrossing.state << 16;
      event = FL_ENTER;
      break;

    case GR_EVENT_TYPE_MOUSE_EXIT:
      set_event_xy ();
      //    Fl::e_state = xevent.xcrossing.state << 16;
      event = FL_LEAVE;
      break;

   
    } else {
#if MSDOS
	// window manager
		switch(fl_xevent->type) {
			case GR_EVENT_TYPE_ERROR:
				//printf("nanowm: error %d\n", fl_xevent->error.code);
				break;
			case GR_EVENT_TYPE_EXPOSURE:
				do_win_exposure(&fl_xevent->exposure);
				break;
			case GR_EVENT_TYPE_BUTTON_DOWN:
				do_button_down(&fl_xevent->button);
				break;
			case GR_EVENT_TYPE_BUTTON_UP:
				do_button_up(&fl_xevent->button);
				break;
			case GR_EVENT_TYPE_MOUSE_ENTER:
				do_mouse_enter(&fl_xevent->general);
				break;
			case GR_EVENT_TYPE_MOUSE_EXIT:
				do_mouse_exit(&fl_xevent->general);
				break;
			case GR_EVENT_TYPE_MOUSE_POSITION:
				do_mouse_moved(&fl_xevent->mouse);
				break;
			case GR_EVENT_TYPE_KEY_DOWN:
				do_key_down(&fl_xevent->keystroke);
				break;
			case GR_EVENT_TYPE_KEY_UP:
				do_key_up(&fl_xevent->keystroke);
				break;
			case GR_EVENT_TYPE_FOCUS_IN:
				do_focus_in(&fl_xevent->general);
				break;
			case GR_EVENT_TYPE_CHLD_UPDATE:
				do_update(&fl_xevent->update);
				break;
			default:
				//fprintf(stderr, "Got unexpected event %d\n",
								//fl_xevent->type);
				break;
		}
	return 1;
#endif
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
      redraw ();
      i->wait_for_expose = 1;
    }
  } else {
    x (X);
    y (Y);
  }

  if (resize_from_program && shown ()) {

    if (is_a_resize) {
      GrMoveWindow (i->xid, X + abs (w () - W), Y);
      GrResizeWindow (i->xid, W > 0 ? W : 1, H > 0 ? H : 1);
    } else {
      GrMoveWindow (i->xid, X, Y);
    }

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

static const int childEventMask = ExposureMask;


int fl_show_it = 1;

static const long XEventMask = GR_EVENT_MASK_ALL;	//tanghao
int
  Fl_X::mw_parent = 1;
int
  Fl_X::mw_parent_xid = 0;
int
  Fl_X::mw_parent_top = 0;

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

  // root = either current window id or the MicroWindows root window id.
  ulong root;


  root = w->parent ()? fl_xid (w->window ()) : GR_ROOT_WINDOW_ID;

  GR_WM_PROPERTIES props;
  props.flags = GR_WM_FLAGS_PROPS | GR_WM_FLAGS_TITLE;
  GR_CHAR *title = (GR_CHAR *) w->label ();
  props.title = title;

  props.props = GR_WM_PROPS_APPWINDOW;

  if (!mw_parent && !Fl::grab ()) {

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

    // This is main for menubars so that they are outside the
    // parent window, yet seem as a child of the parent.
    GR_WINDOW_INFO info;
    if (!mw_parent) {
      GrGetWindowInfo (mw_parent_xid, &info);
      X += info.x;
      Y += info.y;
    }

    GR_WINDOW_ID wid;
    wid = GrNewWindow (root, X, Y, W, H, 0, 1, 0);

    if (mw_parent_top == 0) {
      mw_parent_xid = wid;
      mw_parent_top = 1;
    }

    if (!mw_parent && Fl::grab ()) {
      mw_parent = 1;
      props.props = GR_WM_PROPS_NODECORATE;
    } else {
      mw_parent = 1;
    }

    props.props |= w->wm_props;
		
    GrSetWMProperties (wid, &props);

    Fl_X *x = set_xid (w, wid);

    // Start up a MicrowWindow's select events as each window is created.
    // This is related with the fl_wait() function above.

    if(root == GR_ROOT_WINDOW_ID) {

      GrSelectEvents (wid, GR_EVENT_MASK_BUTTON_DOWN | GR_EVENT_MASK_BUTTON_UP |
		      GR_EVENT_MASK_MOUSE_POSITION | GR_EVENT_MASK_KEY_DOWN |
		      GR_EVENT_MASK_KEY_UP | GR_EVENT_MASK_TIMEOUT |
		      GR_EVENT_MASK_FOCUS_IN | GR_EVENT_MASK_FOCUS_OUT |
		      GR_EVENT_MASK_EXPOSURE | GR_EVENT_MASK_CLOSE_REQ |
		      GR_EVENT_MASK_UPDATE | GR_EVENT_MASK_FDINPUT);
    } else {
      GrSelectEvents (wid, GR_EVENT_MASK_KEY_DOWN |
		      GR_EVENT_MASK_KEY_UP | GR_EVENT_MASK_TIMEOUT |
		      GR_EVENT_MASK_FOCUS_IN | GR_EVENT_MASK_FOCUS_OUT |
		      GR_EVENT_MASK_EXPOSURE | GR_EVENT_MASK_CLOSE_REQ |
		      GR_EVENT_MASK_UPDATE | GR_EVENT_MASK_FDINPUT);
      
    }

    w->set_visible ();
    w->handle (FL_SHOW);	// get child windows to appear
    w->redraw ();


    GrMapWindow (x->xid);
    fl_window = x->xid;		//tanghao

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
    GrRaiseWindow (i->xid);
  }
}

Window fl_window;
Fl_Window *Fl_Window::current_;
GC fl_gc;

// make X drawing go into this window (called by subclass flush() impl.)
void Fl_Window::make_current() {
  static GC gc;	// the GC used by all X windows
  if (!gc) gc = XCreateGC(fl_display, i->xid, 0, 0);
  fl_window = i->xid;
  fl_gc = gc;
  current_ = this;
  fl_clip_region(0);
}


//
// End of "$Id: Fl_x.cxx,v 1.24.2.24.2.23 2002/08/09 03:17:30 easysw Exp $".
//
