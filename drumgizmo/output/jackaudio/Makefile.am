
jackaudiosources = \
	jackaudio.cc \
	$(top_srcdir)/src/semaphore.cc \
	$(top_srcdir)/hugin/hugin.c

if HAVE_OUTPUT_JACKAUDIO

jackaudioltlibs = jackaudio.la
jackaudiobuildsources = $(jackaudiosources)

else

jackaudioltlibs =
jackaudiobuildsources =

endif

EXTRA_DIST = $(jackaudiosources)

lib_LTLIBRARIES = $(jackaudioltlibs)

libdir = $(OUTPUT_PLUGIN_DIR)

AM_CPPFLAGS = -I$(top_srcdir)/include $(JACK_CFLAGS) -I$(top_srcdir)/hugin -I$(top_srcdir)/src
jackaudio_la_LDFLAGS = $(JACK_LIBS) -module -avoid-version -shared
jackaudio_la_LIBADD =
jackaudio_la_SOURCES = $(jackaudiobuildsources)

install-exec-hook:
	rm -f $(DESTDIR)$(libdir)/jackaudio.la
