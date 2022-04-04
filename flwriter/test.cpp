
#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>

class MyButton : public Fl_Button {
	char *message;
	
	static void cb_timeout(MyButton *self) {
		printf("%s\n", self->message);
	}
public:
	MyButton(int X, int Y, int W, int H, char *n) : Fl_Button(X,Y,W,H,n) {
		message = "hello world!";
	}
	void test_timeout(void) {
		Fl::add_timeout(1.0, (Fl_Timeout_Handler)cb_timeout, this);
		if (Fl::has_timeout((Fl_Timeout_Handler)cb_timeout, this)) {
			Fl::remove_timeout((Fl_Timeout_Handler)cb_timeout, this);
		}
		Fl::add_timeout(0.1, (Fl_Timeout_Handler)cb_timeout, this);
	}
};

int main(int argc, char ** argv) {
  	Fl_Window *window = new Fl_Window(320,65);
  	MyButton *b1 = new MyButton(20, 20, 80, 25, "Hello");
  	window->end();
  	window->show(argc,argv);
	b1->test_timeout();
  	return Fl::run();
}

