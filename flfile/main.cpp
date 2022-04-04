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


#include "main.h"
#include "xd640/Xd6ConfigFile.h"
#include "xd640/Xd6DefaultFonts.h"
#include <stdlib.h>
#include <libintl.h>
#include "xd640/Xd6XmlUtils.h"

#define _(String) gettext((String))

Xd6ConfigFile *cfg;
int host_len = 0;
char *target = NULL;
int action = 0;
int verbose = 1;
char **urls = NULL;
int nburl = 0;

static void help()
{
	fprintf(stderr, _(
		"Usage : flfile [OPTION] [TARGET]\n"
		"\n"
		"  --exec		execute TARGET url\n"
		"  --delete		delete urls passed to the standard"
			" input\n"
		"  --copy		copy urls passed to the standard \n"
		"			 	input to TARGET directory url\n"
		"  --move		move urls passed to the standard\n"
		" 			 	input to TARGET directory url\n"
		"  --link		symlink urls passed to the standard\n"
		" 			 	input to TARGET directory url\n"
		"  --properties		display properties of urls passed\n"
		"			 	to the standard input\n"
		"  --help		display this help and exit\n"
		));
}


int local_file()
{
	if (action == COPY && target) {	// *** copy ***
		Copy *cpy = new Copy(400, 100);
		cpy->setup(urls, nburl, target, verbose);
		cpy->exec();
		delete(cpy);
	} else if (action == MOVE && target) {	// *** move ***
		Move *mv = new Move(400, 100);
		mv->setup(urls, nburl, target, verbose);
		mv->exec();
		delete(mv);
	} else if (action == LINK && target) {	// *** link ***
		Link *lnk = new Link(400, 100);
		lnk->setup(urls, nburl, target, verbose);
		lnk->exec();
		delete(lnk);
	} else if (action == DELETE) {
		Unlink *unl = new Unlink(400, 100);
		unl->setup(urls, nburl, verbose);
		unl->exec();
		delete(unl); 
	} else if (action == PROP) {
		Properties *pro = new Properties(450, 300);
		pro->setup(urls, nburl, verbose);
		pro->exec();
		delete(pro);
	} else if (action == OPEN && target) {
		execute(get_mime_exec(target), &target, 1);
	} else if (action == EXEC && target) {
		execute(target, urls, nburl);
	} else {
		return 1;
	} 
	return 0;
}

char *url2filename(char *url) 
{
	char *ptr, *ret;

	if (!(ptr = strstr(url, "file:"))) {
		// this is not an file url
		return strdup(url);
	}
	
	ptr += 5;

	if (ptr[0] != '/') return NULL;
	
	if (ptr[1] == '/') {
		if (ptr[2] == '/') {
			ptr += 2;
		} else if (!strncmp(ptr + 2, cfg->host_name, host_len) &&
			*(ptr + 2 + host_len) == '/')
		{
			ptr += 2 + host_len;
		} else if (!strncmp(ptr + 2, "localhost", 9) && 
			*(ptr + 11) == '/')
		{
			ptr += 11;
		} else {
			return NULL; // url is not local !
		}
	} else {
		; // bad url (KDE / GNOME), but we can try to use it
	}

	ret = ptr = strdup(ptr);
	while (*ptr) {
		if (*ptr == '\n' || *ptr == '\r') {
			*ptr = '\0';
			break;
		}
		ptr++;
	}
	url2latin1(ret, strlen(ret));
	return ret;
}

static int cmdlp (int argc, char **argv, int &i)
{
	if (!strcmp("--quiet", argv[i])) {
		verbose = 0;
		i++;
		return 1;
	} else if (!strcmp("--exec", argv[i])) {
		action = EXEC;
	} else if (!strcmp("--delete", argv[i])) {
		action = DELETE;
		i++;
		return 1;
	} else if (!strcmp("--open", argv[i])) {
		action = OPEN;
	} else if (!strcmp("--copy", argv[i])) {
		action = COPY;
	} else if (!strcmp("--link", argv[i])) {
		action = LINK;
	} else if (!strcmp("--move", argv[i])) {
		action = MOVE;
	} else if (!strcmp("--properties", argv[i])) {
		action = PROP;
		i++;
		return 1;
	} else if (!strcmp("--help", argv[i])) {
		help();
		exit(0);
	} else {
		return 0;
	}
	i++;
	if (i < argc) {
		target = argv[i];
	}
	return 1;
}


int main(int argc, char **argv)
{
	int index = 0;
	char buf[2048];
	Fl_Font font = (Fl_Font) 0;
	int size = 16;
	Xd6ConfigFileItem *itm;
	const char *val;
	Xd6ConfigFileSection *cfg_sec;

	Fl::args(argc, argv, index, cmdlp);

	if (!action) {
		help();
		exit(-1);
	}

	fl_open_display();
	
        // initialize an FLE configuration object
        cfg = new Xd6ConfigFile("flfile", "Utilities");
	cfg_sec = cfg->get_config_section(cfg->app_name);
	host_len = strlen(cfg->host_name);

        // GNU gettext initialization
        bindtextdomain(PACKAGE, LOCALEDIR);
        textdomain(PACKAGE);

      	// load FLE default fonts
        Xd6DefaultFonts::load(cfg);

        if (cfg_sec) {
                itm = cfg_sec->get_item("FontId", NULL);
                val = NULL; if (itm) val = itm->get_value();
                if (val) font = (Fl_Font) strtoul(val, NULL, 0);
                itm = cfg_sec->get_item("FontSize", NULL);
                val = NULL; if (itm) val = itm->get_value();
		if (val) size = (int) strtoul(val, NULL, 0);
		
		if (font >= Xd6DefaultFonts::free_font) {
			font = (Fl_Font) 0;
		}
	}
	
	fl_message_font(font, size);
	Fl::txt_no = _("No");
	Fl::txt_yes = _("Yes");
	Fl::txt_ok = _("OK");
	Fl::txt_cancel = _("Cancel");


	if (!geteuid()) {
		fl_alert(_("flfile cannot run as \"root\" !\n"));
		Fl::run();
		return -1;
	}

	if (getegid() != getgid()) {
		fl_alert(_("flfile cannot run with privileges !\n"));
		Fl::run();
		return -1;
	}
	

	while (!feof(stdin) && fgets(buf, 2048, stdin)) 
	{
		char *ptr;
		if (action != OPEN && action != EXEC) {
			ptr = url2filename(buf);
		} else {
			ptr = strdup(buf);
		}
		if (ptr) {
			nburl++;
			urls = (char **) realloc(urls, sizeof(char*) * nburl);
			urls[nburl - 1] = ptr;
		}
	}

	if (nburl > 0 || target) {
		if (target && (action != OPEN && action != EXEC)) {
			target = url2filename(target);
		}
		return local_file();
	}

	return 1;
}
