# Rules for the test code (use `make check` to execute)
include $(top_srcdir)/src/Makefile.am.drumgizmo

TESTS = engine gui resampler lv2

check_PROGRAMS = $(TESTS)

engine_CXXFLAGS = -DOUTPUT=\"engine\" $(CPPUNIT_CFLAGS) \
	-I$(top_srcdir)/src -I$(top_srcdir)/include \
	-I$(top_srcdir)/hugin -DDISABLE_HUGIN
engine_CFLAGS = -DDISABLE_HUGIN
engine_LDFLAGS = $(CPPUNIT_LIBS) $(DRUMGIZMO_LIBS) $(PTHREAD_LIBS)
engine_SOURCES = \
	$(DRUMGIZMO_SOURCES) \
	$(top_srcdir)/hugin/hugin.c \
	test.cc \
	engine.cc

gui_CXXFLAGS = -DOUTPUT=\"gui\" $(CPPUNIT_CFLAGS)
gui_LDFLAGS = $(CPPUNIT_LIBS)
gui_SOURCES = \
	test.cc \
	gui.cc

resampler_CXXFLAGS = -DOUTPUT=\"resampler\" $(CPPUNIT_CFLAGS) \
	$(ZITA_CXXFLAGS) $(SAMPLERATE_CFLAGS) \
	-I$(top_srcdir)/hugin -DDISABLE_HUGIN
resampler_LDFLAGS = $(ZITA_LIBS) $(SAMPLERATE_LIBS) $(CPPUNIT_LIBS)
resampler_SOURCES = \
	$(top_srcdir)/src/chresampler.cc \
	test.cc \
	resampler.cc

lv2_CXXFLAGS = -DOUTPUT=\"lv2\" $(CPPUNIT_CFLAGS) \
	 `pkg-config --cflags serd-0` `pkg-config --cflags lilv-0` \
	-DLV2_PATH=\"$(libdir)/lv2\"
lv2_LDFLAGS = $(CPPUNIT_LIBS) `pkg-config --libs serd-0` \
	`pkg-config --libs lilv-0` -lcrypto
lv2_SOURCES = \
	test.cc \
	lv2_test_host.cc \
	lv2.cc
