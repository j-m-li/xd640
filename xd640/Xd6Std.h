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


#ifndef Xd6Std_h
#define Xd6Std_h

#  ifdef WIN32

#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <winsock2.h>
#include <direct.h>
#include <process.h>
#include <locale.h>
#define snprintf _snprintf
#define read _read
#define open _open
#define close _close
#define strcasecmp strcmp
#define strncasecmp strncmp
#define fl_open_display() 
#define SYSCONFDIR getsysconfdir()
#define PREFIX getprefix()
#define LOCALEDIR getlocaledir()
#define lstat _stat
#define getcwd _getcwd
#define getpid _getpid
#define umask _umask
#define mkdir(p, m) _mkdir(p)
#define stat _stat

const char* getsysconfdir(void);
const char* getprefix(void);
const char* getlocaledir(void);
void XSetLocaleModifiers(const char *l);
void bindtextdomain(const char *package, const char *loclaledir);
void textdomain(const char *package);
const char *gettext(const char *String);
const char *dgettext(const char *Pack, const char *String); 

#  else
#include <locale.h>
#include <stdlib.h>
#include <libintl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#  endif
#endif

//
// End of "$Id: $".
//

