
### MAKEMAKE STARTS HERE #######################################################


### Created by makemake.pl on Fri Nov 19 02:20:53 2021 #########################


### GLOBAL TARGETS #############################################################

default: mm_update  libvslib.a  libvscon.a  libvscony.a 

re: mm_update rebuild

li: mm_update link

all: mm_update libvslib.a libvscon.a libvscony.a test 

clean: mm_update clean-libvslib.a clean-libvscon.a clean-libvscony.a clean-test 

rebuild: mm_update rebuild-libvslib.a rebuild-libvscon.a rebuild-libvscony.a rebuild-test 

link: mm_update link-libvslib.a link-libvscon.a link-libvscony.a link-test 

### GLOBAL (AND USER) DEFS #####################################################


AR ?= ar
LD = $(CXX)
MKDIR = mkdir -p
RANLIB ?= ranlib
RMDIR = rm -rf
RMFILE = rm -f
SRC = *.c *.cpp *.cc *.cxx


### TARGET 1: libvslib.a #######################################################

CC_1       = $(CXX)
LD_1       = $(CXX)
AR_1       = $(AR) rv
RANLIB_1   = $(RANLIB)
CCFLAGS_1  = -I../vstring -I. -O2 $(CFLAGS) $(CPPFLAGS) $(CCDEF)  
LDFLAGS_1  = $(LDFLAGS) $(LDDEF) 
DEPFLAGS_1 = 
ARFLAGS_1  = 
TARGET_1   = libvslib.a

### SOURCES FOR TARGET 1: libvslib.a ###########################################

SRC_1= \
     clusters.cpp \
     dlog.cpp \
     eval.cpp \
     fnmatch2.cpp \
     getopt2.cpp \
     scroll.cpp \
     vslib.cpp \
     vsuti.cpp \
     vscrc.cpp \

#### OBJECTS FOR TARGET 1: libvslib.a ##########################################

OBJ_1= \
     .OBJ.libvslib.a/clusters.o \
     .OBJ.libvslib.a/dlog.o \
     .OBJ.libvslib.a/eval.o \
     .OBJ.libvslib.a/fnmatch2.o \
     .OBJ.libvslib.a/getopt2.o \
     .OBJ.libvslib.a/scroll.o \
     .OBJ.libvslib.a/vslib.o \
     .OBJ.libvslib.a/vsuti.o \
     .OBJ.libvslib.a/vscrc.o \

### TARGET DEFINITION FOR TARGET 1: libvslib.a #################################

.OBJ.libvslib.a: 
	$(MKDIR) .OBJ.libvslib.a

libvslib.a:   .OBJ.libvslib.a $(OBJ_1)
	$(AR_1) $(ARFLAGS_1) $(TARGET_1) $(OBJ_1)
	$(RANLIB_1) $(TARGET_1)

clean-libvslib.a: 
	$(RMFILE) $(TARGET_1)
	$(RMDIR) .OBJ.libvslib.a

rebuild-libvslib.a: clean-libvslib.a libvslib.a

re-libvslib.a: rebuild-libvslib.a

link-libvslib.a: .OBJ.libvslib.a $(OBJ_1)
	$(RMFILE) libvslib.a
	$(AR_1) $(ARFLAGS_1) $(TARGET_1) $(OBJ_1)
	$(RANLIB_1) $(TARGET_1)


### TARGET OBJECTS FOR TARGET 1: libvslib.a ####################################

.OBJ.libvslib.a/clusters.o: clusters.cpp 
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c clusters.cpp         -o .OBJ.libvslib.a/clusters.o
.OBJ.libvslib.a/dlog.o: dlog.cpp 
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c dlog.cpp             -o .OBJ.libvslib.a/dlog.o
.OBJ.libvslib.a/eval.o: eval.cpp 
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c eval.cpp             -o .OBJ.libvslib.a/eval.o
.OBJ.libvslib.a/fnmatch2.o: fnmatch2.cpp 
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c fnmatch2.cpp         -o .OBJ.libvslib.a/fnmatch2.o
.OBJ.libvslib.a/getopt2.o: getopt2.cpp 
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c getopt2.cpp          -o .OBJ.libvslib.a/getopt2.o
.OBJ.libvslib.a/scroll.o: scroll.cpp 
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c scroll.cpp           -o .OBJ.libvslib.a/scroll.o
.OBJ.libvslib.a/vslib.o: vslib.cpp 
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c vslib.cpp            -o .OBJ.libvslib.a/vslib.o
.OBJ.libvslib.a/vsuti.o: vsuti.cpp 
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c vsuti.cpp            -o .OBJ.libvslib.a/vsuti.o
.OBJ.libvslib.a/vscrc.o: vscrc.cpp 
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c vscrc.cpp            -o .OBJ.libvslib.a/vscrc.o


