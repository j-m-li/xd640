//
// "$Id: $"
//
// Unicode to UTF-8 conversion functions header.
//
// Copyright 2000 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
//

/*** NOTE : all functions are LIMITED to 24 bits Unicode values !!! ***/

#ifndef fl_utf8_h
#define fl_utf8_h

#ifndef FL_EXPORT
#if defined(FL_DLL) && defined(_MSC_VER)
#  ifdef FL_LIBRARY
#    define FL_EXPORT   __declspec(dllexport)
#  else
#    define FL_EXPORT   __declspec(dllimport)
#  endif /* FL_LIBRARY */
#else
#  define FL_EXPORT
#endif /* FL_DLL && _MSC_VER */
#endif /* FL_EXPORT */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <X11/Xlocale.h>
#include <X11/Xlib.h>
#include <locale.h>
#endif

#  ifdef __cplusplus
extern "C" {
#  endif

FL_EXPORT int fl_utf2ucs( const unsigned char *buf, int len, unsigned int *ucs);
FL_EXPORT int fl_ucs2utf(unsigned int ucs, char *buf);
FL_EXPORT int fl_utflen(const unsigned char *buf, int len);
FL_EXPORT int fl_utf_nb_char(const unsigned char *buf, int len);
FL_EXPORT int fl_utf_strncasecmp(const char *s1, const char *s2, int n);
FL_EXPORT int fl_utf_strcasecmp(const char *s1, const char *s2);
FL_EXPORT int fl_tolower(unsigned int ucs);
FL_EXPORT int fl_toupper(unsigned int ucs);
FL_EXPORT int fl_utf_tolower(const unsigned char *str, int len, char *buf);
FL_EXPORT int fl_utf_toupper(const unsigned char *str, int len, char *buf);
FL_EXPORT int fl_utf2unicode(const unsigned char *str, int len, 
		unsigned short *buf);
FL_EXPORT int fl_unicode2utf(const unsigned short *str, int len, char *buf);
FL_EXPORT int fl_utf2latin1(const unsigned char *str, int len, char *buf);
FL_EXPORT int fl_latin12utf(const unsigned char *str, int len, char *buf);
FL_EXPORT unsigned int fl_nonspacing(unsigned int ucs);
FL_EXPORT char* fl_utf2mbcs(const char * s);
FL_EXPORT char* fl_mbcs2utf(const char *s);

FL_EXPORT int fl_is_nt4(void);
FL_EXPORT int fl_chmod(const char* f, int mode);
FL_EXPORT int fl_access(const char* f, int mode);
FL_EXPORT int fl_stat( const char *path, struct stat *buffer );
FL_EXPORT char* fl_getcwd( char *buf, int maxlen);
/*FL_EXPORT*/ FILE* fl_fopen(const char *f, const char *mode);
FL_EXPORT int fl_open(const char* f, int o, int p);
#define fl_open(a, b) fl_open(a, b, -1);
FL_EXPORT int fl_unlink(const char *f);
FL_EXPORT char* fl_getenv(const char *name);
FL_EXPORT int fl_mkdir(const char* f, int mode);

#  ifdef __cplusplus
}
#  endif

#endif
 
//
// End of "$Id: $".
//

