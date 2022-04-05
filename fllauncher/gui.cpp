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


#include "gui.h"
#include "xd640/Xd6DefaultFonts.h"
#include "xd640/Xd6System.h"
#include <FL/Fl_Multi_Label.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ftw.h>
#include <fcntl.h>
#include <libintl.h>
#include <FL/Fl_Pixmap.H>

#define _(String) gettext((String))

static char *exec_xpm[] = {
"14 14 2 1",
"       c None",
".      c #000000",
"              ",
"              ",
"              ",
"              ",
"     ....     ",
"    .   ..    ",
"   .     ..   ",
"   .     ..   ",
"   .     ..   ",
"   ..   ...   ",
"    ......    ",
"     ....     ",
"              ",
"              "};

static char * submenu_xpm[] = {
"14 14 2 1",
"       c None",
".      c #000000",
" ..........   ",
" .        ..  ",
" ...........  ",
"  ..........  ",
"              ",
" ..........   ",
" .        ..  ",
" .        ..  ",
" .        ..  ",
" .        ..  ",
" .        ..  ",
" .        ..  ",
" ...........  ",
"  ..........  "};


static void scan_dir(const char *dir, Xd6ConfigFileSection *sec);

/*
 *  top box event handler (move the window)
 */
int My_Box::handle(int e) 
{
	static int bx = 0, by = 0;
  	static int button1 = 0;
	char val[25];
	int ex, ey;

  	if (e == FL_PUSH) button1 = (Fl::event_button() == 1);
  	if (button1) switch(e) {
    		case FL_DRAG:
			// move the window
			Fl::get_mouse(ex, ey);
      			window()->position(ex - bx, ey - by);
			return 1;
    		case FL_PUSH:
			// popup the window
      			bx = Fl::event_x(); by = Fl::event_y();
      			window()->show(); 
      			return 1;
		case FL_RELEASE:
			// save window position in config file
			snprintf(val, 25, "%d", window()->x());
			ui->cfg_section->add_item("X", val, NULL);
			snprintf(val, 25, "%d", window()->y());
			ui->cfg_section->add_item("Y", val, NULL);
			ui->cfg->write_config_section(ui->cfg_section);
			return 1;
  	}
	return Fl_Box::handle(e);
}

UserInterface *UserInterface::self = NULL;

/*
 *  User interface initialization (read config files)
 */
UserInterface::UserInterface(Xd6ConfigFile *config)
{
	Xd6ConfigFileItem *itm;
	const char *val;

    	height = 0;
    	width = 0;
	cfg = config;
	cfg_section = cfg->get_config_section(cfg->app_name);
	
	font = (Fl_Font)0;
	font_size = 14;
	verti = 0;
	terminal = "xterm -e";

	// get config files values

	itm = cfg_section->get_item("FontId", NULL);	
	if (itm && (val = itm->get_value())) {
		font = (Fl_Font) strtoul(val, NULL, 0);
	} else {
		cfg_section->add_item("FontId", "0", NULL);
	}
	if (font >= Xd6DefaultFonts::free_font) font = (Fl_Font)0;	

	itm = cfg_section->get_item("FontSize", NULL);
	if (itm && (val = itm->get_value())) {
		font_size = (int) strtoul(val, NULL, 0);
	} else {
		cfg_section->add_item("FontSize", "14", NULL);
	}
	itm = cfg_section->get_item("Vertical", NULL);
	if (itm && (val = itm->get_value())) {
		verti = (int) strtoul(val, NULL, 0);
	} else {
		cfg_section->add_item("Vertical", "0", NULL);
	}
	itm = cfg_section->get_item("Terminal", NULL);
	if (itm && (val = itm->get_value())) {
		terminal = val;
	} else {
		cfg_section->add_item("Terminal", terminal, NULL);
	}

	self = this;

	// create default pixmaps
	dir_pix = new Fl_Pixmap(submenu_xpm);
	file_pix = new Fl_Pixmap(exec_xpm);
	
	pixmaps = NULL;
	nb_pixmaps = 0;
	pix_names = NULL;
	pix_data = NULL;

	w = NULL;
}

