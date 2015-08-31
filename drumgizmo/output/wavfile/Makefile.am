
wavfilesources = \
	wavfile.cc

if HAVE_OUTPUT_WAVFILE

wavfileltlibs = wavfile.la
wavfilebuildsources = $(wavfilesources)

else

wavfileltlibs =
wavfilebuildsources =

endif

EXTRA_DIST = $(wavfilesources)

lib_LTLIBRARIES = $(wavfileltlibs)

libdir = $(OUTPUT_PLUGIN_DIR)

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include $(SNDFILE_CFLAGS)
wavfile_la_LDFLAGS = $(SNDFILE_LIBS) -module -avoid-version -shared
wavfile_la_LIBADD =
wavfile_la_SOURCES = $(wavfilebuildsources)

install-exec-hook:
	rm -f $(DESTDIR)$(libdir)/wavfile.la
