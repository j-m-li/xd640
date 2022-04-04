/*******************************************************************************
 *  "$Id: $"
 *
 *  	Xd6ConfigFile definitions for Xd640.
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
 *   Author : Jean-Marc Lienher <oksid@bluewin.ch>
 *
 ******************************************************************************/

#ifndef Xd6ConfigFile_h
#define Xd6ConfigFile_h

#include <stdlib.h>
#include "Xd6Std.h"
#include <string.h>

class Xd6ConfigFileGroup;

enum {
	CONFIG_FILE_NONE	= 0x00,
	CONFIG_FILE_ITEM	= 0x01,
	CONFIG_FILE_GROUP	= 0x02,
	CONFIG_FILE_SECTION	= 0x04,
	CONFIG_FILE_GLOBAL	= 0x08,
};


class Xd6ConfigFileItem {
public:
	char type;
	char *name;
	char *value;
	char *locale;
	char flags;

	Xd6ConfigFileItem(const char *n, const char *v, const char *l) 
	{
		type = CONFIG_FILE_ITEM;
		flags = 0;

		if (n) { name = strdup(n);
		} else { name = strdup("(null)"); }
		
		if (v) { value = strdup(v); 
		} else { value = NULL; }
		
		if (l) { locale = strdup(l);
		} else { locale = strdup("C"); }
	}

	~Xd6ConfigFileItem() 
	{
		free(value);
		free(name);
		free(locale);
	}

	Xd6ConfigFileItem *get_diff(Xd6ConfigFileItem *other);
	int get_type(void) { return type; }
	char *get_name(void) { return name; }
	char *get_locale(void) { return locale; }
	const char *get_value(void) { return value; }
	char *set_value(const char *v) 
	{
		flags &= ~CONFIG_FILE_GLOBAL;
		free(value);
		if (v) { value = strdup(v);
		} else { value = NULL; }
		return value;
	}
};

class Xd6ConfigFileGroup : public Xd6ConfigFileItem {
public:
	int nb_items;
	Xd6ConfigFileItem **items;
	Xd6ConfigFileGroup *parent;

	Xd6ConfigFileGroup(const char *name, const char *locale) : 
		Xd6ConfigFileItem(name, NULL, locale) 
	{
		type = CONFIG_FILE_GROUP;
		nb_items = 0;
		items = NULL;
		parent = NULL;
	}

	~Xd6ConfigFileGroup() 
	{
		while (nb_items > 0) {
			nb_items--;
			delete(items[nb_items]);
		}
	}

	void dump(FILE *fp, int level = 0);
	void set_global_flags(void);

	Xd6ConfigFileItem *get_item(const char *n, const char *l); 
	
	Xd6ConfigFileItem *get_create_item(const char *n, const char *l)
	{
		Xd6ConfigFileItem *itm;
		itm = get_item(n, l);
		if (!itm) {
			itm = add_item(n, NULL, l);
		}
		return itm;
	
	} 

	Xd6ConfigFileGroup *get_group(const char *n, const char *l)
	{
		Xd6ConfigFileItem *itm;
		itm = get_item(n, l);
		if (!itm) {
			return add_group(n, l);
		}
		if (itm && itm->type != CONFIG_FILE_GROUP) itm = NULL; 
		return (Xd6ConfigFileGroup*) itm;
	}

	Xd6ConfigFileItem *add_item(const char *name, const char *value, 
		const char *locale, int t = CONFIG_FILE_ITEM);

	Xd6ConfigFileGroup *add_group(const char *name,  const char *locale) {
		return (Xd6ConfigFileGroup*)
			add_item(name, NULL, locale, CONFIG_FILE_GROUP);
	}
	
};


class Xd6ConfigFileSection : public Xd6ConfigFileGroup {
public:
	Xd6ConfigFileSection(const char *name) : Xd6ConfigFileGroup(name, NULL) {
		type = CONFIG_FILE_SECTION;
	};
};


class Xd6ConfigFilePaths {
public:
	char *appslnk;
	char *apps;
	char *config;
	char *doc;
	char *icons_small;
	char *icons_medium;

	Xd6ConfigFilePaths(const char *home, const char *project,
		const char *app_name); 
	Xd6ConfigFilePaths(const char *base, const char *app_name); 

	~Xd6ConfigFilePaths()
	{
		free(appslnk);
		free(apps);
		free(config);
		free(doc);
		free(icons_small);
		free(icons_medium);
	}
	
	void check_paths(void);
};

class Xd6ConfigFile {
public:
	Xd6ConfigFileSection *cfs;
	Xd6ConfigFilePaths *global_paths;
	Xd6ConfigFilePaths *user_paths;
	char *host_name;
	char *home_dir;
	char *app_name;
	char *app_category;
	char *country;
	char *lang;
	char *locale;
	char pid[16];

	Xd6ConfigFile(const char *app_name, const char *app_category);
	~Xd6ConfigFile();

	void parse_file(const char *file, const char *section);
	void write_in_file(const char *file, Xd6ConfigFileSection *s1);

	Xd6ConfigFileSection *get_config_section(const char *section);
	Xd6ConfigFileSection *get_xd640_section(void);
	Xd6ConfigFileSection *get_section(const char *global, const char *user, 
		const char *section);
	
	int write_config_section(Xd6ConfigFileSection *section);
	int write_xd640_section(Xd6ConfigFileSection *section);
	int write_section(const char *user, Xd6ConfigFileSection *section);

};

#endif // !Xd6ConfigFile_h

/*
 *  End of "$Id:  $".
 */
