/*******************************************************************************
 *  "$Id: $"
 *
 *  	Xd6ConfigFile for the Fast Light Environement.
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

#include "Xd6ConfigFile.h"
#include "Xd6CfgParser.h"
#include "Xd6XmlUtils.h"
#include "Xd6Std.h"
#include <locale.h>
#include <sys/stat.h>
#include <sys/types.h>

Xd6ConfigFileItem *Xd6ConfigFileGroup::get_item(const char *n, const char *l)
{
        int i = 0;
        Xd6ConfigFileItem *itm = NULL;

        if (!l) l = "C";

        while (i < nb_items) {
                if (!strcmp(n, items[i]->name)) {
                        if (!strcmp(items[i]->locale, l)) {
                                return items[i];
                        } else if (!itm && !strcmp(items[i]->locale, "C")) {
                                itm = items[i];
                        } else if (strlen(items[i]->locale) == 2 &&
                                !strncmp(items[i]->locale, l, 2))
                        {
                                itm = items[i];
                        }
                }
                i++;
        }
        return itm;
}

Xd6ConfigFileItem *Xd6ConfigFileGroup::add_item(const char *name, const char *value,
	const char *locale, int t)
{
                Xd6ConfigFileItem *itm;
		Xd6ConfigFileGroup *p = this;

		if (!locale) {
			locale = "C";
		}
		while (p->parent) {
			p = p->parent;
			p->flags &= ~CONFIG_FILE_GLOBAL;
		}
		flags &= ~CONFIG_FILE_GLOBAL;
		itm = get_item(name, locale);
		if (itm && !strcmp(locale, itm->get_locale()))
		{
			// item already exists
			itm->set_value(value);
			itm->flags &= ~CONFIG_FILE_GLOBAL;
			return itm;
		}
		itm = NULL;

                items = (Xd6ConfigFileItem**) realloc(items,
                                sizeof(Xd6ConfigFileItem*) * (nb_items + 1));
                switch (t) {
                case CONFIG_FILE_GROUP:
                        itm = (Xd6ConfigFileItem*) 
				new Xd6ConfigFileGroup(name, locale);
                        break;
                case CONFIG_FILE_SECTION:
                        itm = (Xd6ConfigFileItem*) new Xd6ConfigFileSection(name);
                        break;
                default:
                        itm = new Xd6ConfigFileItem(name, value, locale);
                }
                items[nb_items] = itm;
                nb_items++;
                return itm;
}

void Xd6ConfigFileGroup::set_global_flags()
{
	int i;
	flags |= CONFIG_FILE_GLOBAL;
	i = 0;
	while (i < nb_items) {
		items[i]->flags |= CONFIG_FILE_GLOBAL;
		if (items[i]->type == CONFIG_FILE_GROUP ||
			items[i]->type == CONFIG_FILE_SECTION)
		{
			((Xd6ConfigFileGroup*)items[i])->set_global_flags();
		}
		i++;
	}
}

void Xd6ConfigFileGroup::dump(FILE *fp, int level) 
{
	int i;
	char *pad;
	int padl;
	char *n;
	int l;

	if (type == CONFIG_FILE_GROUP && flags & CONFIG_FILE_GLOBAL) return;

	padl = level * 2 + 2;
	pad = (char*) malloc(level * 2 + 3);
	pad[padl] = '\0';
	while (padl > 0) {
		padl--;
		pad[padl] = ' ';
	}

	l =  strlen(name);
	n = (char*) malloc(6 * l);
	utf2xml(name, l, n);

	if (type == CONFIG_FILE_GROUP) {
		if (!strcmp(locale, "C")) {
			fprintf(fp, "%s<g n=\"%s\" >\n", pad, n);
		} else {
			fprintf(fp, "%s<g n=\"%s\" l=\"%s\" >\n", 
				pad, n, locale);
		} 
		free(n);
	} else if (type == CONFIG_FILE_SECTION) {
		fprintf(fp, "%s<s n=\"%s\" >\n", pad, n); 
		free(n);
	} else {
		free(n);
		free(pad);
		return;
	}
	
	i = 0;
	while(i < nb_items) {
		if (items[i]->flags & CONFIG_FILE_GLOBAL) {
			i++;
			continue;
		}
		if (items[i]->type == CONFIG_FILE_GROUP ||
			items[i]->type == CONFIG_FILE_SECTION) 
		{
			((Xd6ConfigFileGroup*)items[i])->dump(fp, level + 1);
		} if (items[i]->type & CONFIG_FILE_ITEM) {
			char *n;
			int l;
			Xd6ConfigFileItem *itm;
			
			itm = items[i];

			l =  strlen(itm->name);
			n = (char*) malloc(6 * l);
			utf2xml(itm->name, l, n);

			if (itm->value && strcmp(itm->locale, "C")) {
				char *v;
				int l;
				l = strlen(itm->value);
				v = (char*) malloc(6 * l);
				utf2xml(itm->value, l, v);
				fprintf(fp, "%s  <i n=\"%s\" l=\"%s\""
					" v=\"%s\" />\n", pad, n, 
					itm->locale, v);
				free(v);
			} else if (!itm->value && strcmp(itm->locale, "C")) {
				fprintf(fp, "%s  <i n=\"%s\""
					" l=\"%s\" />\n", pad, n, itm->locale);
			} else if (itm->value) {
				char *v;
				int l;
				l = strlen(itm->value);
				v = (char*) malloc(6 * l);
				utf2xml(itm->value, l, v);
				fprintf(fp, "%s  <i n=\"%s\" v=\"%s\" />\n", 
					pad, n, v);
				free(v);
			} else {
				fprintf(fp, "%s  <i n=\"%s\" />\n", 
					pad, n);
			}
			free(n);
		}
		i++;
	}

	if (type == CONFIG_FILE_GROUP) {
		fprintf(fp, "%s</g>\n", pad); 
	} else if (type == CONFIG_FILE_SECTION) {
		fprintf(fp, "%s</s>\n", pad); 
	} 
	free(pad);
}

static char *create_long_path(const char *b, const char *p, int bl, int pl,
	const char *pa, const char *a)
{
	char *ret;
	int len;
	len = bl + pl + strlen(pa) + strlen(a) + 10;
	ret = (char*) malloc(len);
	snprintf(ret, len, "%s/%s%s/%s", b, p, pa, a);
	return ret;
}

static char *create_path(const char *b, const char *p, int bl, int pl,
	const char *pa)
{
	char *ret;
	int len;
	len = bl + pl + strlen(pa) + 5;
	ret = (char*) malloc(len);
	snprintf(ret, len, "%s/%s%s", b, p, pa);
	return ret;
}

Xd6ConfigFilePaths::Xd6ConfigFilePaths(const char *b, const char *p, const char *a)
{
	int pl;
	int bl;

	if (!p) p = "xd640/";
	if (!b) b = "/tmp";

	pl = strlen(p);
	bl = strlen(b);

        apps = create_long_path(b, p, bl, pl, "apps", a);
        config = create_path(b, p, bl, pl, "etc");
        doc = create_path(b, p, bl, pl, "doc/xd640");
	appslnk = create_path(b, p, bl, pl, "xd640lnk");
	icons_small = strdup("");
	icons_medium = strdup("");
}

Xd6ConfigFilePaths::Xd6ConfigFilePaths(const char *b, const char *a)
{
	char buf[2048];

	snprintf(buf, 2048, "%s/share/apps/%s", b, a);
	apps = strdup(buf);
	snprintf(buf, 2048, "%s/share/doc/xd640", b);
	doc = strdup(buf);
	snprintf(buf, 2048, "%s/share/xd640lnk", b);
	appslnk = strdup(buf);
	snprintf(buf, 2048, "%s/xd640", SYSCONFDIR);
	config = strdup(buf);
	snprintf(buf, 2048, "%s/share/icons/small/locolor", b);
	icons_small = strdup(buf);
	snprintf(buf, 2048, "%s/share/icons/medium/locolor", b);
	icons_medium = strdup(buf);
}

static void check_path(const char *p)
{
	struct stat s;
	int i = 0;
#ifndef WIN32
	char *cmd;
#endif

	if (!lstat(p, &s)) return;
#ifdef WIN32
	{
		int o = 0;
		char f = '\0';
		int len = strlen(p);
		unsigned short buf[1024];
		unsigned int ucs;
		buf[0] = 0;
		while (o < len) {
			if (!f) f = p[o];
			if (p[o] == '/' || p[o] == '\\') {
				_wmkdir(buf);
				if (f == '.') {
					SetFileAttributesW(buf, 
						FILE_ATTRIBUTE_HIDDEN);
				}
				buf[i] = '\\';
				o++;
				f = '\0';	
			} else {
				int l;
				l = fl_utf2ucs((unsigned char *)p + o, len - o, 
					&ucs);
				buf[i] = (unsigned short) ucs; 
				if (l < 1) l = 1;
				o += l;
			}
			i++;
			buf[i] = 0;
		}
		_wmkdir(buf);
		if (f == '.') {
			SetFileAttributesW(buf, FILE_ATTRIBUTE_HIDDEN);
		}
	}
#else	
	cmd = strdup(p);
	while (p[i]) {
		if (i > 0 && p[i] == '/') {
			cmd[i] = '\0';
			mkdir(cmd, 0700);
			cmd[i] = '/';
		}
		i++;
	}
	mkdir(p, 0700);
	free(cmd);
#endif
}

void Xd6ConfigFilePaths::check_paths()
{
	check_path(apps);
	check_path(config);
	check_path(appslnk);
	check_path(doc);
}

Xd6ConfigFile::Xd6ConfigFile(const char *name, const char *category)
{
#ifdef WIN32
	char home[1024];
	long ret;
	unsigned long size = 1024;
#else
	char *home;
#endif
	char *loc;

	
	if (name) {
		app_name = strdup(name);
	} else {
		app_name = strdup("unknown");
	}
	if (category) {
		app_category = strdup(category);
	} else {
		app_category = strdup("Applications");
	}
	
	host_name = (char*) malloc(256);
	if (gethostname(host_name, 256)) {
		sprintf(host_name, "localhost");
	}
#ifdef WIN32
	{
		HKEY key;
		unsigned short buf[1024];
		int i = 0;
		char *ptr = "Software\\Microsoft\\Windows\\CurrentVersion"
			"\\Explorer\\User Shell Folders";
		while (*ptr) {
			buf[i] = (unsigned short) *ptr;
			ptr++;
			i++;
		}
		buf[i] = 0;
		ret = RegOpenKeyExW(HKEY_CURRENT_USER, buf, 0, KEY_QUERY_VALUE,
			&key);
		i = 0;
		ptr = "Personal";
		while (*ptr) {
			buf[i] = (unsigned short) *ptr;
			ptr++;
			i++;
		}
		buf[i] = 0;
		ret = RegQueryValueExW(key, buf,
			NULL, NULL, (unsigned char*)home, &size);
		RegCloseKey(key);
	}
	if (ret == ERROR_SUCCESS) {
		unsigned short buf[1024];
		int i = 0;
		char *ptr = home;
		ret = ExpandEnvironmentStringsW((unsigned short*)home, 
				buf, 1024);
		while (i < ret) {
			int l;
			l = fl_ucs2utf((unsigned int)buf[i], ptr);
			if (l < 1) l = 1; 
			ptr += l;
			i++;
			if (ptr - home > 1000) {
				*ptr = '\0';
				break;
			}
		}	
	}  else {
		sprintf(home, "C:\\temp");
	}
#else
	home = getenv("HOME");
	if (!home) {
		home = "/tmp";
	}
#endif	
	home_dir = strdup(home);

	loc = setlocale(LC_ALL, "");
	if (!loc) {
		loc = "C";
	}
	locale = strdup(loc);

	lang = (char*)malloc(3);
	lang[0] = loc[0];
	lang[1] = loc[1];
	lang[2] = '\0';

	snprintf(pid, 16, "%d", getpid());

	country = (char*)malloc(3);
	if (loc[1] && loc[2] == '_') {
		country[0] = loc[3];
		country[1] = loc[4];
		country[2] = '\0';
	} else {
		country[0] = '\0';
	}

	global_paths = new Xd6ConfigFilePaths(PREFIX, app_name);
	user_paths = new Xd6ConfigFilePaths(home_dir, ".xd640/", app_name);
	user_paths->check_paths();
}

Xd6ConfigFile::~Xd6ConfigFile()
{
	delete(global_paths);
	delete(user_paths);
	free(host_name);
	free(home_dir);
	free(app_name);
}


void Xd6ConfigFile::parse_file(const char *name, const char *section)
{
	struct stat s;
	FILE *fp;
	char *buf;

	if (lstat(name, &s) || s.st_size < 1) return;

	fp = fopen(name, "r");
	if (!fp) {
		return;
	}

	buf = (char*) malloc(s.st_size + 1);
	fread(buf, s.st_size + 1, 1, fp);

	Xd6CfgParser::parse(cfs, buf, s.st_size);

	free(buf);
	fclose(fp);
}

Xd6ConfigFileSection *Xd6ConfigFile::get_xd640_section()
{
	char *name;

	cfs = new Xd6ConfigFileSection("xd640");

	name = (char*) malloc(strlen(global_paths->config) + 20);
	sprintf(name, "%s/xd640.cfg", global_paths->config);

	parse_file(name, "xd640");
	cfs->set_global_flags();
			
	name = (char*) realloc(name, strlen(user_paths->config) + 20);
	sprintf(name, "%s/xd640.cfg", user_paths->config);

	parse_file(name, "xd640");

	free(name);
	return cfs;
}

Xd6ConfigFileSection *Xd6ConfigFile::get_config_section(const char *section)
{
	char *name;
	int alen;

	if (!section) return NULL;
	cfs = new Xd6ConfigFileSection(section);

	alen = strlen(app_name);
	name = (char*) malloc(strlen(global_paths->config) + alen + 10);
	sprintf(name, "%s/%s.cfg", global_paths->config, app_name);
	
	parse_file(name, section);
	cfs->set_global_flags();
			
	name = (char*) realloc(name, strlen(user_paths->config) + alen + 10);
	sprintf(name, "%s/%s.cfg", user_paths->config, app_name);

	parse_file(name, section);

	free(name);
	return cfs;
}

Xd6ConfigFileSection *Xd6ConfigFile::get_section(const char *global, const char *user,
                const char *section)
{
	if (!section || (!global && !user)) return NULL;

	cfs = new Xd6ConfigFileSection(section);

	if (global) {
		parse_file(global, section);
		cfs->set_global_flags();
	}
	
	if (user) {
		parse_file(user, section);
	}

	return cfs;
}

void Xd6ConfigFile::write_in_file(const char *name, Xd6ConfigFileSection *s1)
{
	int l;
	char *tmpn;
	struct stat s;
	char *buf;
	FILE *fpi;
	FILE *fpo;
	int mask;


	l = strlen(name) + 10;
	tmpn = (char*) malloc(l);
	sprintf(tmpn, "%s.tmp", name);

	fpi = fopen(name, "r");

	mask = umask(0077);	
	fpo = fopen(tmpn, "w");
	umask(mask);

	if (!fpo) {
		free(tmpn);
		fclose(fpi);
		fclose(fpo);
		return;
	}

	fputs("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n", fpo);
	fputs("<!DOCTYPE xd640cfg >\n", fpo);
	fputs("<xd640cfg>", fpo);


	if (fpi) {
		lstat(name, &s);
		buf = (char*) malloc(s.st_size + 1);
		fread(buf, s.st_size + 1, 1, fpi);
		fclose(fpi);
	
		if (buf) {
			Xd6CfgParser::copy_other_sections(buf, s.st_size, fpo, 
				s1->get_name());
		}

		free(buf);
	}

	fputs("\n", fpo);
	s1->dump(fpo);
	
	fputs("</xd640cfg>\n\n", fpo);
	fclose(fpo);
	
	unlink(name);
	rename(tmpn, name);
	
	free(tmpn);
}


int Xd6ConfigFile::write_xd640_section(Xd6ConfigFileSection *section)
{
	char *name;
	if (!section || !section->name || strcmp("xd640", section->name)) {
		return -1;
	}
			
	name = (char*) malloc(strlen(user_paths->config) + 20);
	sprintf(name, "%s/xd640.cfg", user_paths->config);

	write_in_file(name, section);

	free(name);
	return 0;
}

int Xd6ConfigFile::write_config_section(Xd6ConfigFileSection *section)
{
	int alen;
	char *name;

	if (!section) return -1;


	alen = strlen(app_name);
			
	name = (char*) malloc(strlen(user_paths->config) + alen + 10);
	sprintf(name, "%s/%s.cfg", user_paths->config, app_name);

	write_in_file(name, section);

	free(name);

	return 0;
}

int Xd6ConfigFile::write_section(const char *user, 
                Xd6ConfigFileSection *section)
{
	if (!section || !user) return -1;

	write_in_file(user, section);

	return 0;
}

/*
 *  End of "$Id:  $".
 */
