/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2001  O'ksi'D
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
