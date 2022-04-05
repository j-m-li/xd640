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


#include "callbacks.h"
#include "BigIcon.h"
#include "IconTree.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libintl.h>
#include <FL/fl_ask.H>
#include <sys/stat.h>
#include <sys/wait.h>
#include "xd640/Xd6XmlUtils.h"

#define _(String) gettext((String))

struct states_struct StatesValues;
static time_t mod_time;

static void save_state()
{
	char buf[80];

	snprintf(buf, 80, "%d", gui->w() + 20);
	cfg_sec->add_item("W", buf, NULL);
	snprintf(buf, 80, "%d", gui->h() + 20);
	cfg_sec->add_item("H", buf, NULL);

	cfg_sec->add_item("sort_size", 
		StatesValues.sort_size ? "1" : "0", NULL);
	cfg_sec->add_item("sort_type", 
		StatesValues.sort_type ? "1" : "0", NULL);
	cfg_sec->add_item("sort_name", 
		StatesValues.sort_name ? "1" : "0", NULL);
	cfg_sec->add_item("view_icon", 
		StatesValues.view_icon ? "1" : "0", NULL);
	cfg_sec->add_item("view_tree", 
		StatesValues.view_tree ? "1" : "0", NULL);
	cfg_sec->add_item("view_detail", 
		StatesValues.view_detail ? "1" : "0", NULL);
	cfg_sec->add_item("show_hide", 
		StatesValues.show_hide ? "1" : "0", NULL);

	cfg->write_config_section(cfg_sec);
}

static char *get_tree_sel()
{
	IconTree *it = ((IconTree*)gui->icon_can);
	int nb = it->item_max();
	int i = 0;
	Entry *e;

	i = it->get_selection();
	if (i >= 0 && i < nb) {
		e = it->get_entry(i);
		return e->url;
	}
	return NULL;
}
	

static char *get_tree_selected()
{
	IconTree *it = ((IconTree*)gui->icon_can);
	int nb = it->item_max();
	int i = 0;
	char *go = NULL;
	Entry *e;
	int mlen = 0;

	while (i < nb) {
		if (it->item_flags(i) & IconTree::FLAG_SELECTED) {
			e = it->get_entry(i);
			if (!go) {
				mlen = strlen(e->url);
				go = (char*) malloc(3*mlen + 5);
				mlen = latin12url(e->url, mlen, go);
				go[mlen] = 0;
			} else {
				int l ;
				l = strlen(e->url);
				strcat(go, "\r\n");
				mlen += 2;
				go = (char*) realloc(go, mlen + l + 5);
				mlen += latin12url(e->url, l, go + mlen); 
				go[mlen] = 0;
			}
		}
		i++;
	}
	if (!go) {
		i = it->get_selection();
		if (i >= 0 && i < nb) {
			e = it->get_entry(i);
			go = strdup(e->url);
		}	
	}	

	return go;
}

char *get_selected_urls()
{
	int i;
	Fl_Widget*const* a;
	BigIcon *b;
	char *buf;
	int index;
	int alloc;

	if (StatesValues.view_tree) {
		return get_tree_selected();
        }
	if (!gui || !gui->icon_can || 
		!((IconCanvas*)gui->icon_can)->group) return NULL;

	a = ((IconCanvas*)gui->icon_can)->group->array();
	i = ((IconCanvas*)gui->icon_can)->group->children();

	alloc = 4096;
	buf = (char*) malloc(alloc);
	buf[0] = '\0';
	index = 0;
	while (i > 0) {
		i--;
		b = (BigIcon*) a[i];
		if (b && b->selected) {
			char *b1;
			char *b2;
			if (index > alloc - 4096) {
				alloc = alloc * 2;
				buf = (char*) realloc(buf, alloc);
			}
			b1 = (char*) malloc(strlen(StatesValues.url) * 3 + 1);
			b2 = (char*) malloc(strlen(b->label()) * 3 + 1);
			latin12url(StatesValues.url, 
				strlen(StatesValues.url), b1);
			latin12url(b->label(), strlen(b->label()), b2);
			snprintf(buf + index, 4096, "file://%s/%s\r\n", b1, b2);
			index += strlen(buf + index);
			free(b1);
			free(b2);
		}		
	}
	return buf;
}


