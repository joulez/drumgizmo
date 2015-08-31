
testsources = \
	test.cc

if HAVE_INPUT_TEST

testltlibs = test.la
testbuildsources = $(testsources)

else

testltlibs =
testbuildsources =

endif

EXTRA_DIST = $(testsources)

lib_LTLIBRARIES = $(testltlibs)

libdir = $(INPUT_PLUGIN_DIR)

AM_CPPFLAGS = -I$(top_srcdir)/include
test_la_LDFLAGS = -module -avoid-version -shared
test_la_LIBADD = 
test_la_SOURCES = $(testbuildsources)

install-exec-hook:
	rm -f $(DESTDIR)$(libdir)/test.la
