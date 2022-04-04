//
// "$Id: $"
//
// Unicode to UTF-8 conversion functions.
//
//
// Copyright 2000-2002 by O'ksi'D.
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
// Please report all bugs and problems to "oksid@bluewin.ch".
//
// Author: Jean-Marc Lienher ( http://oksid.ch )
//
//


#include <config.h>
#include "spacing.h"

#ifndef WIN32
#if HAVE_XUTF8
#include <libXutf8/Xutf8.h>
#else 
#include <ctype.h>

#endif //HAVE_XUTF8
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#else
#include <ctype.h>
#include <io.h>
#include <direct.h>
#include <windows.h>
#include <winbase.h>
#endif //WIN32

#include <FL/fl_utf8.H>
#include <string.h>
#include <stdlib.h>

#undef fl_open

/*** NOTE : all functions are LIMITED to 24 bits Unicode values !!! ***/
/***        but only 16 bits are realy used under Linux and win32  ***/


static int 
Tolower(
	int ucs)
{
	int ret;

	if (ucs <= 0x02B6) {
		if (ucs >= 0x0041) {
			ret = ucs_table_0041[ucs - 0x0041];
			if (ret > 0) return ret;
		}
		return ucs;
	}

	if (ucs <= 0x0556) {
		if (ucs >= 0x0386) {
			ret = ucs_table_0386[ucs - 0x0386];
			if (ret > 0) return ret;
		}
		return ucs;
	}

	if (ucs <= 0x10C5) {
		if (ucs >= 0x10A0) {
			ret = ucs_table_10A0[ucs - 0x10A0];
			if (ret > 0) return ret;
		}
		return ucs;
	}

	if (ucs <= 0x1FFC) {
		if (ucs >= 0x1E00) {
			ret = ucs_table_1E00[ucs - 0x1E00];
			if (ret > 0) return ret;
		}
		return ucs;
	}

	if (ucs <= 0x2133) {
		if (ucs >= 0x2102) {
			ret = ucs_table_2102[ucs - 0x2102];
			if (ret > 0) return ret;
		}
		return ucs;
	}

	if (ucs <= 0x24CF) {
		if (ucs >= 0x24B6) {
			ret = ucs_table_24B6[ucs - 0x24B6];
			if (ret > 0) return ret;
		}
		return ucs;
	}

	if (ucs <= 0x33CE) {
		if (ucs >= 0x33CE) {
			ret = ucs_table_33CE[ucs - 0x33CE];
			if (ret > 0) return ret;
		}
		return ucs;
	}

	if (ucs <= 0xFF3A) {
		if (ucs >= 0xFF21) {
			ret = ucs_table_FF21[ucs - 0xFF21];
			if (ret > 0) return ret;
		}
		return ucs;
	}

	return ucs;
}

static int 
Toupper(
	int ucs)
{
	int i;
	static unsigned short *table = NULL;

	if (!table) {
		table = (unsigned short*) malloc(
			sizeof(unsigned short) * 0x10000);
		for (i = 0; i < 0x10000; i++) {
			table[i] = (unsigned short) i;
		}	
		for (i = 0; i < 0x10000; i++) {
			int l;
			l = Tolower(i);			
			if (l != i) table[l] = (unsigned short) i;
		}	

	}
	if (ucs >= 0x10000 || ucs < 0) return ucs;
	return table[ucs];
}


