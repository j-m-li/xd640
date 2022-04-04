
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



