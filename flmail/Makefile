
include ../Makedefs

#
# Binary
#

PROG = flmail
DEFS = -DPACKAGE=\"XD640\" -DLOCALEDIR=\"$(prefix)/share/locale\"

#
# Object files...
#

OBJS	=  \
main.o \
gui.o \
callbacks.o \
Xd6HtmlBrowser.o \
Xd6HtmlNavigation.o \
MailParser.o \
date.o \

#
# Build everything...
#

all:	smtp-send 
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
	cp smtp-send $(bindir)/smtp-send
	chmod 755 $(bindir)/smtp-send

#
# Clean output files...
#

clean:
	$(RM) $(OBJS)
	$(RM) $(PROG) 

#
# Make the program.
#

smtp-send: 	smtp.o
	$(CXX) $(LDFLAGS) $(DEFS) -o smtp-send smtp.o $(LIBS)
	

$(PROG):	$(OBJS) 
	echo Linking $@...
	$(CXX) $(LDFLAGS) $(DEFS) -o $(PROG) $(OBJS) $(LIBS) 

test:
	$(CXX) $(LDFLAGS) $(DEFS) -o test test.cpp $(LIBS) 




