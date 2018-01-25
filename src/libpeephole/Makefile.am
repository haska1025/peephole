lib_LTLIBRARIES = libpeephole.la

libpeephole_la_SOURCES = ph_iothread.cpp \
						 log_manager.cpp \
						 tcplistener.cpp \
						 tcphandler.cpp \
						 iohandler.cpp \
						 iobuffer.cpp

libpeephole_la_CPPFLAGS = $(LIBUV_CFLAGS) 
libpeephole_la_LDFLAGS = $(LIBUV_LIBS) 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libpeephole.pc

EXTRA_DIST = libpeephole.pc.in
