
include ../Makedefs


#
# Object files...
#

OBJS	=  \
flppp.o \
PppGui.o

PROG = flppp
DEFS = -DPACKAGE=\"flppp\" -DLOCALEDIR=\"$(prefix)/share/locale\"

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
	cp $(PROG) $(bindir)/$(PROG)
	chmod 755 $(bindir)/$(PROG)
	make -C po instroot=$(prefix) package=$(PROG) install

#
# Clean output files...
#

clean:
	$(RM) $(OBJS)
	$(RM) $(PROG)
	make -C po clean

#
# Make the fldesk program.
#

$(PROG):	$(OBJS) 
	echo Linking $@...
	$(CXX) $(LDFLAGS) -o $(PROG)  $(OBJS) $(LIBS)

po:     dummy
	make -C po package=$(PROG)

dummy:


