 X Desktop 640x480 16Mb
=======================

Xd640
-----

```
flclock		: a clock
fldesk		: desktop icons
fleffect	: laucher effect
flfile		: file utilities (link, remove, copy, properties)
flfm		: file manager (flfile user interface)
fllauncher	: menu bar
flmail		: mail client 
flnews		: news client 
flnotepad	: plain text editor (UTF-8)
flpaint		: bitmap picture editor (uncompressed GIF and Xpm)
flppp		: Internet dialer
flspider	: web browser (uses libcurl)
flwriter	: word processor (XHTML)
flshutdown	: computer shutdown dialog
```

We use fltk-utf8 as the widget library. 
FVWM 2 is our standard windows manager.
Debian is our default Linux distribution.
Test computer is a 486 DX/2 66MHz, 16Mb of RAM, 15" 640x480 256 colors screen.

```
Requiered software to compile Xd640 :
	- a standard GNU C++ development environement.
	- X11R6 headers and development libraries
	- Xutf8 	(http://www.lienher.org/jean-marc/xd640)
	- fltk-utf8 	(http://www.lienher.org/jean-marc/xd640)
	- openssl 	(http://www.openssl.org/)
	- libcurl	(http://curl.haxx.se/)
	- FreeType 2	(http://www.freetype.org/)
```

BUILD UNDER Debian 11:
---------------------

	# sudo apt install libcurl4-openssl-dev gettext build-essential git libx11-dev libgl1-mesa-dev libglu1-mesa-dev xorg-dev autoconf automake libtool linpng-dev  libz-dev libjpeg-dev 
	# sudo ldconfig
	# cd xd640*/
	
	# cd xutf8-0.1.1/
	# ./configure
	# make
	# sudo make install
	# cd ..

	# cd fltk-utf8-1.1.4/
	# ./configure
	# make
	# sudo make install
	# cd ..

	# ./configure
	# make
	# sudo make install
	# cd flwriter/
	# make flwriter-static 


If you want to install the software to an other location use this in place
of "./configure" :
	# ./configure --prefix=/usr --sysconfdir=/etc --mandir=/usr/share/man
