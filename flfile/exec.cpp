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
#include "main.h"
#include "xd640/Xd6ConfigFile.h"
#include "xd640/Xd6DefaultFonts.h"
#include <stdlib.h>
#include <libintl.h>
#include "xd640/Xd6System.h"

#define _(String) gettext((String))

const char *get_mime_exec(const char *url)
{
	int len;
	struct stat st;
	char *file = url2filename(strdup(url));
	char *exec = "flnotepad '%f'";
	len = strlen(file);

	if (!stat(file, &st) && S_ISDIR(st.st_mode)) {
		return "flfm '%f'";
	} else if (len > 4 && !strcasecmp(file + len - 4, ".htm")) {
		exec = "flwriter '%f'";
	} else if (len > 5 && !strcasecmp(file + len - 5, ".html")) {
		exec = "flwriter '%f'";
	}
	
	//FIXME
	//printf("get_mime_exec(%s)\n", file);
	free(file);
	return exec;
}

/***************************************      
	  %i = -icon
          %m = -miniicon
          %u = file://hostname/file
          %f = /file
         "%c" = window title
 **************************************/

void execute(const char *act, char **urls, int nb)
{
	char buf[2048];
	char *action = url2filename(strdup(act));
	char *ptr = action;
	char *bptr = action;
	int len = 0;
	char *f;

	if (!*ptr) {
		free(action);
		return;
	}

	snprintf(buf, 2048, " ");
	len = strlen(buf);

	ptr++;
	while (*ptr) {
		if (*ptr == '%' && *(ptr-1) != '\\') {
			*ptr = '\0';
			ptr++;
			switch(*ptr) {
			case 'i':
			case 'm':
			case 'c':
				snprintf(buf + len, 2048-len, "%s", bptr);
				len = strlen(buf);
				ptr++;
				bptr = ptr;
				break;
			case 'f':
				if (nb > 0) {
					f = url2filename(urls[0]);
				} else {
					f = strdup("");
				}
				snprintf(buf + len, 2048-len, "%s%s", bptr, f);
				len = strlen(buf);
				ptr++;
				free(f);
				bptr = ptr;
				break;
			case 'u':
				if (nb > 0) {
					char *p;
					f = urls[0];
					p = f;
					while (*p) {
						if (*p == '\n' || *p == '\r') {
							*p = '\0';
							break;
						}
						p++;
					}
				} else {
					f = "";
				}			
				snprintf(buf + len, 2048-len, "%s%s", bptr, f);
				len = strlen(buf);
				ptr++;
				bptr = ptr;
				break;
			default:
				*(ptr-1) = '%';
				break;
			}			

		}			
		ptr++;
	}	

	snprintf(buf + len, 2048-len, "%s &", bptr);
	Xd6System(buf);

	free(action);
}
