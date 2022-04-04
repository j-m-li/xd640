
include ../Makedefs

#
# Binary
#

PROG = flfile
DEFS = -DPACKAGE=\"$(PROG)\" -DLOCALEDIR=\"$(prefix)/share/locale\"

#
# Object files...
#

OBJS	=  \
copy.o \
link.o \
properties.o \
unlink.o \
main.o \
move.o \
exec.o

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
# Make the program.
#

$(PROG):	$(OBJS) 
	echo Linking $@...
	$(CXX) $(LDFLAGS) $(DEFS) -o $(PROG) $(OBJS) $(LIBS) 

po: 	dummy
	make -C po instroot=$(prefix) package=$(PROG)

dummy:



