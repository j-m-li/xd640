/******************************************************************************
 *   "$Id: $"
 *
 *   Xd6CfgParser for the Fast Light Environement.
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

#include "Xd6CfgParser.h"
#include "Xd6XmlUtils.h"
#include "Xd6Std.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//
// set the tag name string
//
void Xd6CfgParser::set_tag_name(CfgObject* o, char* txt, int begin, int end) 
{
	char *ptr;
	int len = end - begin;
	if (!o || o->tag_name) return;
	o->tag_name = txt + begin;
	*(o->tag_name + len) = '\0';
	ptr = o->tag_name;
	while (*ptr) {
		*ptr = (char) tolower((int)*ptr);
		ptr++;
	}
	//printf("TAG : %s\n", o->tag_name);
}

//
// add plain utf-8 text to the object
//
void Xd6CfgParser::set_text(CfgObject* o, char* txt, int begin, int end) 
{
	if (!o) return;
	return;
}

//
// add a new parameter to the tag
//
void Xd6CfgParser::set_param(CfgObject* o, char* txt, int begin, int end)
{
	int len = end - begin;
	char *value; 

	if (!o) return;

	value = txt + begin;
	len = xml2utf(txt + begin, len, value);
	*(value + len) = '\0';

	if (o->params) {
		o->add_param(o->params, "");
	}
	o->params = value;
	//printf("PARAM : %s\n", value);
}

//
// set the value of the last tag parameter
//
void Xd6CfgParser::set_param_value(CfgObject* o, char* txt, int begin, 
	int end)
{
        int len = end - begin;
        char *value;

	if (!o || !o->params || len < 0) return;
	
	if (len > 0) {
		value = txt + begin;
		len = xml2utf(txt + begin, len, value);
		value[len] = '\0';
	} else {
		value = "";
	}

	o->add_param(o->params, value);

        //printf("(%s) VALUE : %s\n", o->params, value);

	o->params = NULL;
}

static int is_section(Xd6ConfigFileGroup *box, CfgObject *o)
{
	// are we in the right section ?
	if (box->type == CONFIG_FILE_SECTION && o &&
		o->type == CFG_TYPE_OPEN) 
	{
		if (o && o->tag_name && o->tag_name[0] == 's') {
			const char *n;
			n = o->get_value("n");
			if (n && !strcmp(box->name, n)) {
				delete(o);
				return 1;
			} 
		}
		delete(o);
	}
	// No !
	return 0;
}

Xd6ConfigFileGroup *Xd6CfgParser::add(Xd6ConfigFileGroup *box, CfgObject *o)
{
	Xd6ConfigFileGroup *b = box;


	// <begin>
	if (o && box && o->tag_name && o->type & CFG_TYPE_OPEN) {
		//printf("ADD  %s n=%s l=%s v=%s\n", o->tag_name, 
		//	o->get_value("n"), o->get_value("l"),  
		//	o->get_value("v"));
		switch(o->tag_name[0]) {
		case 'i':			// item
			box->add_item(o->get_value("n"), o->get_value("v"),
				o->get_value("l"));
			break;
		case 'g':			// begin of group
			b = (Xd6ConfigFileGroup*) box->add_item(o->get_value("n"),
				NULL, o->get_value("l"), CONFIG_FILE_GROUP);
			b->parent = box;
			break;
		default:
			break;
		}
	}
	
	// </end>
	if (o && b && o->tag_name && o->type & CFG_TYPE_CLOSING) {
		switch(o->tag_name[0]) {
		case 'g':			// end of group
			b = b->parent;
			break;
		case 's':			// end of section
			b = b->parent;
			break;
		default:
			break;
		}
	}

	delete(o);
	return b;
}

// *************************
// ** THE XML/HTML PARSER **
// *************************
void Xd6CfgParser::parse(Xd6ConfigFileGroup* box, char *text, int text_len)
{
	int i = -1;
	char *comment = strdup("comment");
	CfgObject *obj = NULL;
	char quote = '\0';
	int tag_name_begin = 0;
	char in_tag = 0;
	char in_quote = 0;
	char in_no_quote = 0;
	char in_value = 0;
	char in_arg = 0;
	char was_in_arg = 0;
	char in_closed_tag = 0;
	char in_closing_tag = 0;
	char in_tag_name = 0;
	char in_text = 0;
	char in_comment = 0;
	char in_exclam = 0;
	char in_inter = 0;
	char in_section = 0;
	int text_begin = 0;

	i = -1;
	while (i < text_len) {
		i++;
		if (!text[i]) continue;

		if (!in_tag && !in_comment && text[i] == '<') {
			if (in_text) {
				Xd6CfgParser::set_text(obj, text, text_begin, i);
			}
			
			// ***** BEGIN TAG ******
			in_text = 0;
			in_tag = 1;
			in_value = 0;
			was_in_arg = 0;
			in_quote = 0;
			in_no_quote = 0;
			in_arg = 0;
			in_closed_tag = 0;
			in_closing_tag = 0;
			if (text[i + 1] == '/') {
				i++;
				in_closing_tag = 1;
				obj = new CfgObject(CFG_TYPE_CLOSING);
			} else if (text[i + 1] == '!' && text[i + 2] != '-') {
				in_exclam = 1;
				continue;
			} else if (text[i + 1] == '?') {
				in_inter = 1;
				continue;
			} else {
				obj = new CfgObject(CFG_TYPE_OPEN);
			}
			// ***** BEGIN TAG NAME ******
			in_tag_name = 1;
			tag_name_begin = i + 1;	
			continue;
		}

		if (in_tag && !in_quote && text[i] == '>') {
			Xd6ConfigFileGroup *b;
			if (in_comment) {
				if (text[i - 1] != '-') {
					 continue;
				}
				Xd6CfgParser::set_param(obj, comment, 0, 7);
				Xd6CfgParser::set_param_value(obj, text, 
							text_begin, i - 2);
				obj->type = CFG_TYPE_CLOSE;
			} else if (in_exclam || in_inter) {
				in_exclam = 0;
				in_inter = 0;
				in_comment = 0;
				in_tag = 0;
				in_tag_name = 0;
				in_no_quote = 0;
				in_value = 0;
				in_arg = 0;
				continue;
			}
			// ***** END COMMENT ****
			in_comment = 0;
			// ***** END TAG  ******
			in_tag = 0;

			if (in_tag_name) {
				Xd6CfgParser::set_tag_name(obj, text, 
							tag_name_begin, i);
				in_tag_name = 0;
			}
			if (in_no_quote) {
				Xd6CfgParser::set_param_value(obj, text, 
								text_begin, i);
				in_no_quote = 0;
				in_value = 0;
			}
			if (in_arg) {
				Xd6CfgParser::set_param(obj, text, text_begin, i);
				Xd6CfgParser::set_param_value(obj, "", 0, 0);
				in_arg = 0;
			}
			if (in_closed_tag) {
				obj->type = CFG_TYPE_CLOSE;
			}

			if (in_section) {
				b = Xd6CfgParser::add(box, obj);
				if (b) {
					box = b;
				} else {
					in_section = 0;
				}
			} else if (box->type == CONFIG_FILE_SECTION) {
				in_section = is_section(box, obj);
			} else {
				delete(obj);
			}
			obj = NULL;
			continue;
		}

		if (in_comment || in_exclam || in_inter) {
			continue;
		}

                if (in_tag && !in_quote && text[i] == '/' &&
                        text[i + 1] == '>')
                {
                        /** we are near the end of a tag **/
                        in_closed_tag = 1;
                        if (in_tag_name) {
                                Xd6CfgParser::set_tag_name(obj, text, 
							tag_name_begin, i);
                                in_tag_name = 0;
                        }
                        if (in_no_quote) {
                                Xd6CfgParser::set_param_value(obj, text, 
								text_begin, i);
                                in_no_quote = 0;
				in_value = 0;
                        }
                        if (in_arg) {
                                Xd6CfgParser::set_param(obj, text, text_begin, i);
                                in_arg = 0;
                        }
                        continue;
                }

		// *****************************************************

		if (in_tag && in_tag_name && isspace(text[i])) {
			// ***** END TAG NAME ******
			in_tag_name = 0;
			Xd6CfgParser::set_tag_name(obj, text, tag_name_begin, i);
			if (text[i - 3] == '!' && text[i - 2] == '-' && 
				text[i - 1] == '-')
			{
				// **** BEGIN COMMENT ****
				in_comment = 1;
				text_begin = i;
			}
			continue;
		}

		if (in_tag && in_tag_name) {
			// *** we are in the tag name, 	  ***
			// *** other tests are not needed ***
			continue;
		}

                if (in_tag && in_quote && text[i] == quote) {
			// **** END QUOTE ****
                        in_quote = 0;
			in_value = 0;
                        Xd6CfgParser::set_param_value(obj, text, text_begin, i);
                        continue;
                }

                if (in_tag && in_value && in_no_quote && isspace(text[i])) {
			// **** END NO QUOTE ****
			// **** END VALUE ****
                        in_no_quote = 0;
                        in_value = 0;
                        Xd6CfgParser::set_param_value(obj, text, text_begin, i);
                        continue;
                }
		
		if (in_tag && in_value && in_quote) {
			// ** we are inside a quotation **
			continue;
		}

                if (in_tag && in_arg && (isspace(text[i]) || text[i] == '=')) {
			// **** END ARG ****
                        in_arg = 0;
                        if (text[i] == '=') {
                        	Xd6CfgParser::set_param(obj, text, text_begin, i);
				in_value = 1;
				text_begin = i;
				was_in_arg = 0;
			} else {
                        	Xd6CfgParser::set_param(obj, text, text_begin, i);
				was_in_arg = 1;
			}
                        continue;
                }

		if (in_tag && in_arg) {
			// ** we are in a parameter name **
			continue;
		}

		if (in_tag && was_in_arg && isspace(text[i])) {
			continue;
		}

		if (in_tag && !in_arg && !in_value && !isspace(text[i])
			&& !(was_in_arg && text[i] == '=')) 
		{
			// **** BEGIN ARG ***
                        in_arg = 1;
                        text_begin = i;
                        continue;
                }
	
		if (in_tag && !in_value && was_in_arg && isspace(text[i])) {
			continue;
		}

		if (in_tag && !in_arg && in_value && !in_quote && 
			!in_no_quote && (text[i] == '\'' || text[i] == '"')) 
		{
			// **** BEGIN QUOTE ****
			in_quote = 1;
			quote = text[i];
			text_begin = i + 1;
			continue;
		}


		if (in_tag && in_value  && !in_no_quote && !isspace(text[i])) 
		{
			// **** BEGIN NO QUOTE ****
			in_no_quote = 1;
			text_begin = i;
			continue;
		}

		if (in_tag && !in_value && was_in_arg && text[i] == '=') {
			// **** BEGIN VALUE ****
			in_value = 1;
			was_in_arg = 0;
			in_no_quote = 0;
			in_quote = 0;
		}

		if (in_tag) {
			continue;
		}

		// ********* Plain text *********
		if (!in_text) {
			in_text = 1;
			text_begin = i;
		}
	}
	if (in_text) {
		Xd6CfgParser::set_text(obj, text, text_begin, i);
	}
	delete(obj);
	free(comment);
	return ;
}

