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
#include <string.h>
#include <ctype.h>
#include "Xd6XmlTree.h"


Xd6XmlAttribute::Xd6XmlAttribute(const char *n, int n_len) 
{
	int i;

	value = (char *)malloc(1);
	value[0] = '\0';
	if (!n || n_len < 1) {
		name = strdup("(null)");
		return;
	}
	name = (char*) malloc(n_len + 1);
	for (i = 0; i < n_len; i++) name[i] = tolower(n[i]);
	name[n_len] = '\0';
}

Xd6XmlAttribute::~Xd6XmlAttribute()
{
	if (value) free(value);
	if (name) free(name);
}
 
void Xd6XmlAttribute::set_value(const char *v, int v_len)
{
	int i;

	if (value) free(value);
	if (!v || v_len < 1) {
		value = strdup("");
		return;
	}
	value = (char*) malloc(v_len + 1);
	for (i = 0; i < v_len; i++) value[i] = v[i];
	value[v_len] = '\0';
}

Xd6XmlTreeSegment::Xd6XmlTreeSegment(Xd6XmlTreeElement *p, int i)
{
	parent = p;
	id = i;
	type = Xd6XmlTreeSegment_none;
}

Xd6XmlTreeSegment::~Xd6XmlTreeSegment()
{
	if (parent) parent->unref_child(id);
}


Xd6XmlTreeText::Xd6XmlTreeText(Xd6XmlTreeElement *p, int i) : Xd6XmlTreeSegment(p, i)
{
	type = Xd6XmlTreeSegment_text;
	data = NULL;
	len = 0;
}

Xd6XmlTreeText::~Xd6XmlTreeText()
{
	if (data) free(data);
}

Xd6XmlTreeElement::Xd6XmlTreeElement(Xd6XmlTreeElement *p, int i) : Xd6XmlTreeSegment(p, i)
{
	type = Xd6XmlTreeSegment_element;
	name = NULL;
	name_len = 0;
	parent = p;
	id = i;
	if (p) {
		//level = p->level + 1;
	} else {
		//level = 0;
	};
	display = DISPLAY_NONE;
	stl = &Xd6XmlStl::def;
	attributes = NULL;
	nb_attributes = 0;
	children = NULL;
	nb_children = 0;
}

Xd6XmlTreeElement::~Xd6XmlTreeElement()
{
	free(name);
	while (nb_attributes > 0) {
		nb_attributes--;
		delete(attributes[nb_attributes]);
	}
	if (attributes) free(attributes);
	while (nb_children > 0) {
		nb_children--;
		if (!children[nb_children]) continue;
		if (children[nb_children]->type == Xd6XmlTreeSegment_element) {
			delete((Xd6XmlTreeElement*)children[nb_children]);
		} else if (children[nb_children]->type & Xd6XmlTreeSegment_text) {
			delete((Xd6XmlTreeText*)children[nb_children]);
		} else {
			delete(children[nb_children]);
		}
	}
	if (children) free(children);
}

Xd6XmlTreeText *Xd6XmlTreeElement::create_child_text()
{
	Xd6XmlTreeText *text;

	children = (Xd6XmlTreeSegment**) realloc(children, 
		(nb_children + 1) * sizeof(Xd6XmlTreeSegment*));
	text = new Xd6XmlTreeText(this, nb_children);
	children[nb_children] = text;
	nb_children++;
	return text;
}

Xd6XmlTreeElement *Xd6XmlTreeElement::create_child_element()
{
	Xd6XmlTreeElement *elem;
	children = (Xd6XmlTreeSegment**) realloc(children, 
		(nb_children + 1) * sizeof(Xd6XmlTreeSegment*));
	elem = new Xd6XmlTreeElement(this, nb_children);
	children[nb_children] = elem;
	nb_children++;
	return elem;
}

void Xd6XmlTreeElement::unref_child(int c_id)
{
	if (c_id >= nb_children || c_id < 0) return;

	//printf("unref child %d\n", c_id);	

	nb_children--;

	while (c_id < nb_children) {
		children[c_id] = children[c_id + 1];
		children[c_id]->id = c_id;
		c_id++;
	}
}

void Xd6XmlTreeElement::add_attribute(Xd6XmlAttribute *attr)
{
	attributes = (Xd6XmlAttribute **) realloc(attributes, 
		sizeof(Xd6XmlAttribute*) * (nb_attributes + 1));
	attributes[nb_attributes] = attr;
	nb_attributes++;
}

