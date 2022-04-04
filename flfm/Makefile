#
# "$Id: Makefile,v 1.1 2000/08/05 19:11:21 nickasil Exp $"
#
# flchest makefile for the Fast Light Environment (FLE).
#
# Copyright 1998-2000 by Bill Spitzak and others.
#
# This is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published
# by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.
#
# Please report all bugs and problems to "fltk-bugs@fltk.org".
#

include ../Makedefs

#
# Binary
#

PROG = flfm
DEFS = -DPACKAGE=\"$(PROG)\" -DLOCALEDIR=\"$(prefix)/share/locale\"

#
# Object files...
#

OBJS	=  \
main.o \
gui.o \
Location.o \
IconCanvas.o \
callbacks.o \
IconGroup.o \
BigIcon.o \
SmallIcon.o

#
# Build everything...
#

all:	$(PROG) po


#
# Install everything...
#

install: $(PROG) po 
	echo Installing $(PROG)... 
	-mkdir -p $(bindir)
	cp $(PROG) $(bindir)/$(PROGRAM)
	chmod 755 $(bindir)/$(PROGRAM)
	make -C po instroot=$(prefix) package=$(PROG) install

#
# Clean output files...
#

clean:
	$(RM) $(OBJS)
	$(RM) $(PROG) 
	make -C po clean

#
# Make the program.
#

$(PROG):	$(OBJS) 
	echo Linking $@...
	$(CXX) $(LDFLAGS) $(DEFS) -o $(PROG) $(OBJS) $(LIBS) 

po: 	dummy
	make -C po package=$(PROG)

dummy:

#
# End of "$Id: Makefile,v 1.1 2000/08/05 19:11:21 nickasil Exp $".
#