static const BigIcon *get_selected_icon()
{
	int i;
	Fl_Widget*const* a;
	BigIcon *b;

        if (StatesValues.view_tree) {
                return NULL;
        }

	if (!gui || !gui->icon_can || 
		!((IconCanvas*)gui->icon_can)->group) return NULL;

	a = ((IconCanvas*)gui->icon_can)->group->array();
	i = ((IconCanvas*)gui->icon_can)->group->children();

	while (i > 0) {
		i--;
		b = (BigIcon*) a[i];
		if (b && b->selected) {
			return b;
		}
	}
	return NULL;
}

static char *url2dir(char *url)
{
	char *ptr;
	char *dir;
	char buf[1024];
	char buf1[1024];

	ptr = strstr(url, "file:/");
	if (ptr) {
		dir = ptr + 5;
		while (*ptr && *ptr != '\n' && *ptr != '\r') {
			ptr++;	
		}
		*ptr = '\0';
		url2latin1(dir, strlen(dir));
		return strdup(dir);	
	}

	if (url[0] == '/') {
		ptr = url;
		dir = ptr;
		while (*ptr && *ptr != '\n' && *ptr != '\r') ptr++;
		*ptr = '\0';
		url2latin1(dir, strlen(dir));
		return strdup(dir);
	}
	
	ptr = strstr(url, "file:");
	if (ptr) {
		ptr += 5;
		url = ptr;
	} else {
		ptr = url;
	}
	while (*ptr && *ptr != '\n' && *ptr != '\r') ptr++;
	*ptr = '\0';
	url2latin1(url, strlen(url));
	buf[0] = '\0';
	getcwd(buf, 1024);
	
	snprintf(buf1, 1024, "%s/%s", buf, url);

	return (strdup(buf1));
}

void cb_rescan(Fl_Widget*, void*)
{
	char *dir;
	char buf[1024];
	struct stat st;

	Fl::remove_timeout(cb_directory, 0);

	if (!StatesValues.newurl) {
		if (StatesValues.url) {
			StatesValues.newurl = strdup(StatesValues.url);
		} else {
			StatesValues.newurl = strdup(cfg->home_dir);
		}
	}

	dir = url2dir(StatesValues.newurl);
	buf[0] = '\0';
	chdir(dir);
	getcwd(buf, 1024);

	if (StatesValues.url && strcmp(buf, StatesValues.url)) {
		gui->loc_inp->new_url(StatesValues.url);
	}

	free(StatesValues.url);
	StatesValues.url = strdup(buf);
	gui->loc_inp->value(StatesValues.url);
	stat(StatesValues.url, &st);

	if (StatesValues.view_tree) {
		((IconTree*)gui->icon_can)->rescan();
	} else {
		((IconCanvas*)gui->icon_can)->rescan();
	}

	free(StatesValues.newurl);
	StatesValues.newurl = NULL;
	free(dir);
	Fl::redraw();

	mod_time = st.st_mtime;
	Fl::add_timeout(1, cb_directory, 0);
}

void cb_newdir(Fl_Widget*, void*)
{
	const char *dir;
	if ((dir = fl_input(_("New Directory :")))) {
		mkdir(dir, 0777);
	}
	cb_rescan(NULL, NULL);
}

void cb_exit(Fl_Widget*, void*)
{
	exit(0);
}