void Xd6XmlTreeElement::set_name(const char *tag, int len)
{
	int i;

	name = (char*) malloc(len + 1);
	for (i = 0; i < len; i++) name[i] = tolower(tag[i]);
	name[len] = '\0';
	name_len = len;
}

const char *Xd6XmlTreeElement::get_attr_value(const char *name)
{
	int i;
	for(i = 0; i < nb_attributes; i++) {
		if (!strcasecmp(name, attributes[i]->name)) {
			return attributes[i]->value;
		}
	}
	return NULL;
}

Xd6XmlTree::Xd6XmlTree()
{
	Xd6XmlStl st;

	root = new Xd6XmlTreeElement(NULL, 0);
	
	st.copy(&st.def);
	//st.is_inline = 0;
	root->stl = st.def.get_style(&st);

	cur_element = root;
	cur_text = NULL;
	cur_text_offset = 0;
	begin_element = root;
	begin_text = NULL;
	begin_text_offset = 0;
	preformated = 0;
	right_to_left = 0;

	txt = NULL;
	len = 0;
	attribute = NULL;
}

Xd6XmlTree::~Xd6XmlTree()
{
	delete(attribute);
	delete(root);
}


int Xd6XmlTree::white_space()
{
	int type = 0;

	if (cur_text->len > 0 && cur_text->data[cur_text->len - 1] == 0x000D &&
		(unsigned char) txt[0] != 0x000A) 
	{
		type  = Xd6XmlTreeSegment_mac_newline;
	} else {
		switch((unsigned char) txt[0]) {
		case 0x0020:	/* ASCII space */
		case 0x000C:	/* ASCII form feed */
			type =  Xd6XmlTreeSegment_space;
			break;
		case 0x001B:	/* Zero-width space 0x200B  utf-8: 0x00C2101B */
			if (cur_text->len > 1 && 
				cur_text->data[cur_text->len - 1] == 0x10 && 
				cur_text->data[cur_text->len - 2] == (char)0xC2)
			{
				type = Xd6XmlTreeSegment_zero_space;
				break;
			}
			return 0;
		case 0x0009:	/* ASCII tab */
			type = Xd6XmlTreeSegment_tab;
			break;
		case 0x000D:	/* ASCII carriage return */
			return 0;
		case 0x000A:	/* ASCII line feed */
			if (cur_text->len > 0 && 
				cur_text->data[cur_text->len - 1] == 0x000D) {
				type = Xd6XmlTreeSegment_dos_newline;
				break;
			}
			type = Xd6XmlTreeSegment_newline;
			break;
		default:
			return 0;
		}
	}
	switch (type) {
	case Xd6XmlTreeSegment_mac_newline:
		if (cur_text->len > 1 && 
			cur_text->data[cur_text->len - 2] != ' ')
		{
			cur_text->data[cur_text->len - 1] = ' ';
		} else if (cur_text->len == 1) {
			cur_text->data[cur_text->len - 1] = ' ';
		} else {
			cur_text->len--;
		}
		return 1;
	case Xd6XmlTreeSegment_dos_newline:
		if (cur_text->len > 1 &&
			cur_text->data[cur_text->len - 2] != ' ')
		{
			cur_text->data[cur_text->len - 1] = ' ';
		} else if (cur_text->len == 1) {
			cur_text->data[cur_text->len - 1] = ' ';
		}
		txt++; len--;
		return 1;
	case Xd6XmlTreeSegment_zero_space:
		return 0;
	default:
		if (cur_text->len > 0 && 
			cur_text->data[cur_text->len - 1] != ' ')
		{
			cur_text->data[cur_text->len] = ' ';
			cur_text->len++;
		} else if (cur_text->len == 0) {
			cur_text->data[cur_text->len] = ' ';
			cur_text->len++;
		}
		txt++; len--;
		return 1;
	}
	return 0;
}

