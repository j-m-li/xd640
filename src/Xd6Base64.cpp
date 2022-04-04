/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2003  O'ksi'D
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

#include "Xd6Std.h"
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

static char buffer[3];
static short linelen;
static short num;
static char *str = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


void Xd6Decode_base64 (FILE *fpin, FILE *fout)
{
 	signed char buf[5];
  	int c1, c2, c3, c4, ch, i;
  	signed char tbl[256];

	if (!fpin || !fout) return;
  	buf[4] = 0;

  	for (i = 0; i < 256; i++) {
		tbl[i] = -1;
  	}
  	for (i = 0; i < 64; i++) {
		tbl[str[i]] = i;
  	}
  	for (;1;) {
    		for (i = 0 ; i < 4; ) {
      			if ((ch = fgetc(fpin)) == EOF) return;
      			if (!isspace(ch)) buf[i++] = ch;
    		}
    		if (i != 4) return; 

    		c1 = tbl[(unsigned int) buf[0]];
    		c2 = tbl[(unsigned int) buf[1]];
    		ch = (c1 << 2) | (c2 >> 4);

    		putc(ch, fout);

    		if (buf[2] == '=') break;

    		c3 = tbl[(unsigned int) buf[2]];
    		ch = ((c2 & 0xf) << 4) | (c3 >> 2);

    		putc(ch, fout);

    		if (buf[3] == '=') break;
    		c4 = tbl[(unsigned int) buf[3]];
    		ch = ((c3 & 0x3) << 6) | c4;

    		putc(ch, fout);
  	}
}

static void flush(FILE *fout)
{
  	int i;

  	if(num < 1) return;

  	if(linelen >= 72) {
    		putc('\n', fout);
    		linelen = 0;
  	}
  
  	for(i = num; i < 3; i++) buffer[i] = '\0';
  
  	putc(str[(*buffer >> 2) & 0x3f], fout); 
  	linelen++;
  	putc(str[((buffer[0] & 0x3) << 4) | 
	           ((buffer[1] >> 4) & 0xf) ], fout);
  	linelen++;
  
  	if(num > 1) {
    		putc(str[((buffer[1] & 0x0f) << 2) | 
		     	((buffer[2] >> 6) & 0x03) ], fout);
    		linelen++;
    		if(num > 2) {
      			putc(str[buffer[2] & 0x3f], fout);
      			linelen++;
    		}
  	}
  
  	while(linelen % 4) {
    		putc('=', fout);
    		linelen++;
  	}
  
  	num = 0;
}
    
  
void Xd6Encode_base64 (FILE * fin, FILE *fout)
{
  	int ch;
  
	if (!fin || !fout) return;

  	num = linelen = 0;
  
  	while ((ch = fgetc(fin)) != EOF) {
    		if(num == 3) flush(fout);
    		buffer[num++] = ch;
  	}
  	flush(fout);
  	putc('\n', fout);
}

