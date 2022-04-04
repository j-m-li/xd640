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
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "Xd6XmlParser.h"

enum {
	NO_STATUS 		= 0,
	IN_TEXT 		= 0x0001,
	IN_TEXT_ENTITY		= 0x0002,
	IN_COMMENT		= 0x0004,
	IN_TAG			= 0x0008,
	IN_ATTRIBUTE_NAME 	= 0x0018,
	IN_ATTRIBUTE_EQUAL 	= 0x0028,
	IN_ATTRIBUTE_VALUE	= 0x0048,
	IN_TAG_NAME		= 0x0088,
	IN_ATTRIBUTE_ENTITY	= 0x0108,
	IN_TAG_ENTITY		= 0x0208,
	IN_CLOSING_TAG		= 0x0408,
};

Xd6XmlParser::Xd6XmlParser()
{
	tree = new Xd6XmlTree();
	dtd = new Xd6XmlDtd();

	buffer_len = 128;
	buffer = (char*) malloc(buffer_len);
	buffer_pos = 0;
	
	entity_len = 16;
	entity = (char*) malloc(entity_len);
	entity_pos = 0;

	status = NO_STATUS;
	quote = '\0';

	userdata = NULL;
	callback = NULL;
}

Xd6XmlParser::~Xd6XmlParser()
{
	delete(tree);
	free(buffer);
	free(entity);
}

int Xd6XmlParser::parse_file(const char *name)
{
	char buf[1024];
	int read_len;

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
	entity_pos = 0;
	entity_len = 16;
	entity = (char*) realloc(entity, entity_len);
	return 0;
}

int Xd6XmlParser::scan_file(const char *name, Xd6XmlTreeCallback *func, void *data)
{
	int ret;
	callback = func;
	userdata = data;
	ret = parse_file(name);
	callback = NULL;
	userdata = NULL;
	return ret;
}

/*
 *  returns true if the char is valid in an attribute name
 */
int Xd6XmlParser::is_name(const char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
		(c >= '0' && c <= '9') || *string == '_' || 
		*string == '-' || *string == ':' || *string == '.')
	{
		return 1;
	}
	return 0; 
}

#if 0
/*
 *  Returns true if the char is a white space
 */ 
int Xd6XmlParser::is_space(const char c) 
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
		c == '\f')
	{
		return 1;
	}
	retrun 0;
}
#endif

/*
 *  Add a single char taken from the input string to the output buffer.
 */
void Xd6XmlParser::add_char_to_buffer()
{
	buffer[buffer_pos++] = *string;
	if (buffer_pos >= buffer_len) {
		buffer_len *= 2;
		buffer = (char*) realloc(buffer, buffer_len);
	}
}

void Xd6XmlParser::add_char_to_entity()
{
	entity[entity_pos++] = *string;
	if (entity_pos >= entity_len) {
		entity_len *= 2;
		entity = (char*) realloc(entity, entity_len);
	}
}

/*
 *  Plain text parsing :
 *  - Copy the input string to the output buffer.
 *  - If an entity or a tag is found then flush the output buffer and
 *    switch the status. 
 *  - flush the output buffer when the end of the input string is reached.
 */
void Xd6XmlParser::text()
{
	while (string_len > 0) {
		if (*string == '<') {
			status = IN_TAG_NAME;
			tree->add_text(buffer, buffer_pos);
			buffer_pos = 0;
			string++; string_len--;
			return;
		} else if (*string == '&') {
			status = IN_TEXT_ENTITY;
			tree->add_text(buffer, buffer_pos);
			entity_pos = 0;
			buffer_pos = 0;
			string++; string_len--;
			return;
		} else {
			add_char_to_buffer();
		}
		string++; string_len--;
	}
}

void Xd6XmlParser::text_entity()
{
	while (string_len > 0) {
		if (*string == ';') {
			decode_entity();
			status = IN_TEXT;
			string++; string_len--;
			return;
		} else if (!is_name(*string) && *string != '#') {
			status = IN_TEXT;
			copy_entity_to_buffer(0);
			return;
		}				
		add_char_to_entity();				
		string++; string_len--;
	}

}

void Xd6XmlParser::attribute_entity()
{
	while (string_len > 0) {
		if (*string == ';') {
			decode_entity();
			status = IN_ATTRIBUTE_VALUE;
			string++; string_len--;
			return;
		} else if (!is_name(*string) && *string != '#') {
			status = IN_ATTRIBUTE_VALUE;
			copy_entity_to_buffer(0);
			return;
		}				
		add_char_to_entity();				
		string++; string_len--;
	}
}


