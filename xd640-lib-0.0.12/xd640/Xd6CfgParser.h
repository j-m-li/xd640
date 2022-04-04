/*******************************************************************************
 *  "$Id: $"
 *
 *  	Xd6CfgParser definitions for the Fast Light Environement.
 *
 *   		Copyright (c) 2000 O'ksi'D
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

#ifndef Xd6CfgParser_h
#define Xd6CfgParser_h

#include "Xd6ConfigFile.h"

enum {
	CFG_TYPE_OPEN 		= 0x01,
	CFG_TYPE_CLOSING	= 0x02,
	CFG_TYPE_CLOSE		= 0x03,
};

class CfgObject {
public:
	int type;
	char *tag_name;
	char *params;
	int nb_values;
	char **names;
	char **values;

	CfgObject(int t) {
		type = t;
		tag_name = NULL;
		params = NULL;
		nb_values = 0;
		names = NULL;
		values = NULL;
	}

	~CfgObject()
	{
		free(names);
		free(values);
	}
	
	void add_param(char *name, char *value) {
		names = (char**) realloc(names, sizeof(char*) * 
					(nb_values + 1));
		values = (char**) realloc(values, sizeof(char*) * 
					(nb_values + 1));
		names[nb_values] = name;
		values[nb_values] = value;
		nb_values++;
	}

	const char *get_value(const char *name) {
		int i;
		i = 0;
		if (!name) return NULL;
		while (i < nb_values) {
			if (names[i] && !strcmp(name, names[i])) {
				return values[i];
			}
			i++;
		}
		return NULL;
	} 
};

class Xd6CfgParser {
public:
	Xd6CfgParser(void){;}
	static void set_tag_name(CfgObject* o, char* txt, int begin, int end);
	static void set_text(CfgObject* o, char* txt, int begin, int end);
	static void set_param(CfgObject* o, char* txt, int begin, int end);
	static void set_param_value(CfgObject* o, char* txt, int begin,int end);
	static Xd6ConfigFileGroup *add(Xd6ConfigFileGroup *box, CfgObject *o);
	static void parse(Xd6ConfigFileGroup* box, char *text, int lext_len);	
	static void copy_other_sections(char *text, int lext_len, FILE *fpo, 
		const char *sect);
};

#endif // !Xd6CfgParser_h

/*
 *  End of "$Id:  $".
 */
