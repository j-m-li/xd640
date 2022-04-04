
include ../Makedefs

#
# Binary
#

PROG = flshutdown
DEFS = -DPACKAGE=\"XD640\" -DLOCALEDIR=\"$(prefix)/share/locale\"

#
# Object files...
#

OBJS	=  \
main.o \

#
# Build everything...
#

all:	$(PROG) 


#
# Install everything...
#

install: $(PROG)  
	echo Installing $(PROG)... 
	-mkdir -p $(bindir)
	cp $(PROG) $(bindir)/$(PROGRAM)
	chmod 4755 $(bindir)/$(PROGRAM)

#
# Clean output files...
#

clean:
	$(RM) $(OBJS)
	$(RM) $(PROG) 

#
# Make the program.
#

$(PROG):	$(OBJS)
	echo Linking $@...
	$(CXX) $(LDFLAGS) $(DEFS) -o $(PROG) $(OBJS) $(LIBS) 

test:
	$(CXX) $(LDFLAGS) $(DEFS) -o test test.cpp $(LIBS) 




