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
#ifndef Xd6HtmlMailParser_h
#define Xd6HtmlMailParser_h

#include <time.h>

enum {
	C_UTF8,
	C_UTF7,
	C_ISO,
};

class MailParser {
public:
	char *xml;
	char *buf;
	int buf_len;
	int xml_len;
	char *Message_ID;
	char *Subject;
	char *To;
	char *Reply_To;
	char *Cc;
	char *From;
	char *Date;
	char *Content_Type;
	char *Content_Transfer_Encoding;
	int charset;
	time_t date_time;

	MailParser();
	~MailParser();
	void mailbox2html(const char *path);
	void mail2html(const char *f, const char *p, FILE *index);
	void double_buffer(void);
	int decode_header(void);
	int get_header_line(FILE *fp);
	int get_quoted_line(FILE *fp, char *b, int bl);
	void print_text_plain(FILE *in, FILE *out, char *b = NULL);
	void print_application(FILE *in, FILE *out, char *b = NULL);
	void print_multipart(FILE *in, FILE *out);
	void print_mixed(FILE *in, FILE *out);
	char *get_boundary(void);
	
};

#endif

/*
 * "$Id: $"
 */
