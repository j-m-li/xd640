#include "gui.h"

extern "C" {

typedef struct {
	long long a,b;
	int e;
} number;

long long max;
number memory;
number result;
number first;
number second;
int sign;
int operation;
int decimal;
int nb_num;
int is_memory;
int grouping = 0;

void clear(void)
{
	result.a = first.a = second.a = 0;
	result.b = first.b = second.b = 0;
	result.e = first.e = second.e = 0;
	sign = 1;
	nb_num = operation = decimal = 0;
}

void add(number *r, number *a)
{
	r->b = r->b + a->b;
	if (r->b >= max) {
		r->b -= max;
		r->a++;
	} else if (r->b <= -max) {
		r->b += max;
		r->a--;
	} 
	r->a = a->a + r->a;
	if (r->a <= -max || r->a >= max) {
		r->e = 1;
	}
}

void clean_decimal(void)
{
	while (decimal < 19) {
		result.b *= 10;
		decimal++;
	}
}

void push(void) 
{
	clean_decimal();
	nb_num = 0;
	decimal = 0;
	if (!result.e) {
		first.a = sign * result.a;
		first.b = sign * result.b;
	}
	sign = 1;
	result.a = result.b = result.e = 0;
}

void refresh(void)
{
	static char buf[300];

	if (result.e) {
		snprintf(buf, 300, "ERROR!");
	} else if (result.b > 0) {
		char *ptr;
		snprintf(buf, 300, "%c%lld.%lld", 
			sign < 0 ? '-' : ' ', result.a, result.b);
		ptr = buf + strlen(buf) - 1;
		while(*ptr == '0') {
			*ptr = 0;
			ptr--;
		}
	} else {
		snprintf(buf, 300, "%c%lld.", sign < 0 ? '-' : ' ', result.a);
	}

	// max : 1000000000000000000

	display->label(buf);
	if (is_memory) {
		mem_flag->label("M");
	} else {
		mem_flag->label("");
	}
}

void cb_group(Fl_Menu_*, void*)
{
	grouping = !grouping;
}

void cb_clear(Fl_Button*, void *d)
{
	int n = (int) d;
	if (n == 1) {
		if (decimal == 1) {
			decimal = 0;
		} else if (decimal > 1) {
			decimal--;
			result.b = result.b / 10;
			nb_num--;
		} else if (nb_num > 0) {
			nb_num--;
			result.a = result.a / 10;
		}
	} else if (n == 3) {
		result.a = 0;
		result.b = 0;
		result.e = 0;
		nb_num = 0;
		decimal = 0;
	} else {
		clear();
	}
	refresh();
}

void cb_number(Fl_Button*, void* d)
{
	if (nb_num >= 18) return;
	if (decimal > 0) {
		decimal++;
		result.b = result.b * 10 + (int)d;
	} else {
		result.a = result.a * 10 + (int)d;
	}
	nb_num++;
	refresh();
}

void cb_memory(Fl_Button*, void *d)
{	
	int n = (int) d;
	switch (n) {
	case 1:
		is_memory = 0;
		memory.a = memory.b = 0;
		break;
	case 2:
		if (is_memory) {
			int s = 1;
			if (memory.a < 0) s = -1;
			result.a = memory.a * s;
			result.b = memory.b * s;
			result.e = memory.e;
			sign = s;
			nb_num = 0;
			decimal = 0;
		}
		break;
	case 4:
		is_memory = 1;
		clean_decimal();
		result.b *= sign;
		result.a *= sign;
		add(&memory, &result);
		result.b *= sign;
		result.a *= sign;
		push();
		break;
	case 3:
		is_memory = 1;
		clean_decimal();
		memory.a = result.a * sign;
		memory.b = result.b * sign;
		push();
		break;
	}
	refresh();
}

void cb_sign(Fl_Button*, void*)
{
	sign = -sign;
	refresh();
}

void cb_display(Fl_Button*, void*)
{
}

void cb_about(Fl_Widget*, void*)
{
}

void cb_copy(Fl_Widget*, void*)
{
}

void cb_paste(Fl_Widget*, void*)
{
}

void cb_op(Fl_Widget*, void*)
{
}

void cb_dot(Fl_Widget*, void*)
{
	if (decimal == 0) decimal = 1;
}

void cb_equal(Fl_Widget*, void*)
{
}

int main(int argc, char **argv) {
	max  =  1000000000;
	max = max * 1000000000;
  	is_memory = 0;
	clear();
	make_window()->show(argc, argv);
	refresh();
  	return Fl::run();
}

};
