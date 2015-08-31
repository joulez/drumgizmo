
alsasources = \
	alsa.cc

if HAVE_OUTPUT_ALSA

alsaltlibs = alsa.la
alsabuildsources = $(alsasources)

else

alsaltlibs =
alsabuildsources =

endif

EXTRA_DIST = $(alsasources)

lib_LTLIBRARIES = $(alsaltlibs)

libdir = $(OUTPUT_PLUGIN_DIR)

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include $(ALSA_CFLAGS)
alsa_la_LDFLAGS = $(ALSA_LIBS) -module -avoid-version -shared
alsa_la_LIBADD =
alsa_la_SOURCES = $(alsabuildsources)

install-exec-hook:
	rm -f $(DESTDIR)$(libdir)/alsa.la
