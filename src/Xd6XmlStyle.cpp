/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2003  O'ksi'D
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


#include "Xd6XmlStyle.h"
#include "Xd6Std.h"
#include <string.h>
#include <ctype.h>

int Xd6XmlStyle::default_style = FONT_SIZE_3;
int *Xd6XmlStyle::tab_stop = NULL;
int Xd6XmlStyle::nb_tab = 0;
Xd6XmlStl* Xd6XmlStyle::stl = NULL;
Xd6XmlStl Xd6XmlStl::para;
Xd6XmlStl Xd6XmlStl::def;
static Xd6XmlStl* para = NULL;

Xd6XmlStyle::Xd6XmlStyle()
{
	type = 0;
}

static int fromhex(char cc)
{
	unsigned char c = (unsigned char) cc;
	if (c >= 0x41 && c <= 0x46) {
		return c - 0x41 + 10;
	} else if (c >= 0x61 && c <= 0x66) {
		return c - 0x61 + 10;
	} else {
		return c - 0x30;
	}
}

static int get_color(const char *c)
{
	if (!strcasecmp(c, "black")) {
		return COLOR_BLACK;
	} else if (!strcasecmp(c, "silver")) {
		return COLOR_SILVER;
	} else if (!strcasecmp(c, "gray")) {
		return COLOR_GRAY;
	} else if (!strcasecmp(c, "grey")) {
		return COLOR_GRAY;
	} else if (!strcasecmp(c, "white")) {
		return COLOR_WHITE;
	} else if (!strcasecmp(c, "maroon")) {
		return COLOR_MAROON;
	} else if (!strcasecmp(c, "red")) {
		return COLOR_RED;
	} else if (!strcasecmp(c, "purple")) {
		return COLOR_PURPLE;
	} else if (!strcasecmp(c, "fuchsia")) {
		return COLOR_FUCHSIA;
	} else if (!strcasecmp(c, "green")) {
		return COLOR_GREEN;
	} else if (!strcasecmp(c, "lime")) {
		return COLOR_LIME;
	} else if (!strcasecmp(c, "olive")) {
		return COLOR_OLIVE;
	} else if (!strcasecmp(c, "yellow")) {
		return COLOR_YELLOW;
	} else if (!strcasecmp(c, "navy")) {
		return COLOR_NAVY;
	} else if (!strcasecmp(c, "blue")) {
		return COLOR_BLUE;
	} else if (!strcasecmp(c, "teal")) {
		return COLOR_TEAL;
	} else if (!strcasecmp(c, "aqua")) {
		return COLOR_AQUA;
	}

	if (c[0] == '#' && c[1] && c[2] && c[3] && c[4] && c[5]) {
		if (fromhex(c[1]) < 0x8) {
			if (fromhex(c[3]) < 0x8) {
				if (fromhex(c[5]) < 0x8) {
					return COLOR_BLACK; // 000000
				} else if (fromhex(c[5]) < 0xC) {
					return COLOR_NAVY; // 000080
				} else {
					return COLOR_BLUE; // 0000FF
				}
			} else if (fromhex(c[3]) < 0xC) {
				if (fromhex(c[5]) < 0x8) {
					return COLOR_GREEN; // 008000
				} else if (fromhex(c[5]) < 0xC) {
					return COLOR_TEAL; // 008080
				} else {
					return COLOR_AQUA; // 0080FF *
				}
			} else {
				if (fromhex(c[5]) < 0x8) { 
					return COLOR_LIME; // 00FF00
				} else if (fromhex(c[5]) < 0xC) {
					return COLOR_TEAL; // 00FF80 *
				} else {
					return COLOR_AQUA; // 00FFFF
				}
			}
		} else if (fromhex(c[1]) < 0xC) {
			if (fromhex(c[3]) < 0x8) {
				if (fromhex(c[5]) < 0x8) {
					return COLOR_MAROON; //800000
				} else if (fromhex(c[5]) < 0xC) {
					return COLOR_PURPLE; //800080
				} else {
					return COLOR_PURPLE; //8000FF *
				}
			} else if (fromhex(c[3]) < 0xC) {
				if (fromhex(c[5]) < 0x8) {
					return COLOR_OLIVE; //808000
				} else if (fromhex(c[5]) < 0xC) {
					return COLOR_GRAY; //808080
				} else {
					return COLOR_GRAY; //8080FF *
				}
			} else {
				if (fromhex(c[5]) < 0x8) {
					return COLOR_OLIVE; //80FF00 *
				} else if (fromhex(c[5]) < 0xC) {
					return COLOR_GRAY; //80FF80 *
				} else {
					return COLOR_WHITE; //80FFFF *
				}
			}
		} else if (fromhex(c[1]) < 0xF) {
			if (fromhex(c[3]) < 0x8) {
				if (fromhex(c[5]) < 0x8) {
					return COLOR_MAROON; //C00000 *
				} else {
					return COLOR_PURPLE; //C000C0 *
				}
			} else {
				if (fromhex(c[5]) < 0x8) {
					return COLOR_OLIVE; // C0C000 *
				} else {
					return COLOR_SILVER; // C0C0C0
				}
			}
		} else {
			if (fromhex(c[3]) < 0x8) {
				if (fromhex(c[5]) < 0x8) {
					return COLOR_RED; //FF0000
				} else if (fromhex(c[5]) < 0xC) {
					return COLOR_FUCHSIA; //FF0080 *
				} else {
					return COLOR_FUCHSIA; //FF00FF
				}
			} else if (fromhex(c[3]) < 0xC) {
				if (fromhex(c[5]) < 0x8) {
					return COLOR_YELLOW; //FF8000 *
				} else if (fromhex(c[5]) < 0xC) {
					return COLOR_GRAY; //FF8080 *
				} else {
					return COLOR_FUCHSIA; //FF80FF *
				}
			} else {
				if (fromhex(c[5]) < 0x8) {
					return COLOR_YELLOW; //FFFF00
				} else if (fromhex(c[5]) < 0xC) {
					return COLOR_WHITE; //FFFF80 *
				} else {
					return COLOR_WHITE; //FFFFFF
				}
			}
		}
	}
	return COLOR_BLACK;
}

