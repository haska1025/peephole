check_PROGRAMS = ph_ut
TESTS = $(check_PROGRAMS)

ph_ut_SOURCES = ph_ut.cpp \
				test_ph.cpp

ph_ut_CPPFLAGS = $(LIBUV_CFLAGS) $(CPPUNIT_CFLAGS)
ph_ut_LDFLAGS = $(top_builddir)/src/libpeephole/.libs/libpeephole.a $(LIBUV_LIBS) $(CPPUNIT_LIBS)

