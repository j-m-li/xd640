
include ../Makedefs

#
# Binary
#

PROG = flmailer
DEFS = -DPACKAGE=\"XD640\" -DLOCALEDIR=\"$(prefix)/share/locale\"

#
# Object files...
#

OBJS	=  \
main.o \
gui.o \
callbacks.o \
HtmlBrowser.o \
HtmlNavigation.o \
rfc822.o \

#
# Build everything...
#

all:	 
	if test "$(HAVE_LIB_CURL)" != ""; then \
		make $(PROG); chmod +x $(PROG); \
	else	\
		echo netscape > $(PROG); \
	fi 	

#
# Install everything...
#

install: $(PROG)  
	echo Installing $(PROG)... 
	-mkdir -p $(bindir)
	cp $(PROG) $(bindir)/$(PROGRAM)
	chmod 755 $(bindir)/$(PROGRAM)

#
# Clean output files...
#

clean:
	$(RM) $(OBJS)
	$(RM) $(PROG) 

#
# Make the program.
#

	

$(PROG):	$(OBJS) ../xd640/libxd640.a
	echo Linking $@...
	$(CXX) $(LDFLAGS) $(DEFS) -o $(PROG) $(OBJS) $(LIBS) 

test:
	$(CXX) $(LDFLAGS) $(DEFS) -o test test.cpp $(LIBS) 