/*** converts the first char UTF-8 string to an Unicode value ***/
/*** returns the byte length of the converted UTF-8 char **/
/*** returns -1 if the UTF-8 string is not valid ***/
int
fl_utf2ucs(
        const unsigned char     *buf,
        int                     len,
        unsigned int          	*ucs)
{
      if (buf[0] & 0x80) {
        if (buf[0] & 0x40) {
          if (buf[0] & 0x20) {
            if (buf[0] & 0x10) {
              if (buf[0] & 0x08) {
                if (buf[0] & 0x04) {
                  if (buf[0] & 0x02) {
                        /* bad UTF-8 string */
                  } else {
                        /* 0x04000000 - 0x7FFFFFFF */
                  }
                } else if (len > 4) {
                  /* 0x00200000 - 0x03FFFFFF */
                  *ucs =  ((buf[0] & ~0xF8) << 24) +
                          ((buf[1] & ~0x80) << 18) +
                          ((buf[2] & ~0x80) << 12) +
                          ((buf[3] & ~0x80) << 6) +
                           (buf[4] & ~0x80);
                  if (*ucs > 0x001FFFFF && *ucs < 0x01000000) return 5;
                }
              } else if (len > 3) {
                /* 0x00010000 - 0x001FFFFF */
                *ucs =  ((buf[0] & ~0xF0) << 18) +
                        ((buf[1] & ~0x80) << 12) +
                        ((buf[2] & ~0x80) << 6) +
                         (buf[3] & ~0x80);
                if (*ucs > 0x0000FFFF) return 4;
              }
            } else if (len > 2) {
              /* 0x00000800 - 0x0000FFFF */
              *ucs =  ((buf[0] & ~0xE0) << 12) +
                      ((buf[1] & ~0x80) << 6) +
                       (buf[2] & ~0x80);
              if (*ucs > 0x000007FF) return 3;
            }
          } else if (len > 1) {
            /* 0x00000080 - 0x000007FF */
            *ucs = ((buf[0] & ~0xC0) << 6) +
                    (buf[1] & ~0x80);
            if (*ucs > 0x0000007F) return 2;
          }
        }
      } else if (len > 0) {
        /* 0x00000000 - 0x0000007F */
        *ucs = buf[0];
        return 1;
      }

      *ucs = (unsigned int) '?'; /* bad utf-8 string */
      return -1;
}

/*** converts an Unicode value to an UTF-8 string  ***/
/*** NOTE : the buffer (buf) must be at least 5 bytes long !!!  ***/
int 
fl_ucs2utf(
	unsigned int 	ucs, 
	char 		*buf)
{
	if (ucs < 0x000080) {
		buf[0] = ucs;
		return 1;
	} else if (ucs < 0x000800) {
		buf[0] = 0xC0 | (ucs >> 6);
		buf[1] = 0x80 | (ucs & 0x3F);
		return 2;
	} else if (ucs < 0x010000) { 
		buf[0] = 0xE0 | (ucs >> 12);
		buf[1] = 0x80 | ((ucs >> 6) & 0x3F);
		buf[2] = 0x80 | (ucs & 0x3F);
		return 3;
	} else if (ucs < 0x00200000) {
		buf[0] = 0xF0 | (ucs >> 18);
		buf[1] = 0x80 | ((ucs >> 12) & 0x3F);
		buf[2] = 0x80 | ((ucs >> 6) & 0x3F);
		buf[3] = 0x80 | (ucs & 0x3F);
		return 4;
	} else if (ucs < 0x01000000) {
		buf[0] = 0xF8 | (ucs >> 24);
		buf[1] = 0x80 | ((ucs >> 18) & 0x3F);
		buf[2] = 0x80 | ((ucs >> 12) & 0x3F);
		buf[3] = 0x80 | ((ucs >> 6) & 0x3F);
		buf[4] = 0x80 | (ucs & 0x3F);
		return 5;
	}
	buf[0] = '?';
	return -1;
}

/*** returns the byte length of the first UTF-8 char ***/
/*** (returns -1 if not valid) ***/
int
fl_utflen(
        const unsigned char     *buf,
        int                     len)
{
	unsigned int ucs;
	return fl_utf2ucs(buf, len, &ucs);
}

/**** returns the number of Unicode chars in the UTF-8 string ***/
int 
fl_utf_nb_char(
	const unsigned char 	*buf, 
	int 			len)
{
	int i = 0;
	int nbc = 0;
	while (i < len) {
		int cl = fl_utflen(buf + i, len - i);
		if (cl < 1) cl = 1;
		nbc++;
		i += cl;
	}
	return nbc;
}

/*
 * compare only the first n bytes
 * return 0 if the strings are equal;
 * return 1 if s1 is greater than s2
 * return -1 if s1 is less than s2
 */
int fl_utf_strncasecmp(const char *s1, const char *s2, int n)
{
        int i;
        int s1_l;
        int s2_l;

        s1_l = 0;
        while (s1_l < n && s1[s1_l]) s1_l++;
        s2_l = 0;
        while (s2_l < n && s2[s2_l]) s2_l++;

        if (s1_l < s2_l) {
                return -1;
        } else if (s1_l > s2_l) {
                return 1;
        }
        for (i = 0; i < n;) {
                int l1, l2;
                unsigned int u1, u2;
                int res;

                l1 = fl_utf2ucs((unsigned char*)s1 + i, n - i, &u1);
                l2 = fl_utf2ucs((unsigned char*)s2 + i, n - i, &u2);
                if (l1 - l2 != 0) return l1 - l2;
                res = Tolower(u1) - Tolower(u2);
                if (res != 0) return res;
                if (l1 < 1) {
                        i += 1;
                } else {
                        i += l1;
                }
        }
        return 0;
}

