/******************************************************************************
 *   "$Id: $"
 *
 *
 *                 Copyright (c) 2000,2001  O'ksi'D
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
#ifndef Xd6HtmlDownload_h
#define Xd6HtmlDownload_h

#include "Xd6ConfigFile.h"

class Xd6HtmlFrame;
class Xd6HtmlTagForm;
class Xd6HtmlDisplay;


class Xd6HtmlDownload {
public:
	static char local_file[1024];
	Xd6ConfigFile *cfg;
        Xd6ConfigFileSection *cfg_sec;
        Xd6ConfigFileItem *itm;
        char *tmp_dir;
	char *file;

	Xd6HtmlDownload();
	virtual ~Xd6HtmlDownload();
	
	virtual const char *request(const char *href, const char *target,
		Xd6HtmlTagForm *form, Xd6HtmlFrame *frame, Xd6HtmlDisplay *wi = NULL);
	static char *create_url(const char *base, const char *u);
	const char *download_data(const char *url);
	const char *Xd6HtmlDownload::curl_download(const char *u, const char *b);
};

extern Xd6HtmlDownload* downloader;

#endif

/*
 * "$Id: $"
 */
