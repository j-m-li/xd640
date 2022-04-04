/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000  O'ksi'D
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