/*
 * return 0 if the strings are equal;
 * return 1 if s1 is greater than s2
 * return -1 if s1 is less than s2
 */
int fl_utf_strcasecmp(const char *s1, const char *s2)
{
	int s1_l = strlen(s1);
	int s2_l = strlen(s2);

        if (s1_l < s2_l) {
                return -1;
        } else if (s1_l > s2_l) {
                return 1;
	}	
	return fl_utf_strncasecmp(s1, s2, s1_l);	
}

/*
 * return the Unicode lower case value of ucs
 */
int fl_tolower(unsigned int ucs) 
{
	return Tolower(ucs);
}

/*
 * return the Unicode upper case value of ucs
 */
int fl_toupper(unsigned int ucs)
{
	return Toupper(ucs);
}

/*
 * converts the str string to the lower case equivalent into buf.
 * Warning: to be safe buf length must be at least 3 * len
 */
int fl_utf_tolower(const unsigned char *str, int len, char *buf)
{
	int i;
	int l = 0;
        for (i = 0; i < len;) {
                int l1, l2;
                unsigned int u1;

                l1 = fl_utf2ucs((unsigned char*)str + i, len - i, &u1);
                l2 = fl_ucs2utf((unsigned int) Tolower(u1), buf + l);
                if (l1 < 1) {
                        i += 1;
                } else {
                        i += l1;
		}
                if (l2 < 1) {
                        l += 1;
                } else {
                        l += l2;
		}

	}
	return l;
}

/*
 * converts the str string to the upper case equivalent into buf.
 * Warning: to be safe buf length must be at least 3 * len
 */
int fl_utf_toupper(const unsigned char *str, int len, char *buf)
{
	int i;
	int l = 0;
        for (i = 0; i < len;) {
                int l1, l2;
                unsigned int u1;

                l1 = fl_utf2ucs((unsigned char*)str + i, len - i, &u1);
                l2 = fl_ucs2utf((unsigned int) Toupper(u1), buf + l);
                if (l1 < 1) {
                        i += 1;
                } else {
                        i += l1;
		}
                if (l2 < 1) {
                        l += 1;
                } else {
                        l += l2;
		}
	}
	return l;
}

 
/*
 * converts a UTF-8 str to unicode
 * Warning: buf must a least len long
 */
int fl_utf2unicode(const unsigned char *str, int len, unsigned short *buf)
{
	int i;
	int l = 0;
        for (i = 0; i < len;) {
                unsigned int u1;
		int l1;

                l1 = fl_utf2ucs((unsigned char*)str + i, len - i, &u1);
		buf[l] = (unsigned short) u1;
                if (l1 < 1) {
                        i += 1;
                } else {
                        i += l1;
		}
		l++;

	}
	return l;
}

/*
 * convert Unicode str to UTF-8
 * Warning: buf must be at least 3 * len long
 */
int fl_unicode2utf(const unsigned short *str, int len, char *buf)
{
	int i;
	int l = 0;
        for (i = 0; i < len; i++) {
		int l1;
		l1 = fl_ucs2utf((unsigned int) str[i], buf + l);
                if (l1 < 1) {
                        l += 1;
                } else {
                        l += l1;
		}

	}
	return l;
}

/*
 * convert UTF-8 str to latin1
 * Warning: buf must be at least len long
 */
int fl_utf2latin1(const unsigned char *str, int len, char *buf)
{
	int i;
	int l = 0;
        for (i = 0; i < len;) {
                unsigned int u1;
		int l1;

                l1 = fl_utf2ucs((unsigned char*)str + i, len - i, &u1);
		if (u1 > 0xFF) u1 = '?';
		buf[l] = (char) u1;
                if (l1 < 1) {
                        i += 1;
                } else {
                        i += l1;
		}
		l++;

	}
	return l;
}

/*
 * convert latin1 str to UTF-8
 * Warning: buf must be at least 2 * len long
 */
