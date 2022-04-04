
include ../Makedefs


DEFS = -DLOCALEDIR=\"$(prefix)/share/locale\"

#
# Build everything...
#

all:	
	echo Nothing to build

#
# Install everything...
#

install: 
	echo Installing Config files in $(prefix)...
	-mkdir -p $(prefix)/share/xd640lnk/Applications
	-mkdir -p $(prefix)/share/xd640lnk/Games
	-mkdir -p $(prefix)/share/xd640lnk/Internet
	-mkdir -p $(prefix)/share/xd640lnk/System
	cp -r xd640lnk $(prefix)/share

	-mkdir -p $(prefix)/share/icons
	cp -r icons/* $(prefix)/share/icons

	-mkdir -p $(prefix)/share/apps/xd640
	-mkdir -p $(prefix)/share/apps/fldesk
	cp -r apps/* $(prefix)/share/apps/

	-mkdir -p $(prefix)/share/doc/xd640
	cp -r doc/* $(prefix)/share/doc/xd640/

	echo Installing Config files in $(sysconfdir)...
	-mkdir -p $(sysconfdir)/xd640
	cp -r etc/* $(sysconfdir)/xd640


#
# Clean output files...
#

clean:


#
# End of "$Id: Makefile,v 1.1 2000/08/05 19:11:21 nickasil Exp $".
#


