
include ../Makedefs

#
# Binary
#

PROG = flwriter
DEFS = -DPACKAGE=\"$(PROG)\" -DLOCALEDIR=\"$(prefix)/share/locale\"

#
# Object files...
#

OBJS	=  \
main.o \
gui.o \
callbacks.o \
Xd6HtmlEditor.o \
Xd6HtmlToolbar.o \
Mailing.o \

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
	$(RM) flwriter-static 
	$(RM) flwriter-static.gz
	make -C po clean

#
# Make the program.
#

$(PROG):	$(OBJS) 
	echo Linking $@...
	echo $(CXX) $(LDFLAGS) $(DEFS) -o $(PROG) $(OBJS) $(LIBS) 
	$(CXX) $(LDFLAGS) $(DEFS) -o $(PROG) $(OBJS) $(LIBS) 

flwriter-static: $(OBJS)
	$(CXX) -static $(LDFLAGS) $(DEFS) -o flwriter-static  $(OBJS) $(LIBS)
	strip  flwriter-static
	gzip -9 flwriter-static

test:
	$(CXX) $(LDFLAGS) $(DEFS) -o test test.cpp $(LIBS) 


po: 	dummy
	make -C po package=$(PROG)

dummy:

