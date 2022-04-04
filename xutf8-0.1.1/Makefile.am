
lib_LTLIBRARIES= libXutf8.la
pkginclude_HEADERS = Xutf8.h

libXutf8_la_LDFLAGS = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) 
#	-release $(LT_RELEASE)

libXutf8_la_SOURCES= case.c is_spacing.c keysym2Ucs.c utf8Utils.c utf8Wrap.c \
	utf8Input.c is_right2left.c

noinst_PROGRAMS = test 
test_SOURCES = test.c
test_LDADD = .libs/libXutf8.a @X_LIBS@ -lX11

#test2_SOURCES = test2.c
#test2_LDADD =  @X_LIBS@ -lX11
