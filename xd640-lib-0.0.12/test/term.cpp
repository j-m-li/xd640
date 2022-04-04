
#include <FL/Fl.h>
#include <FL/Fl_File_Chooser.h>
#include <FL/Fl_Menu_Bar.h>
#include <FL/Fl_Double_Window.h>
#include <xd640/Xd6HtmlView.h>
#include <xd640/Xd6XmlParser.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#define _(str) (str)

extern char **environ;

void cb_add_text(Fl_Widget*, void*);


class command : public Xd6HtmlView
{
public:
	Fl_Callback *cb;
	int p_in[2];
	int p_out[2];
	int pid;
	static command *self;
	static void read_shell(void*);
	static void quit(void);

	command(int x, int y, int w, int h) : Xd6HtmlView(x, y, w, h)
	{
		char *argv[4];
		self = this;
		frame->wysiwyg = 0;
		cb = NULL;
		pipe(p_in);
		pipe(p_out);
		fcntl(p_in[0], F_SETFL, O_NONBLOCK);
		pid = fork();
		if (pid != 0) {
			atexit(quit);
			Fl::add_timeout(.1, read_shell, NULL);
			return;
		}
		argv[0] = "sh";
		argv[1] = 0;
		close(0);
		dup(p_out[0]);
		close(1);
		dup(p_in[1]);
		execve("/bin/sh", argv, environ);
		_exit(127);
	}

	int handle(int e) {
		char c;
		if (Fl::e_length == 1 && Fl::e_text[0] == '\r') {
			c = '\r';
		} else {
			c = ' ';
		}
		if (e == FL_KEYBOARD) {
			frame->cursor_to_end();
		}
		int ret = Xd6HtmlView::handle(e);

		if (c == '\r') {
			cb(this, NULL);
		}
		return ret;
	}
};
command *command::self = NULL; 

void command::read_shell(void*) 
{
	char buf[2048];
	int l;
	l = read(command::self->p_in[0], buf, 2048);
	if (l > 0) {
		command::self->frame->cursor_to_end();
		command::self->frame->insert_text(buf, l);
		command::self->redraw();
	}
	Fl::add_timeout(.1, read_shell, NULL);
}

void command::quit()
{
	kill(self->pid, 2);
}

class chat : public Fl_Window
{
public:
	command *input;
	chat() : Fl_Window(500, 400)
	{
		input = new command(0, 0, w(), h());
		input->cb = cb_add_text;
	}
};


void cb_add_text(Fl_Widget*, void*)
{
	Xd6HtmlBlock *b;
	int i;
	char buf[4096];
	buf[0] = 0;
	i = 0;
	command::self->frame->cursor_to_end();
	b = command::self->frame->cur_block;
	b = command::self->frame->blocks[b->id - 1];
	while (i < b->nb_segs) {
		strncat(buf, b->segs[i]->text, b->segs[i]->len);
		i++;
	}
	strcat(buf, "\n");
	write(command::self->p_out[1], buf, strlen(buf));
	command::self->redraw();
}


int main(int argc, char **argv, char **environ)
{
	chat *chater = new chat();	
	chater->show();
	return Fl::run();
}
	
