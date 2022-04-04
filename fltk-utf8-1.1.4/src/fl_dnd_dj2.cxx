//
// "$Id: fl_dnd_x.cxx,v 1.5.2.5 2002/08/09 03:17:30 easysw Exp $"
//
// Drag & Drop code for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>
#include <stdio.h>


extern char fl_i_own_selection[2];
extern char *fl_selection_buffer[2];
extern int fl_selection_length[2];
extern int fl_selection_buffer_length[2];



// send an event to an fltk window belonging to this program:
static int local_handle(int event, Fl_Window* window) {
  Fl::e_x = Fl::e_x_root-window->x();
  Fl::e_y = Fl::e_y_root-window->y();
  int ret = Fl::handle(event,window);
  return ret;
}

int Fl::dnd() {
  static char is_here = 0;
  if (is_here) return 0;
  Fl_Widget *foc = NULL;
  Fl::first_window()->cursor((Fl_Cursor)21);
  Window source_window = fl_xid(Fl::first_window());
  Fl_Window* local_window = 0;
  int dndversion = 4; int dest_x, dest_y;

  while (Fl::pushed()) {

    // figure out what window we are pointing at:
    Window new_window = 0; int new_version = 0;
    Fl_Window* new_local_window = 0;
      if (local_window) {
	local_handle(FL_DND_LEAVE, local_window);
      }
      local_window = new_local_window;
      if (local_window) {
	local_handle(FL_DND_ENTER, local_window);
      }
    if (local_window) {
      local_handle(FL_DND_DRAG, local_window);
      foc = focus_;
    }
    Fl::wait();
    Fl::check();
    Fl::flush();
  }

  if (local_window) {
    fl_i_own_selection[0] = 1;
    if (foc && foc->handle(FL_DND_RELEASE)) {
	e_text = fl_selection_buffer[0];
        e_length = fl_selection_length[0];
	foc->handle(FL_DROP);
    }
  }

  Fl::first_window()->cursor(FL_CURSOR_DEFAULT);
  is_here = 0;
  return 1;
}


//
// End of "$Id: fl_dnd_x.cxx,v 1.5.2.1 2002/01/09 21:50:02 easysw Exp $".
//
