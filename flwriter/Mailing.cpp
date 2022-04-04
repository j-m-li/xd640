/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000,2001  O'ksi'D
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


#include <xd640/Xd6Std.h>
#include "Mailing.h"
#include "callbacks.h"
#include <FL/Fl_File_Chooser.H>
#include <xd640/Xd6XmlParser.h>

#define _(String) gettext((String))

Mailing::Mailing(Xd6HtmlEditor *ed) : Fl_Window(400, 300)
{
	e = ed;
	t = NULL;
	table = NULL;
	nb_col = -1;
	nb_row = -1;

	bo_file = new Fl_Box(5, 5, 390, 25);
	bo_file->label(_("Data file :"));
	bo_file->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	in_file = new Fl_Input(5, 35, 290, 25);
	in_file->callback(cb_in_file, this);
	bu_file = new Fl_Button(300, 35, 90, 25, _("Browse"));
	in_from = new Fl_Input(60, 65, 40, 25, _("From:"));
	in_to = new Fl_Input(170, 65, 40, 25, _("To:"));

	bo_text = new Fl_Box(5, 95, 390, 25);
	bo_text->label(_("Text file :"));
	bo_text->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	in_text = new Fl_Input(5, 125, 290, 25);
	bu_text = new Fl_Button(300, 125, 90, 25, _("Browse"));
	in_text->value(e->file);

	quit = new Fl_Button(320, 270, 75, 25, _("Cancel"));
	bu_run = new Fl_Button(100, 270, 75, 25, _("Run"));

}


Mailing::~Mailing()
{

}

void Mailing::load(const char *f)
{
}

void Mailing::go()
{
	cb_atexit();
	show();
	for (;;) {
		Fl_Widget *o;
		o = Fl::readqueue();
		if (o == this || o == quit) break;
		if (o == bu_file) {
			const char *f;
			f = fl_file_chooser(_("Open data file..."), "*.html",
				NULL);
			if (f) {
				in_file->value(f);
				cb_in_file(in_file, (void*) this);
			}
		} else if (o == bu_text) {
			const char *f;
			f = fl_file_chooser(_("Open text file..."), "*.html",
				NULL);
			if (f) {
				in_text->value(f);
			}
		} else if (o == bu_run) {
			run();
			break;
		}
		Fl::wait();
	}
}

void Mailing::run()
{
	int i;
	int from = 1;
	int to = 1;
	int fro;
	Xd6XmlParser *p = new Xd6XmlParser();
	Xd6HtmlFrame *of;

	p->parse_file(in_text->value());
	t = p->tree->root;
	
	from = (int) atol(in_from->value());
	to = (int) atol(in_to->value());
	
	i = 0;
	of = e->view->frame;
	e->view->frame = NULL;	
	fro = from;
	while (from <= to) {
		line = from;
		e->view->new_frame(0, 0);
		f = e->view->frame;
		tree_replace(t);
		f->tree2block(t);
		f->measure();
		if (fro == from) {
			e->view->printer->print(f);
		} else {
			e->view->printer->direct_print(f);
		}
		delete(f);
		from++;
	}
	delete(e->view->printer);
	e->view->printer = new Xd6HtmlPrint(of, "output.ps");
	e->view->frame = of;
	
	delete(p);
}

int Mailing::set_word(Xd6XmlTreeElement* e, int style)
{
	char *w;
	const char *h;
	int c, i;
	int l, ll;
	char *buf;
	Xd6XmlTreeText *txt;

	h = e->get_attr_value("href");
	if (!h) return 0;
	
	while(e->nb_children > 0) {
                e->nb_children--;
                if (!e->children[e->nb_children]) continue;
                if (e->children[e->nb_children]->type == 
			Xd6XmlTreeSegment_element) 
		{
                        delete((Xd6XmlTreeElement*)e->children[e->nb_children]);
                } else if (e->children[e->nb_children]->type & 
                        Xd6XmlTreeSegment_text) 
                {
                        delete((Xd6XmlTreeText*)e->children[e->nb_children]);
                } else {
                        delete(e->children[e->nb_children]);
                }
	}
	txt = e->create_child_text();
	c = 0;
	while (c <= nb_col) {
		if (table[0][c][strlen(table[0][c]) - 1] == ' ') {
			table[0][c][strlen(table[0][c]) - 1] = '\0';
		}
		if (!strcmp(h, table[0][c])) break;
		c++;
	}
	if (c > nb_col) return 0;
	w = table[line][c];
	l = 0;
	ll = 0;
	while (w[l]) {
		if (w[l] == ' ') {
			buf = (char*) malloc(ll + 2);
			i = 0;
			while (i <= ll) {	
				buf[i] = w[l - ll + i];
				i++;
			}	
			buf[i] = '\0';
			txt->data = buf;
			txt->len = ll + 1;
			txt = e->create_child_text();
			ll = -1;
		}
		l++;
		ll++;
	}
	if (ll > 0) {
		buf = (char*) malloc(ll + 1);
		i = 0;
		while (i < ll) {	
			buf[i] = w[l - ll + i];
			i++;
		}	
		buf[i] = '\0';
		txt->data = buf;
		txt->len = ll;
	}
	e->style = style & ~(TOP_MARGIN|DISPLAY|PAGE_BREAK|LIST|IS_BLOCK);
	e->style |= IS_INLINE;
	return 1;
}

