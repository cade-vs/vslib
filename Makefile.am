noinst_LIBRARIES = libvslib.a libvscon.a
check_LIBRARIES = $(noinst_LIBRARIES)

AM_CXXFLAGS = -Wall

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

libvscon_a_SOURCES = \
     ansiterm.cpp \
     conmenu.cpp \
     form_in.cpp \
     unicon.cpp

libvscon_a_CXXFLAGS = $(AM_CXXFLAGS)

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
