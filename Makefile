
SHELL	=	/bin/sh
DIRS	=	\
src \
share \
flwriter \
flnotepad \
flclock \
flshutdown \
fleffect \
flfm \
flfile \
fllauncher \
flppp \
fldesk \
test \
flspider \


all:	Makedefs
	@for dir in $(DIRS); do\
		echo "=== making $$dir ===";\
		if test ! -f $$dir/makedepend; then\
			touch $$dir/makedepend;\
		fi;\
		(cd $$dir; $(MAKE) -$(MAKEFLAGS)) || break;\
	done

install: Makedefs
	@for dir in $(DIRS); do\
		echo "=== installing $$dir ===";\
		if test ! -f $$dir/makedepend; then\
			touch $$dir/makedepend;\
		fi;\
		(cd $$dir; $(MAKE) -$(MAKEFLAGS) install) || break;\
	done
	(cd documentation; $(MAKE) -$(MAKEFLAGS) install)

clean:
	-@ rm -f core config.cache *.o *.bck
	@for dir in $(DIRS); do\
		echo "=== cleaning $$dir ===";\
		(cd $$dir; $(MAKE) -$(MAKEFLAGS) clean) || break;\
	done

distclean: clean
	rm -f config.log config.h config.status Makedefs

Makedefs: configure config.h.in Makedefs.in
	./configure