void Mailing::tree_replace(Xd6XmlTreeElement *elem)
{
        int i;
        if (!elem) return;

        for (i = 0; i < elem->nb_children; i++) {
                Xd6XmlTreeElement *e;
                switch(elem->children[i]->type) {
                case Xd6XmlTreeSegment_none:
                        break;
                case Xd6XmlTreeSegment_element:
                        e = (Xd6XmlTreeElement*)elem->children[i];
			if (e->display == DISPLAY_A_LINK) {
				if (set_word(e, elem->style)) break;
			}
                        tree_replace(e);
                        break;
                default:
			break;
                }
        }

}

int Mailing::to_text(Xd6XmlTreeElement *elem, char **text, int l)
{
	int i, j;
	for (i = 0; i < elem->nb_children; i++) {
		Xd6XmlTreeText *txt;
		switch(elem->children[i]->type) {
		case Xd6XmlTreeSegment_element:
			l = to_text((Xd6XmlTreeElement*)elem->children[i], 
				text, l);
			break;
		default:
			txt = (Xd6XmlTreeText*)elem->children[i];
			*text = (char*) realloc(*text, l + txt->len + 1);
			j = 0;
			while (j < txt->len) {
				(*text)[l] = txt->data[j];
				j++; l++;
			}	
			(*text)[l] = '\0';
		}
	}
	return l;
}

void Mailing::get_cell(Xd6XmlTreeElement *elem)
{
	int i;
	if ((elem->style & DISPLAY)) {
	  	if (elem->display == DISPLAY_TABLE_CELL) {
			if (status == 2) {
				nb_col++;
				table[nb_row] = (char**) realloc(table[nb_row],
					sizeof(char**) * (nb_col + 1));
			} else if (col > nb_col) {
				return;
			}
			table[nb_row][col] = NULL;
			to_text(elem, &table[nb_row][col], 0);
			col++;
			return;
		} else if (elem->display == DISPLAY_TABLE) {
			if (status == 0) {
				status = 1;
			} else {
				return;
			}
		} else if (elem->display == DISPLAY_TABLE_ROW) {
			if (status == 1) {
				status = 2;
			} else if (status == 2) {
				status = 3;
			}
			nb_row++;
			table = (char***) realloc(table, 
				sizeof(char**) * (nb_row + 1));
			if (status == 2) {
				table[nb_row] = NULL;
			} else {
				table[nb_row] = (char**) malloc(sizeof(char*)
					* (nb_col + 1));
			}
			col = 0;
		}
	}
	for (i = 0; i < elem->nb_children; i++) {
		switch(elem->children[i]->type) {
		case Xd6XmlTreeSegment_element:
			get_cell((Xd6XmlTreeElement*) elem->children[i]);
			break;
		default:
			break;
		}
	}
}

void Mailing::get_table(Xd6XmlTreeElement *tb)
{
	char buf[25];
	int i;
	while (nb_row > 0) {
		i = nb_col;
		nb_row--;
		while (i > 0) {
			i--;
			free(table[nb_row][i]);
		}
		free(table[nb_row]);
	}
	nb_col = -1;
	nb_row = -1;
	col = 0;
	if (table) free(table);
	status = 0;
	table = NULL;
	get_cell(tb);
	snprintf(buf, 25, "%d", nb_row);
	in_to->value(buf);
	in_from->value("1");
}

void Mailing::cb_in_file(Fl_Widget *w, void *d)
{
	Mailing *self = (Mailing*) d;
	Xd6XmlParser *p = new Xd6XmlParser();
	p->parse_file(self->in_file->value());
	self->get_table(p->tree->root);
	delete(p);
}	

/*
 * "$Id: $"
 */