static void get_style1(Xd6XmlTreeElement *e)
{
	switch (e->name[0]) {
	case 'a':
		para->flags[0] |= IS_INLINE;
		para->is_inline = 1;

		if (e->get_attr_value("href")) {
			para->display = 1;
			para->underline = 1;
			para->fg_color = COLOR_BLUE;
			para->a_link = 1;
			para->flags[1] |= UNDERLINE|FG_COLOR|A_LINK;
			e->display = DISPLAY_A_LINK;
		} else if (e->get_attr_value("name")) {
			para->display = 1;
			e->display = DISPLAY_ANCHOR;
		}
		return;
	case 'b':
		para->font_bold = 1;
		para->is_inline = 1;
		para->flags[0] |= IS_INLINE;
		para->flags[1] |= FONT_BOLD;
		return;
	case 'g':
		para->display = 1;
		e->display = DISPLAY_SVG_G;
		return;
	case 'i':
		para->font_italic = 1;
		para->is_inline = 1;
		para->flags[0] |= IS_INLINE;
		para->flags[1] |= FONT_ITALIC;
		return;
	case 'p':
		para->is_block = 1;
		para->top_margin = 2 * para->font_size;
		para->list = LIST_NONE;
		para->flags[0] |= IS_BLOCK|TOP_MARGIN|LIST;
		return;
	case 'q':
		para->font_italic = 1;
		para->is_inline = 1;
		para->flags[0] |= IS_INLINE;
		para->flags[1] |= FONT_ITALIC;
		return;
	case 's':
		para->strike = 1;
		para->is_inline = 1;
		para->flags[0] |= IS_INLINE;
		para->flags[1] |= STRIKE;
		return;
	case 'u':
		para->underline = 1;
		para->is_inline = 1;
		if (e->get_attr_value("double")) {
			para->double_under = 1;
		}
		para->flags[0] |= IS_INLINE;
		para->flags[1] |= UNDERLINE|DOUBLE_UNDER;
		return;
	}
	para->is_inline = 1;
	para->flags[0] |= IS_INLINE;
}

static void get_style2(Xd6XmlTreeElement *e)
{
	const char *a;
	switch (e->name[0]) {
	case 'b':
	  switch (e->name[1]) {
	  case 'r':	//br
		para->is_block = 1;
		para->rtl_direction = 0;
		para->list = LIST_NONE;
		para->flags[0] |= IS_BLOCK|LIST;
		para->flags[1] |= RTL_DIRECTION;
		a = e->get_attr_value("style");
		if (a) a = strstr(a, "page-break-before:");
		if (a) a = strstr(a, "always");
		if (a) {
			para->page_break = 1;
			para->flags[0] |= PAGE_BREAK;
		}
		return;
	  }
	  break;
	case 'd':
	  switch (e->name[1]) {
	  case 'd':	//dd
		para->is_block = 1;
		para->flags[0] |= IS_BLOCK;
		return;
	  case 'l':	//dl
		para->is_block = 1;
		para->blockquote++;
		para->list = LIST_DISC;
		para->top_margin =  FONT_SIZE_3;
		para->flags[0] |= IS_BLOCK|BLOCKQUOTE|LIST|TOP_MARGIN;
		return;
	  case 't':	//dt
		para->is_block = 1;
		para->flags[0] |= IS_BLOCK;
		return;
	  }
	  break;
	case 'e':
	  switch (e->name[1]) {
	  case 'm':	//em
		para->is_inline = 1;
		para->flags[0] |= IS_INLINE;
		para->font_italic = 1;
		para->flags[1] |= FONT_ITALIC;
		return;
	  }
	  break;
	case 'h':
	  switch (e->name[1]) {
	  case '1':	//h1
		para->is_block = 1;
		para->top_margin =  2 * FONT_SIZE_7;
		para->flags[0] |= IS_BLOCK|TOP_MARGIN;
		para->font_size = FONT_SIZE_7;
		para->flags[1] |= FONT_SIZE;
		return;
	  case '2':	//h2
		para->is_block = 1;
		para->top_margin =  2 * FONT_SIZE_6;
		para->flags[0] |= IS_BLOCK|TOP_MARGIN;
		para->font_size = FONT_SIZE_6;
		para->flags[1] |= FONT_SIZE;
		return;
	  case '3':	//h3
		para->is_block = 1;
		para->top_margin =  2 * FONT_SIZE_5;
		para->flags[0] |= IS_BLOCK|TOP_MARGIN;
		para->font_size = FONT_SIZE_5;
		para->flags[1] |= FONT_SIZE;
		return;
	  case '4':	//h4
		para->is_block = 1;
		para->top_margin =  2 * FONT_SIZE_4;
		para->flags[0] |= IS_BLOCK|TOP_MARGIN;
		para->font_size = FONT_SIZE_4;
		para->flags[1] |= FONT_SIZE;
		return;
	  case '5':	//h5
		para->is_block = 1;
		para->top_margin =  2 * FONT_SIZE_3;
		para->flags[0] |= IS_BLOCK|TOP_MARGIN;
		para->font_size = FONT_SIZE_3;
		para->flags[1] |= FONT_SIZE;
		return;
	  case '6':	//h6
		para->is_block = 1;
		para->top_margin =  2 * FONT_SIZE_2;
		para->flags[0] |= IS_BLOCK|TOP_MARGIN;
		para->font_size = FONT_SIZE_2;
		para->flags[1] |= FONT_SIZE;
		return;
	  case '7':	//h7
		para->is_block = 1;
		para->top_margin =  2 * FONT_SIZE_1;
		para->flags[0] |= IS_BLOCK|TOP_MARGIN;
		para->font_size = FONT_SIZE_1;
		return;
	  case 'r':	//hr
		a = e->get_attr_value("break");
		if (a) {
			para->page_break = 1;
			para->is_block = 1;
			para->flags[0] |= IS_BLOCK|PAGE_BREAK;
		} else {
			para->is_block = 1;
			para->top_margin = 2 * para->font_size;
			para->flags[0] |= IS_BLOCK|TOP_MARGIN;
			para->display = 1;
			e->display = DISPLAY_HR;
		}
		return;
	  }
	  break;
	case 'l':
	  switch (e->name[1]) {
	  case 'i':	//li
		a = e->get_attr_value("disc");
		if (a) {
			para->list = LIST_DISC;
		}
		a = e->get_attr_value("decimal");
		if (a) {
			para->list = LIST_NUMBER;
		}
		para->is_block = 1;
		para->flags[0] |= IS_BLOCK|LIST;
		return;
	  }
	  break;
	case 'o':	
	  switch (e->name[1]) {
	  case 'l':	//ol
		para->is_block = 1;
		para->blockquote++;
		para->list = LIST_NUMBER;
		para->top_margin =  FONT_SIZE_3;
		a = e->get_attr_value("nomargin");
		if (a) {
			para->top_margin = 0;
		}
		para->flags[0] |= IS_BLOCK|BLOCKQUOTE|LIST|TOP_MARGIN;
		return;
	  }
	  break;
	case 't':
	  switch (e->name[1]) {
	  case 'd':	//td
		para->is_block = 1;
		para->flags[0] |= IS_BLOCK;
		para->display = 1;
		e->display = DISPLAY_TABLE_CELL;
		return;
	  case 'h':	//th
		para->is_block = 1;
		para->flags[0] |= IS_BLOCK;
		para->display = 1;
		e->display = DISPLAY_TABLE_CELL;
		return;
	  case 'r':	//tr
		para->is_block = 1;
		para->flags[0] |= IS_BLOCK;
		para->display = 1;
		e->display = DISPLAY_TABLE_ROW;
		return;
	  case 't':	//tt
		para->is_inline = 1;
		para->flags[0] |= IS_INLINE;
		para->font = FONT_MONOSPACE;
		para->flags[1] |= FONT;
		return;
	  }
	case 'u':
	  switch (e->name[1]) {
	  case 'l':	//ul
		para->is_block = 1;
		para->blockquote++;
		para->list = LIST_NONE;
		para->top_margin =  FONT_SIZE_3;
		a = e->get_attr_value("nomargin");
		if (a) {
			para->top_margin = 0;
		}
		para->flags[0] |= IS_BLOCK|BLOCKQUOTE|LIST|TOP_MARGIN;
		return;
	  }
	  break;
	}
	para->is_inline = 1;
	para->flags[0] |= IS_INLINE;
}

