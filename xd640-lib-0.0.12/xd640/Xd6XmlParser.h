/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000  O'ksi'D
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
#ifndef Xd6XmlParser_h
#define Xd6XmlParser_h

#include "Xd6XmlTree.h"
#include "Xd6XmlDtd.h"

class Xd6XmlParser {
protected:
	int fd;
	char *buffer;
	int buffer_len;
	int buffer_pos;
	
	char *entity;
	int entity_len;
	int entity_pos;

	const char *string;
	int string_len;
	char quote;

	Xd6XmlTreeCallback* callback;
	void *userdata;
public:
	int status;
	Xd6XmlTree *tree;
	Xd6XmlDtd *dtd;

	Xd6XmlParser();
	~Xd6XmlParser();
	int parse_file(const char *name);
	void parse_string(const char *buf, int buf_len);
	int is_name(const char c);
	int scan_file(const char *name, Xd6XmlTreeCallback *func, void *data);
protected:
	inline void add_char_to_buffer();
	inline void add_char_to_entity();
	void text();
	void tag();
	void tag_name();
	void attribute_name();
	void attribute_equal();
	void attribute_value();
	void comment_tag();
	void text_entity();
	void attribute_entity();
	void tag_entity();
	void decode_entity();
	void copy_entity_to_buffer(int valid);
	void copy_string_to_buffer(const char *str, int len);
};


#endif

/*
 * "$Id: $"
 */
