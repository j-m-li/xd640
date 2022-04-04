/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000, 2001  O'ksi'D
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
 *   Author : Jean-Marc Lienher < http://oksid.ch >
 *
 ******************************************************************************/

#ifndef Xd6XmlTree_h
#define Xd6XmlTree_h

#include "Xd6HtmlBlock.h"
#include "Xd6XmlStyle.h"
#include <stdlib.h>

enum {
	Xd6XmlTreeSegment_none 		= 0x00,
	Xd6XmlTreeSegment_text 		= 0x01,
	Xd6XmlTreeSegment_tab 		= 0x03,
	Xd6XmlTreeSegment_space 		= 0x05,
	Xd6XmlTreeSegment_zero_space 	= 0x25,
	Xd6XmlTreeSegment_newline 		= 0x09,
	Xd6XmlTreeSegment_dos_newline 	= 0x19,
	Xd6XmlTreeSegment_mac_newline 	= 0x29,
	Xd6XmlTreeSegment_element 		= 0x80
};

class Xd6XmlAttribute {
public:
	char *name;
	char *value;

	Xd6XmlAttribute(const char *n, int n_len);
	~Xd6XmlAttribute();
	void set_value(const char *v, int v_len);
};

class Xd6XmlTreeElement;

class Xd6XmlTreeSegment {
public:
	int type;
	int id;
	
	Xd6XmlTreeElement *parent;

	Xd6XmlTreeSegment(Xd6XmlTreeElement *p, int i);
	~Xd6XmlTreeSegment();
};

class Xd6XmlTreeText : public Xd6XmlTreeSegment {
public:
	char *data;
	int len;

	Xd6XmlTreeText(Xd6XmlTreeElement *p, int i);
	~Xd6XmlTreeText();
};


class Xd6XmlStyle;

class Xd6XmlTreeElement : public Xd6XmlTreeSegment {
public:
	char *name;
	int name_len;
	unsigned int style;
	unsigned int display;
	Xd6XmlStyle *xstyle;
	Xd6XmlAttribute **attributes;
	int nb_attributes; 
	Xd6XmlTreeSegment **children;
	int nb_children;
	
	Xd6XmlTreeElement(Xd6XmlTreeElement *p, int i);
	virtual ~Xd6XmlTreeElement();
	virtual Xd6XmlTreeText *create_child_text();
	virtual Xd6XmlTreeElement *create_child_element();
	void unref_child(int c_id);
	void add_attribute(Xd6XmlAttribute *attr);
	void set_name(const char *name, int len);
	const char *get_attr_value(const char *name);
};

class Xd6XmlCss;

typedef void (Xd6XmlTreeCallback)(Xd6XmlTreeElement*, void*);
class Xd6XmlTree {
private:
	const char *txt;
	int len;
	Xd6XmlAttribute *attribute;
public:
	int preformated;
	int right_to_left;
	int *dirs_stack;
	int nb_dirs;
	Xd6XmlTreeElement *root;
	Xd6XmlTreeElement *cur_element;
	Xd6XmlTreeText *cur_text;
	int cur_text_offset;
	Xd6XmlTreeElement *begin_element;
	Xd6XmlTreeText *begin_text;
	int begin_text_offset;
	
	Xd6XmlTree();
	virtual ~Xd6XmlTree();
	virtual void add_text(const char *txt, int len);
	virtual void add_plain_text(const char *txt, int len);
	void add_element(const char *tag, int len);
	virtual void set_element_style(void);
	void close_element(const char *tag, int len, 
		Xd6XmlTreeCallback *func = NULL, void *data = NULL);
	void element_attribute(const char *name, int len);
	void attribute_value(const char *value, int len);
	void dump_text(Xd6XmlTreeElement *elem);

private:
	int white_space(void);
	int ignore_space(void);
};

#endif  /* Xd6XmlTree_h */

/*
 * "$Id: $"
 */

