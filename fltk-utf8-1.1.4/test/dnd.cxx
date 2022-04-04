//
// "$Id: $"
//
// dnd example program for the Fast Light Tool Kit (FLTK).
//
//
//
// Copyright (c) 2000-2002 by Bill Spitzak and others.
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
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if !defined(WIN32) && !NANO_X
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <FL/x.H>
Atom XdndActionAsk;
extern Atom fl_dnd_source_action;
extern Atom fl_dnd_action;
extern Window fl_dnd_source_window;
#endif 
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Group.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>

// a input which is a drop target
class My_Input : public Fl_Input {
public:
        My_Input(int x, int y, int w, int h) : Fl_Input(x, y, w, h) { };
        int handle(int);
};

// a button which is a drag source
class My_Button : public Fl_Button {
public:
        My_Button(int x, int y, int w, int h, const char *c = 0)
                : Fl_Button(x, y, w, h, c) { };
        int handle(int);
};

My_Input *inp;
My_Button *button;

// drop target handle
int My_Input::handle(int event)
{
        static char buffer[64 * 1024];
        switch(event) {
        case FL_DND_ENTER:		
		printf("FL_DND_ENTER\n");
		button->color(2);
                button->redraw();
                return 1;
        case FL_DND_DRAG:
		printf("FL_DND_DRAG\n");
#if !defined(WIN32) && !NANO_X
		if (fl_dnd_source_action == XdndActionAsk) {
			fl_dnd_action = XdndActionAsk;
		}
#endif
                return 1;
        case FL_DND_LEAVE:
		printf("FL_DND_LEAVE\n");
		button->color(1);
                button->redraw();
                return 1;
        case FL_DND_RELEASE:
		printf("FL_DND_RELEASE\n");
		button->color(1);
                button->redraw();
                return 1;
	case FL_DROP:
		if (!Fl::event_text()) return 0;
#if !defined(WIN32) && !NANO_
		if (fl_dnd_source_action == XdndActionAsk) {
		  sprintf(buffer, "Ask Action: %s", Fl::event_text());
		} else {
		  sprintf(buffer, "Copied: %s", Fl::event_text());
		}
#else
		sprintf(buffer, "Copied: %s", Fl::event_text());
#endif
		printf("%s\n", buffer);
		value(buffer);
		return 1;
        case FL_PASTE:
		printf("paste, %s\n",  Fl::event_text());
        default:
		break;
	}
	return Fl_Input::handle(event);
}

// drag source handle
int My_Button::handle(int event)
{
        static char buffer[64 * 1024];
        sprintf(buffer, "%s", inp->value());
        switch(event) {
        case FL_PUSH:
		Fl::copy(buffer, strlen(buffer), 0);
		Fl::dnd();
		return 1;
        default:
                break;
        }
	return Fl_Button::handle(event);
}

int main(int argc, char ** argv) {
#if !defined(WIN32) && !NANO_X
   fl_open_display();
   XdndActionAsk     = XInternAtom(fl_display, "XdndActionAsk",     0);
#endif
   Fl_Window *window = new Fl_Window(250,100);
   Fl_Group g(0, 0, 250,100);
   g.end();
   button = new My_Button(10,10,80,40,"Drag'n'Drop");
   button->color(1);
   inp = new My_Input(10, 60, 230, 30);
#if !defined(WIN32) && !NANO_X
   inp->value("file:///tmp/dnd_test.txt");
#else 
   inp->value("C:\\Temp\\dnd_test.txt");
#endif
   window->show(argc,argv);
   Fl::flush();
   return Fl::run();
}

//
// End of "$Id:  $".
//
