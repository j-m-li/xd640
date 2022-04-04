//
// Sample table with a single Fl_Input for editing cells
//
//	1.00 04/18/03 ciudadsatan@hotmail.com -- Initial implementation
//      1.10 05/17/03 erco at seriss dot com -- Small mods to follow changes to Fl_Table
//
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Table.H>
#include <stdio.h>
#include <stdlib.h>

const int MAX_COLS = 26;
const int MAX_ROWS = 50;

Fl_Callback input_cb;

class SingleInput : public Fl_Table
{
    Fl_Int_Input* input;
    int values[MAX_ROWS][MAX_COLS];
    int row_edit, col_edit;

protected:
    void draw_cell(TableContext context, int=0, int=0, int=0, int=0, int=0, int=0);
    static void event_callback(Fl_Widget*, void*);
    void event_callback2();

public:
    void set_value() { values[row_edit][col_edit] = atoi(input->value()); input->hide(); }

    SingleInput(int x, int y, int w, int h, const char *l=0) : Fl_Table(x,y,w,h,l)
    {
        callback(&event_callback, (void*)this);
	input = new Fl_Int_Input(w/2,h/2,0,0);
	input->hide();
	input->callback(input_cb, (void*)this);
	input->when(FL_WHEN_ENTER_KEY_ALWAYS);
	input->maximum_size(5);

	(new Fl_Box(9999,9999,0,0))->hide();  // HACK: prevent flickering in Fl_Scroll
	end();
    }
    ~SingleInput() { }

    void rows(int val) { if (input->visible()) input->do_callback(); Fl_Table::rows(val); }
    void cols(int val) { if (input->visible()) input->do_callback(); Fl_Table::cols(val); }
    inline int rows() { return Fl_Table::rows(); }
    inline int cols() { return Fl_Table::cols(); }
};

void input_cb(Fl_Widget*, void* v)
    { ((SingleInput*)v)->set_value(); }

// Handle drawing all cells in table
void SingleInput::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H)
{
    static char s[30];

    switch ( context )
    {
	case CONTEXT_COL_HEADER:
	    fl_font(FL_HELVETICA | FL_BOLD, 14);
	    fl_push_clip(X, Y, W, H);
	    {
		fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, col_header_color());
		fl_color(FL_BLACK);
		if (C != cols()-1)
		{
		    s[0] = 'A' + C;
		    s[1] = '\0';
		    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
		}
		else
		    { fl_draw("TOTAL", X, Y, W, H, FL_ALIGN_CENTER); }
	    }
	    fl_pop_clip();
	    return;

	case CONTEXT_ROW_HEADER:
	    fl_font(FL_HELVETICA | FL_BOLD, 14);
	    fl_push_clip(X, Y, W, H);
	    {
		fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
		fl_color(FL_BLACK);
		if (R != rows()-1)
		{
		    sprintf(s, "%d", R+1);
		    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
		}
		else
		    { fl_draw("TOTAL", X, Y, W, H, FL_ALIGN_CENTER); }
	    }
	    fl_pop_clip();
	    return;

	case CONTEXT_CELL:
	{
	    if (R == row_edit && C == col_edit && input->visible())
	        { return; }

	    // BACKGROUND
	    fl_push_clip(X, Y, W, H);
	    fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, FL_WHITE);
	    fl_pop_clip();

	    // TEXT
	    fl_push_clip(X+3, Y+3, W-6, H-6);
	    {
		fl_color(FL_BLACK);

		if (C != cols()-1 && R != rows()-1)
		{
		    fl_font(FL_HELVETICA, 14);
		    sprintf(s, "%d", values[R][C]);
		    fl_draw(s, X+3, Y+3, W-6, H-6, FL_ALIGN_RIGHT);
		}
		else
		{
		    int T = 0;
		    fl_font(FL_HELVETICA | FL_BOLD, 14);

		    if (C == cols()-1 && R == rows()-1)		// TOTAL
		    {
			for (int c=0; c<cols()-1; ++c)
			    for (int r=0; r<rows()-1; ++r)
				T += values[r][c];
		    }
		    else if (C == cols()-1)			// ROW SUBTOTAL
		    {
			for (int c=0; c<cols()-1; ++c)
			    T += values[R][c];
		    }
		    else if (R == rows()-1)			// COL SUBTOTAL
		    {
			for (int r=0; r<rows()-1; ++r)
			    T += values[r][C];
		    }

		    sprintf(s, "%d", T);
		    fl_draw(s, X+3, Y+3, W-6, H-6, FL_ALIGN_RIGHT);
		}

	    }
	    fl_pop_clip();

	    return;
	}

	case CONTEXT_RC_RESIZE:
	{
	    if (!input->visible()) return;
	    find_cell(CONTEXT_TABLE, row_edit, col_edit, X, Y, W, H);
	    if (X==input->x() && Y==input->y() && W==input->w() && H==input->h()) return;
	    input->resize(X,Y,W,H);
	    return;
	}

	default:
	    return;
    }
}