/*
 *  user interface object cleanup
 */
UserInterface::~UserInterface()
{
	delete(file_pix);
	delete(dir_pix);
	while (nb_pixmaps > 0) {
		nb_pixmaps--;
		delete(pixmaps[nb_pixmaps]);
		free(pix_names[nb_pixmaps]);
		free(pix_data[nb_pixmaps]);
	}
	free(pixmaps);
	free(pix_names);
	free(pix_data);
	delete(popup_menu);
}

/*
 *  parse a buffer which is a copy of an XPM file and return an array
 *  of pointer to the begining of each xpm data strings
 */
static char **parse_xpm(char *d, int l)
{
        char **p = NULL;
        int in_quote = 0;
        int r = 0;
        int i = 0;


        while (i < l) {
		// skip comments
                if (!in_quote && d[i] == '/' && d[i + 1] == '*') {
                        i += 2;
                        while (i < l) {
                                if (d[i] == '*' && d[i + 1] == '/') {
                                        i += 2;
                                        break;
                                }
                                i++;
                        }
                }

                if (!in_quote && d[i] == '"') {
			// string beginning
                        p = (char**) realloc(p, sizeof(char*) * (r + 1));
                        p[r] = d + i + 1;
                        in_quote = 1;
                } else if (in_quote && d[i] == '"') {
			// string end
                        d[i] = '\0';
                        in_quote = 0;
                } else if (!in_quote && d[i] == ',') {
			// end of line
                        r++;
                }
                i++;
        }

        return p;
}

/*
 *  return a pixmap named "${file}.xpm". 
 *  Already created pixmaps are not re-created.
 *  XPM files are searched in the global and user small icon directory
 */
Fl_Pixmap* UserInterface::load_icon(const char* file) 
{
	char **pix;
	int i;
	char *buf;
	char filename[1024];
	Fl_Pixmap* img;
	struct stat s;
	int fd;
	i = 0;

	while (i < nb_pixmaps) {
		if (!strcmp(file, pix_names[i])) {
			// pixmap already exists : return it.
			return pixmaps[i];	
		}
		i++;
	}

	// search XPM file
	snprintf(filename, 1024, "%s/%s.xpm", 
		cfg->user_paths->icons_small, file);
	if (stat(filename, &s)) {
		snprintf(filename, 1024, "%s/%s.xpm",
			cfg->user_paths->icons_small, file);
		if (stat(filename, &s)) return NULL;
	}
	if (s.st_size < 10) return NULL;

	// read XPM file	
	buf = (char*) malloc(s.st_size + 1);
	fd = open(filename, O_RDONLY);
	read(fd, buf, s.st_size);
	close(fd);

	// convert data
	pix = parse_xpm(buf, s.st_size);

	// create pixmap
	img = new Fl_Pixmap(pix);
	
	// grow the pixmap array
	pixmaps = (Fl_Pixmap**) realloc(pixmaps, 
			sizeof(Fl_Pixmap*) * (nb_pixmaps + 1));
	pix_names = (char**) realloc(pix_names, 
			sizeof(char*) * (nb_pixmaps + 1));
	pix_data = (char**) realloc(pix_data, 
			sizeof(char*) * (nb_pixmaps + 1));
	pix_names[nb_pixmaps] = strdup(file);
	pix_data[nb_pixmaps] = buf;
	pixmaps[nb_pixmaps] = img;
	nb_pixmaps++;

	return img;
}

/*
 *  top box menu callback. (change orientation, popup about box, exit).
 */
void UserInterface::cb_item(Fl_Widget* w, void* d)
{
	int i = *((int*)&d);

	// exit menu item
	if (i == 3) {
		self->w->hide();
		exit(0);
	}

	// change orientation item
	if (i == 1) {
		if (!self->verti) {
			self->verti = 1;
		} else {
			self->verti = 0;
		}

		// save new orientation in config file
		self->cfg_section->add_item("Vertical", 
			(char*)(self->verti ? "1" : "0"), NULL);
		self->cfg->write_config_section(self->cfg_section);
		
		// hide the window and the launch effect
		Xd6SystemHide();
		self->w->hide();
		Fl::flush();
		// restart a new fllauncher
		system("fllauncher &");
		exit(0);
	}

	// about item
  	fl_message(
		_(" fllauncher - Copyright (c) 2000-2001 O'ksi'D\n"
		"This application is an Open Source software.\n"
		"        http://oksid.ch"));
}

