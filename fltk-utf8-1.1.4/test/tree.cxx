//
// "$Id:  $"
//
// Tree test program for the Fast Light Tool Kit (FLTK).
//
//              Copyright 2003 by O'ksi'D.
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
//  Report bugs to oksid@bluewin.ch
//


#include <FL/Fl.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tree.H>
#include <FL/fl_draw.H>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


class Directory : public Fl_Tree
{
public:
  class Entry {
  public:
    char *name;
    int nbd;
    struct dirent **dirent;
    Entry *parent;

    Entry(char *n, char is, Entry *p) {
      name = n;
      dirent = NULL; 
      parent = p;
      nbd = is;
    }

    ~Entry() {
      free(name);
      if (dirent) {
        while (nbd > 0) free(dirent[--nbd]);
        free(dirent);
      }
    }
  };
  
  Directory(int X, int Y, int W, int H) :  Fl_Tree (X,Y,W,H) {
    char *n = strdup("/");
    struct dirent **dir = NULL;
    int nb = fl_filename_list(n, &dir, fl_alphasort);
    root(new Entry(n, nb, NULL));
    callback(my_item_cb);
  }

  static void my_item_cb(Fl_Widget *w, void *data) {
    Directory *d = (Directory*) w;
    int n = d->get_selection();
    Entry *e = d->get_entry(n);
    if (Fl::event_clicks()) {
      if (d->item_flags(n) & FLAG_OPEN) {
        d->item_close(n);
      } else {
        d->item_open(n);
      }
      printf("double click on %s\n", e->name);
      Fl::event_clicks(0);
    } else {
      printf("click on %s\n", e->name);
    }
  }

  Entry *get_entry(int n) {return (Entry*)datas[n];}
 
  void item_draw(int n,int X,int Y, 
	int CX,int CY, int CW,int CH) 
  {
    Entry *e = get_entry(n);
    fl_font(0, 14);
    fl_color((item_flags(n) & FLAG_SELECTED) ? FL_RED : FL_GREEN);
    fl_draw(e->name, X + 2 , Y + fl_height() - fl_descent()); 
    if (sel == n) {
      dot_rect(X + 2, Y, widths[n], heights[n]);   
    }
  }

  void item_measure(int n) {
    Entry *e = get_entry(n);
    fl_font(0, 14);
    heights[n] = fl_height();
    widths[n] = (int) fl_width(e->name);
  }

  int item_has_children(int n) {
    Entry *e = get_entry(n);
    return e->nbd;
  }

  int item_nb_children(int n) {
    Entry *e = get_entry(n);
    char *b1;
    if (e->nbd >= 0 && e->dirent ==  NULL) {
      dirent **dir = NULL;
      b1 = item_get_path(n, 0);
      e->nbd = fl_filename_list(b1, &dir, fl_alphasort);
      e->dirent = dir;
    }  
    if (e->dirent && e->nbd >= 2) {
      return e->nbd;
    }
    return -1;
  }

  char *item_get_path(int n, int c) {
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

  void *item_get_child(int n, int c) {
    Entry *e = get_entry(n);
    Entry *m;
    char *b1;
    char is_dir = -1;
    b1 = item_get_path(n, c);
    if (fl_filename_isdir(b1)) {
      is_dir = 0;
    }
    m = new Entry(strdup(e->dirent[c-1]->d_name), is_dir, e); 
    if (!strcmp(m->name, ".") || !strcmp(m->name, "..")) m->nbd = -1;
    return m;  
  }

  void item_free(int n) {
    Entry *e = get_entry(n);
    delete(e);
  }
 
};

int main(int argc, char** argv) {
  Fl_Window window(300,400);
  Directory dir(0,0, 300, 400);
  window.resizable(window);
  window.end();
  window.show(argc,argv);
  return Fl::run();
}

//
// End of "$Id:  $".
//
