/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2002  O'ksi'D
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


#include <FL/x.H>
#include <FL/fl_ask.H>
#include <stdlib.h>
#include <libintl.h>
#include "xd640/Xd6DefaultFonts.h"
#include "xd640/Xd6ConfigFile.h"
#define _(String) gettext((String))

int main(int argc, char **argv, char **environ)
{
	int i;
	int index = 0;
	Xd6ConfigFile *cfg;
	Xd6ConfigFileSection *cfg_sec;

	Fl::args(argc, argv, index);

	// initialize an FLE configuration object
        cfg = new Xd6ConfigFile("flshutdown", "Utilities");
        cfg_sec = cfg->get_config_section(cfg->app_name);

	// GNU gettext initialization
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	// load XD640 default fonts	
	Xd6DefaultFonts::load(cfg);

	i = 0;
	while (environ[i]) {
		if (!strncmp("DISPLAY=", environ[i], 8)) {
			if (environ[i][8] != ':') {
				printf("Bad boy :-(\n");
				exit(0);
			}
		}
		i++;
	}
	fl_open_display();
	if (fl_ask(_("Do you really want to SHUTDOWN the computer ?"))) {
		system("/sbin/shutdown -ah now");
	}
    	return 0;
}

