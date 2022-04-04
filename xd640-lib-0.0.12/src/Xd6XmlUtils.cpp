/******************************************************************************
 *   "$Id: $"
 *
 *   Xd6XmlUtils for the Fast Light Environement.
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

#include "Xd6XmlUtils.h"
#include "Xd6Std.h"
#include <string.h>
#include <ctype.h>

extern "C" {

char is_xd6(void)
{
	return 1;
}

}

static char *hex = "0123456789ABCDEF";

static char *base64 = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int index64[128] = {
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,62, -1,-1,-1,63,
    52,53,54,55, 56,57,58,59, 60,61,-1,-1, -1,-1,-1,-1,
    -1, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,-1, -1,-1,-1,-1,
    -1,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
    41,42,43,44, 45,46,47,48, 49,50,51,-1, -1,-1,-1,-1
};

//
// converts &#1234; and &lt;... to utf-8
//
int xml2utf(const char *txt, int txt_len, char *text_value)
{
	int i = 0;
	int n = 0;
        while (i < txt_len) {
                if (txt[i] == '&' && (i + 3) < txt_len) {
                        int ii = 1;
                        while (ii < 15 && txt[i + ii] &&
                                (i + ii) < txt_len &&
                                txt[i + ii] != ';')
                        {
                                ii++;
                        }
                        if (txt[i + ii] != ';' || ii < 3) {
                                text_value[n] = txt[i];
                                i++;
                                n++;
                        } else if (txt[i + 1] == '#') {
                                int l;
                                unsigned int ucs;

				if (txt[i + 2] == 'x' || txt[i + 2] == 'X') {
					ucs = (unsigned int)
						strtol(txt + i + 3, NULL, 16);	
				} else {
					ucs = (unsigned int) atol(txt + i + 2);
				}
                                l = fl_ucs2utf(ucs, text_value + n);
                                if (l < 1) l = 1;
                                i += ii + 1;
                                n += l;
                        } else {
                                const char *off = txt + i;
                                if (!strncmp(off, "&lt;", 4)) {
                                        text_value[n] = '<';
                                } else if (!strncmp(off, "&gt;", 4)) {
                                        text_value[n] = '>';
                                } else if (!strncmp(off, "&amp;", 5)) {
                                        text_value[n] = '&';
                                } else if (!strncmp(off, "&quot;", 6)) {
                                        text_value[n] = '"';
                                } else if (!strncmp(off, "&apos;", 6)) {
                                        text_value[n] = '\'';
                                } else if (!strncmp(off, "&nbsp;", 6)) {
                                        fl_ucs2utf(160, text_value + n);
                                        n++; // "&#160;" utf8 char is 2 bytes
                                } else {
                                        text_value[n] = '?';
                                }
                                i += ii + 1;
                                n++;
                        }
                } else {
                        text_value[n] = txt[i];
                        i++;
                        n++;
                }
	}
	text_value[n] = '\0';
	return n;
}

//
// converts utf-8 to &#1234; and &lt; ...
// WARNING : *buffer must be 6 * txt_len + 1 long !!!!
//			     ===============	
//
int utf2xml(const char *txt, int txt_len, char *buffer)
{
	int i = 0;
	int n = 0;
	char *text_value = buffer;

	if (!txt) {
		buffer[0] = '\0';
		return 0;
	}
	while (i < txt_len) {
		if (txt[i] >= 0x7F || (txt[i] < 0x20 && txt[i] != '\t' &&
			   txt[i] != '\n' && txt[i] != '\r')) 
		{
			unsigned int ucs;
			int l;
	
			l  = fl_utf2ucs((unsigned char*)txt + i, 
					txt_len - i, &ucs);
			if (l < 1) l = 1;
			if (ucs < 10000) {
				// (utf8 char is 3 or less bytes long)
				// "&#0;" -> "&#9999;"			diff
				// 1 : "&#0;"      -> "&#127;" 		  5  **
				// 2 : "&#128;"    -> "&#2047;"		  6
				// 3 : "&#2048;"   -> "&#9999;"		  5
				//
				// **) 1 byte utf8 char is the critical value
				// for the destination buffer length, => buffer
				// must be 6* text length long.

				snprintf(text_value + n, 13, "&#%d;", ucs);
			} else {
				// (utf8 char is 3 or more bytes long)
				// "&#x2710;" -> "&#xFFFFFFFF;"         diff
				// 3 : "&#x2710;"     -> "&#xFFFF;"       5
				// 4 : "&#x10000;"    -> "&#x1FFFFF;"     6
				// 5 : "&#x200000;"   -> "&#x3FFFFFF;"    5
				// 6 : "&#x4000000:"  -> "&#x7FFFFFFF;"   5
				snprintf(text_value + n, 13, "&#x%x;", ucs);
			}
			n += strlen(text_value + n);
			i += l;
			continue;
		} else if (txt[i] == '\'') {
			// critical length for the destination buffer !
			memcpy(text_value + n, "&apos;", 6);
			n += 6;
		} else if (txt[i] == '"') {
			// critical length for the destination buffer !
			memcpy(text_value + n, "&quot;", 6);
			n += 6;
		} else if (txt[i] == '<') {
			memcpy(text_value + n, "&lt;", 4);
			n += 4;
		} else if (txt[i] == '>') {
			memcpy(text_value + n, "&gt;", 4);
			n += 4;
		} else if (txt[i] == '&') {
			memcpy(text_value + n, "&amp;", 5);
			n += 5; 
		} else {
			text_value[n] = txt[i];
			n++;
		}

		i++;
	}
	text_value[n] = '\0';
	return n;
}

// buffer must be txt_len * 3 + 1 long

int latin12url(const char *txt, int txt_len, char *buffer)
{
	int l = 0;
	int i = 0;
	
	while (i < txt_len) {
		if (txt[i] == ' ') {
			buffer[l] = '+';
			l++;
		} else if (txt[i] < 0x2C || txt[i] == '@' || txt[i] > 0x7E ||
			txt[i] == '?' || txt[i] == ':') 
		{
			sprintf(buffer + l, "%%%2X", (unsigned char) txt[i]);
			l += 3;
		} else {
			buffer[l] = txt[i];
			l++;
		}
		i++;
	}
	buffer[l] = '\0';
	return l;
}

int url2latin1(char *txt, int txt_len)
{
	int i = 0;
	int l = 0;

	while (i < txt_len) {
		if (txt[i] == '+') {
			txt[l] = ' ';
			i++;
		} else if (txt[i] == '%' && txt_len - i > 2) {
			char buf[3];
			i++;
			buf[0] = txt[i++];
			buf[1] = txt[i++];
			buf[2] = '\0';
			txt[l] = (unsigned char)strtol(buf, NULL, 16);
		} else {
			txt[l] = txt[i++];
		}
		l++;
	}
	txt[l] = '\0';
	return l;
}

//
// converts utf-8 to &#1234; and &lt; ...
// WARNING : *buffer must be 6 * txt_len + 1 long !!!!
//			     ===============	
//
int latin12xml(const char *txt, int txt_len, char *buffer)
{
	int i = 0;
	int n = 0;
	char *text_value = buffer;

	if (!txt) {
		buffer[0] = '\0';
		return 0;
	}
	while (i < txt_len) {
		if (txt[i] >= 0x7F || (txt[i] < 0x20 && txt[i] != '\t' &&
			   txt[i] != '\n' && txt[i] != '\r')) 
		{
			unsigned int ucs;
			int l;
	
			l = 1;
			ucs = (unsigned char)txt[i];
			if (ucs < 10000) {
				// (utf8 char is 3 or less bytes long)
				// "&#0;" -> "&#9999;"			diff
				// 1 : "&#0;"      -> "&#127;" 		  5  **
				// 2 : "&#128;"    -> "&#2047;"		  6
				// 3 : "&#2048;"   -> "&#9999;"		  5
				//
				// **) 1 byte utf8 char is the critical value
				// for the destination buffer length, => buffer
				// must be 6* text length long.

				snprintf(text_value + n, 13, "&#%d;", ucs);
			} else {
				// (utf8 char is 3 or more bytes long)
				// "&#x2710;" -> "&#xFFFFFFFF;"         diff
				// 3 : "&#x2710;"     -> "&#xFFFF;"       5
				// 4 : "&#x10000;"    -> "&#x1FFFFF;"     6
				// 5 : "&#x200000;"   -> "&#x3FFFFFF;"    5
				// 6 : "&#x4000000:"  -> "&#x7FFFFFFF;"   5
				snprintf(text_value + n, 13, "&#x%x;", ucs);
			}
			n += strlen(text_value + n);
			i += l;
			continue;
		} else if (txt[i] == '\'') {
			// critical length for the destination buffer !
			memcpy(text_value + n, "&apos;", 6);
			n += 6;
		} else if (txt[i] == '"') {
			// critical length for the destination buffer !
			memcpy(text_value + n, "&quot;", 6);
			n += 6;
		} else if (txt[i] == '<') {
			memcpy(text_value + n, "&lt;", 4);
			n += 4;
		} else if (txt[i] == '>') {
			memcpy(text_value + n, "&gt;", 4);
			n += 4;
		} else if (txt[i] == '&') {
			memcpy(text_value + n, "&amp;", 5);
			n += 5; 
		} else {
			text_value[n] = txt[i];
			n++;
		}

		i++;
	}
	text_value[n] = '\0';
	return n;
}


int utf72utf8(char *tx, int txt_len)
{
	unsigned char *txt = (unsigned char*) tx;
	int i, n;
	int in_escape = 0;
	char c[4];
	int cn = 0;

	i = 0;
	n = 0;
	
	while (i < txt_len) {
		if (!in_escape && txt[i] < '\x7F' && txt[i] != '+') {
			txt[n++] = txt[i];
		} else if (!in_escape && txt[i] == '+') {
			if (i < txt_len - 1 && txt[i + 1] == '-') {
				txt[n++] = txt[i];
				i++;
			} else {
				in_escape = 1;
				cn = 0;
				c[0] = 0; c[1] = 0; c[2] = 0; c[3] = 0;
			}
		} else if (in_escape && 
			((unsigned char)txt[i] > '\x7F' || 
			index64[txt[i]] == -1)) 
		{
			in_escape = 0;
			if (cn == 0) {
			} else if (cn < 3) {
				txt[n++] = (c[0] << 2) | (c[1] >> 4);
			} else {
				txt[n++] = ((c[1] & 0x0F) << 4) | (c[2] >> 2);
			}
			if (txt[i] != '-') txt[n++] = txt[i];
		} else if (in_escape) {
			c[cn] = index64[txt[i]];
			cn++;
			if (cn == 4) {
				txt[n++] = (c[0] << 2) | (c[1] >> 4);
				txt[n++] = ((c[1] & 0x0F) << 4) | (c[2] >> 2);
				txt[n++] = ((c[2] & 0x03) << 6) | c[3];
				cn = 0;
				c[0] = 0; c[1] = 0; c[2] = 0; c[3] = 0;
			}	
		}
		i++;
	}
	if (in_escape) {
		if (cn == 0) {
		} else if (cn < 3) {
			txt[n++] = (c[0] << 2) | (c[1] >> 4);
		} else {
			txt[n++] = ((c[1] & 0x0F) << 4) | (c[2] >> 2);
		}
	}
	txt[n] = '\0';
	return n;
}

/*
 *  buffer must be 4 * txt_len long
 */
