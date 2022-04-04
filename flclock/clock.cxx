/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000  O'ksi'D
 *
 *   This program is freedom software; you can redistribute it and/or modify
 *   it under the terms of the O'ksi'D Freedom Software License as published by
 *   O'ksi'D; either version 1.2 of the License, or (at your option) any
 *   later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   O'ksi'D Freedom Software License for more details.
 *
 *   You should have received a copy of the O'ksi'D Freedom Software License
 *   along with this program; if not, ask the distributor of this program.
 *
 *   Author : Jean-Marc Lienher ( http://oksid.ch )
 *
 ******************************************************************************/


#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Clock.H>
#include <FL/Fl_Round_Clock.H>

int main(int argc, char **argv) {
  Fl_Window window2(220,220,"flclock");
  Fl_Round_Clock c2(0,0,220,220); // c2.color(3,4);
  window2.resizable(c2);
  window2.end();
  window2.xclass("Fl_Clock");
  window2.show(argc,argv);
  window2.size(60, 60);
  return Fl::run();
}