/*
 *  convert SortOrder string to an array of string placed in "launched".
 */ 
static int create_launch(const char *sortorder, char ***launched)
{
	int nb_launch = 1;
	char *p;
	char *tmp;
	char *ptr;

	tmp = strdup(sortorder);
	ptr = tmp;
	
	// count the number of string
	while (*ptr) { if (*ptr == ',') nb_launch++; ptr++; }

	*launched = (char**) malloc(sizeof(char*) * nb_launch);
			
	ptr = tmp;
	while (isspace(*ptr)) ptr++;
	p = ptr;
	nb_launch = 0;
	while (*ptr) {
		ptr++;
		if (!*ptr || isspace(*ptr) || *ptr == ',') {
			// remove spaces after the string
			while (*ptr && *ptr != ',') {
				*ptr = '\0';
				ptr++;
			}
			if (*ptr) { *ptr = '\0'; ptr++; }
			
			// skip spaces before the next string
			while (isspace(*ptr)) ptr++;

			// copy the string to the array
			(*launched)[nb_launch] = strdup(p);
			nb_launch++;
			
			// set the begining of the next string
			p = ptr;
		}
	}
	free(tmp);
	return nb_launch;
}

/*
 *  create the menu buttons of the main window (requested by the "SortOrder")
 *  
 */
void UserInterface::make_buttons()
{
	const char *val;
	char *tmp;
	char *ptr;
	char *buf1;
	char *buf2;
	int len1;
	int len2;
	Xd6ConfigFileSection *sec;
	Xd6ConfigFileItem *itm;

	nb_buttons = 0;

	len1 = strlen(cfg->global_paths->appslnk) + 512;
	len2 = strlen(cfg->user_paths->appslnk) + 512;

	buf1 = (char*) malloc (len1);	
	buf2 = (char*) malloc (len2);	

	// read the root ".launch" file in the user and global directory
	sprintf(buf1, "%s/.launch", cfg->global_paths->appslnk);
	sprintf(buf2, "%s/.launch", cfg->user_paths->appslnk);
	sec = cfg->get_section(buf1, buf2, "Desktop Entry");

	if (sec) {
		tmp = NULL;
		val = NULL;
		itm = sec->get_item("SortOrder", NULL);
		if (itm) val = itm->get_value();
		if (val) {
			int nb_launch;
			int i;
			char **launched;

			nb_launch = create_launch(val, &launched);

			// create buttons requested by the SortOrder config
			// string
			i = 0;
			while (i < nb_launch) {
				// get button ".launch" file
				ptr = cfg->global_paths->appslnk;
				if (launched[i][0] == '/') ptr = "";
				snprintf(buf1, len1, "%s/%s.launch", ptr,
					launched[i]); 
				ptr = cfg->user_paths->appslnk;
				if (launched[i][0] == '/') ptr = "";
				snprintf(buf2, len2, "%s/%s.launch", ptr,
					launched[i]);

				sec = cfg->get_section(buf1, buf2, 
						"Desktop Entry");
				if (sec) make_button(sec, launched[i]);
				free(launched[i]);
				i++;
			}
			free(tmp);
			free(launched);
		}
	}
	free(buf1);
	free(buf2);
}

/*
 * count the quantity of files in the applink subdirectory
 */
static int item_count = 0;
static int ftw_count(const char *file, const struct stat *sb, int flag)
{
	if (flag == FTW_DNR || flag == FTW_NS) return 0;
	item_count++;
	return 0;
}

