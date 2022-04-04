/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000  O'ksi'D
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
