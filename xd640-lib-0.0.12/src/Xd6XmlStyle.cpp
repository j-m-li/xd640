/******************************************************************************
 *   "$Id: $"
 *
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

#include "Xd6XmlStyle.h"
#include "Xd6Std.h"
#include <string.h>
#include <ctype.h>

int Xd6XmlStyle::default_style = FONT_SIZE_3;
int *Xd6XmlStyle::tab_stop = NULL;
int Xd6XmlStyle::nb_tab = 0;

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
		e->style |= IS_INLINE;
		if (e->get_attr_value("href")) {
			e->style |= UNDERLINE|COLOR_BLUE|A_LINK|DISPLAY;
			e->display = DISPLAY_A_LINK;
		} else if (e->get_attr_value("name")) {
			e->style |= DISPLAY;
			e->display |= DISPLAY_ANCHOR;
		}
		return;
	case 'b':
		e->style |= FONT_BOLD|IS_INLINE;
		return;
	case 'i':
		e->style |= FONT_ITALIC|IS_INLINE;
		return;
	case 'p':
		e->style |= IS_BLOCK|TOP_MARGIN;
		return;
	case 'q':
		e->style |= FONT_ITALIC|IS_INLINE;
		return;
	case 's':
		e->style |= IS_INLINE|STRIKE;
		return;
	case 'u':
		e->style |= UNDERLINE|IS_INLINE;
		return;
	}
	e->style |= IS_INLINE;
}

static void get_style2(Xd6XmlTreeElement *e)
{
	const char *a;
	int b;
	switch (e->name[0]) {
	case 'b':
	  switch (e->name[1]) {
	  case 'r':	//br
		e->style |= IS_BLOCK;
		a = e->get_attr_value("style");
		if (a) a = strstr(a, "page-break-before:");
		if (a) a = strstr(a, "always");
		if (a) e->style |= PAGE_BREAK;
		return;
	  }
	  break;
	case 'd':
	  switch (e->name[1]) {
	  case 'd':	//dd
		e->style |= IS_BLOCK;
		return;
	  case 'l':	//dl
		e->style |= IS_BLOCK|LIST_DISC;
		b = (e->style + BLOCKQUOTE1) & BLOCKQUOTES;
		e->style &= ~BLOCKQUOTES;
		e->style |= b;
		return;
	  case 't':	//dt
		e->style |= IS_BLOCK;
		return;
	  }
	  break;
	case 'e':
	  switch (e->name[1]) {
	  case 'm':	//em
		e->style |= FONT_ITALIC|IS_INLINE;
		return;
	  }
	  break;
	case 'h':
	  switch (e->name[1]) {
	  case '1':	//h1
		e->style &= ~FONT_SIZES;
		e->style |= FONT_SIZE_7|IS_BLOCK|TOP_MARGIN;
		return;
	  case '2':	//h2
		e->style &= ~FONT_SIZES;
		e->style |= FONT_SIZE_6|IS_BLOCK|TOP_MARGIN;
		return;
	  case '3':	//h3
		e->style &= ~FONT_SIZES;
		e->style |= FONT_SIZE_5|IS_BLOCK|TOP_MARGIN;
		return;
	  case '4':	//h4
		e->style &= ~FONT_SIZES;
		e->style |= FONT_SIZE_4|IS_BLOCK|TOP_MARGIN;
		return;
	  case '5':	//h5
		e->style &= ~FONT_SIZES;
		e->style |= FONT_SIZE_3|IS_BLOCK|TOP_MARGIN;
		return;
	  case '6':	//h6
		e->style &= ~FONT_SIZES;
		e->style |= FONT_SIZE_2|IS_BLOCK|TOP_MARGIN;
		return;
	  case '7':	//h7
		e->style &= ~FONT_SIZES;
		e->style |= FONT_SIZE_1|IS_BLOCK|TOP_MARGIN;
		return;
	  case 'r':	//hr
		e->style |= IS_BLOCK;
		a = e->get_attr_value("break");
		if (a) {
			e->style |= PAGE_BREAK;
		} else {
			e->style |= DISPLAY|TOP_MARGIN;
			e->display = DISPLAY_HR;
		}
		return;
	  }
	  break;
	case 'l':
	  switch (e->name[1]) {
	  case 'i':	//li
		e->style |= IS_BLOCK|DISPLAY|TOP_MARGIN;
		e->display = DISPLAY_LIST_ITEM;
		return;
	  }
	  break;
	case 'o':	
	  switch (e->name[1]) {
	  case 'l':	//ol
		e->style |= IS_BLOCK|LIST_NUMBER;
		b = (e->style + BLOCKQUOTE1) & BLOCKQUOTES;
		e->style &= ~BLOCKQUOTES;
		e->style |= b;
		return;
	  }
	  break;
	case 't':
	  switch (e->name[1]) {
	  case 'd':	//td
		e->style |= IS_INLINE|DISPLAY;
		e->display = DISPLAY_TABLE_CELL;
		return;
	  case 'h':	//th
		e->style |= IS_BLOCK|DISPLAY;
		e->display = DISPLAY_TABLE_CELL;
		return;
	  case 'r':	//tr
		e->style |= IS_BLOCK|DISPLAY;
		e->display = DISPLAY_TABLE_ROW;
		return;
	  case 't':	//tt
		e->style |= FONT_MONOSPACE|IS_INLINE;
		return;
	  }
	case 'u':
	  switch (e->name[1]) {
	  case 'l':	//ul
		e->style |= IS_BLOCK|LIST_NONE;
		b = (e->style + BLOCKQUOTE1) & BLOCKQUOTES;
		e->style &= ~BLOCKQUOTES;
		e->style |= b;
		return;
	  }
	  break;
	}
	e->style |= IS_INLINE;
}

static void get_style3(Xd6XmlTreeElement *e)
{
	int b;
	switch (e->name[0]) {
	case 'b':
		if (!strcmp(e->name, "bdo")) {
			e->style |= IS_INLINE;
			return;
		} else if (!strcmp(e->name, "big")) {
			e->style |= IS_INLINE;
			b = (e->style + FONT_SIZE_1) & FONT_SIZES;
			e->style &= ~FONT_SIZES;
			e->style |= b;
			return;
		}
		break;
	case 'c':
		if (!strcmp(e->name, "col")) {
			e->style |= IS_INLINE|DISPLAY;
			e->display = DISPLAY_TABLE_COL;
			return;
		}
		break;
	case 'd':
		if (!strcmp(e->name, "del")) {
			e->style &= ~COLORS;
			e->style |= IS_INLINE|COLOR_RED;
			return;
		} else if (!strcmp(e->name, "dfn")) {
			e->style |= IS_INLINE|FONT_ITALIC;
			return;
		} else if (!strcmp(e->name, "dir")) {
			e->style |= IS_BLOCK|LIST_NONE;
			b = (e->style + BLOCKQUOTE1) & BLOCKQUOTES;
			e->style &= ~BLOCKQUOTES;
			e->style |= b;
			return;
		} else if (!strcmp(e->name, "div")) {
			e->style |= IS_BLOCK;
			e->style &= ~RTL_DIRECTION;
			return;
		}
		break;
	case 'i':
		if (!strcmp(e->name, "ins")) {
			e->style &= ~COLORS;
			e->style |= IS_INLINE|COLOR_GREEN;
			return;
		} else if (!strcmp(e->name, "img")) {
			e->style |= IS_INLINE|DISPLAY;
			e->display = DISPLAY_IMG;
			return;
		}
		break;
	case 'k':
		if (!strcmp(e->name, "kbd")) {
			e->style &= ~(FONTS);
			e->style |= IS_INLINE|FONT_MONOSPACE;
			return;
		}
		break;
	case 'm':
		if (!strcmp(e->name, "map")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_MAP;
			return;
		}
		break;
	case 'p':
		if (!strcmp(e->name, "pre")) {
			e->style &= ~(FONTS);
			e->style |= IS_BLOCK|PREFORMATED|FONT_MONOSPACE;
			return;
		}
		break;
	case 's':
		if (!strcmp(e->name, "sub")) {
			e->style |= IS_INLINE|SUB_TEXT;
			b = (e->style - FONT_SIZE_1) & FONT_SIZES;
			e->style &= ~FONT_SIZES;
			e->style |= b;
			return;
		} else if (!strcmp(e->name, "sup")) {
			e->style |= IS_INLINE|SUP_TEXT;
			b = (e->style - FONT_SIZE_1) & FONT_SIZES;
			e->style &= ~FONT_SIZES;
			e->style |= b;
			return;
		}
		break;
	case 'v':
		if (!strcmp(e->name, "var")) {
			 e->style |= IS_INLINE|FONT_ITALIC;
			return;
		}
		break;
	}
	e->style |= IS_INLINE;
}

static void get_style4(Xd6XmlTreeElement *e)
{
	const char *a;
	int b;
	switch (e->name[0]) {
	case 'a':
		if (!strcmp(e->name, "abbr")) {
			e->style |= FONT_ITALIC|IS_INLINE;
			return;
		} else if (!strcmp(e->name, "area")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_AREA;
			return;
		}
		break;
	case 'b':
		if (!strcmp(e->name, "base")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_BASE;
			return;
		} else if (!strcmp(e->name, "body")) {
			e->style &= FONT_SIZES;
			e->style |= IS_INLINE|FONT_SIZE_3|FONT_SANS_SERIF;
			return;
		}
		break;
	case 'c':
		if (!strcmp(e->name, "cite")) {
			e->style &= ~FONTS;
			e->style |= IS_INLINE|FONT_MONOSPACE;
			return;
		} else if (!strcmp(e->name, "code")) {
			e->style &= ~FONTS;
			e->style |= IS_INLINE|FONT_MONOSPACE;
			return;
		}
		break;
	case 'f':
		if (!strcmp(e->name, "font")) {
			int s;
			e->style |= IS_INLINE;
			s = e->style;
			a = e->get_attr_value("size");
			if (a) s &= ~FONT_SIZES;
			if (a && !strcmp(a, "1")) s |= FONT_SIZE_1;
			if (a && !strcmp(a, "2")) s |= FONT_SIZE_2;
			if (a && !strcmp(a, "3")) s |= FONT_SIZE_3;
			if (a && !strcmp(a, "4")) s |= FONT_SIZE_4;
			if (a && !strcmp(a, "5")) s |= FONT_SIZE_5;
			if (a && !strcmp(a, "6")) s |= FONT_SIZE_6;
			if (a && !strcmp(a, "7")) s |= FONT_SIZE_7;
			a = e->get_attr_value("face");
			if (a) s &= ~FONTS;
			if (a && strstr(a, "Times")) s |= FONT_SERIF;
			if (a && strstr(a, "Arial")) s |= FONT_SANS_SERIF;
			if (a && strstr(a, "Courier")) s |= FONT_MONOSPACE;
			a = e->get_attr_value("color");
			if (a) {
				s &= ~COLORS;
				s |= get_color(a);
			}
			e->style = s;
			return;
		} else if (!strcmp(e->name, "form")) {
			e->style |= IS_BLOCK|DISPLAY;
			e->display = DISPLAY_FORM;
			return;
		} 
		break;
	case 'h':
		if (!strcmp(e->name, "head")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_HEAD;
			return;
		} else if (!strcmp(e->name, "html")) {
			e->style |= IS_INLINE;
			return;
		}
		break;
	case 'l':
		if (!strcmp(e->name, "link")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_LINK;
			return;
		}
		break;
	case 'm':
		if (!strcmp(e->name, "menu")) {
			e->style |= IS_BLOCK|LIST_NONE;
			b = (e->style + BLOCKQUOTE1) & BLOCKQUOTES;
			e->style &= ~BLOCKQUOTES;
			e->style |= b;
			return;
		} else if (!strcmp(e->name, "meta")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_META;
			return;
		}
		break;
	case 's':
		if (!strcmp(e->name, "samp")) {
			e->style &= ~(FONTS);
			e->style |= IS_INLINE|PREFORMATED|FONT_MONOSPACE;
			return;
		} else if (!strcmp(e->name, "span")) {
			e->style |= IS_INLINE;
			a = e->get_attr_value("style");
			if (a) {
				if (strstr(a, "mso-tab-count:")) {
					e->style |= DISPLAY;
					e->display = DISPLAY_TAB;
				}
			}
			return;
		}
		break;
	}
	e->style |= IS_INLINE;
}

static void get_style5(Xd6XmlTreeElement *e)
{
	const char *a;
	int b;
	switch (e->name[0]) {
	case 'f':
		if (!strcmp(e->name, "frame")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_FRAME;
			return;
		}
		break;
	case 'i':
		if (!strcmp(e->name, "input")) {
			e->style |= IS_INLINE|DISPLAY;
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
		}
		break;
	case 'l':
		if (!strcmp(e->name, "label")) {
			e->style |= IS_INLINE;
			return;
		}
		break;
	case 'p':
		if (!strcmp(e->name, "param")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_PARAM;
			return;
		}
		break;
	case 's':
		if (!strcmp(e->name, "small")) {
			e->style |= IS_INLINE;
			b = (e->style - FONT_SIZE_1) & FONT_SIZES;
			e->style &= ~FONT_SIZES;
			e->style |= b;
			return;
		} else if (!strcmp(e->name, "style")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_STYLE;
			return;
		}
		break;
	case 't':
		if (!strcmp(e->name, "table")) {
			e->style |= IS_BLOCK|DISPLAY;
			e->display = DISPLAY_TABLE;
			return;
		} else if (!strcmp(e->name, "tbody")) {
			e->style |= IS_BLOCK|DISPLAY;
			e->display = DISPLAY_TABLE_BODY;
			return;
		} else if (!strcmp(e->name, "tfoot")) {
			e->style |= IS_BLOCK|DISPLAY;
			e->display = DISPLAY_TABLE_FOOT;
			return;
		} else if (!strcmp(e->name, "thead")) {
			e->style |= IS_BLOCK|DISPLAY;
			e->display = DISPLAY_TABLE_HEAD;
			return;
		} else if (!strcmp(e->name, "title")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_TITLE;
			return;
		}
		break;
	}
	e->style |= IS_INLINE;
}

static void get_style6(Xd6XmlTreeElement *e)
{
	switch (e->name[0]) {
	case 'a':
		if (!strcmp(e->name, "applet")) {
			e->style |= IS_INLINE|DISPLAY;
			e->display = DISPLAY_APPLET;
			return;
		}
		break;
	case 'b':
		if (!strcmp(e->name, "button")) {
			e->style |= IS_INLINE|DISPLAY;
			e->display = DISPLAY_BUTTON;
			return;
		}
		break;
	case 'c':
		if (!strcmp(e->name, "center")) {
			e->style &= ~TEXT_ALIGN;
			e->style |= IS_BLOCK|TEXT_ALIGN_CENTER;
			return;
		}
		break;
	case 'i':
		if (!strcmp(e->name, "iframe")) {
			e->style |= IS_INLINE|DISPLAY;
			e->display = DISPLAY_IFRAME;
			return;
		}
		break;
	case 'l':
		if (!strcmp(e->name, "legend")) {
			e->style |= IS_BLOCK;
			return;
		}
		break;
	case 'o':
		if (!strcmp(e->name, "object")) {
			e->style |= IS_INLINE|DISPLAY;
			e->display = DISPLAY_OBJECT;
			return;
		} else if (!strcmp(e->name, "option")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_OPTION;
			return;
		}
		break;
	case 's':
		if (!strcmp(e->name, "script")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_SCRIPT;
			return;
		} else if (!strcmp(e->name, "select")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_SELECT;
			return;
		} else if (!strcmp(e->name, "strike")) {
			e->style |= IS_INLINE|STRIKE;
			return;
		} else if (!strcmp(e->name, "strong")) {
			e->style |= FONT_BOLD|IS_INLINE;
			return;
		}
		break;
	}
	e->style |= IS_INLINE;
}

static void get_style7(Xd6XmlTreeElement *e)
{
	int b;
	switch (e->name[0]) {
	case 'a':
		if (!strcmp(e->name, "acronym")) {
			e->style |= FONT_ITALIC|IS_INLINE;
			return;
		}  else if (!strcmp(e->name, "address")) {
			e->style |= IS_BLOCK;
			b = (e->style + BLOCKQUOTE1) & BLOCKQUOTES;
			e->style &= ~BLOCKQUOTES;
			e->style |= b;
			return;
		}
		break;
	case 'c':
		if (!strcmp(e->name, "caption")) {
			e->style |= IS_BLOCK;
			return;
		}
		break;
	case 'i':
		if (!strcmp(e->name, "isindex")) {
			e->style |= IS_INLINE|DISPLAY;
			e->display = DISPLAY_ISINDEX;
			return;
		}
		break;
	}
	e->style |= IS_INLINE;
}

static void get_style8(Xd6XmlTreeElement *e)
{
	switch (e->name[0]) {
	case 'b':
		if (!strcmp(e->name, "basefont")) {
			e->style |= IS_INLINE|DISPLAY;
			e->display = DISPLAY_BASEFONT;
			return;
		}
		break;
	case 'c':
		if (!strcmp(e->name, "colgroup")) {
			e->style |= IS_INLINE|DISPLAY;
			e->display = DISPLAY_TABLE_COLGROUP;
			return;
		}
		break;
	case 'f':
		if (!strcmp(e->name, "fieldset")) {
			e->style |= IS_BLOCK;
			return;
		} else if (!strcmp(e->name, "frameset")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_FRAMESET;
			return;
		}
		break;
	case 'n':
		if (!strcmp(e->name, "noframes")) {
			e->style |= DISPLAY;
			e->display = DISPLAY_NOFRAMES;
			return;
		} else if (!strcmp(e->name, "noscript")) {
			e->style |= DISPLAY|IS_INLINE;
			e->display = DISPLAY_NOSCRIPT;
			return;
		}
		break;
	case 'o':
		if (!strcmp(e->name, "optgroup")) {
			e->style |= DISPLAY|IS_INLINE;
			e->display = DISPLAY_OPTGROUP;
			return;
		}
		break;
	case 't':
		if (!strcmp(e->name, "textarea")) {
			e->style |= DISPLAY|IS_INLINE;
			e->display = DISPLAY_TEXTAREA;
			return;
		}
		break;
	}
	e->style |= IS_INLINE;

}

static void get_style9(Xd6XmlTreeElement *e)
{
	e->style |= IS_INLINE;
}

static void get_style10(Xd6XmlTreeElement *e)
{
	int b;
	switch (e->name[0]) {
	case 'b':
		if (!strcmp(e->name, "blockquote")) {
			e->style |= IS_BLOCK;
			b = (e->style + BLOCKQUOTE1) & BLOCKQUOTES;
			e->style &= ~BLOCKQUOTES;
			e->style |= b;
			return;
		}
		break;
	}
	e->style |= IS_INLINE;
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

int Xd6XmlStyle::get_style(Xd6XmlTreeElement *e, int rtl)
{
	const char *a;

	if (e->name_len < 1 || e->name_len > 256) return 0;
	
	if (e->parent) {
		e->style = e->parent->style;
	} else {
		e->style = default_style;
	}

	e->style &= ~(IS_BLOCK|DISPLAY|IS_INLINE|LIST|TOP_MARGIN|PAGE_BREAK);

	if (rtl) {
		e->style |= RTL_DIRECTION;
	} else {
		e->style &= ~RTL_DIRECTION;
	}


	a = e->get_attr_value("align");
	if (a)  e->style &= ~TEXT_ALIGN;
	if (a && !strcasecmp(a, "left"))  e->style |= TEXT_ALIGN_LEFT;
	if (a && !strcasecmp(a, "right"))  e->style |= TEXT_ALIGN_RIGHT;
	if (a && !strcasecmp(a, "center"))  e->style |= TEXT_ALIGN_CENTER;


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
		e->style |= RTL_DIRECTION;
	} else if (a) {
		e->style &= ~RTL_DIRECTION;
	}
	a = e->get_attr_value("style");
	if (a) {
		parse_style(e, a);
	}
	return e->style;
}

/*
 * "$Id: $"
 */