static void pixmap_labeltype(
	const Fl_Label* o, int x, int y, int w, int h, Fl_Align a)
{
  Fl_Pixmap* b = (Fl_Pixmap*)(o->value);
  int cx;
  if (a & FL_ALIGN_LEFT) cx = 0;
  else if (a & FL_ALIGN_RIGHT) cx = b->w()-w;
  else cx = (b->w()-w)/2;
  int cy;
  if (a & FL_ALIGN_TOP) cy = 0;
  else if (a & FL_ALIGN_BOTTOM) cy = b->h()-h;
  else cy = (b->h()-h)/2;
  b->draw(x,y,w,h,cx,cy);
}

static void pixmap_measure(const Fl_Label* o, int& w, int& h) {
  Fl_Pixmap* b = (Fl_Pixmap*)(o->value);
  w = b->w();
  h = b->h();
}

static void pix_label(Fl_Menu_Item *it, Fl_Pixmap *p)
{
  Fl::set_labeltype(FL_FREE_LABELTYPE, pixmap_labeltype, pixmap_measure);
  it->label(FL_FREE_LABELTYPE, (const char*)p);
}

/*
 *  create a menu item
 */
static My_Menu_Button *current_button = NULL;
static int create_item(char *file)
{
	Fl_Pixmap *p = NULL;
	Fl_Menu_Item *it;
        Xd6ConfigFileSection *sec;
        Xd6ConfigFileItem *itm;
	char *ptr;
	const char *val;
	int i = -1;
	int len;

	// is there enough item alloced ?
	if (!item_count) return -1;
	item_count--;

	// find file ".launch" extension
	len =  strlen(file);
	ptr = file + len - 7;

	sec = UserInterface::self->cfg->get_section(NULL, file,"Desktop Entry");

	// get the icon	
	itm = NULL;
	if (sec) itm = sec->get_item("Icon", UserInterface::self->cfg->locale);
	val = NULL;
	if (itm) val = itm->get_value();
	if (val) p = UserInterface::self->load_icon(val);
	itm = sec->get_item("Type", NULL);

	// get the name
	itm = NULL;
	if (sec) itm = sec->get_item("Name", UserInterface::self->cfg->locale);
	val = NULL;
	if (itm) val = itm->get_value();
		
	if (val && *(ptr - 1) == '/') {
		// create an item
		i = current_button->items->add(val, 0, 
			UserInterface::callback, sec, 0);

		// read the directory and create submenu items
		*(ptr - 1) = '\0';
		scan_dir(file, sec);
		*(ptr - 1) = '/';

		if (!p) p = UserInterface::self->dir_pix;

		// tranform the item to a submenu
		it = current_button->items + i;
		it->flags = FL_SUBMENU;
	} else if (val) {
		// create a normal item
		i = current_button->items->add(val, 0, 
			UserInterface::callback, sec, 0);
	}
	
	// default pixmap
	if (!p) p = UserInterface::self->file_pix;

	if (i >= 0) {
		// set menu item font and pixmap
		Fl_Multi_Label *m = new Fl_Multi_Label();
		it = current_button->items + i;
		it->labelsize(UserInterface::self->font_size);
		it->labelfont(UserInterface::self->font);
		m->labelb = it->label();
		m->typeb = it->labeltype();
		pix_label(it, p);
		m->labela = it->label();
		m->typea = it->labeltype();
		m->label(it);
	}
	return 0;
}

/*
 *  select only directories and "xxx.launch" files 
 */
static int sel_launch(const struct dirent *d)
{
	int len;
	struct stat s;

	len = strlen(d->d_name);
	if (d->d_name[0] == '.') return 0;
	if (lstat(d->d_name, &s)) return 0;	
	if (S_ISDIR(s.st_mode)) return 1;
	if (strcmp(d->d_name + len - 7, ".launch")) return 0;
	return 1;
}

/*
 *  read a directory and create submenu items
 */
