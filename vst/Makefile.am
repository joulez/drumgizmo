include $(top_srcdir)/plugingui/Makefile.am.plugingui
include $(top_srcdir)/src/Makefile.am.drumgizmo

VST_BASE = ${VST_SOURCE_PATH}
VST_SRC_BASE = ${VST_BASE}/public.sdk/source/vst2.x/
VST_SOURCES = \
	${VST_SRC_BASE}/audioeffectx.cpp \
	${VST_SRC_BASE}/audioeffect.cpp \
	${VST_SRC_BASE}/vstplugmain.cpp

AM_CPPFLAGS = -I$(top_srcdir)/hugin -DWITH_HUG_SYSLOG -DWITH_HUG_MUTEX

EXTRA_DIST = \
	Makefile.mingw32.in \
	constants.h \
	drumgizmo_vst.cc \
	drumgizmo_vst.h \
	input_vst.cc \
	input_vst.h \
	output_vst.cc \
	output_vst.h

if ENABLE_VST
plugin_LTLIBRARIES =
plugindir = $(prefix)/lib/vst
plugin_LTLIBRARIES += libdrumgizmo_vst.la
noinst_LTLIBRARIES = libdg.la
endif

libdg_la_SOURCES = \
	$(DRUMGIZMO_SOURCES) \
	$(PLUGIN_GUI_SOURCES)

libdg_la_LDFLAGS = -all-static -static -static-libgcc
libdg_la_LIBADD = $(PLUGIN_GUI_LIBS) $(DRUMGIZMO_LIBS)
libdg_la_CXXFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/plugingui \
	-I$(top_srcdir)/include $(SNDFILE_CXXFLAGS) \
	$(PTHREAD_CFLAGS) $(EXPAT_CFLAGS) $(VST_CPPFLAGS) \
	$(PLUGIN_GUI_CFLAGS) $(SSEFLAGS)\
	-DUSE_THREAD $(SAMPLERATE_CFLAGS)

nodist_libdrumgizmo_vst_la_SOURCES = \
  $(VST_SOURCES)

libdrumgizmo_vst_la_SOURCES = \
	drumgizmo_vst.cc \
	input_vst.cc \
	output_vst.cc

libdrumgizmo_vst_la_LDFLAGS = -no-undefined -shared
libdrumgizmo_vst_la_LIBADD = libdg.la
libdrumgizmo_vst_la_CXXFLAGS = -w $(VST_CPPFLAGS) \
	-I$(top_srcdir)/src -I$(top_srcdir)/include -I$(top_srcdir)/plugingui

