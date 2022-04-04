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
#ifndef Xd6TextParser_h
#define Xd6TextParser_h

#include "Xd6XmlTree.h"

class Xd6TextParser {
public:
        char *buffer;
        int buffer_len;
        int buffer_pos;
	int fd;
	Xd6XmlTree *tree;

	Xd6TextParser();
	~Xd6TextParser();
	int parse_file(const char *name);
	void parse_string(const char *buf, int buf_len);
};


#endif

/*
 * "$Id: $"
 */