void cb_sort(Fl_Widget *w, void *d)
{
	int itm = *((int*) &d);

	switch (itm) {
	case 1:
		StatesValues.sort_name = 1;
		StatesValues.sort_type = 0;
		StatesValues.sort_size = 0;
		break;
	case 2:
		StatesValues.sort_type = 1;
		StatesValues.sort_size = 0;
		StatesValues.sort_name = 0;
		break;
	case 3:
		StatesValues.sort_size = 1;
		StatesValues.sort_name = 0;
		StatesValues.sort_type = 0;
		break;
	case 4:
		if (StatesValues.view_tree && gui && gui->icon_can) {
			int x, y, w, h;
			x = gui->icon_can->x();
			y = gui->icon_can->y();
			w = gui->icon_can->w();
			h = gui->icon_can->h();
			gui->remove(gui->icon_can);
			delete(gui->icon_can);
			gui->begin();
			gui->icon_can = new IconCanvas(x, y, w, h);		
			gui->end();
		}
		StatesValues.view_icon = 1;
		StatesValues.view_detail = 0;
		StatesValues.view_tree = 0;
		break;
	case 5:
		StatesValues.view_icon = 0;
		StatesValues.view_detail = 0;
		StatesValues.view_tree = 1;
		if (gui && gui->icon_can) {
			int x, y, w, h;
			x = gui->icon_can->x();
			y = gui->icon_can->y();
			w = gui->icon_can->w();
			h = gui->icon_can->h();
			gui->remove(gui->icon_can);
			delete(gui->icon_can);
			gui->begin();
			gui->icon_can = new IconTree(x, y, w, h);
			gui->resizable(gui->icon_can);		
			gui->end();
		}
		break;
	case 6:
		if (StatesValues.view_tree && gui && gui->icon_can) {
			int x, y, w, h;
			x = gui->icon_can->x();
			y = gui->icon_can->y();
			w = gui->icon_can->w();
			h = gui->icon_can->h();
			gui->remove(gui->icon_can);
			delete(gui->icon_can);
			gui->begin();
			gui->icon_can = new IconCanvas(x, y, w, h);		
			gui->end();
		}
		StatesValues.view_detail = 1;
		StatesValues.view_icon = 0;
		StatesValues.view_tree = 0;
		break;
	case 7:
		StatesValues.show_hide = (char) !(StatesValues.show_hide);
		break;
	default:
		break;
	}
	save_state();
	cb_rescan(w, d);
}

void cb_about(Fl_Widget*, void*)
{
	fl_message(
	      _("  flfm - Copyright (c) 2000-2002 O'ksi'D\n"
		"This application is an Open Source  software.\n"
		"        http://www.oksid.ch"));
}

void cb_help(Fl_Widget*, void*)
{
	char buf[2048];
	char b[2048];
	snprintf(buf, 2048, "%s/HTML/%s/%s/index.html",
		cfg->global_paths->doc, cfg->lang, cfg->app_name);
	if (!access(buf, 0)) {
		snprintf(b, 2048, "flspider '%s' &", buf);
	} else {
		snprintf(b, 2048, "flspider '%s/HTML/en/%s/index.html' &",
			cfg->global_paths->doc, cfg->app_name);
	}
	system(b);
}

void cb_exec(Fl_Widget*, void  *d)
{
	const BigIcon *b;
	char buf[2048];
	char *b1, *b2;

	
        if (StatesValues.view_tree) {
		b1 = get_tree_sel();
		if (!b1) return;
		snprintf(buf, 2048, "flfile --exec \"%s\" &", b1);
		system(buf);
		return;
	}
	b = get_selected_icon();
	if (!b) return;
	
	b1 = (char*) malloc(strlen(StatesValues.url) * 3 + 1);
	b2 = (char*) malloc(strlen(b->label()) * 3 + 1);
	latin12url(StatesValues.url, strlen(StatesValues.url), b1);
	latin12url(b->label(), strlen(b->label()), b2);
	snprintf(buf, 2048, "flfile --exec \"file://%s/%s\r\n\" &", b1, b2);
	free(b1);
	free(b2);
	system(buf);
}

void cb_open(Fl_Widget*, void  *d)
{
	const BigIcon *b;
	char buf[2048];
	char *b1, *b2;

        if (StatesValues.view_tree) {
		b1 = get_tree_sel();
		if (!b1) return;
		snprintf(buf, 2048, "flfile --open \"%s\" &", b1);
		system(buf);
		return;
	}
	
	b = get_selected_icon();
	if (!b) return;
	b1 = (char*) malloc(strlen(StatesValues.url) * 3 + 1);
	b2 = (char*) malloc(strlen(b->label()) * 3 + 1);
	latin12url(StatesValues.url, strlen(StatesValues.url), b1);
	latin12url(b->label(), strlen(b->label()), b2);
	
	snprintf(buf, 2048, "flfile --open \"file://%s/%s\" &", b1, b2);
	system(buf);
	free(b1);
	free(b2);
}