static void get_style3(Xd6XmlTreeElement *e)
{
	switch (e->name[0]) {
	case 'b':
		if (!strcmp(e->name, "bdo")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			return;
		} else if (!strcmp(e->name, "big")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->flags[1] |= FONT_SIZE;
			if (para->font_size <= FONT_SIZE_1) {
				para->font_size = FONT_SIZE_2;
			} else if (para->font_size <= FONT_SIZE_2) {
				para->font_size = FONT_SIZE_3;
			} else if (para->font_size <= FONT_SIZE_3) {
				para->font_size = FONT_SIZE_4;
			} else if (para->font_size <= FONT_SIZE_4) {
				para->font_size = FONT_SIZE_5;
			} else if (para->font_size <= FONT_SIZE_5) {
				para->font_size = FONT_SIZE_6;
			} else if (para->font_size <= FONT_SIZE_6) {
				para->font_size = FONT_SIZE_7;
			}
			return;
		}
		break;
	case 'c':
		if (!strcmp(e->name, "col")) {
			para->is_block = 1;
			para->flags[0] |= IS_BLOCK;
			para->display = 1;
			e->display = DISPLAY_TABLE_COL;
			return;
		}
		break;
	case 'd':
		if (!strcmp(e->name, "del")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->fg_color = COLOR_RED;
			para->flags[1] |= FG_COLOR;
			return;
		} else if (!strcmp(e->name, "dfn")) {
			para->font_italic = 1;
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->flags[1] |= FONT_ITALIC;
			return;
		} else if (!strcmp(e->name, "dir")) {
			para->is_block = 1;
			para->blockquote++;
			para->list = LIST_NONE;
			para->top_margin =  FONT_SIZE_3;
			para->flags[0] |= IS_BLOCK|BLOCKQUOTE|LIST|TOP_MARGIN;
			return;
		} else if (!strcmp(e->name, "div")) {
			para->is_block = 1;
			para->list = LIST_NONE;
			if (e->parent && e->parent->nb_children == 1) {
				para->list = e->parent->stl->list;
			}
			para->flags[0] |= IS_BLOCK|LIST;
			para->rtl_direction = 0;
			para->flags[1] |= RTL_DIRECTION;
			return;
		}
		break;
	case 'i':
		if (!strcmp(e->name, "ins")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->fg_color = COLOR_GREEN;
			para->flags[1] |= FG_COLOR;
			return;
		} else if (!strcmp(e->name, "img")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->display = 1;
			e->display = DISPLAY_IMG;
			return;
		}
		break;
	case 'k':
		if (!strcmp(e->name, "kbd")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->font = FONT_MONOSPACE;
			para->flags[1] |= FONT;
			return;
		}
		break;
	case 'm':
		if (!strcmp(e->name, "map")) {
			para->display = 1;
			e->display = DISPLAY_MAP;
			return;
		}
		break;
	case 'p':
		if (!strcmp(e->name, "pre")) {
			para->is_block = 1;
			para->preformated = 1;
			para->flags[0] |= IS_BLOCK|PREFORMATED;
			para->font = FONT_MONOSPACE;
			para->flags[1] |= FONT;
			return;
		}
		break;
	case 's':
		if (!strcmp(e->name, "sub")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->sub_text = 1;
			para->flags[1] |= SUB_TEXT|FONT_SIZE;
			if (para->font_size >= FONT_SIZE_7) {
				para->font_size = FONT_SIZE_6;
			} else if (para->font_size >= FONT_SIZE_6) {
				para->font_size = FONT_SIZE_5;
			} else if (para->font_size >= FONT_SIZE_5) {
				para->font_size = FONT_SIZE_4;
			} else if (para->font_size >= FONT_SIZE_4) {
				para->font_size = FONT_SIZE_3;
			} else if (para->font_size >= FONT_SIZE_3) {
				para->font_size = FONT_SIZE_2;
			} else if (para->font_size >= FONT_SIZE_2) {
				para->font_size = FONT_SIZE_1;
			}
			return;
		} else if (!strcmp(e->name, "sup")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->sup_text = 1;
			para->flags[1] |= SUP_TEXT|FONT_SIZE;
			if (para->font_size <= FONT_SIZE_1) {
				para->font_size = FONT_SIZE_2;
			} else if (para->font_size <= FONT_SIZE_2) {
				para->font_size = FONT_SIZE_3;
			} else if (para->font_size <= FONT_SIZE_3) {
				para->font_size = FONT_SIZE_4;
			} else if (para->font_size <= FONT_SIZE_4) {
				para->font_size = FONT_SIZE_5;
			} else if (para->font_size <= FONT_SIZE_5) {
				para->font_size = FONT_SIZE_6;
			} else if (para->font_size <= FONT_SIZE_6) {
				para->font_size = FONT_SIZE_7;
			}
			return;
		} else if (!strcmp(e->name, "svg")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->display = 1;
			e->display = DISPLAY_SVG;
			return;
		}
		return;
		break;
	case 'v':
		if (!strcmp(e->name, "var")) {
			para->font_italic = 1;
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->flags[1] |= FONT_ITALIC;
			return;
		}
		break;
	}
	para->is_inline = 1;
	para->flags[0] |= IS_INLINE;
}

