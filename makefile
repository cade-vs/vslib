### MAKEMAKE STARTS HERE #########################################
#
# Created by makemake.pl on Sat Aug 17 15:26:08 2002
#
##################################################################

### GLOBAL TARGETS ###############################################

default: all

re: rebuild


li: link

all: libvslib.a libvscon.a test 

clean: clean-libvslib.a clean-libvscon.a clean-test 

rebuild: rebuild-libvslib.a rebuild-libvscon.a rebuild-test 

link: link-libvslib.a link-libvscon.a link-test 

### GLOBAL DEFS ##################################################

MKDIR      = mkdir -p
RMDIR      = rm -rf
RMFILE     = rm -f

### TARGET 0: libvslib.a #########################################

CC_0      = g++
LD_0      = g++
AR_0      = ar rv
RANLIB_0  = ranlib
CFLAGS_0  = 
CCFLAGS_0 = -I. -O2 $(CCDEF)
LDFLAGS_0 = $(LDDEF)
ARFLAGS_0 = 
TARGET_0  = libvslib.a

### SOURCES FOR TARGET 0: libvslib.a #################################

SRC_0= \
     clusters.cpp \
     dlog.cpp \
     eval.cpp \
     fnmatch2.cpp \
     getopt2.cpp \
     regexp3.cpp \
     scroll.cpp \
     vscrc.cpp \
     vslib.cpp \
     vstring.cpp \
     vstrlib.cpp \
     vsuti.cpp \

#### OBJECTS FOR TARGET 0: libvslib.a ################################

OBJ_0= \
     .OBJ.0.libvslib.a/clusters.o \
     .OBJ.0.libvslib.a/dlog.o \
     .OBJ.0.libvslib.a/eval.o \
     .OBJ.0.libvslib.a/fnmatch2.o \
     .OBJ.0.libvslib.a/getopt2.o \
     .OBJ.0.libvslib.a/regexp3.o \
     .OBJ.0.libvslib.a/scroll.o \
     .OBJ.0.libvslib.a/vscrc.o \
     .OBJ.0.libvslib.a/vslib.o \
     .OBJ.0.libvslib.a/vstring.o \
     .OBJ.0.libvslib.a/vstrlib.o \
     .OBJ.0.libvslib.a/vsuti.o \

### TARGET DEFINITION FOR TARGET 0: libvslib.a #######################

.OBJ.0.libvslib.a: 
	$(MKDIR) .OBJ.0.libvslib.a

libvslib.a: .OBJ.0.libvslib.a $(OBJ_0)
	$(AR_0) $(ARFLAGS_0) $(TARGET_0) $(OBJ_0)
	$(RANLIB_0) $(TARGET_0)

clean-libvslib.a: 
	$(RMFILE) $(TARGET_0)
	$(RMDIR) .OBJ.0.libvslib.a

rebuild-libvslib.a: clean-libvslib.a libvslib.a

link-libvslib.a: .OBJ.0.libvslib.a $(OBJ_0)
	$(RMFILE) libvslib.a
	$(AR_0) $(ARFLAGS_0) $(TARGET_0) $(OBJ_0)
	$(RANLIB_0) $(TARGET_0)

### TARGET OBJECTS FOR TARGET 0: libvslib.a ##########################

.OBJ.0.libvslib.a/clusters.o:  clusters.cpp clusters.h
	$(CC_0) $(CFLAGS_0) $(CCFLAGS_0) -c clusters.cpp -o .OBJ.0.libvslib.a/clusters.o
.OBJ.0.libvslib.a/dlog.o:  dlog.cpp dlog.h
	$(CC_0) $(CFLAGS_0) $(CCFLAGS_0) -c dlog.cpp -o .OBJ.0.libvslib.a/dlog.o
.OBJ.0.libvslib.a/eval.o:  eval.cpp eval.h
	$(CC_0) $(CFLAGS_0) $(CCFLAGS_0) -c eval.cpp -o .OBJ.0.libvslib.a/eval.o
.OBJ.0.libvslib.a/fnmatch2.o:  fnmatch2.cpp fnmatch2.h
	$(CC_0) $(CFLAGS_0) $(CCFLAGS_0) -c fnmatch2.cpp -o .OBJ.0.libvslib.a/fnmatch2.o
.OBJ.0.libvslib.a/getopt2.o:  getopt2.cpp getopt2.h
	$(CC_0) $(CFLAGS_0) $(CCFLAGS_0) -c getopt2.cpp -o .OBJ.0.libvslib.a/getopt2.o
.OBJ.0.libvslib.a/regexp3.o:  regexp3.cpp
	$(CC_0) $(CFLAGS_0) $(CCFLAGS_0) -c regexp3.cpp -o .OBJ.0.libvslib.a/regexp3.o
.OBJ.0.libvslib.a/scroll.o:  scroll.cpp scroll.h
	$(CC_0) $(CFLAGS_0) $(CCFLAGS_0) -c scroll.cpp -o .OBJ.0.libvslib.a/scroll.o
.OBJ.0.libvslib.a/vscrc.o:  vscrc.cpp vsuti.h target.h vstring.h
	$(CC_0) $(CFLAGS_0) $(CCFLAGS_0) -c vscrc.cpp -o .OBJ.0.libvslib.a/vscrc.o
