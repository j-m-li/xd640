

CFILES = scandir.c vsnprintf.c fl_call_main.c flstring.c numericsort.c
CPPFILES = \
	Fl.cxx \
	Fl_Adjuster.cxx \
	Fl_Bitmap.cxx \
	Fl_Browser.cxx \
	Fl_Browser_.cxx \
	Fl_Browser_load.cxx \
	Fl_Box.cxx \
	Fl_Button.cxx \
	Fl_Chart.cxx \
	Fl_Check_Browser.cxx \
	Fl_Check_Button.cxx \
	Fl_Choice.cxx \
	Fl_Clock.cxx \
	Fl_Color_Chooser.cxx \
	Fl_Counter.cxx \
	Fl_Dial.cxx \
	Fl_Double_Window.cxx \
	Fl_File_Browser.cxx \
	Fl_File_Chooser.cxx \
	Fl_File_Chooser2.cxx \
	Fl_File_Icon.cxx \
	Fl_File_Input.cxx \
	Fl_Group.cxx \
	Fl_Help_View.cxx \
	Fl_Image.cxx \
	Fl_Input.cxx \
	Fl_Input_.cxx \
	Fl_Light_Button.cxx \
	Fl_Menu.cxx \
	Fl_Menu_.cxx \
	Fl_Menu_Bar.cxx \
	Fl_Menu_Button.cxx \
	Fl_Menu_Window.cxx \
	Fl_Menu_add.cxx \
	Fl_Menu_global.cxx \
	Fl_Multi_Label.cxx \
	Fl_Overlay_Window.cxx \
	Fl_Pack.cxx \
	Fl_Pixmap.cxx \
	Fl_Positioner.cxx \
	Fl_Preferences.cxx \
	Fl_Progress.cxx \
	Fl_Ps.cxx \
	Fl_Repeat_Button.cxx \
	Fl_Return_Button.cxx \
	Fl_Roller.cxx \
	Fl_Round_Button.cxx \
	Fl_Scroll.cxx \
	Fl_Scrollbar.cxx \
	Fl_Shaped_Window.cxx \
	Fl_Shared_Image.cxx \
	Fl_Single_Window.cxx \
	Fl_Slider.cxx \
	Fl_Tabs.cxx \
	Fl_Text_Buffer.cxx \
	Fl_Text_Display.cxx \
	Fl_Text_Editor.cxx \
	Fl_Tile.cxx \
	Fl_Tiled_Image.cxx \
	Fl_Tooltip.cxx \
	Fl_Valuator.cxx \
	Fl_Value_Input.cxx \
	Fl_Value_Output.cxx \
	Fl_Value_Slider.cxx \
	Fl_Widget.cxx \
	Fl_Window.cxx \
	Fl_Window_fullscreen.cxx \
	Fl_Window_hotspot.cxx \
	Fl_Window_iconize.cxx \
	Fl_Wizard.cxx \
	Fl_XBM_Image.cxx \
	Fl_XPM_Image.cxx \
	Fl_abort.cxx \
	Fl_add_idle.cxx \
	Fl_arg.cxx \
	Fl_compose.cxx \
	Fl_display.cxx \
	Fl_get_key.cxx \
	Fl_get_system_colors.cxx \
	Fl_grab.cxx \
	Fl_lock.cxx \
	Fl_own_colormap.cxx \
	Fl_visual.cxx \
	Fl_x.cxx \
	filename_absolute.cxx \
	filename_expand.cxx \
	filename_ext.cxx \
	filename_isdir.cxx \
	filename_list.cxx \
	filename_match.cxx \
	filename_setext.cxx \
	fl_arc.cxx \
	fl_arci.cxx \
	fl_ask.cxx \
	fl_boxtype.cxx \
	fl_color.cxx \
	fl_cursor.cxx \
	fl_curve.cxx \
	fl_diamond_box.cxx \
	fl_dnd.cxx \
	fl_draw.cxx \
	fl_draw_image.cxx \
	fl_draw_pixmap.cxx \
	fl_engraved_label.cxx \
	fl_file_dir.cxx \
	fl_font.cxx \
	fl_labeltype.cxx \
	fl_line_style.cxx \
	fl_oval_box.cxx \
	fl_overlay.cxx \
	fl_overlay_visual.cxx \
	fl_plastic.cxx \
	fl_read_image.cxx \
	fl_rect.cxx \
	fl_round_box.cxx \
	fl_rounded_box.cxx \
	fl_set_font.cxx \
	fl_set_fonts.cxx \
	fl_scroll_area.cxx \
	fl_shadow_box.cxx \
	fl_shortcut.cxx \
	fl_show_colormap.cxx \
	fl_symbols.cxx \
	fl_vertex.cxx \
	Flv_List.cxx \
	Flv_Table.cxx \
	Flve_Input.cxx \
	Flv_CStyle.cxx \
	Flv_Style.cxx \
	Flve_Check_Button.cxx \
	Flv_Data_Source.cxx \
	Flve_Combo.cxx \
	fl_utf8.cxx \
	fl_images_core.cxx \
	Fl_BMP_Image.cxx \
	Fl_File_Icon2.cxx \
	Fl_GIF_Image.cxx \
	Fl_Help_Dialog.cxx \
	Fl_JPEG_Image.cxx \
	Fl_PNG_Image.cxx \
	Fl_PNM_Image.cxx\
	forms_compatability.cxx \
	forms_bitmap.cxx \
	forms_free.cxx \
	forms_fselect.cxx \
	forms_pixmap.cxx \
	forms_timer.cxx





cflags = -ml -dNANO_X -bt=DOS -dMSDOS=1 -dHAVE_XUTF8=1\
 	-I.. -I..\..\microwin-0.89\src\include

OBJECTS = $(CFILES:.c=.obj) $(CPPFILES:.cxx=.obj) Fl_compose.obj
LOBJ = +$(CFILES:.c=.obj +) $(CPPFILES:.cxx=.obj +) Fl_compose.obj

all: fltk.lib .symbolic

fltk.lib:  $(OBJECTS) 
	erase fltk.lib
	wlib fltk.lib $(LOBJ)

clean : .SYMBOLIC
	@if exist *.exe del *.exe
	@if exist *.err del *.err
	@if exist *.lib del *.lib
	@if exist *.obj del *.obj
	@if exist *.map del *.map
	@if exist *.bak del *.bak

	

.cxx.obj:
	wpp $(cflags) $[@
	@wlib fltk.lib -+$^*.obj

.c.obj:
	wcc $(cflags) $[@
	@wlib fltk.lib -+$^*.obj

