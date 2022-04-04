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
#ifndef Xd6XmlStyle_h
#define Xd6XmlStyle_h

#include "Xd6XmlTree.h"

enum Xd6XmlStyles {
	COLOR_BLACK	= 0,
	COLOR_SILVER,
	COLOR_GRAY,
	COLOR_WHITE,
	COLOR_MAROON,
	COLOR_RED,
	COLOR_PURPLE,
	COLOR_FUCHSIA,
	COLOR_GREEN,
	COLOR_LIME,
	COLOR_OLIVE,
	COLOR_YELLOW,
	COLOR_NAVY,
	COLOR_BLUE,
	COLOR_TEAL,
	COLOR_AQUA,
	COLORS		= 0x0F,

	FREE_BIT1	= 0x10,
	FREE_BIT2	= 0x20,

	SUP_TEXT	= 0x40,
	SUB_TEXT	= 0x80,

        RTL_DIRECTION   = 0x0100,

        FONT_BOLD       = 0x0200,
        FONT_ITALIC     = 0x0400,
        UNDERLINE       = 0x0800,

        FONT_SERIF      = 0x1000,
        FONT_SANS_SERIF = 0x2000,
        FONT_MONOSPACE  = 0x3000,
        FONTS           = 0x3000,

	TEXT_ALIGN_LEFT		= 0,
	TEXT_ALIGN_CENTER 	= 0x04000,
	TEXT_ALIGN_RIGHT 	= 0x08000,
	TEXT_ALIGN		= 0x0C000,

        FONT_SIZE_1     = 0x10000,
        FONT_SIZE_2     = 0x20000,
        FONT_SIZE_3     = 0x30000,
        FONT_SIZE_4     = 0x40000,
        FONT_SIZE_5     = 0x50000,
        FONT_SIZE_6     = 0x60000,
        FONT_SIZE_7     = 0x70000,
        FONT_SIZES      = 0x70000,

        PREFORMATED     = 0x080000,

	BLOCKQUOTE1	= 0x100000,
	BLOCKQUOTE2	= 0x200000,
	BLOCKQUOTE3	= 0x300000,
	BLOCKQUOTE4	= 0x400000,
	BLOCKQUOTE5	= 0x500000,
	BLOCKQUOTE6	= 0x600000,
	BLOCKQUOTE7	= 0x700000,
	BLOCKQUOTES	= 0x700000,

	TOP_MARGIN	= 0x800000,

	PAGE_BREAK	= 0x01000000,

	IS_BLOCK	= 0x02000000,
	IS_INLINE	= 0x04000000,
	
	DISPLAY		= 0x08000000,
	
	LIST_NONE	= 0x10000000,
	LIST_NUMBER	= 0x20000000,
	LIST_DISC	= 0x30000000,
	LIST		= 0x30000000,

	STRIKE		= 0x40000000,
	A_LINK		= 0x80000000,
};

enum Xd6XmlStylesDisplays {
	DISPLAY_NONE 		= 0,
	DISPLAY_LIST_ITEM,
	DISPLAY_IMG,
	DISPLAY_ANCHOR,
	DISPLAY_TABLE,
	DISPLAY_TABLE_ROW,
	DISPLAY_TABLE_COL,
	DISPLAY_TABLE_COLGROUP,
	DISPLAY_TABLE_CELL,
	DISPLAY_TABLE_BODY,
	DISPLAY_TABLE_FOOT,
	DISPLAY_TABLE_HEAD,
	DISPLAY_TITLE,
        DISPLAY_SELECT,
        DISPLAY_HIDDEN_TEXT,
        DISPLAY_TEXTAREA,
        DISPLAY_INPUT_TEXT,
        DISPLAY_INPUT_PASSWORD,
        DISPLAY_INPUT_CHECKBOX,
        DISPLAY_INPUT_RADIO,
	DISPLAY_INPUT_SUBMIT,
	DISPLAY_INPUT_IMAGE,
	DISPLAY_INPUT_RESET,
	DISPLAY_INPUT_BUTTON,
	DISPLAY_INPUT_HIDDEN,
	DISPLAY_INPUT_FILE,
        DISPLAY_BUTTON,
	DISPLAY_ISINDEX,
        DISPLAY_FRAME,
        DISPLAY_IFRAME,
        DISPLAY_FRAMESET,
        DISPLAY_APPLET,
        DISPLAY_OBJECT,
        DISPLAY_AREA,
        DISPLAY_MAP,
        DISPLAY_BASE,
        DISPLAY_BASEFONT,
        DISPLAY_PARAM,
        DISPLAY_OPTION,
        DISPLAY_OPTGROUP,
        DISPLAY_STYLE,
        DISPLAY_SCRIPT,
        DISPLAY_SCROLLBAR,
        DISPLAY_FORM,
	DISPLAY_LINK,
	DISPLAY_HR,
	DISPLAY_HEAD,
	DISPLAY_META,
	DISPLAY_NOFRAMES,
	DISPLAY_NOSCRIPT,
	DISPLAY_A_LINK,
	DISPLAY_TOP_FRAME,
	DISPLAY_TAB,
};

class Xd6XmlTreeElement;

class Xd6XmlStyle {
public:
	int type;
	static int default_style;
	static int *tab_stop;
	static int nb_tab;

	Xd6XmlStyle() ;
	~Xd6XmlStyle() {};
	static int get_style(Xd6XmlTreeElement *e, int rtl);
	static void add_tab(int t);
	static void clean_tab(void);
	static int *get_tabs(void);
};


#endif

/*
 * "$Id: $"
 */
