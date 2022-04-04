
#include <FL/Fl.h>
#include <FL/Fl_File_Chooser.h>
#include <FL/Fl_Menu_Bar.h>
#include <FL/Fl_Double_Window.h>
#include <xd640/Xd6HtmlView.h>
#include <xd640/Xd6XmlParser.h>
#define _(str) (str)

void cb_add_text(Fl_Widget*, void*);


class command : public Xd6HtmlView
{
public:
	Fl_Callback *cb;
	
	
	command(int x, int y, int w, int h) : Xd6HtmlView(x, y, w, h)
	{
		frame->wysiwyg = 0;
		cb = NULL;
	}
	int handle(int e) {
		char c;
		if (Fl::e_length == 1 && Fl::e_text[0] == '\r') {
			c = '\r';
		} else {
			c = ' ';
		}
		int ret = Xd6HtmlView::handle(e);

		if (c == '\r') {
			cb(this, NULL);
		}
		return ret;
	}
};

class chat : public Fl_Double_Window
{
public:
	Xd6HtmlView *display;
	command *input;
	chat() : Fl_Double_Window(500, 400)
	{
		display = new Xd6HtmlView(0, 0, w(), h() / 2);
		display->frame->editor = 0;
		display->frame->wysiwyg = 0;
		input = new command(0, h() / 2, w(), h() / 2);
		input->cb = cb_add_text;
	}
};

chat *chater;

void auto_text(void *)
{
	char *str = "<html><body><font color=\"red\">ME&gt; Hello it's "
			"<strong>me</strong>.</font></body></html>";
	Xd6XmlParser *p;
	Xd6HtmlFrame *f;

	p = new Xd6XmlParser();
	p->parse_string(str, strlen(str));
	f = new Xd6HtmlFrame(0);
	f->tree2block(p->tree->root);
	chater->display->frame->cursor_to_end();
	chater->display->frame->insert_frame(f);
	chater->display->frame->cursor_to_end();
	chater->display->frame->insert_text("\r", 1);
	chater->redraw();
	delete(p);
	Fl::add_timeout(2, auto_text, NULL);
	
}

void cb_add_text(Fl_Widget*, void*)
{
	chater->input->frame->cursor_to_begin();
	chater->input->frame->insert_text("YOU> ", 5);
	chater->display->frame->cursor_to_end();
	chater->display->frame->insert_frame(chater->input->frame);
	chater->display->frame->cursor_to_end();
	chater->display->frame->insert_text("\r", 1);
	chater->input->frame = NULL;
	chater->input->new_frame(chater->input->w(), chater->input->h());
	chater->input->frame->wysiwyg = 0;
	chater->redraw();
}


int main(int argc, char **argv, char **environ)
{
	chater = new chat();	
	chater->show();
	Fl::add_timeout(1, auto_text, NULL);
	return Fl::run();
}
	