static void get_style4(Xd6XmlTreeElement *e)
{
	const char *a;
	switch (e->name[0]) {
	case 'a':
		if (!strcmp(e->name, "abbr")) {
			para->font_italic = 1;
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->flags[1] |= FONT_ITALIC;
			return;
		} else if (!strcmp(e->name, "area")) {
			para->display = 1;
			e->display = DISPLAY_AREA;
			return;
		}
		break;
	case 'b':
		if (!strcmp(e->name, "base")) {
			para->display = 1;
			e->display = DISPLAY_BASE;
			return;
		} else if (!strcmp(e->name, "body")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->font_size = FONT_SIZE_3;
			para->font = FONT_SANS_SERIF;
			para->flags[1] |= FONT|FONT_SIZE;
			//para->display = 1;
			//e->display = DISPLAY_BODY;
			return;
		}
		break;
	case 'c':
		if (!strcmp(e->name, "cite")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->font = FONT_MONOSPACE;
			para->flags[1] |= FONT;
			return;
		} else if (!strcmp(e->name, "code")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->font = FONT_MONOSPACE;
			para->flags[1] |= FONT;
			return;
		}
		break;
	case 'd':
		if (!strcmp(e->name, "defs")) {
			para->display = 1;
			e->display = DISPLAY_SVG_DEFS;
			return;
		} else if (!strcmp(e->name, "desc")) {
			para->display = 1;
			e->display = DISPLAY_SVG_DESC;
			return;
		}
		break;
	case 'f':
		if (!strcmp(e->name, "font")) {
			int s;
			para->is_inline = 1;
			para->flags[0] = IS_INLINE;
			s = para->font_size;
			a = e->get_attr_value("size");
			if (a) para->flags[1] |= FONT_SIZE;
			if (a && !strcmp(a, "1")) s = FONT_SIZE_1;
			if (a && !strcmp(a, "2")) s = FONT_SIZE_2;
			if (a && !strcmp(a, "3")) s = FONT_SIZE_3;
			if (a && !strcmp(a, "4")) s = FONT_SIZE_4;
			if (a && !strcmp(a, "5")) s = FONT_SIZE_5;
			if (a && !strcmp(a, "6")) s = FONT_SIZE_6;
			if (a && !strcmp(a, "7")) s = FONT_SIZE_7;
			para->font_size = s;

			s = para->font;
			a = e->get_attr_value("face");
			if (a) para->flags[1] |= FONT;
			if (a && strstr(a, "Times")) s = FONT_SERIF;
			if (a && strstr(a, "Arial")) s = FONT_SANS_SERIF;
			if (a && strstr(a, "Courier")) s = FONT_MONOSPACE;
			para->font = s;

			a = e->get_attr_value("color");
			if (a) {
				para->fg_color = get_color(a);
				para->flags[1] |= FG_COLOR;
			}
			return;
		} else if (!strcmp(e->name, "form")) {
			para->flags[0] = IS_BLOCK;
			para->is_block = 1;
			para->display = 1;
			e->display = DISPLAY_FORM;
			return;
		} 
		break;
	case 'h':
		if (!strcmp(e->name, "head")) {
			para->display = 1;
			e->display = DISPLAY_HEAD;
			return;
		} else if (!strcmp(e->name, "html")) {
			para->is_inline = 1;
			para->flags[0] = IS_INLINE;
			return;
		}
		break;
	case 'l':
		if (!strcmp(e->name, "link")) {
			para->display = 1;
			e->display = DISPLAY_LINK;
			return;
		}
		break;
	case 'm':
		if (!strcmp(e->name, "menu")) {
			para->is_block = 1;
			para->blockquote++;
			para->list = LIST_NONE;
			para->top_margin =  FONT_SIZE_3;
			para->flags[0] |= IS_BLOCK|BLOCKQUOTE|LIST|TOP_MARGIN;
			return;
		} else if (!strcmp(e->name, "meta")) {
			para->display = 1;
			e->display = DISPLAY_META;
			return;
		} else if (!strcmp(e->name, "math")) {
			para->is_inline = 1;
			para->flags[0] = IS_INLINE;
			para->display = 1;
			e->display = DISPLAY_MATH;
			return;
		}
		break;
	case 'p':
		if (!strcmp(e->name, "path")) {
			para->display = 1;
			e->display = DISPLAY_SVG_PATH;
			return;
		}
		break;
	case 's':
		if (!strcmp(e->name, "samp")) {
			para->is_block = 1;
			para->preformated = 1;
			para->flags[0] |= IS_BLOCK|PREFORMATED;
			para->font = FONT_MONOSPACE;
			para->flags[1] |= FONT;
			return;
		} else if (!strcmp(e->name, "span")) {
			para->is_inline = 1;
			para->flags[0] = IS_INLINE;
			a = e->get_attr_value("style");
			if (a) {
				if (strstr(a, "mso-tab-count:")) {
					para->display = 1;
					e->display = DISPLAY_TAB;
				}
			}
			return;
		}
		break;
	case 't':
		if (!strcmp(e->name, "text")) {
			para->display = 1;
			e->display = DISPLAY_SVG_TEXT;
			return;
		}
		break;
	}
	para->is_inline = 1;
	para->flags[0] |= IS_INLINE;
}

