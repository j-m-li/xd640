/*
 * 	taken from mutt-1.0 (http://www.mutt.org/) modified by O'ksi'D
 *
 * Copyright (C) 1996-2000 Michael R. Elkins <me@cs.hmc.edu>
 * 
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */ 

#include "Xd6Std.h"
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#define base64val(c) Index_64[(unsigned int)(c)]

static int Index_64[128] = {
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,62, -1,-1,-1,63,
    52,53,54,55, 56,57,58,59, 60,61,-1,-1, -1,-1,-1,-1,
    -1, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,-1, -1,-1,-1,-1,
    -1,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
    41,42,43,44, 45,46,47,48, 49,50,51,-1, -1,-1,-1,-1
};


void Xd6Decode_base64 (FILE *fpin, FILE *fout)
{
  char buf[5];
  int c1, c2, c3, c4, ch, i;

  buf[4] = 0;


  while (1)
  {
    for (i = 0 ; i < 4; )
    {
      if ((ch = fgetc (fpin)) == EOF)
	return;
      if (!isspace (ch))
	buf[i++] = ch;
    }
    if (i != 4)
      return; /* didn't get a multiple of four chars! */

    c1 = base64val (buf[0]);
    c2 = base64val (buf[1]);
    ch = (c1 << 2) | (c2 >> 4);

    putc(ch, fout);

    if (buf[2] == '=')
      break;
    c3 = base64val (buf[2]);
    ch = ((c2 & 0xf) << 4) | (c3 >> 2);

    putc(ch, fout);

    if (buf[3] == '=') break;
    c4 = base64val (buf[3]);
    ch = ((c3 & 0x3) << 6) | c4;

    putc(ch, fout);

  }
}

/*
 *      taken from mutt-1.0 (http://www.mutt.org/) modified by O'ksi'D
 *
 * Copyright (C) 1996-2000 Michael R. Elkins <me@cs.hmc.edu>
 * 
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */ 


static char B64Chars[64] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
  'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
  'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
  't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', '+', '/'
};



static char b64_buffer[3];
static short b64_num;
static short b64_linelen;

static void b64_flush(FILE *fout)
{
  short i;

  if(!b64_num)
    return;

  if(b64_linelen >= 72)
  {
    putc('\n', fout);
    b64_linelen = 0;
  }
  
  for(i = b64_num; i < 3; i++)
    b64_buffer[i] = '\0';
  
  putc(B64Chars[(b64_buffer[0] >> 2) & 0x3f], fout); 
  b64_linelen++;
  putc(B64Chars[((b64_buffer[0] & 0x3) << 4) | 
	           ((b64_buffer[1] >> 4) & 0xf) ], fout);
  b64_linelen++;
  
  if(b64_num > 1)
  {
    putc(B64Chars[((b64_buffer[1] & 0xf) << 2) | 
		     ((b64_buffer[2] >> 6) & 0x3) ], fout);
    b64_linelen++;
    if(b64_num > 2)
    {
      putc(B64Chars[b64_buffer[2] & 0x3f], fout);
      b64_linelen++;
    }
  }
  
  while(b64_linelen % 4)
  {
    putc('=', fout);
    b64_linelen++;
  }
  
  b64_num = 0;
}
    
  
static void b64_putc(char c, FILE *fout)
{
  if(b64_num == 3)
    b64_flush(fout);
  
  b64_buffer[b64_num++] = c;
}
  
  
void Xd6Encode_base64 (FILE * fin, FILE *fout)
{
  int ch, ch1 = EOF;
  
  b64_num = b64_linelen = 0;
  
  while((ch = fgetc(fin)) != EOF)
  {
    b64_putc(ch, fout);
    ch1 = ch;
  }
  b64_flush(fout);
  putc('\n', fout);
}

