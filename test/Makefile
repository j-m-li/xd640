
include ../Makedefs


#
# Documents to build...
#

CPPFILES = htmledit.cpp chat.cpp term.cpp

ALL = htmledit chat term


#
# Build everything...
#

all:	$(ALL)

htmledit: htmledit.o
	$(CXX) $(LDFLAGS) -o htmledit  htmledit.o  $(LIBS)

chat: chat.o
	$(CXX) $(LDFLAGS) -o chat chat.o $(LIBS)

term: term.o
	$(CXX) $(LDFLAGS) -o term term.o  $(LIBS)

#
#
# Install everything...
#

install: 


#
# Clean output files...
#

clean:
	$(RM) $(ALL) *.o *.html


#
# End of "$Id: Makefile,v 1.2 2000/06/25 22:00:17 easysw Exp $".
#
