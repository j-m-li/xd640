#
# "$Id: Makefile,v 1.1 2000/08/05 19:11:21 nickasil Exp $"
#
# flfm makefile for the Fast Light Environment (FLE).
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
#

include ../Makedefs


#
# Object files...
#

OBJS	=  \
fldesk.o \
icon.o \
proxy.o

PROG = fldesk
DEFS = -DPACKAGE=\"fldesk\" -DLOCALEDIR=\"$(prefix)/share/locale\"

#
# Build everything...
#

all:	$(PROG) po


#
# Install everything...
#

install: fldesk po
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
	$(RM) fldesk
	make -C po clean

#
# Make the fldesk program.
#

fldesk:	$(OBJS) 
	echo Linking $@...
	$(CXX) $(LDFLAGS) -o fldesk  $(OBJS) $(LIBS)

po:     dummy
	make -C po package=$(PROG)

dummy:

#
# End of "$Id: Makefile,v 1.1 2000/08/05 19:11:21 nickasil Exp $".
#