int Xd6XmlTree::ignore_space()
{
	Xd6XmlTreeElement *e;

	
	if (cur_element->nb_children == 0) return 1;

	if (cur_element->nb_children == 1) {
		e = cur_element->parent;
		while (e && e->nb_children == 1) e = e->parent;
		if (!e) return 1;
	} else {
		e = cur_element;
	}

	if (e->nb_children > 1 &&
		e->children[e->nb_children - 2]->type & 
			Xd6XmlTreeSegment_element)
	{
		e = (Xd6XmlTreeElement*) e->children[e->nb_children - 2];
	} else {
		return 1;
	}

	while (e->nb_children > 0 && 
		e->children[e->nb_children - 1]->type & Xd6XmlTreeSegment_element)
	{
		e = (Xd6XmlTreeElement*) e->children[e->nb_children - 1];
	}
	if (e->nb_children < 1) return 0;
	if (e->children[e->nb_children - 1]->type & Xd6XmlTreeSegment_text) {
		Xd6XmlTreeText *t;
		t = (Xd6XmlTreeText*) e->children[e->nb_children - 1];
		if (t->len < 1) return 0;
		if (t->data[t->len - 1] == ' ') return 1;
	}

	return 0;

}

void Xd6XmlTree::add_plain_text(const char *text, int l)
{
	int mlen;

	txt = text;
	len = l;

	if (!cur_element || len < 1) return;

	if (!cur_text) {
		cur_text = cur_element->create_child_text();
	}

	mlen = cur_text->len + 32;
	cur_text->data = (char*) realloc(cur_text->data, mlen);
	for (;len > 0;) {
		if (txt[0] == '\t') {
			Xd6XmlTreeElement *e;
			Xd6XmlAttribute *a;
			cur_text->data = (char*) realloc(cur_text->data,
	 					cur_text->len + 1);
			cur_text->data[cur_text->len] = '\0';
			e = cur_element->create_child_element();
			e->set_name("span", 4);
			a = new Xd6XmlAttribute("style", 5);
			a->set_value("mso-tab-count:1", 15);
			e->add_attribute(a);
			Xd6XmlStl *sty = Xd6XmlStyle::get_style(
					e, right_to_left);
			right_to_left = sty->rtl_direction;
			cur_text = cur_element->create_child_text();
			mlen = 32;
			cur_text->data = (char*) malloc(mlen);
			txt++; len--;
			continue;
		} else if (txt[0] == '\n') {
			cur_text->data = (char*) realloc(cur_text->data,
	 					cur_text->len + 1);
			cur_text->data[cur_text->len] = '\0';
			if (cur_element->parent && 
				cur_element->parent->stl->preformated) 
			{
				cur_element = cur_element->parent->
						create_child_element();
			} else {
				cur_element = cur_element->
						create_child_element();
				
			}
			cur_element->stl = cur_element->parent->stl;
			cur_text = cur_element->create_child_text();
			mlen = 32;
			cur_text->data = (char*) malloc(mlen);
			txt++; len--;
			continue;
		}
		if (mlen < cur_text->len + 3) {
			mlen = mlen * 2;
			cur_text->data = (char*) 
				realloc(cur_text->data, mlen);
		}
		cur_text->data[cur_text->len] = txt[0];
		cur_text->len++;
		txt++; len--;
	}
	cur_text->data = (char*) realloc(cur_text->data,
		 				cur_text->len + 1);
	cur_text->data[cur_text->len] = '\0';
}