int utf82utf7(const char *txt, int txt_len, char *buffer)
{
	int i, n;
	int in_escape = 0;
	unsigned char cc[4];
	int cn;

	i = 0;
	n = 0;
	cn = 0;	

	while (i < txt_len) {
		unsigned char c = (unsigned char) txt[i];
		if (isalnum(c) || c == '\'' || c == '(' || c == ')' ||
			c == ',' || c == '-' || c == '.' || c == '/' ||
			c == ':' || c == '?')
		{
			if (in_escape) {
				if (cn > 0) {
					buffer[n++] = base64[cc[0]];
					buffer[n++] = base64[cc[1]];
					if (cn > 1) {
						buffer[n++] = base64[cc[2]];
					}
				}
				buffer[n++] = '-';
			}
			buffer[n++] = c;
			in_escape = 0;
		} else {
			if (!in_escape) {
				in_escape = 1;
				buffer[n++] = '+';
				cn = 0;
				
			}
			if (cn == 0) {
				buffer[n] = 0; buffer[n + 1] = 0;
				buffer[n + 2] = 0;buffer[n + 3] = 0;
			}
			if (cn == 0) {
				cc[0] = (c >> 2) & 0x3F;
				cc[1] = (c & 0x03) << 4;
			} else if (cn == 1) {
				cc[1] |= ((c >> 4) & 0x0F);
				cc[2] = ((c & 0x0F) << 2);
			} else {
				cc[2] |= ((c >> 6) & 0x03);
				cc[3] = (c & 0x3F);
			}
			cn++;
			if (cn == 3) {
				buffer[n++] = base64[cc[0]];
				buffer[n++] = base64[cc[1]];
				buffer[n++] = base64[cc[2]];
				buffer[n++] = base64[cc[3]];
				cn = 0;
			} 
		}
		i++;
	}
	if (in_escape) {
		if (cn > 0) {
			buffer[n++] = base64[cc[0]];
			buffer[n++] = base64[cc[1]];
			if (cn > 1) {
				buffer[n++] = base64[cc[2]];
			}
		}
	}
	buffer[n] = '\0';
	return n;
}


