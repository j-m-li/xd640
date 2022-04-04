/******************************************************************************
              Copyright (c) 2000-2002 by O'ksi'D

Permission to use, copy, modify, distribute, and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and
that both that copyright notice and this permission notice appear
in supporting documentation, and that the name of O'ksi'D
not be used in advertising or publicity pertaining to distribution
of the software without specific, written prior permission.
O'ksi'D makes no representations about the suitability of
this software for any purpose.  It is provided "as is" without
express or implied warranty.

O'ksi'D DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
IN NO EVENT SHALL O'ksi'D BE LIABLE FOR ANY SPECIAL, INDIRECT
OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
OR PERFORMANCE OF THIS SOFTWARE.

******************************************************************************/

#include "Xutf8.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#if HAVE_LIBC_ICONV
#include <iconv.h>
#endif
/*
  I haven't found much doc on the web about EUC encodings, so I've used
  GNU libiconv source code as a reference.
  http://clisp.cons.org/~haible/packages-libiconv.html
*/

#if !HAVE_X11_UTF8

#define RET_ILSEQ -1
#define RET_TOOFEW(x) (-10 - x)
#define RET_TOOSMALL -2
#define conv_t void*
#define ucs4_t unsigned int
typedef struct {
        unsigned short indx;
        unsigned short used;
} Summary16;

#include "lcUniConv/big5.h"
#include "lcUniConv/gb2312.h"
#include "lcUniConv/jisx0201.h"
#include "lcUniConv/jisx0208.h"
#include "lcUniConv/jisx0212.h"
#include "lcUniConv/ksc5601.h"

int 
XConvertEucTwToUtf8(
	char*		buffer_return,
	int		len)
{
 /* FIXME */
#if HAVE_LIBC_ICONV
	iconv_t cd;
	int cdl;
#endif
	int i = 0, l = 0;
	char *buf;

	buf = (char*) malloc(len);
	memcpy(buf, buffer_return, len);

#if HAVE_LIBC_ICONV
	l = cdl = len;
	cd = iconv_open("EUC-TW", "UTF-8");
	iconv(cd, &buf, &len, &buffer_return, &cdl);
	iconv_close(cd);
	l -= cdl;	
#else
	while (i < len) {
		unsigned int ucs;
		unsigned char c, c1;
		c = (unsigned char) buf[i];
		if (c < 0x80) {
			ucs = c;	
			i++;
		} else if (c >= 0xa1 && c < 0xff && i - len > 1 ) {
			unsigned char b[2];
			b[0] = (unsigned char) c - 0x80;
			b[1] = (unsigned char) buf[i + 1] - 0x80;
			ucs = ' '; i += 2;
		} else if (c == 0x8e &&  i - len > 3) {
			unsigned char b[2];
			unsigned char c1 =  buf[i + 1];
			unsigned char c2 =  buf[i + 2];
			unsigned char c3 =  buf[i + 3];
			b[0] = (unsigned char)  buf[i + 2] - 0x80;
			b[1] = (unsigned char)  buf[i + 3] - 0x80;
			if (c1 >= 0xa1 && c1 <= 0xb0) {
				if (c2 >= 0xa1 && c2 < 0xff && c3 >= 0xa1 &&
					c3 < 0xff)
				{
					ucs = ' '; i += 4;
				} else {
					ucs = '?'; i++;
				}
			} else {
				ucs = '?'; i++;
			}
		} else {
			ucs = '?';
			i++;
		}
		l += XConvertUcsToUtf8(ucs, buffer_return + l);
	}
#endif
	free(buf);
	return l;
}

int 
XConvertEucKrToUtf8(
	char*		buffer_return,
	int		len)
{
	int i = 0, l = 0;
	char *buf;

	buf = (char*) malloc(len);
	memcpy(buf, buffer_return, len);

	while (i < len) {
		unsigned int ucs;
		unsigned char c, c1;
		c = (unsigned char) buf[i];
		if (c < 0x80) {
			ucs = c;	
			i++;
		} else if (c >= 0xA1 && c < 0xFF && len - i > 1) {
			c1 = (unsigned char) buf[i + 1];
			if (c1 >= 0xa1 && c1 < 0xff) {
				unsigned char b[2];
				b[0] = c - 0x80;
				b[1] = c1 - 0x80;
				if (ksc5601_mbtowc(NULL, &ucs, b, 2) < 1) {
					ucs = '?';
				}
			} else {
				ucs = '?';
			}
			i += 2;
		} else {
			ucs = '?';
			i++;
		}
		l += XConvertUcsToUtf8(ucs, buffer_return + l);
	}
	free(buf);
	return l;
}

int 
XConvertBig5ToUtf8(
	char*		buffer_return,
	int		len)
{
	int i = 0, l = 0;
	char *buf;

	buf = (char*) malloc(len);
	memcpy(buf, buffer_return, len);

	while (i < len) {
		unsigned int ucs;
		unsigned char b[2];
		b[0] = (unsigned char) buf[i];
		b[1] = (unsigned char) buf[i + 1];
		if (big5_mbtowc(NULL, &ucs, b, 2) == 2) {
			i += 2;
		} else {
			ucs = '?';
			i++;
		}
		l += XConvertUcsToUtf8(ucs, buffer_return + l);
	}
	free(buf);
	return l;
}