static void get_style5(Xd6XmlTreeElement *e)
{
	const char *a;
	switch (e->name[0]) {
	case 'f':
		if (!strcmp(e->name, "frame")) {
			para->display = 1;
			e->display = DISPLAY_FRAME;
			return;
		}
		break;
	case 'i':
		if (!strcmp(e->name, "input")) {
			para->is_inline = 1;
			para->flags[0] = IS_INLINE;
			para->display = 1;
			a = e->get_attr_value("type");
			if (a) {
				if (!strcasecmp(a, "password")) {
					e->display = DISPLAY_INPUT_PASSWORD;
				} else if (!strcasecmp(a, "checkbox")) {
					e->display = DISPLAY_INPUT_CHECKBOX;
				} else if (!strcasecmp(a, "radio")) {
					e->display = DISPLAY_INPUT_RADIO;
				} else if (!strcasecmp(a, "submit")) {
					e->display = DISPLAY_INPUT_SUBMIT;
				} else if (!strcasecmp(a, "image")) {
					e->display = DISPLAY_INPUT_IMAGE;
				} else if (!strcasecmp(a, "reset")) {
					e->display = DISPLAY_INPUT_RESET;
				} else if (!strcasecmp(a, "button")) {
					e->display = DISPLAY_INPUT_BUTTON;
				} else if (!strcasecmp(a, "hidden")) {
					e->display = DISPLAY_INPUT_HIDDEN;
				} else if (!strcasecmp(a, "file")) {
					e->display = DISPLAY_INPUT_FILE;
				} else {
					e->display = DISPLAY_INPUT_TEXT;
				}
			} else {
					e->display = DISPLAY_INPUT_TEXT;
			}
			return;
		} else if (!strcmp(e->name, "image")) {
			para->display = 1;
			e->display = DISPLAY_SVG_IMAGE;
			return;
		}
		break;
	case 'l':
		if (!strcmp(e->name, "label")) {
			para->is_inline = 1;
			para->flags[0] = IS_INLINE;
			return;
		}
		break;
	case 'p':
		if (!strcmp(e->name, "param")) {
			e->display = DISPLAY_PARAM;
			para->display = 1;
			return;
		}
		break;
	case 's':
		if (!strcmp(e->name, "small")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->flags[1] |= FONT_SIZE;
			if (para->font_size >= FONT_SIZE_7) {
				para->font_size = FONT_SIZE_6;
			} else if (para->font_size >= FONT_SIZE_6) {
				para->font_size = FONT_SIZE_5;
			} else if (para->font_size >= FONT_SIZE_5) {
				para->font_size = FONT_SIZE_4;
			} else if (para->font_size >= FONT_SIZE_4) {
				para->font_size = FONT_SIZE_3;
			} else if (para->font_size >= FONT_SIZE_3) {
				para->font_size = FONT_SIZE_2;
			} else if (para->font_size >= FONT_SIZE_2) {
				para->font_size = FONT_SIZE_1;
			}
			return;
		} else if (!strcmp(e->name, "style")) {
			para->display = 1;
			e->display = DISPLAY_STYLE;
			return;
		}
		break;
	case 't':
		if (!strcmp(e->name, "table")) {
			para->is_block = 1;
			para->flags[0] |= IS_BLOCK;
			para->display = 1;
			e->display = DISPLAY_TABLE;
			return;
		} else if (!strcmp(e->name, "tbody")) {
			para->is_block = 1;
			para->flags[0] |= IS_BLOCK;
			para->display = 1;
			e->display = DISPLAY_TABLE_BODY;
			return;
		} else if (!strcmp(e->name, "tfoot")) {
			para->is_block = 1;
			para->flags[0] |= IS_BLOCK;
			para->display = 1;
			e->display = DISPLAY_TABLE_FOOT;
			return;
		} else if (!strcmp(e->name, "thead")) {
			para->is_block = 1;
			para->flags[0] |= IS_BLOCK;
			para->display = 1;
			e->display = DISPLAY_TABLE_HEAD;
			return;
		} else if (!strcmp(e->name, "title")) {
			para->display = 1;
			e->display = DISPLAY_TITLE;
			return;
		}
		break;
	}
	para->is_inline = 1;
	para->flags[0] |= IS_INLINE;
}

