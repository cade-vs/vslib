
### MAKEMAKE STARTS HERE #######################################################


### Created by makemake.pl on Sat Jan  4 19:07:55 2003 #########################


### GLOBAL TARGETS #############################################################

default: all

re: rebuild

li: link

all: pcre.a 

clean: clean-pcre.a 

rebuild: rebuild-pcre.a 

link: link-pcre.a 

### GLOBAL (AND USER) DEFS #####################################################


AR = ar rv
CC = gcc
LD = gcc
MKDIR = mkdir -p
RANLIB = ranlib
RMDIR = rm -rf
RMFILE = rm -f
SRC = *.c *.cpp *.cc *.cxx


### TARGET 1: pcre.a ###########################################################

CC_1       = gcc
LD_1       = gcc
AR_1       = ar rv
RANLIB_1   = ranlib
CCFLAGS_1  = -I. -O2 $(CCDEF) 
LDFLAGS_1  = $(LDDEF)
DEPFLAGS_1 = 
ARFLAGS_1  = 
TARGET_1   = pcre.a

### SOURCES FOR TARGET 1: pcre.a ###############################################

SRC_1= \
     chartables.c \
     pcre.c \
     pcreposix.c \
     study.c \

#### OBJECTS FOR TARGET 1: pcre.a ##############################################

OBJ_1= \
     .OBJ.pcre.a/chartables.o \
     .OBJ.pcre.a/pcre.o \
     .OBJ.pcre.a/pcreposix.o \
     .OBJ.pcre.a/study.o \

### TARGET DEFINITION FOR TARGET 1: pcre.a #####################################

.OBJ.pcre.a: 
	$(MKDIR) .OBJ.pcre.a

pcre.a: .OBJ.pcre.a $(OBJ_1)
	$(AR_1) $(ARFLAGS_1) $(TARGET_1) $(OBJ_1)
	$(RANLIB_1) $(TARGET_1)

clean-pcre.a: 
	$(RMFILE) $(TARGET_1)
	$(RMDIR) .OBJ.pcre.a

rebuild-pcre.a: clean-pcre.a pcre.a

link-pcre.a: .OBJ.pcre.a $(OBJ_1)
	$(RMFILE) pcre.a
	$(AR_1) $(ARFLAGS_1) $(TARGET_1) $(OBJ_1)
	$(RANLIB_1) $(TARGET_1)


### TARGET OBJECTS FOR TARGET 1: pcre.a ########################################

.OBJ.pcre.a/chartables.o: chartables.c  chartables.c
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c chartables.c -o .OBJ.pcre.a/chartables.o
.OBJ.pcre.a/pcre.o: pcre.c  pcre.c internal.h config.h pcre.h chartables.c
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c pcre.c -o .OBJ.pcre.a/pcre.o
.OBJ.pcre.a/pcreposix.o: pcreposix.c  pcreposix.c internal.h config.h pcre.h pcreposix.h
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c pcreposix.c -o .OBJ.pcre.a/pcreposix.o
.OBJ.pcre.a/study.o: study.c  study.c internal.h config.h pcre.h
	$(CC_1) $(CFLAGS_1) $(CCFLAGS_1) -c study.c -o .OBJ.pcre.a/study.o


### MAKEMAKE ENDS HERE #########################################################