void Xd6XmlParser::tag_entity()
{
	while (string_len > 0) {
		if (*string == ';') {
			decode_entity();
			status = IN_TAG;
			string++; string_len--;
			return;
		} else if (!is_name(*string) && *string != '#') {
			status = IN_TAG;
			buffer_pos = 0;
			return;
		}
		add_char_to_entity();				
		string++; string_len--;
	}
}


void Xd6XmlParser::decode_entity()
{
	int len;
	//int stat;
	//int sl;
	//const char *s;
	const char *value = NULL;

	value = dtd->get_entity_value(entity, entity_pos); 

	if (!value) {
		copy_entity_to_buffer(1);
		return;
	}
/*
	stat = status;
	switch (status) {
	case IN_ATTRIBUTE_ENTITY:
		status = IN_ATTRIBUTE_VALUE;
		break;
	case IN_TEXT_ENTITY:
		status = IN_TEXT;
		break;
	case IN_TAG_ENTITY:
		status = IN_TAG;
		break;
	}
*/	
	len = strlen(value);
	copy_string_to_buffer(value, len);

//status = stat;
}

void Xd6XmlParser::copy_string_to_buffer(const char *str, int len)
{
	const char *ptr;
	if (buffer_pos + len + 2 >= buffer_len) {
		buffer_len += len + 4;
		buffer = (char*) realloc(buffer, buffer_len);
	}

	ptr = str;
	while (len > 0) {
		buffer[buffer_pos++] = *ptr;
		len--; ptr++;
	}
}

void Xd6XmlParser::copy_entity_to_buffer(int valid)
{
	const char *ptr;

	if (buffer_pos + entity_pos + 2 >= buffer_len) {
		buffer_len += entity_pos + 4;
		buffer = (char*) realloc(buffer, buffer_len);
	}

	ptr = entity;
	buffer[buffer_pos++] = '&';
	while (entity_pos > 0) {
		buffer[buffer_pos++] = *ptr;
		entity_pos--; ptr++;
	}
	if (valid) buffer[buffer_pos++] = ';';
}

/*
 *  Tag body:
 *  - wait for the closing '>' and switch back to text parsing.
 *  - if a '/>' is found close the tag.
 *  - if something that is [a-zA-z]  is found then switch to attibute
 *    parsing.
 */
void Xd6XmlParser::tag()
{
	while (string_len > 0) {
		if (*string == '>') {
			if (status == IN_TAG) {
				tree->set_element_style();
			}
			status = IN_TEXT;
			if (buffer[buffer_pos - 1] == '/' &&
				tree->cur_element &&
				tree->cur_element->name) 
			{
				/* <tag_name  /> */
				tree->close_element(tree->cur_element->name,
				 	strlen(tree->cur_element->name), 
					callback, userdata);
			}
			buffer_pos = 0;
			string++; string_len--;
			return;
		} else if (isalpha(*string)) {
			status = IN_ATTRIBUTE_NAME;
			buffer_pos = 0;
			return;
		}
		add_char_to_buffer();			
		string++; string_len--;
	}
}

void Xd6XmlParser::tag_name()
{
	while (string_len > 0) {
		if (*string == '>') {
			status = IN_TEXT;
			if (buffer[buffer_pos - 1] == '/') {
				/* <tag_name/> */
				tree->add_element(buffer, buffer_pos - 1);
				tree->set_element_style();
				tree->close_element(buffer, buffer_pos - 1,
					callback, userdata);
			} else if (buffer[0] == '/') {
				/* </tag_name> */
				tree->close_element(buffer + 1, buffer_pos - 1,
					callback, userdata);
			} else {
				/* <tag_name> */
				tree->add_element(buffer, buffer_pos);
				tree->set_element_style();
			}
			buffer_pos = 0;
			string++; string_len--;
			return;
		} else if (buffer_pos == 0 && *string == '/') {
			/* </tag_name .... */
			add_char_to_buffer();
		} else if (buffer_pos == 0 && *string == '!') {
			/* <!tag_name .... */
			status = IN_COMMENT;
			return;
		} else if (is_name(*string)) {
			add_char_to_buffer();
		} else {
			if (buffer[0] == '/') {
				status = IN_CLOSING_TAG;
				/* </tag_name ... */
				tree->close_element(buffer + 1, buffer_pos - 1,
					callback, userdata);
				buffer_pos = 0;
				return;
			}
			status = IN_TAG;
			/* <tag_name ... */
			tree->add_element(buffer, buffer_pos);
			buffer_pos = 0;
			return;
		}
		string++; string_len--;
	}
}

