
midifilesources = \
	midifile.cc \
	$(top_srcdir)/src/midimapper.cc \
	$(top_srcdir)/src/midimapparser.cc \
	$(top_srcdir)/src/saxparser.cc \
	$(top_srcdir)/hugin/hugin.c


if HAVE_INPUT_MIDIFILE

midifileltlibs = midifile.la
midifilebuildsources = $(midifilesources)

else

midifileltlibs =
midifilebuildsources =

endif

EXTRA_DIST = $(midifilesources)

lib_LTLIBRARIES = $(midifileltlibs)

libdir = $(INPUT_PLUGIN_DIR)

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include $(SMF_CFLAGS) \
	$(EXPAT_CFLAGS) -I$(top_srcdir)/hugin -DWITH_HUG_MUTEX
midifile_la_LDFLAGS = $(SMF_LIBS) $(EXPAT_LIBS) -module -avoid-version -shared
midifile_la_LIBADD =
midifile_la_SOURCES = $(midifilebuildsources)

install-exec-hook:
	rm -f $(DESTDIR)$(libdir)/midifile.la