static void get_style6(Xd6XmlTreeElement *e)
{
	switch (e->name[0]) {
	case 'a':
		if (!strcmp(e->name, "applet")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->display = 1;
			e->display = DISPLAY_APPLET;
			return;
		}
		break;
	case 'b':
		if (!strcmp(e->name, "button")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->display = 1;
			e->display = DISPLAY_BUTTON;
			return;
		}
		break;
	case 'c':
		if (!strcmp(e->name, "center")) {
			para->is_block = 1;
			para->text_align = TEXT_ALIGN_CENTER;
			para->flags[0] |= IS_BLOCK|TEXT_ALIGN;
			return;
		}
		break;
	case 'i':
		if (!strcmp(e->name, "iframe")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->display = 1;
			e->display = DISPLAY_IFRAME;
			return;
		}
		break;
	case 'l':
		if (!strcmp(e->name, "legend")) {
			para->is_block = 1;
			para->flags[0] |= IS_BLOCK;
			return;
		}
		break;
	case 'o':
		if (!strcmp(e->name, "object")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->display = 1;
			e->display = DISPLAY_OBJECT;
			return;
		} else if (!strcmp(e->name, "option")) {
			para->display = 1;
			e->display = DISPLAY_OPTION;
			return;
		}
		break;
	case 's':
		if (!strcmp(e->name, "script")) {
			para->display = 1;
			e->display = DISPLAY_SCRIPT;
			return;
		} else if (!strcmp(e->name, "select")) {
			para->display = 1;
			e->display = DISPLAY_SELECT;
			return;
		} else if (!strcmp(e->name, "strike")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->strike = 1;
			para->flags[1] |= STRIKE;
			return;
		} else if (!strcmp(e->name, "strong")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->font_bold = 1;
			para->flags[1] |= FONT_BOLD;
			return;
		}
		break;
	}
	para->is_inline = 1;
	para->flags[0] |= IS_INLINE;
}

static void get_style7(Xd6XmlTreeElement *e)
{
	switch (e->name[0]) {
	case 'a':
		if (!strcmp(e->name, "acronym")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->font_italic = 1;
			para->flags[1] |= FONT_ITALIC;
			return;
		}  else if (!strcmp(e->name, "address")) {
			para->is_block = 1;
			para->blockquote++;
			para->flags[0] |= IS_BLOCK|BLOCKQUOTE;
			return;
		}
		break;
	case 'c':
		if (!strcmp(e->name, "caption")) {
			para->flags[0] |= IS_BLOCK;
			para->is_block = 1;
			return;
		}
		break;
	case 'i':
		if (!strcmp(e->name, "isindex")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->display = 1;
			return;
		}
		break;
	}
	para->is_inline = 1;
	para->flags[0] |= IS_INLINE;
}

static void get_style8(Xd6XmlTreeElement *e)
{
	switch (e->name[0]) {
	case 'b':
		if (!strcmp(e->name, "basefont")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->display = 1;
			e->display = DISPLAY_BASEFONT;
			return;
		}
		break;
	case 'c':
		if (!strcmp(e->name, "colgroup")) {
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			para->display = 1;
			e->display = DISPLAY_TABLE_COLGROUP;
			return;
		}
		break;
	case 'f':
		if (!strcmp(e->name, "fieldset")) {
			para->flags[0] |= IS_BLOCK;
			para->is_block = 1;
			return;
		} else if (!strcmp(e->name, "frameset")) {
			para->display = 1;
			e->display = DISPLAY_FRAMESET;
			return;
		}
		break;
	case 'n':
		if (!strcmp(e->name, "noframes")) {
			para->display = 1;
			e->display = DISPLAY_NOFRAMES;
			return;
		} else if (!strcmp(e->name, "noscript")) {
			para->display = 1;
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			e->display = DISPLAY_NOSCRIPT;
			return;
		}
		break;
	case 'o':
		if (!strcmp(e->name, "optgroup")) {
			para->display = 1;
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			e->display = DISPLAY_OPTGROUP;
			return;
		}
		break;
	case 't':
		if (!strcmp(e->name, "textarea")) {
			para->display = 1;
			para->is_inline = 1;
			para->flags[0] |= IS_INLINE;
			e->display = DISPLAY_TEXTAREA;
			return;
		}
		break;
	}
	para->is_inline = 1;
	para->flags[0] |= IS_INLINE;

}

static void get_style9(Xd6XmlTreeElement *e)
{
	para->is_inline = 1;
	para->flags[0] |= IS_INLINE;
}

static void get_style10(Xd6XmlTreeElement *e)
{
	switch (e->name[0]) {
	case 'b':
		if (!strcmp(e->name, "blockquote")) {
			para->is_block = 1;
			para->blockquote++;
			para->flags[0] |= IS_BLOCK|BLOCKQUOTE;
			return;
		}
		break;
	}
	para->is_inline = 1;
	para->flags[0] |= IS_INLINE;
}

