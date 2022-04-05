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


#include "Xd6Std.h"
#include "Xd6VirtualKeyboard.h"
#include <FL/Fl.h>
#include <FL/Fl_Button.h>
#include <FL/fl_utf8.h>
#define _(String) gettext((String))

Xd6VirtualKeyboard::Xd6VirtualKeyboard(Fl_Widget *t) : Fl_Window(620, 80)
{
	int i, ii;
	target = t;
	base = 0;
	offset = 0;
	for (ii = 0; ii < 4; ii++) {
		for (i = 0; i < 24; i++) {
			keys[ii * 24 + i] = new Fl_Button(i * 20, 
				ii * 20, 20, 20);
			keys[ii * 24 + i]->callback(cb_button);
		}
	}
	update();
	item = new Fl_Menu_Item[200];
	oitem = new Fl_Menu_Item[30];
	for (i = 0; i < 200; i++) item[i].text = NULL;
	for (i = 0; i < 30; i++) oitem[i].text = NULL;

	i = item->add(_("U0000 - U05FF"), 0, cb_base, (void*) 0, 0);
	item->add(_("Basic Latin"), 0, cb_base, (void*)	0, 0);
	item->add(_("Latin-1 Supplement"), 0, cb_base, (void*)0x80, 0);
	item->add(_("Latin Extended-A"), 0, cb_base, (void*)0x100, 0);
	item->add(_("Latin Extended-B"), 0, cb_base, (void*)0x180, 0);
	item->add(_("IPA Extensions"), 0, cb_base, (void*)0x250, 0);
	item->add(_("Spacing Modifier Letters"), 0, cb_base, (void*)0x2B0, 0);
	item->add(_("Combining Diacritical Marks"), 0, cb_base,(void*)0x300, 0);
	item->add(_("Greek"), 0, cb_base, (void*)0x370, 0);
	item->add(_("Cyrillic"), 0, cb_base, (void*)0x400, 0);
	item->add(_("Armenian"), 0, cb_base, (void*)0x530, 0);
	item->add(_("Hebrew"), 0, cb_base, (void*)0x590, 0);
	item[i].flags = FL_SUBMENU;

	i = item->add(_("U0600 - U0CFF"), 0, cb_base, (void*) 0, 0);
	item->add(_("Arabic"), 0, cb_base, (void*)0x0600, 0);
	item->add(_("Syriac"), 0, cb_base, (void*)0x0700, 0);
	item->add(_("Thaana"), 0, cb_base, (void*)0x0780, 0);
	item->add(_("Devanagari"), 0, cb_base, (void*)0x0900, 0);
	item->add(_("Bengali"), 0, cb_base, (void*)0x0980, 0);
	item->add(_("Gurmukhi"), 0, cb_base, (void*)0x0a00, 0);
	item->add(_("Gujarati"), 0, cb_base, (void*)0x0a80, 0);
	item->add(_("Oriya"), 0, cb_base, (void*)0x0b00, 0);
	item->add(_("Tamil"), 0, cb_base, (void*)0x0b80, 0);
	item->add(_("Telugu"), 0, cb_base, (void*)0x0c00, 0);
	item->add(_("Kannada"), 0, cb_base, (void*)0x0c80, 0);
	item[i].flags = FL_SUBMENU;

	i = item->add(_("U0D00 - U13FF"), 0, cb_base, (void*) 0, 0);
	item->add(_("Malayalam"), 0, cb_base, (void*)0x0d00, 0);
	item->add(_("Sinhala"), 0, cb_base, (void*)0x0d80, 0);
	item->add(_("Thai"), 0, cb_base, (void*)0x0e00, 0);
	item->add(_("Lao"), 0, cb_base, (void*)0x0e80, 0);
	item->add(_("Tibetan"), 0, cb_base, (void*)0x0f00, 0);
	item->add(_("Myanmar"), 0, cb_base, (void*)0x1000, 0);
	item->add(_("Georgian"), 0, cb_base, (void*)0x10a0, 0);
	item->add(_("Hangul Jamo"), 0, cb_base, (void*)0x1100, 0);
	item->add(_("Ethiopic"), 0, cb_base, (void*)0x1200, 0);
	item->add(_("Cherokee"), 0, cb_base, (void*)0x13a0, 0);
	item[i].flags = FL_SUBMENU;

	i = item->add(_("U1400 - U20CF"), 0, cb_base, (void*) 0, 0);
	item->add(_("Unified Canadian Aboriginal Syllabic"), 
		0, cb_base, (void*)0x1400, 0);
	item->add(_("Ogham"), 0, cb_base, (void*)0x1680, 0);
	item->add(_("Runic"), 0, cb_base, (void*)0x16a0, 0);
	item->add(_("Khmer"), 0, cb_base, (void*)0x1780, 0);
	item->add(_("Mongolian"), 0, cb_base, (void*)0x1800, 0);
	item->add(_("Latin Extended Additional"), 0, cb_base, (void*)0x1e00, 0);
	item->add(_("Greek Extended"), 0, cb_base, (void*)0x1f00, 0);
	item->add(_("General Punctuation"), 0, cb_base, (void*)0x2000, 0);
	item->add(_("Superscripts and Subscripts"), 0, cb_base, 
		(void*)0x2070, 0);
	item->add(_("Currency Symbols"), 0, cb_base, (void*)0x20a0, 0);
	item[i].flags = FL_SUBMENU;

	i = item->add(_("U20D0 - U214F"), 0, cb_base, (void*) 0, 0);
	item->add(_("Combining Marks for Symbols"), 0, cb_base, 
		(void*)0x20d0, 0);
	item->add(_("Letterlike Symbols"), 0, cb_base, (void*)0x2100, 0);
	item[i].flags = FL_SUBMENU;

	i = item->add(_("U2150 - U25FF"), 0, cb_base, (void*) 0, 0);
	item->add(_("Number Forms"), 0, cb_base, (void*)0x2150, 0);
	item->add(_("Arrows"), 0, cb_base, (void*)0x2190, 0);
	item->add(_("Mathematical Operators"), 0, cb_base, (void*)0x2200, 0);
	item->add(_("Miscellaneous Technical"), 0, cb_base, (void*)0x2300, 0);
	item->add(_("Control Pictures"), 0, cb_base, (void*)0x2400, 0);
	item->add(_("Optical Character Recognition"), 0, cb_base, 
		(void*)0x2400, 0);
	item->add(_("Enclosed Alphanumerics"), 0, cb_base, (void*)0x2460, 0);
	item->add(_("Box Drawing"), 0, cb_base, (void*)0x2500, 0);
	item->add(_("Block Elements"), 0, cb_base, (void*)0x2580, 0);
	item->add(_("Geometric Shapes"), 0, cb_base, (void*)0x25a0, 0);
	item[i].flags = FL_SUBMENU;

	i = item->add(_("U2600 - U312F"), 0, cb_base, (void*) 0, 0);
	item->add(_("Miscellaneous Symbols"), 0, cb_base, (void*)0x2600, 0);
	item->add(_("Dingbats"), 0, cb_base, (void*)0x2700, 0);
	item->add(_("Braille Patterns"), 0, cb_base, (void*)0x2800, 0);
	item->add(_("CJK Radicals Supplement"), 0, cb_base, (void*)0x2e80, 0);
	item->add(_("Kangxi Radicals"), 0, cb_base, (void*)0x2f00, 0);
	item->add(_("Ideographic Description Characters"), 0, cb_base, 
		(void*)0x2ff0, 0);
	item->add(_("CJK Symbols and Punctuation"), 0, cb_base, 
		(void*)0x3000, 0);
	item->add(_("Hiragana"), 0, cb_base, (void*)0x3040, 0);
	item->add(_("Katakana"), 0, cb_base, (void*)0x30a0, 0);
	item->add(_("Bopomofo"), 0, cb_base, (void*)0x3100, 0);
	item[i].flags = FL_SUBMENU;

	i = item->add(_("U3130 - U4DFF"), 0, cb_base, (void*) 0, 0);
	item->add(_("Hangul Compatibility Jamo"), 0, cb_base, (void*)0x3130, 0);
	item->add(_("Kanbun"), 0, cb_base, (void*)0x3190, 0);
	item->add(_("Bopomofo Extended"), 0, cb_base, (void*)0x31a0, 0);
	item->add(_("Enclosed CJK Letters and Months"), 0, cb_base, 
		(void*)0x3200, 0);
	item->add(_("CJK Compatibility"), 0, cb_base, (void*)0x3300, 0);
	item->add(_("CJK Unified Ideographs Extension A"), 0, cb_base, 
		(void*)0x3400, 0);
	item[i].flags = FL_SUBMENU;

	i = item->add(_("U4E00 - UBFFF"), 0, cb_base, (void*) 0, 0);
	item->add(_("CJK Unified Ideographs U4E00"), 0, cb_base, 
		(void*)0x4e00, 0);
	item->add(_("CJK Unified Ideographs U5300"), 0, cb_base, 
		(void*)0x5300, 0);
	item->add(_("CJK Unified Ideographs U5700"), 0, cb_base, 
		(void*)0x5700, 0);
	item->add(_("CJK Unified Ideographs U5B00"), 0, cb_base, 
		(void*)0x5b00, 0);
	item->add(_("CJK Unified Ideographs U5E00"), 0, cb_base, 
		(void*)0x5e00, 0);
	item->add(_("CJK Unified Ideographs U6300"), 0, cb_base, 
		(void*)0x6300, 0);
	item->add(_("CJK Unified Ideographs U6700"), 0, cb_base, 
		(void*)0x6700, 0);
	item->add(_("CJK Unified Ideographs U6B00"), 0, cb_base, 
		(void*)0x6b00, 0);
	item->add(_("CJK Unified Ideographs U6E00"), 0, cb_base, 
		(void*)0x6e00, 0);
	item->add(_("CJK Unified Ideographs U7300"), 0, cb_base, 
		(void*)0x7300, 0);
	item->add(_("CJK Unified Ideographs U7700"), 0, cb_base, 
		(void*)0x7700, 0);
	item->add(_("CJK Unified Ideographs U7B00"), 0, cb_base, 
		(void*)0x7b00, 0);
	item->add(_("CJK Unified Ideographs U7E00"), 0, cb_base, 
		(void*)0x7e00, 0);
	item->add(_("CJK Unified Ideographs U8300"), 0, cb_base, 
		(void*)0x8300, 0);
	item->add(_("CJK Unified Ideographs U8700"), 0, cb_base, 
		(void*)0x8700, 0);
	item->add(_("CJK Unified Ideographs U8B00"), 0, cb_base, 
		(void*)0x8b00, 0);
	item->add(_("CJK Unified Ideographs U8E00"), 0, cb_base, 
		(void*)0x8e00, 0);
	item->add(_("CJK Unified Ideographs U9300"), 0, cb_base, 
		(void*)0x9300, 0);
	item->add(_("CJK Unified Ideographs U9700"), 0, cb_base, 
		(void*)0x9700, 0);
	item->add(_("CJK Unified Ideographs U9B00"), 0, cb_base, 
		(void*)0x9b00, 0);
	item->add(_("CJK Unified Ideographs U9E00"), 0, cb_base, 
		(void*)0x9e00, 0);
	item[i].flags = FL_SUBMENU;

	i = item->add(_("UA000 - UD7FF"), 0, cb_base, (void*) 0, 0);
	item->add(_("Yi Syllables"), 0, cb_base, (void*)0xa000, 0);
	item->add(_("Yi Radicals"), 0, cb_base, (void*)0xa490, 0);
	item->add(_("Hangul Syllables"), 0, cb_base, (void*)0xac00, 0);
	item[i].flags = FL_SUBMENU;

	i = item->add(_("UD800 - UFE6F"), 0, cb_base, (void*) 0, 0);
	item->add(_("High Surrogates"), 0, cb_base, (void*)0xd800, 0);
	item->add(_("High Private Use Surrogates"), 0, cb_base, 
		(void*)0xdb80, 0);
	item->add(_("Low Surrogates"), 0, cb_base, (void*)0xdc00, 0);
	item->add(_("Private Use"), 0, cb_base, (void*)0xe000, 0);
	item->add(_("CJK Compatibility Ideographs"), 0, cb_base, 
		(void*)0xf900, 0);
	item->add(_("Alphabetic Presentation Forms"), 0, cb_base, 
		(void*)0xfb00, 0);
	item->add(_("Arabic Presentation Forms-A"), 0, cb_base, 
		(void*)0xfb50, 0);
	item->add(_("Combining Half Marks"), 0, cb_base, (void*)0xfe20, 0);
	item->add(_("CJK Compatibility Forms"), 0, cb_base, (void*)0xfe30, 0);
	item->add(_("Small Form Variants"), 0, cb_base, (void*)0xfe50, 0);
	item[i].flags = FL_SUBMENU;

	i = item->add(_("UFE70 - UFFFF"), 0, cb_base, (void*) 0, 0);
	item->add(_("Arabic Presentation Forms-B"), 0, cb_base, 
		(void*)0xfe70, 0);
	item->add(_("Specials"), 0, cb_base, (void*)0xfeff, 0);
	item->add(_("Halfwidth and Fullwidth Forms"), 0, cb_base, 
		(void*)0xff00, 0);
	item->add(_("Specials"), 0, cb_base, (void*)0xfff0, 0);
	item[i].flags = FL_SUBMENU;

	oitem->add(_("+0"), 0, cb_off, (void*) 0, 0);
	oitem->add(_("+40"), 0, cb_off, (void*) 0x40, 0);
	oitem->add(_("+80"), 0, cb_off, (void*) 0x80, 0);
	oitem->add(_("+C0"), 0, cb_off, (void*) 0xC0, 0);
	oitem->add(_("+100"), 0, cb_off, (void*) 0x100, 0);
	oitem->add(_("+140"), 0, cb_off, (void*) 0x140, 0);
	oitem->add(_("+180"), 0, cb_off, (void*) 0x180, 0);
	oitem->add(_("+1C0"), 0, cb_off, (void*) 0x1C0, 0);
	oitem->add(_("+200"), 0, cb_off, (void*) 0x200, 0);
	oitem->add(_("+240"), 0, cb_off, (void*) 0x240, 0);
	oitem->add(_("+280"), 0, cb_off, (void*) 0x280, 0);
	oitem->add(_("+2C0"), 0, cb_off, (void*) 0x2C0, 0);
	oitem->add(_("+300"), 0, cb_off, (void*) 0x300, 0);
	oitem->add(_("+340"), 0, cb_off, (void*) 0x340, 0);
	oitem->add(_("+380"), 0, cb_off, (void*) 0x380, 0);
	oitem->add(_("+3C0"), 0, cb_off, (void*) 0x3C0, 0);

	c_base = new Fl_Choice(500, 0, 120, 20);
	c_base->menu(item); 
	c_base->callback(cb_base);
	c_base->picked(item + 1);
	
	c_off = new Fl_Choice(500, 20, 120, 20);
	c_off->menu(oitem);
	c_off->callback(cb_off);
	
	b_back = new Fl_Button(500, 40, 60, 20, _("Backspc"));
	b_back->user_data((void*)'\b');
	b_back->callback(cb_button);
	b_del = new Fl_Button(560, 40, 60, 20, _("Del"));
	b_del->user_data((void*)'\x7F');
	b_del->callback(cb_button);
	b_enter = new Fl_Button(500, 60, 120, 20, _("Enter"));
	b_enter->user_data((void*)'\r');
	b_enter->callback(cb_button);

}	


