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