void cb_open_width(Fl_Widget*, void*)
{
	const BigIcon *b = NULL;
	char buf[2048];
	static char *last_cmd = NULL; 
	char *b1 = get_tree_sel();
	char *r;

	
	if (!last_cmd) {
		last_cmd = strdup("flnotepad");
	}	
        if (StatesValues.view_tree) {
		if (!b1) return;
		r = (char *)fl_input(_("Open \"%s\" with :"), last_cmd, 
			 b1 + 7);
	} else {
		b = get_selected_icon();
		if (!b) return;
		r = (char *)fl_input(_("Open \"%s%s\" with :"), last_cmd, 
			StatesValues.url, b->label());
	}

	if (!r) return;
	free(last_cmd);
	last_cmd = strdup(r);

        if (StatesValues.view_tree) {
		snprintf(buf, 2048, "%s \"%s\" &", r, b1 + 7);
		system(buf);
		return;
	}

	snprintf(buf, 2048, "%s \"%s/%s\" &", r, 
			StatesValues.url, b->label());
	system(buf);
}

void cb_new_fm(Fl_Widget*, void*)
{
	char buf[2048];
	
	snprintf(buf, 2048, "flfm \"%s\" &", StatesValues.url);
	system(buf);
}

void cb_open_dir(Fl_Widget*, void*)
{
	const BigIcon *b;
	char buf[2048];

	b = get_selected_icon();
	
	if (b) {
		snprintf(buf, 2048, "flfm \"%s/%s\" &",  
			StatesValues.url, b->label());
	} else {
		snprintf(buf, 2048, "flfm \"%s\" &",  
			StatesValues.url);
	}
	system(buf);	
}

void cb_change_dir(Fl_Widget*, void*)
{
	const BigIcon *b;
	char buf[2048];
	char *b1, *b2;
	b = get_selected_icon();
	if (!b) return;  
	b1 = (char*) malloc(strlen(StatesValues.url) * 3 + 1);
	b2 = (char*) malloc(strlen(b->label()) * 3 + 1);
	latin12url(StatesValues.url, strlen(StatesValues.url), b1);
	latin12url(b->label(), strlen(b->label()), b2);
	snprintf(buf, 2048, "file://%s/%s\r\n", b1, b2);
	StatesValues.newurl = strdup(buf);
	cb_rescan(NULL, NULL);
	free(b1);
	free(b2);
};
	
void cb_up(Fl_Widget*, void*)
{
	char buf[2048];
	int l;
	char *b1 = (char*) malloc(strlen(StatesValues.url) * 3 + 1);
	latin12url(StatesValues.url, strlen(StatesValues.url), b1);
	snprintf(buf, 2000, "file://%s", b1);
	l = strlen(buf);
	while (l > 0) {
		l--;
		if (buf[l] == '/') {
			buf[l] = '\0';
			break;
		}
	}
	snprintf(buf + l, 5, "/\r\n");
	StatesValues.newurl = strdup(buf);
	cb_rescan(NULL, NULL);
	free(b1);
}

void cb_back(Fl_Widget*, void*)
{
	int i;

	i = 10;
	while (i > 0) {
		i--;
		if (StatesValues.history[i]) break;
	}
	if (!StatesValues.history[i]) return;

	StatesValues.newurl = StatesValues.history[i];
	StatesValues.history[i] = NULL;
	free(StatesValues.url);
	StatesValues.url = NULL;
	cb_rescan(NULL, NULL);
}

void cb_delete(Fl_Widget*, void*)
{
	char buf[1024];
	FILE *fp;
	char *urls;
	static int pid = -1;


	if (pid > 0) {
		int s;
		waitpid(pid, &s, WNOHANG);
	}
	pid = fork();
	if (pid == -1 || pid > 0) return;

	urls = get_selected_urls();
	if (!urls) return;
	
	snprintf(buf, 1024, "flfile --delete");
	fp = popen(buf, "w");	
	fwrite(urls, strlen(urls), 1, fp);
	pclose(fp);

	free(urls);
	_exit(0);
}

