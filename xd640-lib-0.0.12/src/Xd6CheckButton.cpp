//
// "$Id: Xd6CheckButton.cxx,v 1.1 2000/06/25 21:41:33 easysw Exp $"
//
//   Xd6CheckButton routines for the Common UNIX Printing System (CUPS).
//
//   Copyright 1997-2000 by Easy Software Products.
//
//   These coded instructions, statements, and computer programs are the
//   property of Easy Software Products and are protected by Federal
//   copyright law.  Distribution and use rights are outlined in the file
//   "LICENSE.txt" which should have been included with this file.  If this
//   file is missing or damaged please contact Easy Software Products
//   at:
//
//       Attn: CUPS Licensing Information
//       Easy Software Products
//       44141 Airport View Drive, Suite 204
//       Hollywood, Maryland 20636-3111 USA
//
//       Voice: (301) 373-9603
//       EMail: cups-info@cups.org
//         WWW: http://www.cups.org
//
// Contents:
//

//
// Include necessary header files...
//

#include <FL/Fl.H>
#include "Xd6CheckButton.h"
#include <FL/fl_draw.H>


//
// Xd6CheckButton is a subclass of Fl_Button like Fl_Check_Button, only the
// toggle and radio images are more like Microsoft Windows.
//


//
// 'Xd6CheckButton::draw()' - Draw the check button.
//

void Xd6CheckButton::draw()
{
  int	size,		// Size of button...
	offset;		// Box Y offset...

  size   = labelsize();
  offset = (h() - size) / 2;

  if (type() == FL_RADIO_BUTTON)
  {
    // Draw the radio "hole"...
    draw_box(FL_ROUND_DOWN_BOX, x(), y() + offset, size, size, FL_WHITE);

    // Then the check...
    if (value())
    {
      fl_color(active_r() ? FL_BLACK : FL_GRAY);

      if (size > 14)
        fl_pie(x() + 5, y() + offset + 5, size - 10, size - 10, 0.0, 360.0);
      else
      {
        // Small circles don't draw well with some X servers...
	fl_rect(x() + 6, y() + offset + 5, 2, 4);
	fl_rect(x() + 5, y() + offset + 6, 4, 2);
      }
    }
  }
  else
  {
    // Draw the check "box"...
    draw_box(FL_DOWN_BOX, x(), y() + offset, size, size, FL_WHITE);

    // Then the check...
    if (value())
    {
      fl_color(active_r() ? FL_BLACK : FL_GRAY);
        int xx = x() + 3;
        int w = 8;
        int d1 = w/3;
        int d2 = w-d1;
        int yy = y()+offset+6;
        fl_line(xx, yy, xx+d1, yy+d1, xx+w-1, yy+d1-d2+1);
        yy++;
        fl_line(xx, yy, xx+d1, yy+d1, xx+w-1, yy+d1-d2+1);
        yy++;
        fl_line(xx, yy, xx+d1, yy+d1, xx+w-1, yy+d1-d2+1);
    }
  }

  // Finally, the label...
  draw_label(x() + 5 * size / 4, y(), w() - 5 * size / 4, h());
}


//
// 'Xd6CheckButton::Xd6CheckButton()' - Construct a Xd6CheckButton widget.
//

Xd6CheckButton::Xd6CheckButton(int x, int y, int w, int h, const char* l)
: Fl_Button(x, y, w, h, l)
{
  type(FL_TOGGLE_BUTTON);
  align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
}


//
// End of "$Id: Xd6CheckButton.cxx,v 1.1 2000/06/25 21:41:33 easysw Exp $".
//