static int get_length(const char *s)
{
	double v;
	v = atof(s);
	
	while (isdigit(*s)) s++;
	while (*s == '.') s++;
	while (isdigit(*s)) s++;
	if (s[0] == 'c' && s[1] == 'm') {
		v = v * 72.0 / 2.54;
	} else if (s[0] == 'm' && s[1] == 'm') {
		v = v * 72.0 / 2.54 / 10.0;
	} else if (s[0] == 'i' && s[1] == 'n') {
		v = v * 72.0;
	} else if (s[0] == 'i' && s[1] == 'n') {
		v = v * 12.0;
	} else if (s[0] == 'p' && s[1] == 'p') {

	}	

	return (int) v;
}

void Xd6XmlStyle::add_tab(int t)
{
	int i;
	i = 0;
	if (t < 1) return;
	while (i < nb_tab) {
		if (tab_stop[i] == t) return;
		i++; 
	}
	tab_stop = (int *) realloc(tab_stop, sizeof(int) * (nb_tab + 2));
	nb_tab++;
	tab_stop[nb_tab] = 0;
	
	i = 0;
	while (i < nb_tab) {
		if (tab_stop[i] > t) {
			int j = nb_tab - 1;
			while (j > i) {
				tab_stop[j] = tab_stop[j - 1];
				j--;
			}
			tab_stop[i] = t;
			break;
		}
		i++;
	}
	if (i == nb_tab) tab_stop[nb_tab - 1] = t;
}


int *Xd6XmlStyle::get_tabs()
{
	int *t = tab_stop;
	nb_tab = 0;
	tab_stop = NULL;
	return t;
}

void Xd6XmlStyle::clean_tab()
{
	free(tab_stop);
	nb_tab = 0;
	tab_stop = NULL;
}

static void parse_style(Xd6XmlTreeElement *e, const char *s)
{
	const char *tab;

	tab = strstr(s, "tab-stops:");
	if (!tab) return;
	
	while (*tab != ':') tab++;
	tab++;
	while (*tab && *tab != ';') {
		while (isspace(*tab)) tab++;
		Xd6XmlStyle::add_tab(get_length(tab));
		while (!isspace(*tab) && *tab != ';' && *tab) tab++;
	}
}

Xd6XmlStl *Xd6XmlStyle::get_style(Xd6XmlTreeElement *e, int rtl)
{
	const char *a;

	if (!stl) {
		stl = new Xd6XmlStl();
		para = &stl->para;
	}

	if (e->name_len < 1 || e->name_len > 256) return 0;
	
	if (e->parent && e->parent->stl) {
		e->stl = e->parent->stl;
	} else {
		e->stl = stl;
	}

	stl->para.copy(e->stl);

	stl->para.clear_flags();
	stl->para.is_block = 0;
	stl->para.display = 0;
	stl->para.is_inline = 0;
	stl->para.list = 0;
	stl->para.top_margin = 0;
	stl->para.page_break = 0;
	stl->para.flags[0] |= IS_BLOCK|IS_INLINE|LIST|TOP_MARGIN|PAGE_BREAK;

	if (rtl) {
		stl->para.rtl_direction = 1;
	} else {
		stl->para.rtl_direction = 0;
	}
	stl->para.flags[1] |= RTL_DIRECTION;


	a = e->get_attr_value("align");
	if (a)  stl->para.flags[0] |= TEXT_ALIGN;
	if (a && !strcasecmp(a, "left"))  
		stl->para.text_align = TEXT_ALIGN_LEFT;
	if (a && !strcasecmp(a, "right"))  
		stl->para.text_align = TEXT_ALIGN_RIGHT;
	if (a && !strcasecmp(a, "center"))  
		stl->para.text_align = TEXT_ALIGN_CENTER;
	if (a && !strcasecmp(a, "justify"))  
		stl->para.text_align = TEXT_ALIGN_JUSTIFY;


	switch (e->name_len) {
	case 1:
		get_style1(e); break;
	case 2:
		get_style2(e); break;
	case 3:
		get_style3(e); break;
	case 4:
		get_style4(e); break;
	case 5:
		get_style5(e); break;
	case 6:
		get_style6(e); break;
	case 7:
		get_style7(e); break;
	case 8:
		get_style8(e); break;
	case 9:
		get_style9(e); break;
	case 10:
		get_style10(e); break;
		
	}
	a = e->get_attr_value("dir");
	if (a && !strcasecmp("RTL", a)) {
		stl->para.rtl_direction = 1;	
		stl->para.flags[1] |= RTL_DIRECTION;
	} else if (a) {
		stl->para.rtl_direction = 0;	
		stl->para.flags[1] |= RTL_DIRECTION;
	}
	a = e->get_attr_value("style");
	if (a) {
		parse_style(e, a);
	}
	e->stl = e->stl->get_style(&stl->para);
	return e->stl;
	
}

Xd6XmlStl::Xd6XmlStl(Xd6XmlStl *p)
{
	static int been_here = 0;

	child = NULL;
	nb_child = 0;

	flags[0] = 0;
	flags[1] = 0;
	flags[2] = 0;
	flags[3] = 0;

	if (!been_here) {
		been_here = 1;

		def.parent = NULL;
		def.nb_child = 0;
		def.child = NULL;

	        def.is_block = 0;
        	def.is_inline = 1;
       		def.text_align = TEXT_ALIGN_LEFT;
        	def.blockquote = 0;
        	def.top_margin = 0;
        	def.page_break = 0;
        	def.list = LIST_NONE;
		def.preformated = 0;

        	def.rtl_direction = 0;
        	def.font = FONT_SANS_SERIF;
        	def.font_size = FONT_SIZE_3;
        	def.bg_color = COLOR_WHITE;
        	def.fg_color = COLOR_BLACK;
        	def.font_bold = 0;
        	def.font_italic = 0;
        	def.underline = 0;
        	def.double_under = 0;
        	def.strike = 0;
        	def.sup_text = 0;
        	def.sub_text = 0;
        	def.a_link = 0;
        	def.bad_spell = 0;
        	def.display = 0;
	}

	if (p) {
		copy(p);
		parent = p;	
	} else  {
		copy(&def);
		parent = &def;	
	}
}