static void scan_dir(const char *dir, Xd6ConfigFileSection *sec)
{
	const char *val = NULL;
	Xd6ConfigFileItem *itm;
	struct dirent **namelist = NULL;
	struct dirent * di;
	int nb;
	int i;
	char *buf;
	int len;
	int nb_launch;
	char **launched;

	// read the directory
	chdir(dir);
	nb = scandir(dir, &namelist, sel_launch, alphasort);
	if (nb < 1) return;
	
	len = strlen(dir) + 280;
	buf = (char*) malloc(len);

	itm = sec->get_item("SortOrder", NULL);
	if (itm) val = itm->get_value();

	if (val) {
		// convert "SortOrder" string
		nb_launch = create_launch(val, &launched);
	} else {
		nb_launch = 0;
		launched = NULL;
	}

	// create "SortOrder" items first and remove them of the namelist
	i = 0;
	while (i < nb_launch) {
		int ii = 0;
		while (ii < nb) {
			int l;

			di = namelist[ii];
			if (di) l = strlen(di->d_name);

			if (di && !strcmp(di->d_name + l - 7, ".launch")) 
			{
				// normal "xxx.launch" file
				if (!strncmp(di->d_name, launched[i], l - 7)) {
					snprintf(buf, len, "%s/%s", 
						dir, di->d_name);
					create_item(buf);
					free(namelist[ii]);
					namelist[ii] = NULL;
				}
			} else if (di) {
				int l2;
			
				// directory
				l2 = strlen(launched[i]);
				if (*(launched[i] + l2 - 1) == '/' &&
					l2 == l + 1 && 
					!strncmp(di->d_name, launched[i], l))
				{ 
					snprintf(buf, len, "%s/%s/.launch", 
						dir, di->d_name);
					create_item(buf);
					free(namelist[ii]);
					namelist[ii] = NULL;
				}
			}
			ii++;
		}
		free(launched[i]);
		i++;
	}
	free(launched);

	// create the non sorted items
	i = 0;
	while (i < nb) {
		int l;
		di = namelist[i];
		if (di) l = strlen(di->d_name);
		if (di && !strcmp(di->d_name + l - 7, ".launch")) 
		{
			snprintf(buf, len, "%s/%s", dir, di->d_name);
			create_item(buf);
			free(namelist[i]);
			namelist[i] = NULL;
		} else if (di) {
			snprintf(buf, len, "%s/%s/.launch", dir, di->d_name);
			create_item(buf);
			free(namelist[i]);
			namelist[i] = NULL;
		}
		i++;
	}
	free(namelist);
	free(buf);
}

/*
 *  create button menu
 */
void UserInterface::make_items(My_Menu_Button* btn, const char* path)
{
	char *buf1;
	int len1;
	int len2;
	int len3;
	int i;

	nb_buttons = 0;

	len1 = strlen(cfg->global_paths->appslnk) + 512;
	len2 = strlen(cfg->user_paths->appslnk) + 512;
	len3 = strlen(path);

	if (len2 > len1) len1 = len2;
	len1 += len3 + 20;
	buf1 = (char*) malloc (len1);	

	// check if path is valid
	if (*path != '/') {	
		sprintf(buf1, "%s/%s.launch", cfg->user_paths->appslnk, path);
	} else {
		sprintf(buf1, "%s.launch", path);
	}
	if (access(buf1, F_OK)) {
		sprintf(buf1, "%s/%s.launch", cfg->global_paths->appslnk, path);
		if (access(buf1, F_OK)) {
			free(buf1);
			return;
		}
	}
	*(buf1 + strlen(buf1) - 8) = '\0';

	// count the number of files and sub-directories
	item_count = 0;
	ftw(buf1, ftw_count, 10);
	if (item_count < 2) {
		free(buf1);
		return;
	}
	
	// allocate the menu structure
	btn->items = new Fl_Menu_Item[item_count];
	for (i = 0; i < item_count; i++) {
		btn->items[i].text = NULL;
	}
	btn->menu(btn->items);
	current_button = btn;

	// create menu items
	scan_dir(buf1, btn->sec);

	free(buf1);
}

/*
 *  create a single menu button
 */ 