Xd6VirtualKeyboard::~Xd6VirtualKeyboard()
{
}

void Xd6VirtualKeyboard::update()
{
	int i, ii;
	
	int ucs = base + offset;

	for (ii = 0; ii < 4; ii++) {
		for (i = 0; i < 24; i++) {
			int x;
			Fl_Button *b;
			int l;
			x = ii * 24 + i;
			b = keys[x];
			l = fl_ucs2utf(ucs, lab[x]);
			if (l < 1) l = 1;
			lab[x][l] = 0;
			b->label(lab[x]);
			b->user_data((void*) ucs);
			ucs++;		
		}
	}
	redraw();
}

void Xd6VirtualKeyboard::cb_base(Fl_Widget *w, void *d)
{
	Xd6VirtualKeyboard *self;
	self = (Xd6VirtualKeyboard*)w->parent();
	
	self->base = (int) d;
	self->update();
}

void Xd6VirtualKeyboard::cb_off(Fl_Widget *w, void *d)
{
	Xd6VirtualKeyboard *self;
	self = (Xd6VirtualKeyboard*)w->parent();
	self->offset = (int) d;
	self->update();
}

void Xd6VirtualKeyboard::cb_button(Fl_Widget *w, void *d)
{
	int l;
	static char buf[8];
	Xd6VirtualKeyboard *self;
	self = (Xd6VirtualKeyboard*)w->parent();
	l = fl_ucs2utf((int)d, buf);
	if (l < 1) l = 1;
	buf[l] = 0;
	Fl::e_text = buf;
	Fl::e_length = l;
	if (self->target) {
		self->target->handle(FL_KEYBOARD);
	}
}

int Xd6VirtualKeyboard::handle(int e) 
{
	if (e == FL_KEYBOARD) {
		if (target) {
			return target->handle(e);
		}
	}
	return Fl_Window::handle(e);
}


/*
 * "$Id: $"
 */