/*
 *  Find the tag attribute name.
 */
void Xd6XmlParser::attribute_name()
{
	while (string_len > 0) {
		if (!is_name(*string)) {
			status = IN_ATTRIBUTE_EQUAL;
			/* <tag_name attr = ...*/
			tree->element_attribute(buffer, buffer_pos);
			buffer_pos = 0;
			return;
		}
		add_char_to_buffer();
		string++; string_len--;
	}
}

/*
 *  Find the '=' after the attribute name or find the next attribute.
 */
void Xd6XmlParser::attribute_equal()
{
	while (string_len > 0) {
		if (*string == '=') {
			status = IN_ATTRIBUTE_VALUE;
			/* <tag_name attr = ...*/
			buffer_pos = 0;
			string++; string_len--;
			quote = '\0';
			return;
		} else if (!isspace(*string)) {
			status = IN_TAG;
			/* <tag_name attr1 attr2 ... or <tag_name attr/> */
			buffer_pos = 0;
			return;
		}
		string++; string_len--;
	}
}

/*
 *  Find the attribute value.
 */
void Xd6XmlParser::attribute_value()
{
	/*
	 *  find the quote char.
	 */
	if (buffer_pos == 0) {
		while (string_len > 0) {
			if (!isspace(*string)) break; 
			string++; string_len--;
		}
		if (string_len > 0) {
			if (*string == '\'') {
				/* <tag_name attr = 'value */
				quote = '\'';
				add_char_to_buffer();
			} else if (*string == '"') {
				/* <tag_name attr = "value */
				quote = '"';
				add_char_to_buffer();
			} else if (is_name(*string) || *string == '#') {
				/* <tag_name attr = value */
				quote = ' ';
				add_char_to_buffer();
			} else {
				status = IN_TAG;
				/* <tag_name attr= /> */
				buffer_pos = 0;
				return;
			}
			if (quote != ' ') {
				string++; string_len--;
			}
		}
	}

	/*
  	 *  Copy the arg value to the output buffer.
	 *  And set the value when the end quote is reatched.
	 */
	while (string_len > 0) {
		if (*string == '&') {
			/* <tag_name attr = "value&lt;" */
			status = IN_ATTRIBUTE_ENTITY;
			entity_pos = 0;
			string++; string_len--;
			return;
		} else if (*string == quote || *string == '>' ||
			(quote == ' ' && (isspace(*string)))) 
		{
			status = IN_TAG;
			/* <tag_name attr = "value" */
			tree->attribute_value(buffer + 1, buffer_pos - 1);
			buffer_pos = 0;
			if (*string != '>') {
				string++; string_len--;
			}
			return;
		}
		add_char_to_buffer();
		string++; string_len--;
	}
}

void Xd6XmlParser::comment_tag()
{
	while (string_len > 0) {
		if (*string == '>'  /* && 
			buffer[buffer_pos - 1] == '-' &&
			buffer[buffer_pos - 2] == '-' */) 
		{
			status = IN_TEXT;
			string++; string_len--;
			return;
		} else {
		//	add_char_to_buffer();
		}
		string++; string_len--;
	}
}

/*
 *  Main parser switch.
 */
void Xd6XmlParser::parse_string(const char *buf, int buf_len)
{

	string = buf;
	string_len = buf_len;

	while (string_len > 0) {
		switch(status) {
		case IN_TEXT:
			text();
			break;
		case IN_TEXT_ENTITY:
			text_entity();
			break;
		case IN_COMMENT:
			comment_tag();
			break;
		case IN_CLOSING_TAG:
		case IN_TAG:
			tag();
			break;
		case IN_ATTRIBUTE_NAME:
			attribute_name();
			break;
		case IN_ATTRIBUTE_EQUAL:
			attribute_equal();
			break;
		case IN_ATTRIBUTE_VALUE:
			attribute_value();
			break;
		case IN_TAG_NAME:
			tag_name();
			break;
		case IN_ATTRIBUTE_ENTITY:
			attribute_entity();
			break;
		case IN_TAG_ENTITY:
			tag_entity();
			break;
		default:
			if (*string == '<') {
				status = IN_TAG_NAME;
			}
			string++; string_len--;
			break;
		}
	}
}

/*
 * "$Id: $"
 */

