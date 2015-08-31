DISTDIRS = input output

if ENABLE_CLI

include $(top_srcdir)/src/Makefile.am.drumgizmo 

SUBDIRS = input output

bin_PROGRAMS = drumgizmo

drumgizmo_LDADD = $(DRUMGIZMO_LIBS) $(PTHREAD_LIBS) -ldl $(JACK_LIBS)

drumgizmo_CXXFLAGS = $(SNDFILE_CXXFLAGS) $(PTHREAD_CFLAGS) $(EXPAT_CFLAGS) \
	-I$(top_srcdir)/include -I$(top_srcdir)/src \
	-I$(top_srcdir)/hugin -DWITH_HUG_MUTEX $(JACK_CFLAGS) $(SSEFLAGS) \
	-DINPUT_PLUGIN_DIR=\"${INPUT_PLUGIN_DIR}\" \
	-DOUTPUT_PLUGIN_DIR=\"${OUTPUT_PLUGIN_DIR}\"

drumgizmo_SOURCES = \
	audioinputenginedl.cc \
	audiooutputenginedl.cc \
	drumgizmoc.cc \
	jackclient.cc \
	$(DRUMGIZMO_SOURCES) \
	$(top_srcdir)/hugin/hugin.c

EXTRA_DIST = \
	audioinputenginedl.h \
	audiooutputenginedl.h \
	drumgizmoc.h \
	jackclient.h

endif