void UserInterface::make_button(Xd6ConfigFileSection* sec, const char* path)
{
	Xd6ConfigFileItem *itm;
	const char *val;
	Fl_Pixmap *p;

	if (!sec) return;

	itm = sec->get_item("Name", cfg->locale);
	val = NULL;
	if (itm) val = itm->get_value();

	if (val) {
		My_Menu_Button * btn;

		// create the button according to the orientation
		if (!verti) {
			fl_font(font, font_size);
			int wi = (int) fl_width(val) + 32;
			btn = new My_Menu_Button(width, 0, 
				wi, height, val);
			btn->verti = 0;
			width += wi;
		} else {
			int he = font_size + 8;
			fl_font(font, font_size);
			int wi = (int) fl_width(val) + 32;
			if (wi > width) width = wi;
			btn = new My_Menu_Button(0, height,
                                       width, he, val);
			btn->verti = 1;
			height += he;
		}
		btn->labelsize(font_size);
		btn->labelfont(font);
		btn->sec = sec;
		btn->callback(callback);
		btn->user_data((void*) sec);

		// set the icon
		itm = sec->get_item("Icon", cfg->locale);
		val = NULL;
		if (itm) val = itm->get_value();
		p = NULL;
		if (val) p = load_icon(val);
		if (!p) {
			if (!itm) {
				p = dir_pix;
			} else {
				p = file_pix;
			}
		}
		btn->image(p);

		// create menu if button has no Type
		itm = sec->get_item("Type", NULL);
		if (!itm) {
			int l = strlen(path);
			if (*(path + l - 1) == '/') {
				make_items(btn, path);
			}
		}
	} else {
		delete(sec);
	}
}

/*
 *  create all widgets contained in the main window
 */ 
int UserInterface::create_menus()
{
	Fl_Group *grp;
	My_Box *mbox;
	int he = font_size + 8;

	if (he < 20) he = 20;
	fl_font(font, font_size);
	width = (int)fl_width(cfg->host_name);
	width += 32;
	height = he;

	// top box with host name
	mbox = new My_Box(FL_UP_BOX, 0, 0, width, he, cfg->host_name, this);
	mbox->labelsize(font_size);
	mbox->labelfont(font);
	grp = (Fl_Group*) mbox->parent();
	
	// top box popup menu
	static Fl_Menu_Button popup(0, 0, width, he);
  	popup.type(Fl_Menu_Button::POPUP3);
	popup.selection_color(137);
	popup_menu = new Fl_Menu_Item[4]; 
	popup_menu[0].text = NULL;
	popup_menu->add(_("Change Orientation"), 0, cb_item, (void*)1, 0);
	popup_menu->add(_("About"), 0, cb_item, (void*)2, 0);
	popup_menu->add(_("Exit"), 0, cb_item, (void*)3, 0);
	popup_menu->labelsize(font_size);
	popup_menu->labelfont(font);
	(popup_menu+1)->labelsize(font_size);
	(popup_menu+1)->labelfont(font);
	(popup_menu+2)->labelsize(font_size);
	(popup_menu+2)->labelfont(font);
  	popup.menu(popup_menu);

	// create menu buttons
	make_buttons();

	// resize all the widgets
	if (verti && grp) {
		const  Fl_Widget *const*a = grp->array();
		for (int i = grp->children(); i--;) {
			Fl_Widget *w = (Fl_Widget *)a[i];
			w->resize(w->x(), w->y(), width, w->h());
		}
	}
    	return 0;
}

/*
 *  menu button and submenu items callback
 */
void UserInterface::callback(Fl_Widget* w, void* data) {
	char *va;
	char buf[2048];
	Xd6ConfigFileItem *itm;
	const char *val;
	Xd6ConfigFileSection *sec;
	char *ptr = va;
        char *lst = va;

  	if (!data) return;

	chdir(self->cfg->home_dir);
	sec = (Xd6ConfigFileSection *)data;

	// nice(?) effect 
	//sprintf(buf, "fleffect ");
	sprintf(buf, " ");

	// must it run in a terminal ?
	itm = sec->get_item("Terminal", NULL);
	if (itm && ((val = itm->get_value()) && val[0] == '1')) {
		strcat(buf, self->terminal);
		strcat(buf, " ");
	}

	val = NULL;
	itm = sec->get_item("Exec", NULL);
	if (itm) val = itm->get_value();
	
	if (!val) {
		// nothing to execute ?
		return;
	}


	va = NULL;
	if (val) va = strdup(val);

	// remove "%s %f %c ..." of the Exec string
	ptr = va;
        lst = va;
        while (ptr && *ptr) {
                 if (*ptr == '%') {
                          *ptr = '\0';
                          ptr++;
                          strcat(buf, lst);
                          if (*ptr == 'c') {
				// Window title
				val = NULL;
				itm = sec->get_item("Name", NULL);
				if (itm) val = itm->get_value();
                                if (val) strcat(buf, val);
                          }
                          lst = ptr + 1;
                 }
                 ptr++;
 	}
        if (lst) strcat(buf, lst);
	if (va) free(va);

	// execute it ...
	Xd6System(buf);
}

/*
 *  create the main window
 */
Fl_Window *UserInterface::make_window()
{
	const char *val;
	Xd6ConfigFileItem *itm;
	Fl_Window *o = new Fl_Window(600,400);
	int x = 0;
	int y = 0;
	
	w = o;
	o->clear_border();
	o->user_data((void *) (this));
	create_menus();
	o->end();
	o->xclass(cfg->app_name);

	itm = cfg_section->get_item("X", NULL);
	if (itm && (val = itm->get_value())) {
		x = (int) strtoul(val, NULL, 0);
	}

	itm = cfg_section->get_item("Y", NULL);
	if (itm && (val = itm->get_value())) {
		y = (int) strtoul(val, NULL, 0);
	}

	o->resize(x, y, width, height);
    	return w;
}

/*
 *  menu button cleanup (never called)
 */
My_Menu_Button::~My_Menu_Button()
{
	for (int i = size(); i--;) {
      		if (items[i].user_data_ ) {
			delete((Xd6ConfigFileSection*)items[i].user_data_);
		}
	}
	delete(sec);
}

/*
 *  set the menu button pixmap
 */
void My_Menu_Button::image(Fl_Pixmap *pix)
{
	pixmap = pix;
}

/*
 *  menu button drawing
 */
void My_Menu_Button::draw()
{
	if (!box() || type()) return;
	Fl_Color col = labelcolor();
	if (Fl::belowmouse()==this) {
		if (Fl::event_state()&FL_BUTTON1) { 
			draw_box(down_box(), selection_color());
			labelcolor(fl_contrast(labelcolor(), 
				selection_color()));
		} else {
			draw_box(box(), color());
		}
	} else {
		draw_box(box(), color());
	}
	draw_label();
	labelcolor(col);
	if (pixmap) {
		int dy = (h() - 16) / 2;
		if (dy < 1) dy = 1;
		pixmap->draw(x() + w() - 19, y() + dy);
	}
} 

/*
 *  menu button event handler
 */ 
int My_Menu_Button::handle(int e)
{
   	const Fl_Menu_Item* m;
    	switch (e) {
    	case FL_FOCUS:
    	case FL_UNFOCUS:
    	case FL_LEAVE:
    	case FL_ENTER:
		redraw();
		return 1;

    	case FL_PUSH:
		redraw();
      	J1:
		// simple button -> execute it
        	if (!menu() || !menu()->text) {
			//if (Fl::event_clicks() > 0) {
				Fl::event_clicks(0);
				do_callback();
			//}
			return 1;
		}

		// menu button : -> popup menu
		value(-1);
		m = NULL;
		if (verti) {
			m = menu()->pulldown(x() + w(), y(), 0, 0, 0, this);
		} else {
			m = menu()->pulldown(x(), y() + h(), 0, 0, 0, this);
		}
		picked(m);
		redraw();
		return 1;
    	case FL_RELEASE:
		redraw();
		return 1;
    	case FL_SHORTCUT:
		if (Fl_Widget::test_shortcut()) goto J1;
		return 0;
    	case FL_KEYBOARD:
		if (Fl::event_key() == ' ') goto J1;
		return 0;
    	default:
		return 0;
    	}
}

/*
 *   "$Id: $"
 */

