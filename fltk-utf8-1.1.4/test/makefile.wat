cflags = -ml -dNANO_X -bt=DOS -dMSDOS=1 -dHAVE_XUTF8=1\
 	-I.. -I..\..\microwin-0.89\src\include

CPPFILES =\
	adjuster.cxx \
	arc.cxx \
	ask.cxx \
	bitmap.cxx \
	boxtype.cxx \
	browser.cxx \
	button.cxx \
	buttons.cxx \
	checkers.cxx \
	clock.cxx \
	colbrowser.cxx \
	color_chooser.cxx \
	cube.cxx \
	CubeMain.cxx \
	CubeView.cxx \
	cursor.cxx \
	curve.cxx \
	demo.cxx \
	dnd.cxx \
	doublebuffer.cxx \
	editor.cxx \
	fast_slow.cxx \
	file_chooser.cxx \
	flv_testedit.cxx \
	fonts.cxx \
	forms.cxx \
	fractals.cxx \
	fullscreen.cxx \
	gl_overlay.cxx \
	glpuzzle.cxx \
	hello.cxx \
	help.cxx \
	iconize.cxx \
	image.cxx \
	inactive.cxx \
	input.cxx \
	keyboard.cxx \
	label.cxx \
	line_style.cxx \
	list_visuals.cxx \
	mandelbrot.cxx \
	menubar.cxx \
	message.cxx \
	minimum.cxx \
	navigation.cxx \
	output.cxx \
	overlay.cxx \
	pack.cxx \
	pixmap_browser.cxx \
	pixmap.cxx \
	preferences.cxx \
	radio.cxx \
	resizebox.cxx \
	resize.cxx \
	scroll.cxx \
	shape.cxx \
	shiny.cxx \
	subwindow.cxx \
	symbols.cxx \
	tabs.cxx \
	threads.cxx \
	tile.cxx \
	tiled_image.cxx \
	utf8.cxx \
	valuators.cxx
EXEEXT=.exe

ALL =	\
	adjuster$(EXEEXT) \
	arc$(EXEEXT) \
	ask$(EXEEXT) \
	bitmap$(EXEEXT) \
	boxtype$(EXEEXT) \
	browser$(EXEEXT) \
	button$(EXEEXT) \
	buttons$(EXEEXT) \
	checkers$(EXEEXT) \
	clock$(EXEEXT) \
	cursor$(EXEEXT) \
	curve$(EXEEXT) \
	doublebuffer$(EXEEXT) \
	editor$(EXEEXT) \
	fast_slow$(EXEEXT) \
	fonts$(EXEEXT) \
	forms$(EXEEXT) \
	hello$(EXEEXT) \
	help$(EXEEXT) \
	iconize$(EXEEXT) \
	inactive$(EXEEXT) \
	input$(EXEEXT) \
	keyboard$(EXEEXT) \
	label$(EXEEXT) \
	line_style$(EXEEXT) \
	mandelbrot$(EXEEXT) \
	menubar$(EXEEXT) \
	message$(EXEEXT) \
	minimum$(EXEEXT) \
	navigation$(EXEEXT) \
	output$(EXEEXT) \
	overlay$(EXEEXT) \
	pack$(EXEEXT) \
	pixmap$(EXEEXT) \
	pixmap_browser$(EXEEXT) \
	preferences$(EXEEXT) \
	radio$(EXEEXT) \
	resize$(EXEEXT) \
	resizebox$(EXEEXT) \
	scroll$(EXEEXT) \
	subwindow$(EXEEXT) \
	symbols$(EXEEXT) \
	tabs$(EXEEXT) \
	$(THREADS) \
	tile$(EXEEXT) \
	utf8$(EXEEXT) \
	valuators$(EXEEXT)

TOOBIG=	colbrowser$(EXEEXT) \
	list_visuals$(EXEEXT) \
	color_chooser$(EXEEXT) \
	demo$(EXEEXT) \
	dnd$(EXEEXT) \
	file_chooser$(EXEEXT) \
	image$(EXEEXT) \
	tiled_image$(EXEEXT) \


