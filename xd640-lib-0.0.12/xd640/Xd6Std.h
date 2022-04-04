/******************************************************************************
 *   "$Id: $"
 *
 *   Xd6Std Xd640.
 *
 *                 Copyright (c) 2002  O'ksi'D
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