int 
XConvertEucCnToUtf8(
	char*		buffer_return,
	int		len)
{
	int i = 0, l = 0;
	char *buf;

	buf = (char*) malloc(len);
	memcpy(buf, buffer_return, len);

	while (i < len) {
		unsigned int ucs;
		unsigned char c, c1;
		unsigned int wc;
		c = (unsigned char) buf[i];
		if (c < 0x80) {
			ucs = c;	
			i++;
		} else if (c >= 0xA1 && c < 0xFF && len - i > 1) {
			c1 = (unsigned char) buf[i + 1];
			if (c1 >= 0xa1 && c1 < 0xff) {	
				unsigned char b[2];
				b[0] = (unsigned char) c;
				b[1] = (unsigned char) c1;
				if (gb2312_mbtowc(NULL, &ucs, b, 2) < 1) {
					 ucs = '?';
				}	
			} else {
				ucs = '?';
			}
			i += 2;
		} else {
			ucs = '?';
			i++;
		}
		l += XConvertUcsToUtf8(ucs, buffer_return + l);
	}
	free(buf);
	return l;
}

int 
XConvertEucJpToUtf8(
	char*		buffer_return,
	int		len)
{
	int i = 0, l = 0;
	char *buf;

	buf = (char*) malloc(len);
	memcpy(buf, buffer_return, len);

	while (i < len) {
		unsigned int ucs;
		unsigned char c, c1;
		unsigned int wc;
		c = (unsigned char) buf[i];
		if (c < 0x80) {
			ucs = c;	
			i++;
		} else if (c >= 0xA1 && c < 0xFF && len - i > 1) {
			c1 = (unsigned char) buf[i + 1];		
			if (c < 0xF5 && c1 >= 0xa1 && c1 <= 0xff) {
				unsigned char b[2];
				b[0] = c - 0x80;
				b[1] = c1 - 0x80;
				if (jisx0208_mbtowc(NULL, &ucs, b, 2) < 1) { 
					ucs = '?';
				}
			} else if (c1 >= 0xA1 && c1 < 0xFF) {
				ucs = 0xE000 + 94 * (c - 0xF5) + (c1 - 0xA1);
			} else {
				ucs = '?';
			}
			i += 2;
		} else if (c == 0x8E && len - i > 1) {
			c1 = (unsigned char) buf[i + 1];		
			if (c1 >= 0xa1 && c1 <= 0xe0) {
				if (jisx0201_mbtowc(NULL, &ucs, &c1, 1) != 1) {
					ucs = '?';
				}
			} else {
				ucs = '?';
			}
			i += 2;
		} else if (c == 0x8F && len - i > 2) {
			c = (unsigned char) buf[i + 1];		
			c1 = (unsigned char) buf[i + 2];	
			if (c >= 0xa1 && c < 0xff) {
				if (c < 0xf5 && c1 >= 0xa1 && c1 < 0xff) {
					unsigned char b[2];
					b[0] = c - 0x80;
					b[1] = c1 - 0x80;
					if (jisx0212_mbtowc(NULL, &ucs, b, 2) 
						< 1) 
					{
						ucs = '?';
					}
				} else {
					ucs = '?';
				}
			} else {
				if (c1 >= 0xa1 && c1 < 0xff) {
					ucs = 0xe3ac + 94 * (c - 0xF5) + 
						(c1 - 0xA1);
				} else {
					ucs = '?';
				}
			}
			i += 3;
		} else {
			ucs = '?';
			i++;
		}
		l += XConvertUcsToUtf8(ucs, buffer_return + l);
	}
	free(buf);
	return l;
}

int
XConvertEucToUtf8(
	const char*	locale,
	char*		buffer_return, 
	int		len, 
	int		bytes_buffer)
{
	if (!locale/* || strstr(locale, "UTF") || strstr(locale, "utf")*/) {
		return len;
	}

	if (strstr(locale, "JP")) {	
		return XConvertEucJpToUtf8(buffer_return, len);
	} else if (strstr(locale, "Big5") || strstr(locale, "big5")) { // BIG5
		return XConvertBig5ToUtf8(buffer_return, len);
	} else if (strstr(locale, "CN")) { 
		return XConvertEucCnToUtf8(buffer_return, len);
	} else if (strstr(locale, "KR")) { 
		return XConvertEucKrToUtf8(buffer_return, len);
	} else if (strstr(locale, "TW")) { 
		return XConvertEucTwToUtf8(buffer_return, len);
	}
	return len;
}
#endif


int
XUtf8LookupString(
    XIC                 ic,
    XKeyPressedEvent*   event,
    char*               buffer_return,
    int                 bytes_buffer,
    KeySym*             keysym,
    Status*             status_return)
{
#if HAVE_X11_UTF8
	return Xutf8LookupString(ic, event, buffer_return, bytes_buffer,
		keysym, status_return);
#else 
        long ucs = -1;
        int len;
        len = XmbLookupString(ic, event, buffer_return, bytes_buffer / 2,
                         keysym, status_return);
	if (*keysym > 0 && *keysym < 0x100) {
		ucs = *keysym;
	} else  if (((*keysym >= 0x100 && *keysym <= 0xf000) ||
                              (*keysym & 0xff000000U) == 0x01000000))
        {
                ucs = XKeysymToUcs(*keysym);
        } else {
                ucs = -2;
        }
        //if (ucs == -1) len = 0;
        if (ucs > 0) {
                len = XConvertUcsToUtf8(ucs, (char *)buffer_return);
        } else if (len > 0) {
		len = XConvertEucToUtf8(XLocaleOfIM(XIMOfIC(ic)), 
			buffer_return, len, bytes_buffer);	
	}
        return len;
#endif
}