### TARGET 2: libvscon.a #######################################################

CC_2       = $(CXX)
LD_2       = $(CXX)
AR_2       = $(AR) rv
RANLIB_2   = $(RANLIB)
CCFLAGS_2  = -I../vstring -I. -I/usr/include/ncurses -O2 $(CFLAGS) $(CPPFLAGS) $(CCDEF)  
LDFLAGS_2  = $(LDFLAGS) $(LDDEF) 
DEPFLAGS_2 = 
ARFLAGS_2  = 
TARGET_2   = libvscon.a

### SOURCES FOR TARGET 2: libvscon.a ###########################################

SRC_2= \
     ansiterm.cpp \
     conmenu.cpp \
     form_in.cpp \
     unicon.cpp \

#### OBJECTS FOR TARGET 2: libvscon.a ##########################################

OBJ_2= \
     .OBJ.libvscon.a/ansiterm.o \
     .OBJ.libvscon.a/conmenu.o \
     .OBJ.libvscon.a/form_in.o \
     .OBJ.libvscon.a/unicon.o \

### TARGET DEFINITION FOR TARGET 2: libvscon.a #################################

.OBJ.libvscon.a: 
	$(MKDIR) .OBJ.libvscon.a

libvscon.a:   .OBJ.libvscon.a $(OBJ_2)
	$(AR_2) $(ARFLAGS_2) $(TARGET_2) $(OBJ_2)
	$(RANLIB_2) $(TARGET_2)

clean-libvscon.a: 
	$(RMFILE) $(TARGET_2)
	$(RMDIR) .OBJ.libvscon.a

rebuild-libvscon.a: clean-libvscon.a libvscon.a

re-libvscon.a: rebuild-libvscon.a

link-libvscon.a: .OBJ.libvscon.a $(OBJ_2)
	$(RMFILE) libvscon.a
	$(AR_2) $(ARFLAGS_2) $(TARGET_2) $(OBJ_2)
	$(RANLIB_2) $(TARGET_2)


### TARGET OBJECTS FOR TARGET 2: libvscon.a ####################################

.OBJ.libvscon.a/ansiterm.o: ansiterm.cpp 
	$(CC_2) $(CFLAGS_2) $(CCFLAGS_2) -c ansiterm.cpp         -o .OBJ.libvscon.a/ansiterm.o
.OBJ.libvscon.a/conmenu.o: conmenu.cpp 
	$(CC_2) $(CFLAGS_2) $(CCFLAGS_2) -c conmenu.cpp          -o .OBJ.libvscon.a/conmenu.o
.OBJ.libvscon.a/form_in.o: form_in.cpp 
	$(CC_2) $(CFLAGS_2) $(CCFLAGS_2) -c form_in.cpp          -o .OBJ.libvscon.a/form_in.o
.OBJ.libvscon.a/unicon.o: unicon.cpp 
	$(CC_2) $(CFLAGS_2) $(CCFLAGS_2) -c unicon.cpp           -o .OBJ.libvscon.a/unicon.o


### TARGET 3: libvscony.a ######################################################

CC_3       = $(CXX)
LD_3       = $(CXX)
AR_3       = $(AR) rv
RANLIB_3   = $(RANLIB)
CCFLAGS_3  = -I../vstring -I. -I../yascreen -DUSE_YASCREEN -O2 $(CFLAGS) $(CPPFLAGS) $(CCDEF)  
LDFLAGS_3  = $(LDFLAGS) $(LDDEF) 
DEPFLAGS_3 = 
ARFLAGS_3  = 
TARGET_3   = libvscony.a

### SOURCES FOR TARGET 3: libvscony.a ##########################################

