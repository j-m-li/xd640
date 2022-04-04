//
// "$Id: Xd6CheckButton.h,v 1.1 2000/06/25 21:41:33 easysw Exp $"
//
//   Xd6CheckButton definitions for the Common UNIX Printing System (CUPS).
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

#ifndef _CHECKBUTTON_H_
#  define _CHECKBUTTON_H_

//
// Include necessary headers.
//

#include <FL/Fl_Button.H>


//
// Xd6CheckButton class...
//

class Xd6CheckButton : public Fl_Button
{
protected:
    virtual void draw();

public:
    Xd6CheckButton(int x, int y, int w, int h, const char *l = 0);
};

#endif // !_CHECKBUTTON_H_

//
// End of "$Id: Xd6CheckButton.h,v 1.1 2000/06/25 21:41:33 easysw Exp $".
//