void Xd6XmlTree::add_text(const char *text, int l)
{
	int mlen;

	txt = text;
	len = l;

	if (!cur_element || len < 1) return;

	if (!cur_text) {
		cur_text = cur_element->create_child_text();
	}

	mlen = cur_text->len + 32;
	cur_text->data = (char*) realloc(cur_text->data, mlen);
	for (;len > 0;) {
		if (txt[0] == '\xE2' && len > 2 && txt[1] == '\x80' &&
			txt[2] == '\xAE')
		{
			// RLO Right-to-left Override 0x202E
			Xd6XmlTreeElement *e;
			len -= 3; txt += 3;
			right_to_left = 1;
			if (cur_element->nb_children == 1 && 
				cur_element->children[0] == cur_text &&
				cur_text->len == 0)
			{
				cur_element->stl->para.copy(cur_element->stl);
				cur_element->stl->para.rtl_direction = 0;
				cur_element->stl = cur_element->stl->get_style(
					&cur_element->stl->para);
			} else {
				
				e = cur_element->create_child_element();
				e->stl->para.copy(e->stl);
				e->stl->para.rtl_direction = 1;
				e->stl->para.is_inline = 1;
				e->stl->para.is_block = 0;
				e->stl->para.display = 0;
				e->stl->para.list = 0;
				e->stl->para.top_margin = 0;
				e->stl->para.page_break = 0;
				e->stl = e->stl->get_style(&e->stl->para);
				cur_element = e;
				cur_text->data = (char*) realloc(cur_text->data,
		 					cur_text->len + 1);
				cur_text->data[cur_text->len] = '\0';
				cur_text = cur_element->create_child_text();
				mlen = cur_text->len + 32;
				cur_text->data = (char*) realloc(
					cur_text->data, mlen);
			}
			continue;
		}
		if (txt[0] == '\xE2' && len > 2 && txt[1] == '\x80' &&
			txt[2] == '\xAC')
		{
			// PDF Pop Directional Format 0x202C
			Xd6XmlTreeElement *e;
			len -= 3; txt += 3;
			right_to_left = 0;
			e = cur_element->create_child_element();
			e->stl->para.copy(e->stl);
			e->stl->para.rtl_direction = 0;
			e->stl->para.is_inline = 1;
			e->stl->para.is_block = 0;
			e->stl->para.display = 0;
			e->stl->para.list = 0;
			e->stl->para.top_margin = 0;
			e->stl->para.page_break = 0;
			e->stl = e->stl->get_style(&e->stl->para);
			cur_element = e;
			cur_text->data = (char*) realloc(cur_text->data,
		 					cur_text->len + 1);
			cur_text->data[cur_text->len] = '\0';
			cur_text = cur_element->create_child_text();
			mlen = cur_text->len + 32;
			cur_text->data = (char*) realloc(cur_text->data, mlen);
			continue;
		}

		if (preformated) {
			if (txt[0] == '\n') {
				if (cur_text->len > 0 && 
					cur_text->data[cur_text->len - 1] == 
					'\r')
				{
					cur_text->len--;
				}
				cur_text->data = (char*) realloc(cur_text->data,
		 					cur_text->len + 1);
				cur_text->data[cur_text->len] = '\0';
				if (cur_element->parent && 
					cur_element->parent->stl->preformated)  
				{
					cur_element = cur_element->parent->
							create_child_element();
				} else {
					cur_element = cur_element->
							create_child_element();
					
				}
				cur_element->set_name("div", 3);
				Xd6XmlStl *sty = Xd6XmlStyle::
					get_style(cur_element,0);
				right_to_left = sty->rtl_direction;
				cur_text = cur_element->create_child_text();
				mlen = 32;
				cur_text->data = (char*) malloc(mlen);
				txt++; len--;
				continue;
			} else if (txt[0] == '\t') {
				Xd6XmlTreeElement *e;
				Xd6XmlAttribute *a;
				cur_text->data = (char*) realloc(cur_text->data,
		 					cur_text->len + 1);
				cur_text->data[cur_text->len] = '\0';
				e = cur_element->create_child_element();
				e->set_name("span", 4);
				a = new Xd6XmlAttribute("style", 5);
				a->set_value("mso-tab-count:1", 15);
				e->add_attribute(a);
				Xd6XmlStl *sty = Xd6XmlStyle::get_style(e, 
					right_to_left);
				right_to_left = sty->rtl_direction;
				cur_text = cur_element->create_child_text();
				mlen = 32;
				cur_text->data = (char*) malloc(mlen);
				txt++; len--;
				continue;

			}
		} else if (cur_text->len > 0 && ( 
			cur_text->data[cur_text->len - 1] == '/')) 
		{
			cur_text->data = (char*) realloc(cur_text->data,
		 				cur_text->len + 1);
			cur_text->data[cur_text->len] = '\0';
			cur_text = cur_element->create_child_text();
			mlen = 32;
			cur_text->data = (char*) malloc(mlen);
		} else if (white_space()) {
			if (cur_text->len == 1 && ignore_space()) {
				cur_text->len = 0;
				continue;
			}
			cur_text->data = (char*) realloc(cur_text->data,
		 				cur_text->len + 1);
			cur_text->data[cur_text->len] = '\0';
			cur_text = cur_element->create_child_text();
			mlen = 32;
			cur_text->data = (char*) malloc(mlen);
			continue;
		} 
		
		if (mlen < cur_text->len + 3) {
			mlen = mlen * 2;
			cur_text->data = (char*) 
				realloc(cur_text->data, mlen);
		}
		cur_text->data[cur_text->len] = txt[0];
		cur_text->len++;
		txt++; len--;
	}
	cur_text->data = (char*) realloc(cur_text->data,
		 				cur_text->len + 1);
	cur_text->data[cur_text->len] = '\0';
}

