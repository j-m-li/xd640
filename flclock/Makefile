
include ../Makedefs

#
# Binary
#

PROG = flclock
DEFS = -DPACKAGE=\"$(PROG)\" -DLOCALEDIR=\"$(prefix)/share/locale\"

#
# Object files...
#

OBJS	=  \
clock.o

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
	cp $(PROG) $(bindir)/$(PROG)
	chmod 755 $(bindir)/$(PROG)
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



