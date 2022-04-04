/******************************************************************************
 *   "$Id:  $"
 *
 *   This file is part of the FLE project.
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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libintl.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Group.H>
#include <FL/fl_ask.H>
#include "icon.h"
#include "proxy.h"
#include "xd640/Xd6DefaultFonts.h"
#include "xd640/Xd6XmlUtils.h"

#define _(String) gettext((String))

static Xd6ConfigFile *cfg;
char pos_name[1024];
static Fl_Font font = (Fl_Font)0;
static int size = 16;


void proxy_cb(Fl_Widget *w, void *data)
{
        if (Fl::e_length) {
		Xd6ConfigFileSection *sec;
		Xd6ConfigFileSection *pos;
   		Icon *icon;
		char name[64];
		char val[255];
		int i;
		FILE *fp;
		char *text;
        	char buf[2048];

		snprintf(name, 21, _("Link_[%d]"), 0);
		snprintf(buf, 2048, "%s/Desktop/%s.launch", 
			cfg->home_dir, name);
		i = 1;
		while (!access(buf, F_OK)) {
			snprintf(name, 21, _("Link_[%d]"), i);
			snprintf(buf, 2048, "%s/Desktop/%s.launch",
				cfg->home_dir, name);
			i++;
		}

		fp = fopen(buf, "w");
		if (!fp) return;

		text = (char*) malloc(Fl::e_length * 6);
		text[0] = '\0';
		utf2xml(Fl::e_text, Fl::e_length, text);
		fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
		fprintf(fp, "<!DOCTYPE xd640cfg >\n");
		fprintf(fp, "<xd640cfg>\n");
		fprintf(fp, "<s n=\"Desktop Entry\" >\n");
		fprintf(fp, "  <i n=\"Name\" v=\"%s\" />\n", name);
		fprintf(fp, "  <i n=\"Icon\" v=\"unknown\" />\n");
		fprintf(fp, "  <i n=\"Type\" v=\"Link\" />\n");
		fprintf(fp, "  <i n=\"URL\"  v=\"%s\" />\n", text);
		fprintf(fp, "</s>\n");
		fprintf(fp, "</xd640cfg>\n\n");
		fclose(fp);
		free(text);

		sec = cfg->get_section(NULL, buf, "Desktop Entry");
		snprintf(val, 255, "%s.launch", name);
		pos = cfg->get_section(NULL, pos_name, val);
		snprintf(val, 25, "%d", Fl::event_x_root());
                pos->add_item("X", val, NULL);
                snprintf(val, 25, "%d", Fl::event_y_root());
                pos->add_item("Y", val, NULL);
		cfg->write_section(pos_name, pos);

   		icon = new Icon(cfg, sec, pos, pos_name, buf);
		icon->font = font;
		icon->size = size;
   		icon->create();
   		icon->end();
   		icon->show();
        }
	Fl::flush();
}

static int sel_launch(const struct dirent *d)
{
        int len;
        struct stat s;

        len = strlen(d->d_name);
        if (d->d_name[0] == '.') return 0;
        if (lstat(d->d_name, &s)) return 0;
        if (S_ISDIR(s.st_mode)) {
		char buf[1024];
		snprintf(buf, 1024, "%s/.launch", d->d_name);
		if (access(buf, F_OK)) return 0;
		return 1;
	}
        if (strcmp(d->d_name + len - 7, ".launch")) return 0;
        return 1;
}


int main(int argc, char ** argv) 
{
	Xd6ConfigFileItem *itm;
	Xd6ConfigFileSection *sec;
	Xd6ConfigFileSection *pos;
	Xd6ConfigFileSection *cfg_sec;
	const char *val;
   	Icon *icon;
	Proxy *proxy;
   	struct dirent **namelist = NULL;
	char dir[1024];
	int nb;

	cfg = new Xd6ConfigFile("fldesk", "Utilities");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	
	Xd6DefaultFonts::load(cfg);
	cfg_sec = cfg->get_config_section(cfg->app_name);
	if (cfg_sec) {
		itm = cfg_sec->get_item("FontId", NULL);
		val = NULL; if (itm) val = itm->get_value();
		if (val) font = (Fl_Font) strtoul(val, NULL, 0);
		itm = cfg_sec->get_item("FontSize", NULL);
		val = NULL; if (itm) val = itm->get_value();
		if (val) size = (int) strtoul(val, NULL, 0);
	}

	snprintf(dir, 1024, "%s/Desktop/Trash/.launch", cfg->home_dir);
	if (access(dir, F_OK)) {
		char buf[2048];
		snprintf(buf, 2048, "/bin/cp -r %s/../xd640/Desktop %s/", 
			cfg->global_paths->apps, cfg->home_dir);
		system(buf);
	}
	snprintf(dir, 1024, "%s/Desktop", cfg->home_dir);
   	chdir(dir); 
   	nb = scandir(dir, &namelist, sel_launch, alphasort);

	snprintf(pos_name, 1024, "%s/positions.cfg", cfg->user_paths->apps);
	

	while (nb > 0) {
		char buf[1024];
		char *name;

		nb--;
		name = namelist[nb]->d_name;
		if (!strcmp(name + strlen(name) - 7, ".launch")) {		
			snprintf(buf, 1024, "%s/%s", dir, namelist[nb]->d_name);
		} else {
			snprintf(buf, 1024, "%s/%s/.launch", dir, 
				namelist[nb]->d_name);
		}
		pos = cfg->get_section(NULL, pos_name, namelist[nb]->d_name);
		sec = cfg->get_section(NULL, buf, "Desktop Entry");
   		icon = new Icon(cfg, sec, pos, pos_name, buf);
		icon->font = font;
		icon->size = size;
   		icon->create();
   		icon->end();
   		icon->show();
		XLowerWindow(fl_display, fl_xid(icon));
	 	if (icon->ic1) XLowerWindow(fl_display, fl_xid(icon->ic1));
                if (icon->ic2) XLowerWindow(fl_display, fl_xid(icon->ic2));

		free(namelist[nb]);
	}

	proxy = new Proxy();
	proxy->init();
	proxy->callback(proxy_cb);

	free(namelist);
   	return Fl::run();
}

//
// End of "$Id:  $".
//