void Xd6XmlTree::add_element(const char *tag, int len)
{
	if (!cur_element) return;

	if (cur_text && cur_text->len == 0) delete(cur_text);
	cur_text = NULL;

	if (!tag || len < 1) {
		tag = "(null)";
		len = 6;
	}
	if (len > 2 && tag[1] == ':') {
		if (tag[0] == 's') {
			tag += 2; // SVG tag
		} else if (tag[0] == 'x') {
			tag += 2; // Excel tag
		} else if (tag[0] == 'm') {
			tag += 2; // MathML tag
		}
	}
	cur_element = cur_element->create_child_element();
	cur_element->set_name(tag, len);
}

void Xd6XmlTree::set_element_style()
{
	Xd6XmlStl *sty;

	if (!cur_element) return;
	if (attribute) {
		cur_element->add_attribute(attribute);
		attribute = NULL;
	}
	sty = Xd6XmlStyle::get_style(cur_element, right_to_left);
	preformated = sty->preformated;
	right_to_left = sty->rtl_direction;
}

void Xd6XmlTree::close_element(const char *tag, int len, 
	Xd6XmlTreeCallback *func, void *data)
{
	if (!cur_element || len < 1) return;
	
	if (cur_text && cur_text->len == 0) delete(cur_text);
	cur_text = NULL;
	
	while (cur_element && cur_element->parent) {
		if (func) {
			int i = 0;
			func(cur_element, data);
			while (i < cur_element->nb_attributes) {
				delete(cur_element->attributes[i]);
				i++;
			}
			cur_element->nb_attributes = 0;
			free(cur_element->attributes);
			cur_element->attributes = NULL;
			i = 0;
			while (i < cur_element->nb_children) {
				delete(cur_element->children[i]);
				i++;
			}
			cur_element->nb_children = 0;
			free(cur_element->children);
			cur_element->children = NULL;
		}
		if (cur_element->name_len == len && 
			!strncasecmp(tag, cur_element->name, len))
		{
			if (cur_element->parent->name == NULL &&
				cur_element->parent->parent &&
				cur_element->stl->is_block) 
			{
				cur_element = cur_element->parent;
				if (func) {
					func(cur_element, data);
				}
				cur_element = cur_element->parent;
			} else {
				cur_element = cur_element->parent;
			}
			if (cur_element->stl->preformated)
			{
				preformated = 1;
			} else {
				preformated = 0;
			}
			return;
		}
		cur_element = cur_element->parent;	
	}
}

void Xd6XmlTree::element_attribute(const char *name, int len)
{
	delete(attribute);
	if (len < 1) {
		attribute = NULL;
		return;
	}
	attribute = new Xd6XmlAttribute(name, len);
}

void Xd6XmlTree::attribute_value(const char *value, int len)
{
	if (!attribute || !cur_element) return;

	attribute->set_value(value, len);
	cur_element->add_attribute(attribute);
	attribute = NULL;
}

void Xd6XmlTree::dump_text(Xd6XmlTreeElement *elem)
{
	int i;
	Xd6XmlTreeElement *p;

	if (!root) return;
	if (!elem) elem = root;

	p = elem->parent;

	while (p) {
		p = p->parent;
		printf("-");
	}
	printf(" <%s>: %p \n", elem->name, elem->stl);	

	for (i = 0; i < elem->nb_attributes; i++) {
		printf("==> %s = '%s'\n", elem->attributes[i]->name, 
			elem->attributes[i]->value);
	}

	for (i = 0; i < elem->nb_children; i++) {
		switch(elem->children[i]->type) {
		case Xd6XmlTreeSegment_none:
			break;
		case Xd6XmlTreeSegment_element:
			dump_text((Xd6XmlTreeElement*)elem->children[i]);
			break;
		default:
			printf("\"%s\"\n", 
				((Xd6XmlTreeText*)elem->children[i])->data);
		} 
	}
	printf("</%s>\n", elem->name);
}

/*
 * "$Id: $"
 */

