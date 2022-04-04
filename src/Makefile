
CPPFILES = \
Xd6XmlTree.cpp \
Xd6XmlParser.cpp \
Xd6XmlDtd.cpp \
Xd6XmlStyle.cpp \
Xd6HtmlBlock.cpp \
Xd6HtmlSegment.cpp \
Xd6HtmlDisplay.cpp \
Xd6HtmlLine.cpp \
Xd6HtmlScroll.cpp \
Xd6HtmlFrame.cpp \
Xd6HtmlFrame_Drawings.cpp \
Xd6HtmlFrame_Edit.cpp \
Xd6HtmlFrame_Export.cpp \
Xd6HtmlFrame_Handlers.cpp \
Xd6HtmlTagImg.cpp \
Xd6Base64.cpp \
Xd6IconWindow.cpp \
Xd6ConfigFile.cpp \
Xd6CfgParser.cpp \
Xd6VirtualKeyboard.cpp \
Xd6TextParser.cpp \
Xd6HtmlTagA.cpp \
Xd6HtmlTagTable.cpp \
Xd6HtmlTagTd.cpp \
Xd6System.cpp \
Xd6HtmlDownload.cpp \
Xd6HtmlTagForm.cpp \
Xd6HtmlTagButton.cpp \
Xd6HtmlTagInputText.cpp \
Xd6DefaultFonts.cpp \
Xd6HtmlPrint.cpp \
Xd6HtmlView.cpp \
Xd6FindDialog.cpp \
Xd6SpellChoice.cpp \
Xd6Gif.cpp \
Xd6Png.cpp \
Xd6Jpeg.cpp \
Xd6Tabulator.cpp \
Xd6HtmlToRtf.cpp \
Xd6SvgTag.cpp \
Xd6MathMl.cpp \
Xd6Cancel.cpp \


CFILES = Xd6XmlUtils.c

CLEAN =

DEFS = -DPACKAGE=\"xd640\" -DLOCALEDIR=\"$(prefix)/share/locale\"

################################################################

include ../Makedefs

LIBRARY = ../lib/$(LIBNAME)

OBJECTS = $(CPPFILES:.cpp=.o) $(CFILES:.c=.o)

all:	$(LIBRARY) $(DSONAME)

$(LIBRARY): $(OBJECTS)
	echo $(LIBCOMMAND) $@ ...
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

libxd640.so.1 libxd640.sl.1: $(OBJECTS)
	echo $(DSOCOMMAND) $@ ...
	$(DSOCOMMAND) $@ $(OBJECTS)

clean :
	-@ rm -f *.o *.do $(DSONAME) $(LIBRARY) $(CLEAN) core *~ ../include/*~ makedepend cmap ../lib/*
	touch makedepend

depend:
	$(MAKEDEPEND) -I.. $(CXXFLAGS) $(CPPFILES) $(CFILES) > makedepend

include makedepend

################################################################

install: $(LIBRARY) $(DSONAME)
	echo "Installing libraries..."
	-mkdir -p $(libdir)
	rm -f $(libdir)/$(LIBNAME)
	-cp $(LIBRARY) $(libdir)
	-chmod 644 $(libdir)/$(LIBNAME)
	echo "Installing include files..."
	-mkdir -p $(includedir)
	rm -rf $(includedir)/xd640
	mkdir $(includedir)/xd640
	cp ../xd640/*.[hH] $(includedir)/xd640
	-chmod 755 $(includedir)/xd640
	-chmod 644 $(includedir)/xd640/*


do_not_install:
	if test "$(DSONAME)" = libxd640.so.1; then\
		rm -f $(libdir)/libxd640.so*;\
		cp libxd640.so.1 $(libdir); \
		chmod 755 $(libdir)/libxd640.so.1; \
		ln -s libxd640.so.1 $(libdir)/libxd640.so;\
	fi
	if test "$(DSONAME)" = libxd640.sl.1; then\
		rm -f $(libdir)/libxd640.sl*;\
		cp libxd640.sl.1 $(libdir); \
		chmod 755 $(libdir)/libxd640.sl.1; \
		ln -s libxd640.sl.1 $(libdir)/libxd640.sl;\
	fi
