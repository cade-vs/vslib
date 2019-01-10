AM_CXXFLAGS = -Wall -I$(top_srcdir)/vslib

check_PROGRAMS = test_vslib

test_vslib_SOURCES = test.cpp

TESTS=$(check_PROGRAMS)

LDFLAGS = -L $(top_builddir)/vslib
LDADD =  -lvslib -lvscon @PCRE_LIBS@ @CURSES_LIBS@