void Xd6CfgParser::copy_other_sections(char *buf, int buf_len, FILE *fpo, 
	const char *sect)
{
	char in_comment = 0;
	char in_quote = 0;
	char in_section = 0;
	char in_sect_name = 0;
	char in_sect_tag = 0;
	char in_tag = 0;
	char dont_copy = 1;
	char *begin;
	char *para;

	int i;
	int slen;

	if (!sect) {
		sect = "_____";
	};

	slen = strlen(sect);

	i = 0;
	begin = buf + buf_len;
	para = begin;

	while (buf[i] && i < buf_len) {
	
		// <
		if (!in_quote && !in_comment && buf[i] == '<') {
			in_tag = 1;
		}

		// <tag xx="
		if (in_tag && !in_quote && !in_comment && buf[i] == '"') {
			in_quote = 1;
			i++;
			para = buf + i;
			continue;
		}
		
		// <!--
		if (!in_quote && !in_comment && buf[i] == '<' &&
			buf[i + 1] == '!' && buf[i + 2] == '-')
		{
			in_comment = 1;
			i++;
			continue;
		}

		// <s 
		if (!in_quote && !in_comment && buf[i] == '<' &&
			buf[i + 1] == 's' && isspace(buf[i + 2]))
		{
			begin = buf + i;
			in_section = 1;
			in_sect_tag = 1;
			i += 2;
			continue;
		}

		
		// <tag ppp="...
		if (in_quote) {
			// <tag ppp="..."
			if (buf[i] == '"') {
				char c;
				c = buf[i];
				buf[i] = '\0';

				// <s n="..."
				if (in_sect_name && !strcmp(para, sect)) {
					dont_copy = 1;
				} else if (in_sect_name) {
					dont_copy = 0;
				}
				buf[i] = c;
				in_sect_name = 0;
				in_quote = 0;
			} 
			i++;
			continue;
		}

		// <!-- ...
		if (in_comment) {
			// <!-- ... -->
			if (buf[i] == '-' &&  buf[i + 1] == '-' &&
				buf[i + 2] == '>')
			{
				in_comment = 0;
				i += 2;
			} else {
				i++;
				continue;
			}
		}

		// <tag pp="xyz" >
		if (in_tag && buf[i] == '>') {
			in_tag = 0;
			in_sect_name = 0;
			in_sect_tag = 0;
			i++;
			continue;
		}

		// <s n="sect"> ... </s>
		if (in_section &&  buf[i] == '<' && buf[i + 1] == '/' &&
			buf[i + 2] == 's' && (isspace(buf[i + 3]) ||
			buf[i + 3] == '>'))
		{
			in_section = 0;
			while (buf[i] && buf[i] != '>') i++;
			if (buf[i]) i++;
			if (!dont_copy) {
				char e;
				e = buf[i];
				buf[i] = '\0';
				fputs("\n  ", fpo);
				fputs(begin, fpo);
				buf[i] = e;
			}
			while (isspace(buf[i])) i++;
			begin = buf + buf_len;
			in_tag = 0;
			in_sect_name = 0;
			continue;
		}
	
		// <s n
		if (in_section && in_sect_tag && buf[i] == 'n') {
			i++;
			while (isspace(buf[i])) i++;
			if (buf[i] == '=' ) {
				in_sect_name = 1;
				i++;
				while (isspace(buf[i])) i++;
			}
			continue;
		}

		// </xd640>
		if (buf[i] == '<' && buf[i + 1] == '/' &&
			buf[i + 2] == 'x' && buf[i + 3] == 'd' &&
			buf[i + 4] == '6' && buf[i + 5] == '4')
		{
			while ((buf[i])) i++;
			// end of file
			continue;
		}

		// <xd640>
		if (buf[i] == '<' && buf[i + 1] == 'x' && buf[i + 2] == 'd' &&
			buf[i + 3] == '6' && buf[i + 4] == '4')
		{
			//dont_copy = 0;
		}
		i++;
	}
}

/*
 * "$Id: $"
 */

