/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2002  O'ksi'D
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
#ifndef Xd6HtmlToRtf_h
#define Xd6HtmlToRtf_h

#include <FL/Fl_Widget.h>
#include <FL/Fl_Output.h>
#include <FL/x.h>
#include "Xd6HtmlBlock.h"
#include "Xd6HtmlFrame.h"
#include "Xd6XmlTree.h"
#include "Xd6ConfigFile.h"
#include <stdio.h>
#include <string.h>


class Xd6HtmlToRtf {
public:
	Xd6HtmlFrame *frame;

	char **open_tags;
	char **close_tags;
	int nb_tags;
	

	FILE *fp;
	int lstyle;
	Xd6HtmlBlock *lb;
	int tab_nb;

	Xd6HtmlToRtf(Xd6HtmlFrame *f);

	static void header(FILE *fp);
	void to_rtf(const char *name);
	void to_rtf_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, Xd6HtmlSegment *s,
        	char *c, int len);
	static void scan_to_rtf_cb(Xd6HtmlBlock *b, Xd6HtmlLine *l, 
        	Xd6HtmlSegment *s, char *c, int len , void *d);
	void block_to_rtf(FILE *fp, Xd6HtmlBlock *b, Xd6HtmlBlock *lb);
	void seg_to_rtf(FILE *fp, Xd6HtmlSegment *s, int style);
	static void text_to_rtf(FILE *fp, char *txt, int len, int nonbsp = 0);
	void push_tag(FILE *fp, char *otag, char *ctag);
	void pop_tag(FILE *fp, char *ctag);
	void pop_all_tags(FILE *fp);
};

#endif

/*
 * "$Id: $"
 */
