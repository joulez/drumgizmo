
dummysources = \
	dummy.cc

if HAVE_OUTPUT_DUMMY

dummyltlibs = dummy.la
dummybuildsources = $(dummysources)

else

dummyltlibs =
dummybuildsources =

endif

EXTRA_DIST = $(dummysources)

lib_LTLIBRARIES = $(dummyltlibs)

libdir = $(OUTPUT_PLUGIN_DIR)

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include
dummy_la_LDFLAGS = -module -avoid-version -shared
dummy_la_LIBADD =
dummy_la_SOURCES = $(dummybuildsources)

install-exec-hook:
	rm -f $(DESTDIR)$(libdir)/dummy.la