Xd6XmlStl::~Xd6XmlStl()
{
	if (child) {
		for (int i = 0; i < nb_child; i++) delete(child[i]);
	}
}

void Xd6XmlStl::copy(Xd6XmlStl *p)
{
	if (!p) p = &def;

        is_block = p->is_block;
       	is_inline = p->is_inline;
	text_align = p->text_align;
       	blockquote = p->blockquote;
       	top_margin = p->top_margin;
       	page_break = p->page_break;
       	list = p->list;
       	preformated = p->preformated;

       	rtl_direction = p->rtl_direction;
       	font = p->font;
       	font_size = p->font_size;
       	bg_color = p->bg_color;
       	fg_color = p->fg_color;
       	font_bold = p->font_bold;
       	font_italic = p->font_italic;
       	underline = p->underline;
       	double_under = p->double_under;
       	strike = p->strike;
       	sup_text = p->sup_text;
       	sub_text = p->sub_text;
       	a_link = p->a_link;
       	bad_spell = p->bad_spell;
       	display = p->display;
}

Xd6XmlStl *Xd6XmlStl::get_style(Xd6XmlStl *p)
{
	Xd6XmlStl *rstl = NULL;
	int i;

	if (is_equal(p)) return this;

	for (i = 0; i < nb_child; i++) {
		if (child[i]->is_equal(p)) return child[i];
	}

	if (parent) {
		int nbc = parent->nb_child;
		Xd6XmlStl **c = parent->child;
		for (i = 0; i < nbc; i++) {
			if (c[i]->is_equal(p)) return c[i];
		}
	}

	rstl = new Xd6XmlStl(p);
	rstl->parent = this;

	child = (Xd6XmlStl**)realloc(child, sizeof(Xd6XmlStl*) * (nb_child+1));
	child[nb_child++] = rstl;

	return rstl;
}

Xd6XmlStl *Xd6XmlStl::get_flag_style(Xd6XmlStl *p)
{
	Xd6XmlStl nstl;
	int f;

	nstl.copy(this);

	f = p->flags[0];

	if (f & 1) nstl.is_block = p->is_block;
	if (f & (1 << 1)) nstl.is_inline = p->is_inline;
	if (f & (1 << 2)) nstl.text_align = p->text_align;
	if (f & (1 << 3)) {
		nstl.blockquote += p->blockquote;
		if (nstl.blockquote < 0) nstl.blockquote = 0;
	}
	if (f & (1 << 4)) nstl.top_margin = p->top_margin;
	if (f & (1 << 5)) nstl.page_break = p->page_break;
	if (f & (1 << 6)) nstl.list = p->list;
	if (f & (1 << 7)) nstl.bg_color = p->bg_color;
	if (f & (1 << 8)) nstl.preformated = p->preformated;

	f = p->flags[1];
	if (f & (1)) nstl.rtl_direction = p->rtl_direction;
	if (f & (1 << 1)) nstl.font = p->font;
	if (f & (1 << 2)) nstl.font_size = p->font_size;
	if (f & (1 << 3)) nstl.fg_color = p->fg_color;
	if (f & (1 << 4)) nstl.font_bold = p->font_bold;
	if (f & (1 << 5)) nstl.font_italic = p->font_italic;
	if (f & (1 << 6)) nstl.underline = p->underline;
	if (f & (1 << 7)) nstl.double_under = p->double_under;
	if (f & (1 << 8)) nstl.strike = p->strike;
	if (f & (1 << 9)) nstl.sup_text = p->sup_text;
	if (f & (1 << 10)) nstl.sub_text = p->sub_text;
	if (f & (1 << 11)) nstl.a_link = p->a_link;

	f = p->flags[2];
	if (f & (1 << 0)) nstl.bad_spell = p->bad_spell;

	return get_style(&nstl);
}

void Xd6XmlStl::clear_flags()
{
	flags[0] = 0;
	flags[1] = 0;
	flags[2] = 0;
	flags[3] = 0;
}

int Xd6XmlStl::can_merge(Xd6XmlStl *p)
{
        if (is_block != p->is_block) return 0;
       	if (is_inline != p->is_inline) return 0;
       	if (page_break != p->page_break) return 0;

       	if (rtl_direction != p->rtl_direction) return 0;
       	if (font != p->font) return 0;
       	if (font_size != p->font_size) return 0;
       	if (bg_color != p->bg_color) return 0;
       	if (fg_color != p->fg_color) return 0;
       	if (font_bold != p->font_bold) return 0;
       	if (font_italic != p->font_italic) return 0;
       	if (underline != p->underline) return 0;
       	if (double_under != p->double_under) return 0;
       	if (strike != p->strike) return 0;
       	if (sup_text != p->sup_text) return 0;
       	if (sub_text != p->sub_text) return 0;
       	if (a_link != p->a_link) return 0;
       	if (display != p->display) return 0;

	return 1;
}


int Xd6XmlStl::is_equal(Xd6XmlStl *p)
{
	if (!can_merge(p)) return 0;

	if (text_align != p->text_align) return 0;
       	if (blockquote != p->blockquote) return 0;
       	if (top_margin != p->top_margin) return 0;
       	if (list != p->list) return 0;
       	if (preformated != p->preformated) return 0;

	if (bad_spell != p->bad_spell) return 0;

	return 1;
}

/*
 * "$Id: $"
 */
