#
# "$Id: Makefile,v 1.12.2.6.2.10 2002/01/06 13:40:27 easysw Exp $"
#
# Top-level makefile for the Fast Light Tool Kit (FLTK).
#
# Copyright 1998-2002 by Bill Spitzak and others.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.
#
# Please report all bugs and problems to "fltk-bugs@fltk.org".
#

include makeinclude

DIRS	=	src fluid test documentation

all: makeinclude
	for dir in $(DIRS); do\
		echo "=== making $$dir ===";\
		(cd $$dir; $(MAKE) $(MFLAGS)) || break;\
	done

install: makeinclude
	-mkdir -p $(bindir)
	$(RM) $(bindir)/fltk-config
	-cp fltk-config $(bindir)
	-chmod 755 $(bindir)/fltk-config
	for dir in FL $(DIRS); do\
		echo "=== installing $$dir ===";\
		(cd $$dir; $(MAKE) $(MFLAGS) install) || break;\
	done

uninstall: makeinclude
	$(RM) $(bindir)/fltk-config
	for dir in FL $(DIRS); do\
		echo "=== uninstalling $$dir ===";\
		(cd $$dir; $(MAKE) $(MFLAGS) uninstall) || break;\
	done

depend: makeinclude
	for dir in $(DIRS); do\
		echo "=== making dependencies in $$dir ===";\
		(cd $$dir; $(MAKE) $(MFLAGS) depend) || break;\
	done

clean:
	$(RM) core *.o
	for dir in $(DIRS); do\
		echo "=== cleaning $$dir ===";\
		(cd $$dir; $(MAKE) $(MFLAGS) clean) || break;\
	done
	(cd lib ;rm -f *.lib *.dll *.exp)
	(rm -f `find . -name .DS_Store -print`)
	(rm -f `find . -name '*~' -print`)
	(cd visualc; rm -rf `find -name '*' -type d  -print`)
	(chmod 0664 `find -name '*' -type f  -print` 2> /dev/null)
	(chmod 775 configure fluid/tounix debian/rules 2>/dev/null)

distclean: clean
	$(RM) config.* makeinclude
	(cd visualc; rm -fr *_; rm -fr Debug fltkdlld; \
		rm -f *.plg *.ncb *.opt *.dll *.ilk *.pdb)
	(cd lib; rm -f fltkd.lib  fltkdlld.exp  fltkdlld.lib *.a st*)

makeinclude: configure configh.in makeinclude.in
	if test -f config.status; then \
		./config.status --recheck; \
		./config.status; \
	else \
		./configure; \
	fi
	touch config.h

configure: configure.in
	autoconf

portable-dist:
	epm -v -s fltk.xpm fltk-utf8

native-dist:
	epm -v -f native fltk-utf8


#
# End of "$Id: Makefile,v 1.12.2.6.2.10 2002/01/06 13:40:27 easysw Exp $".
#
