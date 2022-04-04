/******************************************************************************
 *   "$Id: $"
 *
 *   Xd6XmlUtils definitions for the Fast Light Environement.
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

#ifndef Xd6XmlUtils_h
#define Xd6XmlUtils_h

#include "FL/fl_utf8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int xml2utf(const char *txt, int text_len, char *text_value);
int utf2xml(const char *txt, int txt_len, char *buffer);
int utf72utf8(char *txt, int txt_len);
int utf82utf7(const char *txt, int txt_len, char *buffer);
int latin12xml(const char *txt, int txt_len, char *buffer);
int latin12url(const char *txt, int txt_len, char *buffer);
int url2latin1(char *txt, int txt_len);
int utf82quoted(const char *txt, int txt_len, char *buffer);
int quoted2utf8(char *txt, int txt_len);


#endif

//
// End of "$Id: $".
//

