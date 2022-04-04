
include ../Makedefs

#
# Binary
#

PROG = flnotepad
DEFS = -DPACKAGE=\"$(PROG)\" -DLOCALEDIR=\"$(prefix)/share/locale\"

#
# Object files...
#

OBJS	=  \
main.o \
gui.o \
callbacks.o \
TextEditor.o \

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
#	make -C po instroot=$(prefix) package=$(PROG) install

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



