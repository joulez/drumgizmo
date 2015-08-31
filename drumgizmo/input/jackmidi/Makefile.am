
jackmidisources = \
	jackmidi.cc \
	$(top_srcdir)/src/midimapper.cc \
	$(top_srcdir)/src/midimapparser.cc \
	$(top_srcdir)/src/saxparser.cc \
	$(top_srcdir)/hugin/hugin.c

if HAVE_INPUT_JACKMIDI

jackmidiltlibs = jackmidi.la
jackmidibuildsources = $(jackmidisources)

else

jackmidiltlibs =
jackmidibuildsources =

endif

EXTRA_DIST = $(jackmidisources)

lib_LTLIBRARIES = $(jackmidiltlibs)

libdir = $(INPUT_PLUGIN_DIR)

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src -I$(top_srcdir)/hugin
jackmidi_la_LDFLAGS = -module -avoid-version -shared
jackmidi_la_LIBADD =
jackmidi_la_SOURCES = $(jackmidibuildsources)

install-exec-hook:
	rm -f $(DESTDIR)$(libdir)/jackmidi.la