// Callback whenever someone clicks on different parts of the table
void SingleInput::event_callback(Fl_Widget*, void *data)
{
    SingleInput *o = (SingleInput*)data;
    o->event_callback2();
}

void SingleInput::event_callback2()
{
    int R = callback_row(),
        C = callback_col();
    TableContext context = callback_context();

    switch ( context )
    {
	case CONTEXT_CELL:
	{
	    if (C == cols()-1 || R == rows()-1) return;
	    if (input->visible()) input->do_callback();
	    row_edit = R;
	    col_edit = C;
	    int XX,YY,WW,HH;
	    find_cell(CONTEXT_CELL, R, C, XX, YY, WW, HH);
	    input->resize(XX,YY,WW,HH);
	    char s[30];
	    sprintf(s, "%d", values[R][C]);
	    input->value(s);
	    input->show();
	    input->take_focus();
	    return;
	}
	default:
	    return;
    }
}

// Change number of columns
void setcols_cb(Fl_Widget* w, void* v)
{
    SingleInput* table = (SingleInput*)v;
    Fl_Valuator *in = (Fl_Valuator*)w;
    int cols = int(in->value()) + 1;
    table->cols(cols);
}

// Change number of rows
void setrows_cb(Fl_Widget* w, void* v)
{
    SingleInput* table = (SingleInput*)v;
    Fl_Valuator *in = (Fl_Valuator*)w;
    int rows = int(in->value()) + 1;
    table->rows(rows);
}

int main()
{
    Fl_Window win(600, 400);

    SingleInput* table = new SingleInput(20, 20, win.w()-80, win.h()-80);

    // ROWS
    table->row_header(1);
    table->row_header_width(70);
    table->row_resize(1);
    table->rows(11);
    table->row_height_all(25);

    // COLS
    table->col_header(1);
    table->col_header_height(25);
    table->col_resize(1);
    table->cols(11);
    table->col_width_all(70);

    // Add children to window
    win.begin();

    // ROW
    Fl_Value_Slider setrows(win.w()-40,20,20,win.h()-80, 0);
    setrows.type(FL_VERT_NICE_SLIDER);
    setrows.bounds(2,MAX_ROWS);
    setrows.step(1);
    setrows.value(table->rows()-1);
    setrows.callback(setrows_cb, (void*)table);
    setrows.when(FL_WHEN_CHANGED);
    setrows.clear_visible_focus();

    // COL
    Fl_Value_Slider setcols(20,win.h()-40,win.w()-80,20, 0);
    setcols.type(FL_HOR_NICE_SLIDER);
    setcols.bounds(2,MAX_COLS);
    setcols.step(1);
    setcols.value(table->cols()-1);
    setcols.callback(setcols_cb, (void*)table);
    setcols.when(FL_WHEN_CHANGED);
    setcols.clear_visible_focus();

    win.end();
    win.resizable(table);
    win.show();

    return Fl::run();
}