int fl_latin12utf(const unsigned char *str, int len, char *buf)
{
	int i;
	int l = 0;
        for (i = 0; i < len; i++) {
		int l1;
		l1 = fl_ucs2utf((unsigned int) str[i], buf + l);
                if (l1 < 1) {
                        l += 1;
                } else {
                        l += l1;
		}

	}
	return l;
}

unsigned int fl_nonspacing(unsigned int ucs)
{
        if (ucs <= 0x0361) {
                if (ucs >= 0x0300) return ucs_table_0300[ucs - 0x0300];
                return 0;
        }

        if (ucs <= 0x0486) {
                if (ucs >= 0x0483) return ucs_table_0483[ucs - 0x0483];
                return 0;
        }

        if (ucs <= 0x05C4) {
                if (ucs >= 0x0591) return ucs_table_0591[ucs - 0x0591];
                return 0;
        }

        if (ucs <= 0x06ED) {
                if (ucs >= 0x064B) return ucs_table_064B[ucs - 0x064B];
                return 0;
        }

        if (ucs <= 0x0D4D) {
                if (ucs >= 0x0901) return ucs_table_0901[ucs - 0x0901];
                return 0;
        }

        if (ucs <= 0x0FB9) {
                if (ucs >= 0x0E31) return ucs_table_0E31[ucs - 0x0E31];
                return 0;
        }

        if (ucs <= 0x20E1) {
                if (ucs >= 0x20D0) return ucs_table_20D0[ucs - 0x20D0];
                return 0;
        }

        if (ucs <= 0x309A) {
                if (ucs >= 0x302A) return ucs_table_302A[ucs - 0x302A];
                return 0;
        }

        if (ucs <= 0xFB1E) {
                if (ucs >= 0xFB1E) return ucs_table_FB1E[ucs - 0xFB1E];
                return 0;
        }

        if (ucs <= 0xFE23) {
                if (ucs >= 0xFE20) return ucs_table_FE20[ucs - 0xFE20];
                return 0;
        }

        return 0;
}

char * fl_utf2mbcs(const char *s)
{
	if (!s) return NULL;
#ifdef WIN32
	int l = strlen(s);
	static char *buf = NULL;
	
	unsigned short* wbuf = (unsigned short*)malloc((l+6) * sizeof(short));
	l = fl_utf2unicode((unsigned char*)s, l, wbuf);
	wbuf[l] = 0;
	buf = (char*)realloc(buf, l * 6 + 1);
	l = wcstombs(buf, wbuf, l);
	free(wbuf);
	buf[l] = 0;
	return buf;
#else
	return (char*) s;
#endif
}

char * fl_mbcs2utf(const char *s)
{
	if (!s) return NULL;
#ifdef WIN32
	int l = strlen(s);
	static char *buf = NULL;
	
	unsigned short* wbuf = (unsigned short*)malloc(
		(l * 6 +6) * sizeof(short));
	l = mbstowcs(wbuf, s, l);
	buf = (char*)realloc(buf, l * 6 + 1);
	l = fl_unicode2utf(wbuf, l, buf);
	free(wbuf);
	buf[l] = 0;
	return buf;
#else
	return (char*) s;
#endif
}

int fl_is_nt4(void)
{
#ifdef WIN32
	static int ret = -1;
	if (ret >= 0) return ret;
	OSVERSIONINFO o;
	o.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&o);
	if (o.dwPlatformId == VER_PLATFORM_WIN32_NT &&
		o.dwMajorVersion >= 4)
	{
		ret = (int) o.dwMajorVersion;
	} else {
		ret = 0;
	}
	return ret;
#else
	return 0;
#endif
}

#ifdef WIN32
static unsigned short *wbuf = NULL;
static unsigned short *wbuf1 = NULL;
#endif 

char *fl_getenv(const char* v)
{
#ifdef WIN32
	int l = strlen(v);
	static unsigned short* wbuf = NULL;
	wbuf = (unsigned short*)realloc(wbuf, sizeof(short) * (l+1));
	wbuf[fl_utf2unicode((const unsigned char*)v, l, wbuf)] = 0;
	unsigned short *ret = _wgetenv(wbuf);
	static char *buf = NULL;
	if (ret) {
		l = wcslen(ret);
		buf = (char*) realloc(buf, l);
		buf[fl_unicode2utf(ret, l, buf)] = 0;
		return buf;
	} else {
		return NULL;
	}
#else 
	return getenv(v);
#endif
}

