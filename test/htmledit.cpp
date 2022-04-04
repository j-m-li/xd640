
#include <xd640/Xd6Std.h>
#include <FL/Fl.h>
#include <FL/Fl_File_Chooser.h>
#include <FL/Fl_Menu_Bar.h>
#include <FL/Fl_Window.h>
#include <xd640/Xd6HtmlView.h>
#include <xd640/Xd6XmlParser.h>
#include <xd640/Xd6Tabulator.h>
#include <xd640/Xd6HtmlTagA.h>
#define _(str) (str)

void cb_save(Fl_Widget*, void*);
void cb_exit(Fl_Widget*, void*);
void cb_cut(Fl_Widget*, void*);
void cb_copy(Fl_Widget*, void*);
void cb_paste(Fl_Widget*, void*);
void cb_bold(Fl_Widget*, void*);
void cb_tabs(Fl_Widget*, void*);
void cb_tag(Fl_Widget*, void*);


class editor : public Fl_Window
{
public:
	Xd6HtmlView *view;
	Fl_Menu_Bar *bar;
	Fl_Menu_Item mnu[50];
	editor() : Fl_Window(500, 400)
	{
		int i;
		bar = new Fl_Menu_Bar(0, 0, w(), 25);
		view = new Xd6HtmlView(0, 25, w(), h() - 25);

		for (i = 0; i < 50; i++) mnu[i].text = NULL;
		i = mnu->add(_("File"), FL_ALT+'f',  0, 0, 0);
		{
			mnu->add(_("Save"), FL_CTRL+'s', 
				(Fl_Callback*)cb_save, 0, 0);
			mnu->add(_("Quit"), FL_ALT+'q', 
				(Fl_Callback*)cb_exit, 0, 0);
		}	
		mnu[i].flags = FL_SUBMENU;
		
		i = mnu->add(_("Edit"), FL_ALT+'e',  0, 0, 0);
		{
			mnu->add(_("Cut"), FL_CTRL+'x', 
				(Fl_Callback*)cb_cut, 0, 0);
			mnu->add(_("Copy"), FL_CTRL+'c', 
				(Fl_Callback*)cb_copy, 0, 0);
			mnu->add(_("Paste"), FL_CTRL+'v', 
				(Fl_Callback*)cb_paste, 0, FL_MENU_DIVIDER);
		}
		mnu[i].flags = FL_SUBMENU;

		i = mnu->add(_("Tools"), FL_ALT+'t',  0, 0, 0);
		{
			mnu->add(_("BOLD"), 0, 
				(Fl_Callback*)cb_bold, 0, 0);
			mnu->add(_("Tabs"), 0, 
				(Fl_Callback*)cb_tabs, 0, 0);
			mnu->add(_("Insert tag"), 0, 
				(Fl_Callback*)cb_tag, 0, 0);
		}
		mnu[i].flags = FL_SUBMENU;

		bar->menu(mnu);
		
	}
	void load(const char *f)
	{
		view->load(f);
	}
};

editor *edit;

void cb_save(Fl_Widget*, void*)
{
	const char *f;
	f = fl_file_chooser("Save as ...", "*.{html,htm,HTM}", NULL);
	if (f) {
		edit->view->frame->to_html(f);
	}
	edit->view->redraw();
}

void cb_exit(Fl_Widget*, void*) { exit(0);}

void cb_cut(Fl_Widget*, void*) 
{
	cb_copy(NULL, NULL);
	edit->view->frame->cut();
	edit->view->redraw();
}

void cb_copy(Fl_Widget*, void*)
{
	edit->view->frame->select_to_html("cut_buf.html");
	edit->view->redraw();
}

void cb_paste(Fl_Widget*, void*)
{
	Xd6HtmlFrame f(0);
	Xd6XmlParser *p = new Xd6XmlParser();
	p->parse_file("cut_buf.html");
	f.tree2block(p->tree->root);
	edit->view->frame->insert_frame(&f);	
	edit->view->redraw();
	delete(p);
}

void cb_bold(Fl_Widget*, void*) {
	Xd6XmlStl st;
	st.clear_flags();
	st.flags[1] = (1 << 4);
	st.font_bold = 1;
	edit->view->frame->change_style(&st);
	edit->view->redraw();
}

void cb_tabs(Fl_Widget*, void*) {
	Xd6Tabulator::setup();
	edit->view->frame->measure();
	edit->view->frame->create_pages();
	edit->view->redraw();
}

void cb_tag(Fl_Widget*, void*) {
	Xd6HtmlDisplay *s;
	Xd6HtmlSegment *se;
	Xd6XmlTreeElement *e;
	Xd6XmlAttribute *a;
	Xd6XmlStl st;
	if (!edit->view->frame->cur_chr) return;

        st.clear_flags();
        st.a_link = 1;
        st.underline = 1;
        st.fg_color = COLOR_BLUE;
        st.flags[1] = (1 << 11) | (1 << 6) | (1 << 3);

	edit->view->frame->change_style(&st);

	e = new Xd6XmlTreeElement(NULL, 0);
	a = new Xd6XmlAttribute("href", 4);
	a->set_value("test", 4);
	e->add_attribute(a);

	st.copy(edit->view->frame->cur_seg->stl);
        st.display = 1;
        st.a_link = 1;
        st.underline = 1;
        st.fg_color = COLOR_BLUE;

	e->stl = edit->view->frame->cur_seg->stl->get_style(&st);

	s = new Xd6HtmlTagA(0, e, NULL); 

	st.copy(edit->view->frame->cur_seg->stl);
        st.display = 0;
        st.a_link = 0;
        st.underline = 0;
        st.fg_color = COLOR_BLACK;
	edit->view->frame->cur_seg->stl = 
		edit->view->frame->cur_seg->stl->get_style(&st);

	edit->view->frame->insert_segment(s);
	se = edit->view->frame->cur_seg;
	se = edit->view->frame->cur_block->segs[se->id + 1];

	st.copy(se->stl);
	st.display = 0;
        st.a_link = 0;
        st.underline = 0;
        st.fg_color = COLOR_BLACK;
	se->stl = se->stl->get_style(&st);

	edit->view->frame->create_pages();
	edit->view->redraw();
	delete(e);
}

int main(int argc, char **argv, char **environ)
{
	Xd6ConfigFile *cfg;
	int index = 0;
	Fl::args(argc, argv, index);
	cfg = new Xd6ConfigFile("htmledit", "Utilities");
	edit = new editor();	
	edit->show();
	if (index != argc) {
		edit->load(argv[index]);
	}
	return Fl::run();
}
	
