/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000  O'ksi'D
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


#ifndef Xd6XmlStyle_h
#define Xd6XmlStyle_h

#include "Xd6XmlTree.h"


enum Xd6XmlStyles {
	COLOR_BLACK	= 0,
	COLOR_SILVER 	= 49,
	COLOR_GRAY	= 43,
	COLOR_WHITE	= 7,
	COLOR_MAROON	= 72,
	COLOR_RED	= 1,
	COLOR_PURPLE	= 152,
	COLOR_FUCHSIA	= 248,
	COLOR_GREEN	= 60,
	COLOR_LIME	= 2,
	COLOR_OLIVE	= 76,
	COLOR_YELLOW	= 3,
	COLOR_NAVY	= 136,
	COLOR_BLUE	= 216,
	COLOR_TEAL	= 140,
	COLOR_AQUA	= 223,

	BG_COLOR	=  1 << 7,
	FG_COLOR	=  1 << 3,

	SUP_TEXT	= 1 << 9,
	SUB_TEXT	= 1 << 10,

        RTL_DIRECTION   = 1 << 0,

        FONT_BOLD       = 1 << 4,
        FONT_ITALIC     = 1 << 5,
        UNDERLINE       = 1 << 6,
	DOUBLE_UNDER	= 1 << 7,
	STRIKE		= 1 << 8,

        FONT_SERIF      = 8,	// +   FL_FREE_FONT
        FONT_SANS_SERIF = 0,
        FONT_MONOSPACE  = 4,
        FONT		= 1 << 1,

	TEXT_ALIGN_LEFT		= 0,
	TEXT_ALIGN_CENTER 	= 1,
	TEXT_ALIGN_RIGHT 	= 2,
	TEXT_ALIGN_JUSTIFY 	= 3,
	TEXT_ALIGN		= 1 << 2,

        FONT_SIZE_1     = 8,
        FONT_SIZE_2     = 10,
        FONT_SIZE_3     = 12,
        FONT_SIZE_4     = 14,
        FONT_SIZE_5     = 18,
        FONT_SIZE_6     = 24,
        FONT_SIZE_7     = 36,

        FONT_SIZE	= 1 << 2,

        PREFORMATED     = 1 << 8,

	BLOCKQUOTE	= 1 << 3,
	BLOCKQUOTEWIDTH = 40,

	TOP_MARGIN	= 1 << 4,

	PAGE_BREAK	= 1 << 5,

	IS_BLOCK	= 1 << 0,
	IS_INLINE	= 1 << 1,
	
	LIST_NONE	= 0,
	LIST_NUMBER	= 1,
	LIST_DISC	= 2,

	LIST		= 1 << 6,

	A_LINK		= 1 << 11,
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
	DISPLAY_SVG,
	DISPLAY_SVG_TEXT,
	DISPLAY_SVG_G,
	DISPLAY_SVG_PATH,
	DISPLAY_SVG_DEFS,
	DISPLAY_SVG_IMAGE,
	DISPLAY_SVG_DESC,
	DISPLAY_MATH,
	DIPSLAY_MATH_MI,
	DIPSLAY_MATH_MN,
	DIPSLAY_MATH_MO,
	DIPSLAY_MATH_MTEXT,
	DIPSLAY_MATH_MSPACE,
	DIPSLAY_MATH_MS,
	DIPSLAY_MATH_MROW,
	DIPSLAY_MATH_MFRAC,
	DIPSLAY_MATH_MSQRT,
	DIPSLAY_MATH_ROOT,
	DIPSLAY_MATH_MSTYLE,
	DIPSLAY_MATH_MERROR,
	DIPSLAY_MATH_MPADDED,
	DIPSLAY_MATH_MPHANTOM,
	DIPSLAY_MATH_MFENCED,
	DIPSLAY_MATH_MSUB,
	DIPSLAY_MATH_MSUP,
	DIPSLAY_MATH_MSUBSUP,
	DIPSLAY_MATH_MUNDER,
	DIPSLAY_MATH_MOVER,
	DIPSLAY_MATH_MUNDEROVER,
	DIPSLAY_MATH_MMULTISCRIPTS,
	DIPSLAY_MATH_MTABLE,
	DIPSLAY_MATH_MTR,
	DIPSLAY_MATH_MTD,
	DIPSLAY_MATH_MALIGNGROUP,
	DIPSLAY_MATH_MALIGNMARK,
	DIPSLAY_MATH_MACTION,
	DISPLAY_BODY,
};

class Xd6XmlTreeElement;

class Xd6XmlStl {
public:
	static Xd6XmlStl para;
	static Xd6XmlStl def;

	Xd6XmlStl(Xd6XmlStl *p = NULL);
	~Xd6XmlStl();

	void copy(Xd6XmlStl *p);
	Xd6XmlStl *get_style(Xd6XmlStl *p);
	Xd6XmlStl *get_flag_style(Xd6XmlStl *p);
	void clear_flags();
	int can_merge(Xd6XmlStl *o);
	int is_equal(Xd6XmlStl *o);

	Xd6XmlStl **child;
	int nb_child;
	Xd6XmlStl *parent;

	int flags[4];

	int display : 1;

	int is_block : 1;	// 0,0
	int is_inline : 1;	// 0,1
	int text_align : 3;	// 0,2
	int blockquote : 8;	// 0,3
	int top_margin : 16;	// 0,4
	int page_break : 1;	// 0,5
	unsigned int list : 2;	// 0,6
	int bg_color;		// 0,7
	int preformated : 1;	// 0,8

	int rtl_direction : 1;	// 1,0
	int font;		// 1,1
	int font_size;		// 1,2
	int fg_color;		// 1,3
	int font_bold : 1;	// 1,4
	int font_italic : 1;	// 1,5
	int underline : 1;	// 1,6
	int double_under : 1;	// 1,7
	int strike : 1;		// 1,8
	int sup_text : 1;	// 1,9
	int sub_text : 1;	// 1,10
	int a_link : 1;		// 1,11

	int bad_spell : 1;	// 2,0

};

class Xd6XmlStyle {
public:
	int type;
	static int default_style;
	static int *tab_stop;
	static int nb_tab;
	static Xd6XmlStl* stl;

	Xd6XmlStyle() ;
	~Xd6XmlStyle() {};
	static Xd6XmlStl *get_style(Xd6XmlTreeElement *e, int rtl);
	static void add_tab(int t);
	static void clean_tab(void);
	static int *get_tabs(void);
};


#endif

/*
 * "$Id: $"
 */
