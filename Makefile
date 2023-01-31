# use this to disable flto optimizations:
#   make NO_FLTO=1
# and this to enable verbose mode:
#   make V=1

AR?=gcc-ar
STRIP?=strip
RANLIB?=ranlib
PKG_CONFIG?=pkg-config

PCRE08_CC?=$(shell $(PKG_CONFIG) --cflags libpcre2-8)
PCRE08_LD?=$(shell $(PKG_CONFIG) --libs libpcre2-8)
PCRE32_CC?=$(shell $(PKG_CONFIG) --cflags libpcre2-32)
PCRE32_LD?=$(shell $(PKG_CONFIG) --libs libpcre2-32)
YASCREEN_CC?=$(shell $(PKG_CONFIG) --cflags yascreen)
YASCREEN_LD?=$(shell $(PKG_CONFIG) --libs yascreen)
NCURSES_CC?=$(shell $(PKG_CONFIG) --cflags ncursesw)
NCURSES_LD?=$(shell $(PKG_CONFIG) --libs ncursesw)

ALIBS:=libvslib.a libvscon.a libvscony.a
ifeq ($(YASCREEN_LD),)
	ALIBS:=$(filter-out libvscony.a,$(ALIBS))
endif
ifeq ($(NCURSES_LD),)
	ALIBS:=$(filter-out libvscon.a,$(ALIBS))
endif

all: $(ALIBS) t/test

SRCS:=\
	ansiterm.cpp \
	clusters.cpp \
	conmenu.cpp \
	dlog.cpp \
	eval.cpp \
	form_in.cpp \
	getopt2.cpp \
	scroll.cpp \
	unicon.cpp \
	vscrc.cpp \
	vslib.cpp \
	vsuti.cpp \
	t/test.cpp
OBJS:=$(SRCS:.cpp=.o)
DEPS:=$(OBJS:.o=.d) $(OBJS:.o=.y.d)

ifndef NO_FLTO
CXXFLAGS?=-O3 -fno-stack-protector -mno-stackrealign
CXXFLAGS+=-flto=auto
else
CXXFLAGS?=-O3 -fno-stack-protector -mno-stackrealign
endif

# some architectures do not have -mno-stackrealign
HAVESREA:=$(shell if $(CXX) -mno-stackrealign -xc -c /dev/null -o /dev/null >/dev/null 2>/dev/null;then echo yes;else echo no;fi)
# old comiplers do not have -Wdate-time
HAVEWDTI:=$(shell if $(CXX) -Wdate-time -xc -c /dev/null -o /dev/null >/dev/null 2>/dev/null;then echo yes;else echo no;fi)

MYCXXFLAGS:=$(CPPFLAGS) $(CXXFLAGS) $(PCRE08_CC) $(PCRE32_CC) $(YASCREEN_CC) $(NCURSES_CC) -Wall -Wextra -Wformat -Werror=format-security -Wdate-time -D_FORTIFY_SOURCE=2 -fPIE -I. -I../vstring
ifeq ("$(HAVESREA)","no")
MYCXXFLAGS:=$(filter-out -mno-stackrealign,$(MYCXXFLAGS))
endif
ifeq ("$(HAVEWDTI)","no")
MYCXXFLAGS:=$(filter-out -Wdate-time,$(MYCXXFLAGS))
endif

MYLDFLAGS:=$(MYCXXFLAGS) $(LDFLAGS) -fPIE -pie
MYLIBS:=$(LIBS) $(PCRE08_LD) $(PCRE32_LD)

ifeq ("$(V)","1")
Q:=
E:=@true
else
Q:=@
E:=@echo
endif

%.o: %.cpp
	$(E) DE $@
	$(Q)$(CXX) $(MYCXXFLAGS) -D_UNICON_USE_CURSES_ -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(E) CXX $@
	$(Q)$(CXX) $(MYCXXFLAGS) -D_UNICON_USE_CURSES_ -c -o $@ $<

%.y.o: %.cpp
	$(E) DE $@
	$(Q)$(CXX) $(MYCXXFLAGS) -D_UNICON_USE_YASCREEN_ -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(E) CXX $@
	$(Q)$(CXX) $(MYCXXFLAGS) -D_UNICON_USE_YASCREEN_ -c -o $@ $<

VSLIBOBJ:=\
	clusters.o \
	dlog.o \
	eval.o \
	getopt2.o \
	scroll.o \
	vslib.o \
	vsuti.o \
	vscrc.o

libvslib.a: $(VSLIBOBJ)
	$(Q)rm -f $@
	$(E) AR $@
	$(Q)$(AR) rv $@ $+
	$(E) RANLIB $@
	$(Q)$(RANLIB) $@

VSCONOBJ:=\
	conmenu.o \
	form_in.o \
	unicon.o

VSCONYOBJ:=$(VSCONOBJ:.o=.y.o)

libvscon.a: $(VSCONOBJ)
	$(Q)rm -f $@
	$(E) AR $@
	$(Q)$(AR) rv $@ $+
	$(E) RANLIB $@
	$(Q)$(RANLIB) $@

libvscony.a: $(VSCONYOBJ)
	$(Q)rm -f $@
	$(E) AR $@
	$(Q)$(AR) rv $@ $+
	$(E) RANLIB $@
	$(Q)$(RANLIB) $@

t/test: t/test.o ../vstring/libvstring.a libvslib.a
	$(E) LD $@
	$(Q)$(CXX) -o $@ $(MYLDFLAGS) $< $(MYLIBS) -L../vstring -lvstring -L. -lvslib

clean:
	$(E) CLEAN
	$(Q) rm -f *.a *.o *.d t/test t/*.o

re:
	$(Q)$(MAKE) --no-print-directory clean
	$(Q)$(MAKE) --no-print-directory -j

-include $(DEPS)

.PHONY: all clean re
