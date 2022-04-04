
include ../Makedefs

#
# Binary
#

PROG = fleffect

#
# Object files...
#

OBJS	=  \
effect.o

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
	$(CXX) $(LDFLAGS) -o $(PROG) $(OBJS) $(LIBS)




