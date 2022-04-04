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

#include "Xd6Std.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "Xd6TextParser.h"


Xd6TextParser::Xd6TextParser()
{
	tree = new Xd6XmlTree();
        buffer_len = 128;
        buffer = (char*) malloc(buffer_len);
        buffer_pos = 0;

}

Xd6TextParser::~Xd6TextParser()
{
}

int Xd6TextParser::parse_file(const char *name)
{
	char buf[1024];
	int read_len;

	tree->preformated = 1;
	fd = open(name, O_RDONLY);
	if (fd < 1) return -1;

	read_len = read(fd, buf, 1023);
	while (read_len > 0) {
		buf[read_len] = '\0';
		parse_string(buf, read_len);
		read_len = read(fd, buf, 1023);
		
	}
	close(fd);

	buffer_pos = 0;
	buffer_len = 128;
	buffer = (char*) realloc(buffer, buffer_len);
	
	return 0;
}

void Xd6TextParser::parse_string(const char *buf, int buf_len)
{
	tree->add_plain_text(buf, buf_len);
}

/*
 * "$Id: $"
 */

