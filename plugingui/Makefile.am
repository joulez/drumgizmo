noinst_PROGRAMS = plugingui rcgen

include Makefile.am.plugingui
#include $(top_srcdir)/src/Makefile.am.drumgizmo

plugingui_LDADD = $(SNDFILE_LIBS) $(PTHREAD_LIBS) $(EXPAT_LIBS) \
	 -ldl $(PLUGIN_GUI_LIBS)

plugingui_CXXFLAGS = $(SNDFILE_CXXFLAGS) $(PTHREAD_CFLAGS) $(EXPAT_CFLAGS) \
	-I$(top_srcdir)/include $(PLUGIN_GUI_CFLAGS) -I$(top_srcdir)/src \
	-DSTANDALONE -DUSE_THREAD

plugingui_CFLAGS = $(plugingui_CXXFLAGS)

plugingui_SOURCES = \
	$(PLUGIN_GUI_SOURCES) \
	$(top_srcdir)/src/thread.cc \
	$(top_srcdir)/src/semaphore.cc \
	$(top_srcdir)/src/mutex.cc \
	$(top_srcdir)/src/messagehandler.cc \
	$(top_srcdir)/src/messagereceiver.cc

rcgen_SOURCES = rcgen.cc

EXTRA_DIST = \
	button.h \
	checkbox.h \
	colour.h \
	combobox.h \
	directory.h \
	eventhandler.h \
	filebrowser.h \
	font.h \
	guievent.h \
	image.h \
	knob.h \
	label.h \
	led.h \
	lineedit.h \
	listbox.h \
	listboxbasic.h \
	listboxthin.h \
	nativewindow.h \
	nativewindow.h \
	nativewindow_win32.h \
	nativewindow_x11.h \
	painter.h \
	pixelbuffer.h \
	pluginconfig.h \
	plugingui.h \
	progressbar.h \
	resource.h \
	resource_data.h \
	scrollbar.h \
	slider.h \
	verticalline.h \
	widget.h \
	window.h \
	lodepng/lodepng.h

