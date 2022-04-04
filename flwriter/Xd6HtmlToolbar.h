/******************************************************************************
 *   "$Id:  $"
 *
 *                 Copyright (c) 2000-2001  O'ksi'D
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


#ifndef Xd6HtmlToolbar_h
#define Xd6HtmlToolbar_h

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Toggle_Button.H>
#include <xd640/Xd6XmlStyle.h>

class Xd6HtmlEditor;

class Xd6HtmlToolbar : public Fl_Group {
public:
        Fl_Button *save;
        Fl_Button *print;
        Fl_Choice *font_family;
        Fl_Menu_Item *font_family_items;
        Fl_Choice *font_size;
        Fl_Menu_Item *font_size_items;
        Fl_Toggle_Button *text_align_left;
        Fl_Toggle_Button *text_align_right;
        Fl_Toggle_Button *text_align_center;
        Fl_Toggle_Button *text_align_justify;
        Fl_Toggle_Button *font_bold;
        Fl_Toggle_Button *font_italic;
        Fl_Toggle_Button *font_underline;
	Fl_Toggle_Button *right_to_left;
	Fl_Button *insert_picture;
	Fl_Button *page_break;
	Fl_Button *spell;
	Fl_Toggle_Button *list_num;
	Fl_Toggle_Button *list_bullet;
	Fl_Button *quote_minus;
	Fl_Button *quote_plus;
	
        Fl_Pixmap *p_center;
        Fl_Pixmap *p_left;
        Fl_Pixmap *p_right;
        Fl_Pixmap *p_bold;
        Fl_Pixmap *p_underline;
        Fl_Pixmap *p_italic;
        Fl_Pixmap *p_save;
	Fl_Pixmap *p_rtl;
	Fl_Pixmap *p_pict;
	Fl_Pixmap *p_print;
	Fl_Pixmap *p_break;
	Fl_Pixmap *p_spell;
	Fl_Pixmap *p_justify;
	Fl_Pixmap *p_num;
	Fl_Pixmap *p_bullet;
	Fl_Pixmap *p_quotem;
	Fl_Pixmap *p_quotep;

	Fl_Input *pict_name;
	Fl_Input *w_size;
	Fl_Input *h_size;

	Xd6HtmlEditor *editor;
	
	Xd6XmlStl  *old_style;
	Xd6XmlStl *old_bstyle;

	void pict_dialog(void);

	static void cb_family(Fl_Widget*, void*);
	static void cb_size(Fl_Widget*, void*);
	static void cb_center(Fl_Widget*, void*);
	static void cb_left(Fl_Widget*, void*);
	static void cb_right(Fl_Widget*, void*);
	static void cb_justify(Fl_Widget*, void*);
	static void cb_bold(Fl_Widget*, void*);
	static void cb_underline(Fl_Widget*, void*);
	static void cb_italic(Fl_Widget*, void*);
	static void cb_save(Fl_Widget*, void*);
	static void cb_rtl(Fl_Widget*, void*);
	static void cb_pict(Fl_Widget*, void*);
	static void cb_print(Fl_Widget*, void*);
	static void cb_break(Fl_Widget*, void*);
	static void cb_spell(Fl_Widget*, void*);
	static void cb_num(Fl_Widget*, void*);
	static void cb_bullet(Fl_Widget*, void*);
	static void cb_quotep(Fl_Widget*, void*);
	static void cb_quotem(Fl_Widget*, void*);

	Xd6HtmlToolbar(Xd6HtmlEditor *e, int X, int Y, int W, int H);
	~Xd6HtmlToolbar(void);
	void create_menus(void);
	void set_style(Xd6XmlStl *s, Xd6XmlStl *bs);
	void update_style(void);
	void cut(void);
	void copy(void);
	void paste(void);
	void insert_file(void);
};

#endif

/*
 * "$Id: $"
 */