SRC_3= \
     ansiterm.cpp \
     conmenu.cpp \
     form_in.cpp \
     unicon.cpp \

#### OBJECTS FOR TARGET 3: libvscony.a #########################################

OBJ_3= \
     .OBJ.libvscony.a/ansiterm.o \
     .OBJ.libvscony.a/conmenu.o \
     .OBJ.libvscony.a/form_in.o \
     .OBJ.libvscony.a/unicon.o \

### TARGET DEFINITION FOR TARGET 3: libvscony.a ################################

.OBJ.libvscony.a: 
	$(MKDIR) .OBJ.libvscony.a

libvscony.a:   .OBJ.libvscony.a $(OBJ_3)
	$(AR_3) $(ARFLAGS_3) $(TARGET_3) $(OBJ_3)
	$(RANLIB_3) $(TARGET_3)

clean-libvscony.a: 
	$(RMFILE) $(TARGET_3)
	$(RMDIR) .OBJ.libvscony.a

rebuild-libvscony.a: clean-libvscony.a libvscony.a

re-libvscony.a: rebuild-libvscony.a

link-libvscony.a: .OBJ.libvscony.a $(OBJ_3)
	$(RMFILE) libvscony.a
	$(AR_3) $(ARFLAGS_3) $(TARGET_3) $(OBJ_3)
	$(RANLIB_3) $(TARGET_3)


### TARGET OBJECTS FOR TARGET 3: libvscony.a ###################################

.OBJ.libvscony.a/ansiterm.o: ansiterm.cpp 
	$(CC_3) $(CFLAGS_3) $(CCFLAGS_3) -c ansiterm.cpp         -o .OBJ.libvscony.a/ansiterm.o
.OBJ.libvscony.a/conmenu.o: conmenu.cpp 
	$(CC_3) $(CFLAGS_3) $(CCFLAGS_3) -c conmenu.cpp          -o .OBJ.libvscony.a/conmenu.o
.OBJ.libvscony.a/form_in.o: form_in.cpp 
	$(CC_3) $(CFLAGS_3) $(CCFLAGS_3) -c form_in.cpp          -o .OBJ.libvscony.a/form_in.o
.OBJ.libvscony.a/unicon.o: unicon.cpp 
	$(CC_3) $(CFLAGS_3) $(CCFLAGS_3) -c unicon.cpp           -o .OBJ.libvscony.a/unicon.o


### TARGET 4: test #############################################################

CC_4       = $(CXX)
LD_4       = $(CXX)
AR_4       = $(AR) rv
RANLIB_4   = $(RANLIB)
CCFLAGS_4  = -g -I../vstring -I. -I../yascreen -DUSE_YASCREEN -O0 -DTEST $(CFLAGS) $(CPPFLAGS) $(CCDEF)  
LDFLAGS_4  = -g -L../vstring -L../vstring/pcre2 -L. -lvstring -lvslib -lvscon -lpcre2 -lncurses $(LDFLAGS) $(LDDEF) 
DEPFLAGS_4 = 
ARFLAGS_4  = 
TARGET_4   = test

### SOURCES FOR TARGET 4: test #################################################

SRC_4= \
     t/test.cpp \

#### OBJECTS FOR TARGET 4: test ################################################

OBJ_4= \
     .OBJ.test/test.o \

### TARGET DEFINITION FOR TARGET 4: test #######################################

.OBJ.test: 
	$(MKDIR) .OBJ.test

test: libvslib.a libvscon.a  .OBJ.test $(OBJ_4)
	$(LD_4) $(OBJ_4) $(LDFLAGS_4) -o $(TARGET_4)

clean-test: 
	$(RMFILE) $(TARGET_4)
	$(RMDIR) .OBJ.test

rebuild-test: clean-test test

re-test: rebuild-test

link-test: .OBJ.test $(OBJ_4)
	$(RMFILE) test
	$(LD_4) $(OBJ_4) $(LDFLAGS_4) -o $(TARGET_4)


### TARGET OBJECTS FOR TARGET 4: test ##########################################

.OBJ.test/test.o: t/test.cpp 
	$(CC_4) $(CFLAGS_4) $(CCFLAGS_4) -c t/test.cpp           -o .OBJ.test/test.o


mm_update:
	


### MAKEMAKE ENDS HERE #########################################################

