/******************************************************************************
              Copyright 2001 by O'ksi'D

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

        Author: Jean-Marc Lienher ( http://oksid.ch )

******************************************************************************/

#include "headers/case.h"
#include <stdlib.h>


int 
XUtf8Tolower(
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

int 
XUtf8Toupper(
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
			l = XUtf8Tolower(i);			
			if (l != i) table[l] = (unsigned short) i;
		}	

	}
	if (ucs >= 0x10000 || ucs < 0) return ucs;
	return table[ucs];
}