void cb_properties(Fl_Widget*, void*)
{
	char buf[1024];
	FILE *fp = NULL;
	char *urls;
	static int pid = -1;

	if (pid > 0) {
		int s;
		waitpid(pid, &s, WNOHANG);
	}
	pid = fork();
	if (pid == -1 || pid > 0) return;

	urls = get_selected_urls();
	if (!urls) _exit(0);
	snprintf(buf, 1024, "flfile --properties");
	fp = popen(buf, "w");	
	fwrite(urls, strlen(urls), 1, fp);
	pclose(fp);
	free(urls);
	_exit(0);
}

void cb_move_link_copy(Fl_Widget*, void *d)
{
	FILE *fp;
	char buf[1024];
	int a = *((int*) &d);
	Fl_Widget *w = Fl::belowmouse();
	const char *dir = "";
	static int pid = -1;
	char *b1, *b2;

	if (Fl::e_length < 2) return;
	if (gui->loc_inp == w || gui->loc_inp->contains(w)) {
		StatesValues.newurl = strdup(Fl::e_text);
		cb_rescan(NULL, NULL);
		return;
	}
	if (StatesValues.view_tree) {	
		dir = "";
		IconTree *it = ((IconTree*)gui->icon_can);
		int n = it->get_selection();
		if (n >= 0 && n < it->item_max()) {
			if (!(it->item_flags(n) & Fl_Tree::FLAG_FOLDER)) return;
			char *u = it->get_entry(n)->url;
			b1 = (char*) malloc(strlen(u + 7) * 3 + 1);
			latin12url(u + 7, strlen(u + 7), b1);
			free(StatesValues.url);
			StatesValues.url = strdup(u + 7);
			gui->loc_inp->value(StatesValues.url);
			chdir(StatesValues.url);
		} else {
			return;
		}
	} else {
		if (w == ((IconCanvas*)gui->icon_can)->group) {
			dir = "";
		} else if (((IconCanvas*)gui->icon_can)->group->contains(w)) {
			BigIcon *b = (BigIcon*) w;
			dir = b->label();
		} else {
			return;
		}
		b1 = (char*) malloc(strlen(StatesValues.url) * 3 + 1);
		latin12url(StatesValues.url, strlen(StatesValues.url), b1);
	}
	b2 = (char*) malloc(strlen(dir) * 3 + 1);
	latin12url(dir, strlen(dir), b2);
	buf[0] = '\0';
	switch(a) {
	default:
	case DND_COPY:
		snprintf(buf, 2040, "flfile --copy \"file:///%s/%s\" ", b1, b2);
		break;
	case DND_MOVE:
		snprintf(buf, 2040, "flfile --move \"file:///%s/%s\" ", b1, b2);
		break;
	case DND_LINK:
		snprintf(buf, 2040, "flfile --link \"file:///%s/%s\" ", b1, b2);
		break;
	}
	free(b1);
	free(b2);
	if (dir[0]) {
		strcat(buf, "/");
	}
	if (pid > 0) {
		int s;
		waitpid(pid, &s, WNOHANG);
	}
	pid = fork();
	if (pid == -1 || pid > 0) return;

	fp = popen(buf, "w");	
	fwrite(Fl::e_text, Fl::e_length, 1, fp);
	pclose(fp);
	_exit(0);
}

void cb_loc_input(Fl_Widget*, void *d)
{
	Location *l = (Location*) d;
	if (!d) return;
	if (!l->url || (l->loc->value() && strcmp(l->url, l->loc->value()))) {
		StatesValues.newurl = strdup(l->loc->value());
		cb_rescan(NULL, NULL);
	}
}

void cb_directory(void*) 
{
	struct stat st;
	if (!StatesValues.url) {
		Fl::add_timeout(.5, cb_directory, 0);
		return;
	}
	stat(StatesValues.url, &st);
	if (st.st_mtime != mod_time) {
		cb_rescan(NULL, NULL);
		return;
	}
	Fl::add_timeout(.5, cb_directory, 0);
}