int utf82quoted(const char *tx, int txt_len, char *buffer)
{
	const unsigned char *txt = (const unsigned char*) tx;
	int i;
	int n;
	int l;

	i = 0;
	n = 0;
	l = 0;
	while (i < txt_len) {
		unsigned char c = txt[i];
		if (isalnum(c)) {
			buffer[n++] = c;
			l++;
		} else {
			buffer[n++] = '=';
			buffer[n++] = hex[(c & 0xF0) >> 4];
			buffer[n++] = hex[(c & 0x0F)];
			l += 3;
		}
		if (l >= 70) {
			buffer[n++] = '=';
			buffer[n++] = '\n';
			l = 0;
		}


		i++;
	}
	buffer[n] = '\0';
	return n;
}

static unsigned char fromhex(char cc)
{
        unsigned char c = (unsigned char) cc;
        if (c >= 0x41 && c <= 0x46) {
                return c - 0x41 + 10;
        } else {
                return c - 0x30;
        }
}

int quoted2utf8(char *tx, int txt_len)
{
	unsigned char *txt = (unsigned char*) tx;
	int i, n;

	i = 0;
	n = 0;
	while (i < txt_len) {
		if (txt[i] == '=' && i < txt_len - 1) {
			i++;
			if (isxdigit(txt[i]) && i < txt_len - 1) {
				txt[n] = fromhex(txt[i++]) << 4;
				txt[n++] |= fromhex(txt[i]);
			} else {
				txt[n++] = txt[i];
			}		
		} else {
			txt[n++] = txt[i];
		}
		i++;
	}

	txt[n] = '\0';
	return n;
}

//
// End of "$Id: $".
//
