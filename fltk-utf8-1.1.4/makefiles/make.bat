echo Making FLTK-UTF8 for 16bits DOS !!!
echo #define BORDER_WIDTH 2  >> config.h
cd src
wmake /u NANO_X=1 HAVE_XUTF8=1 -f makefile.wat %1
cd ..
cd test
wmake /u NANO_X=1 HAVE_XUTF8=1 -f makefile.wat %1
cd ..
erase config.h
echo Done.