.OBJ.0.libvslib.a/vslib.o:  vslib.cpp
	$(CC_0) $(CFLAGS_0) $(CCFLAGS_0) -c vslib.cpp -o .OBJ.0.libvslib.a/vslib.o
.OBJ.0.libvslib.a/vstring.o:  vstring.cpp vstring.h
	$(CC_0) $(CFLAGS_0) $(CCFLAGS_0) -c vstring.cpp -o .OBJ.0.libvslib.a/vstring.o
.OBJ.0.libvslib.a/vstrlib.o:  vstrlib.cpp vstrlib.h regexp3.h vstring.h
	$(CC_0) $(CFLAGS_0) $(CCFLAGS_0) -c vstrlib.cpp -o .OBJ.0.libvslib.a/vstrlib.o
.OBJ.0.libvslib.a/vsuti.o:  vsuti.cpp vsuti.h target.h vstring.h regexp3.h
	$(CC_0) $(CFLAGS_0) $(CCFLAGS_0) -c vsuti.cpp -o .OBJ.0.libvslib.a/vsuti.o

### TARGET 1: libvscon.a #########################################

CC_1      = g++
LD_1      = g++
AR_1      = ar rv
RANLIB_1  = ranlib
CFLAGS_1  = 
CCFLAGS_1 = -I. -I/usr/include/ncurses -O2 $(CCDEF)
LDFLAGS_1 = $(LDDEF)
ARFLAGS_1 = 
TARGET_1  = libvscon.a

### SOURCES FOR TARGET 1: libvscon.a #################################

SRC_1= \
     ansiterm.cpp \
     conmenu.cpp \
     form_in.cpp \
     unicon.cpp \

#### OBJECTS FOR TARGET 1: libvscon.a ################################

OBJ_1= \
     .OBJ.1.libvscon.a/ansiterm.o \
     .OBJ.1.libvscon.a/conmenu.o \
     .OBJ.1.libvscon.a/form_in.o \
     .OBJ.1.libvscon.a/unicon.o \

### TARGET DEFINITION FOR TARGET 1: libvscon.a #######################

.OBJ.1.libvscon.a: 
	$(MKDIR) .OBJ.1.libvscon.a

libvscon.a: .OBJ.1.libvscon.a $(OBJ_1)
	$(AR_1) $(ARFLAGS_1) $(TARGET_1) $(OBJ_1)
	$(RANLIB_1) $(TARGET_1)

clean-libvscon.a: 
	$(RMFILE) $(TARGET_1)
	$(RMDIR) .OBJ.1.libvscon.a

rebuild-libvscon.a: clean-libvscon.a libvscon.a

link-libvscon.a: .OBJ.1.libvscon.a $(OBJ_1)
	$(RMFILE) libvscon.a
	$(AR_1) $(ARFLAGS_1) $(TARGET_1) $(OBJ_1)
	$(RANLIB_1) $(TARGET_1)

### TARGET OBJECTS FOR TARGET 1: libvscon.a ##########################

.OBJ.1.libvscon.a/ansiterm.o:  ansiterm.cpp ansiterm.h
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c ansiterm.cpp -o .OBJ.1.libvscon.a/ansiterm.o
.OBJ.1.libvscon.a/conmenu.o:  conmenu.cpp conmenu.h
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c conmenu.cpp -o .OBJ.1.libvscon.a/conmenu.o
.OBJ.1.libvscon.a/form_in.o:  form_in.cpp form_in.h unicon.h target.h vstring.h \
 clusters.h scroll.h
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c form_in.cpp -o .OBJ.1.libvscon.a/form_in.o
.OBJ.1.libvscon.a/unicon.o:  unicon.cpp unicon.h target.h
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c unicon.cpp -o .OBJ.1.libvscon.a/unicon.o

### TARGET 2: test #########################################

CC_2      = g++
LD_2      = g++
AR_2      = ar rv
RANLIB_2  = ranlib
CFLAGS_2  = 
CCFLAGS_2 = -g -I. $(CCDEF) -DTEST
LDFLAGS_2 = -g -L. -lvslib -lvscon -lncurses $(LDDEF)
ARFLAGS_2 = 
TARGET_2  = test

### SOURCES FOR TARGET 2: test #################################

SRC_2= \
     vslib.cpp \

#### OBJECTS FOR TARGET 2: test ################################

OBJ_2= \
     .OBJ.2.test/vslib.o \

### TARGET DEFINITION FOR TARGET 2: test #######################

.OBJ.2.test: 
	$(MKDIR) .OBJ.2.test

test: .OBJ.2.test $(OBJ_2)
	$(LD_2) $(OBJ_2) $(LDFLAGS_2) -o $(TARGET_2)

clean-test: 
	$(RMFILE) $(TARGET_2)
	$(RMDIR) .OBJ.2.test

rebuild-test: clean-test test

link-test: .OBJ.2.test $(OBJ_2)
	$(RMFILE) test
	$(LD_2) $(OBJ_2) $(LDFLAGS_2) -o $(TARGET_2)

### TARGET OBJECTS FOR TARGET 2: test ##########################

.OBJ.2.test/vslib.o:  vslib.cpp
	$(CC_2) $(CFLAGS_2) $(CCFLAGS_2) -c vslib.cpp -o .OBJ.2.test/vslib.o


### MAKEMAKE ENDS HERE ###########################################
