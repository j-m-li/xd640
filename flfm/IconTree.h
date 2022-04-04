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


#ifndef IconTree_h
#define IconTree_h

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Menu_Item.H>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <FL/Fl_File_Chooser.H>

class Entry {
public:
    	char *name;
	char *url;
    	int nbd;
    	struct dirent **dirent;
    	Entry *parent;
	Fl_Pixmap *pix;
	const Fl_Menu_Item *menu;
    	Entry(char *n, char *fn, char is, Entry *p) {
		url = fn;
      		name = n;
      		dirent = NULL;
      		parent = p;
      		nbd = is;
		pix = NULL;
		menu = NULL;
	}

	~Entry() {
      		free(name);
		free(url);
      		if (dirent) {
        		while (nbd > 0) free(dirent[--nbd]);
        		free(dirent);
      		}
    	}
};

class IconTree : public Fl_Tree 
{
public:
	char no_scan;
	int dragging;

	IconTree(int X, int Y, int W, int H);
	~IconTree();
	void rescan();
	void item_draw(int,int,int, int,int,int,int);
	void item_measure(int);
	int item_nb_children(int);
	int item_has_children(int);
	void *item_get_child(int, int);
	int item_handle(int, int);
	void item_free(int);

	static void item_cb(Fl_Widget*, void*);
	char *item_get_path(int n, int c);
	Entry *get_entry(int n) {return (Entry*)datas[n];}
};


#endif