int fl_open(const char* f, int oflags, int pmode)
{
#ifdef WIN32
	if (fl_is_nt4()) {
		int l = strlen(f);
		wbuf = (unsigned short*)realloc(wbuf, sizeof(short) * (l+1));
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0;
		if (pmode == -1) return _wopen(wbuf, oflags);
		else return _wopen(wbuf, oflags, pmode);
	} else {
		if (pmode == -1) return _open(fl_utf2mbcs(f), oflags);
		else return _open(fl_utf2mbcs(f), oflags, pmode);
	}
#else
	if (pmode == -1) return open(f, oflags);
	else return open(f, oflags, pmode);
#endif
}

FILE *fl_fopen(const char* f, const char *mode)
{
#ifdef WIN32
	if (fl_is_nt4()) {
		int l = strlen(f);
		wbuf = (unsigned short*)realloc(wbuf, sizeof(short) * (l+1));
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0;
		l = strlen(mode);
		wbuf1 = (unsigned short*)realloc(wbuf1, sizeof(short) * (l+1));
		wbuf1[fl_utf2unicode((const unsigned char*)mode, l, wbuf1)] = 0;
		return _wfopen(wbuf, wbuf1);
	} else {
		return fopen(fl_utf2mbcs(f), mode);
	}
#else
	return fopen(f, mode);
#endif
}

int fl_chmod(const char* f, int mode)
{
#ifdef WIN32
	if (fl_is_nt4()) {
		int l = strlen(f);
		wbuf = (unsigned short*)realloc(wbuf, sizeof(short) * (l+1));
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0;
		return _wchmod(wbuf, mode);
	} else {
		return _chmod(fl_utf2mbcs(f), mode);
	}
#else
	return chmod(f, mode);
#endif
}

int fl_access(const char* f, int mode)
{
#ifdef WIN32
	if (fl_is_nt4()) {
		int l = strlen(f);
		wbuf = (unsigned short*)realloc(wbuf, sizeof(short) * (l+1));
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0;
		return _waccess(wbuf, mode);
	} else {
		return _access(fl_utf2mbcs(f), mode);
	}
#else
	return access(f, mode);
#endif
}

int fl_stat(const char* f, struct stat *b)
{
#ifdef WIN32
	if (fl_is_nt4()) {
		int l = strlen(f);
		wbuf = (unsigned short*)realloc(wbuf, sizeof(short) * (l+1));
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0;
		return _wstat(wbuf, (struct _stat*)b);
	} else {
		return _stat(fl_utf2mbcs(f), (struct _stat*)b);
	}
#else
	return stat(f, b);
#endif
}

char *fl_getcwd(char* b, int l)
{
	if (b == NULL) {
		b = (char*) malloc(l+1);
	}
#ifdef WIN32
	if (fl_is_nt4()) {
		static unsigned short *wbuf = NULL;
		wbuf = (unsigned short*)realloc(wbuf, sizeof(short) * (l+1));
		unsigned short *ret = _wgetcwd(wbuf, l / 5);
		if (ret) {
				l = wcslen(wbuf);
				b[fl_unicode2utf(wbuf, l, b)] = 0;
				return b;
		} else {
			return NULL;
		}
	} else {
		char *ret = _getcwd(b, l / 5);
		if (ret) {
			char *s = fl_mbcs2utf(b);
			l = strlen(s);
			for(int i= 0; i < l; i++) b[i] = s[i];
			b[l] = 0;
			return b;
		} else {
			return NULL;
		}
	}
#else
	return getcwd(b, l);
#endif
}



int fl_unlink(const char* f)
{
#ifdef WIN32
	if (fl_is_nt4()) {
		int l = strlen(f);
		wbuf = (unsigned short*)realloc(wbuf, sizeof(short) * (l+1));
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0;
		return _wunlink(wbuf);
	} else {
		return _unlink(fl_utf2mbcs(f));
	}
#else
	return unlink(f);
#endif
}

int fl_mkdir(const char* f, int mode)
{
#if defined(WIN32) && !defined(__CYGWIN__)
	if (fl_is_nt4()) {
		int l = strlen(f);
		wbuf = (unsigned short*)realloc(wbuf, sizeof(short) * (l+1));
		wbuf[fl_utf2unicode((const unsigned char*)f, l, wbuf)] = 0;
		return _wmkdir(wbuf);
	} else {
		return _mkdir(fl_utf2mbcs(f));
	}
#else
	return mkdir(f, mode);
#endif
}


//
// End of "$Id: $".
//
 