all:	$(ALL)

# All demos depend on the FLTK library...
$(ALL): ../src/fltk.lib

# General demos...
adjuster$(EXEEXT): adjuster.obj

arc$(EXEEXT): arc.obj

ask$(EXEEXT): ask.obj

bitmap$(EXEEXT): bitmap.obj

boxtype$(EXEEXT): boxtype.obj

browser$(EXEEXT): browser.obj

button$(EXEEXT): button.obj

buttons$(EXEEXT): buttons.obj

checkers$(EXEEXT): checkers.obj

clock$(EXEEXT): clock.obj

colbrowser$(EXEEXT): colbrowser.obj

color_chooser$(EXEEXT): color_chooser.obj

cursor$(EXEEXT): cursor.obj

curve$(EXEEXT): curve.obj

demo$(EXEEXT): demo.obj

doublebuffer$(EXEEXT): doublebuffer.obj

editor$(EXEEXT): editor.obj

fast_slow$(EXEEXT): fast_slow.obj

file_chooser$(EXEEXT): file_chooser.obj 

fonts$(EXEEXT): fonts.obj

forms$(EXEEXT): forms.obj

hello$(EXEEXT): hello.obj

help$(EXEEXT): help.obj 

iconize$(EXEEXT): iconize.obj

image$(EXEEXT): image.obj

inactive$(EXEEXT): inactive.obj

input$(EXEEXT): input.obj

keyboard$(EXEEXT): keyboard_ui.obj keyboard.obj
	wcl -k19000  -fe=keyboard.exe -ml -bt=DOS -dMSDOS=1 -dNANO_X=1 -dHAVE_XUTF8=1\
	-I=.. -I..\..\microwin-0.89\src\include keyboard_ui.obj\
	 keyboard.obj ..\..\microwin-0.89\src\lib\nanox.lib  ..\src\fltk.lib



label$(EXEEXT): label.obj

line_style$(EXEEXT): line_style.obj

list_visuals$(EXEEXT): list_visuals.obj

mandelbrot$(EXEEXT): mandelbrot_ui.obj mandelbrot.obj
	wcl -k19000  -fe=mandelbrot.exe -ml -bt=DOS -dMSDOS=1 -dNANO_X=1 -dHAVE_XUTF8=1\
	-I=.. -I..\..\microwin-0.89\src\include\
	 mandelbrot_ui.obj mandelbrot.obj\
	 ..\..\microwin-0.89\src\lib\nanox.lib  ..\src\fltk.lib

menubar$(EXEEXT): menubar.obj

message$(EXEEXT): message.obj

minimum$(EXEEXT): minimum.obj

navigation$(EXEEXT): navigation.obj

output$(EXEEXT): output.obj 

overlay$(EXEEXT): overlay.obj

pack$(EXEEXT): pack.obj

pixmap$(EXEEXT): pixmap.obj

pixmap_browser$(EXEEXT): pixmap_browser.obj 

preferences$(EXEEXT):	preferences.obj

radio$(EXEEXT): radio.obj

resize$(EXEEXT): resize.obj

resizebox$(EXEEXT): resizebox.obj

scroll$(EXEEXT): scroll.obj

subwindow$(EXEEXT): subwindow.obj

symbols$(EXEEXT): symbols.obj

tabs$(EXEEXT): tabs.obj

threads$(EXEEXT): threads.obj

tile$(EXEEXT): tile.obj

tiled_image$(EXEEXT): tiled_image.obj

valuators$(EXEEXT): valuators.obj


clean : .SYMBOLIC
	@if exist *.exe del *.exe
	@if exist *.err del *.err
	@if exist *.lib del *.lib
	@if exist *.objbj del *.objbj
	@if exist *.map del *.map

	


.cxx.obj:
	wpp $(cflags) $[@

.obj.exe:
	wcl -k19000  -ml -bt=DOS -dMSDOS=1 -dNANO_X=1 -dHAVE_XUTF8=1-I=.. -I..\..\microwin-0.89\src\include $[@ ..\..\microwin-0.89\src\lib\nanox.lib  ..\src\fltk.lib

