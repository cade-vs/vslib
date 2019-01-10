noinst_LIBRARIES = libvslib.a libvscon.a libvscony.a
check_LIBRARIES = $(noinst_LIBRARIES)

AM_CXXFLAGS = -Wall

# DEFS is needed because we are using different sets of Makefiles
DEFS = -DHAVE_CONFIG_H

libvslib_a_SOURCES = \
     clusters.cpp \
     dlog.cpp \
     eval.cpp \
     fnmatch2.cpp \
     getopt2.cpp \
     scroll.cpp \
     vslib.cpp \
     vstring.cpp \
     vstrlib.cpp \
     vsuti.cpp \
     vscrc.cpp

#libvslib_a_LIBADD = @CURSES_LIBS@

# Rules for libvscony.a
libvscon_a_SOURCES = \
     ansiterm.cpp \
     conmenu.cpp \
     form_in.cpp \
     unicon.cpp

libvscon_CXXFLAGS = $(AM_CXXFLAGS)
libvscon_CXXFLAGS += @CURSES_CFLAGS@

# Rules for libvscony.a
libvscony_a_CXXFLAGS = $(AM_CXXFLAGS)
libvscony_a_CXXFLAGS += -I$(top_srcdir)/yascreen -DUSE_YASCREEN
libvscony_a_SOURCES = $(libvscon_a_SOURCES)

# needed for 'make dist' and 'make distcheck'
noinst_HEADERS = \
    dlog.h  \
    target.h  \
    vstring.h \
    clusters.h  \
    eval.h  \
    getopt2.h \
    vstrlib.h \
    fnmatch2.h  \
    scroll.h  \
    vslib.h \
    vsuti.h

# headers for libvscon    
noinst_HEADERS += \
  ansiterm.h  \
  unicon.h  \
  form_in.h \
  conmenu.h
