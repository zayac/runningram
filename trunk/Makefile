## -*- Makefile -*-
##
## User: necto
## Time: Jan 5, 2010 6:04:22 PM
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CCC = g++
CXX = g++
BASICOPTS = -g
CCFLAGS = $(BASICOPTS) `sdl-config --cflags --libs`
CXXFLAGS = $(BASICOPTS) `sdl-config --cflags --libs`
CCADMIN = 


# Define the target directories.
TARGETDIR_runningram=GNU-i386-Linux


all: $(TARGETDIR_runningram)/runningram

## Target: runningram
OBJS_runningram =  \
	$(TARGETDIR_runningram)/sdltest.o \
	$(TARGETDIR_runningram)/initparser.o \
	$(TARGETDIR_runningram)/Init_client.o
SYSLIBS_runningram = -lm 
USERLIBS_runningram = $(SYSLIBS_runningram) 
DEPLIBS_runningram =  
LDLIBS_runningram = $(USERLIBS_runningram)


# Link or archive
$(TARGETDIR_runningram)/runningram: $(TARGETDIR_runningram) $(OBJS_runningram) $(DEPLIBS_runningram)
	$(LINK.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ $(OBJS_runningram) $(LDLIBS_runningram)


# Compile source files into .o files
$(TARGETDIR_runningram)/sdltest.o: $(TARGETDIR_runningram) sdltest.cpp
	$(COMPILE.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ sdltest.cpp

$(TARGETDIR_runningram)/initparser.o: $(TARGETDIR_runningram) initparser.cpp
	$(COMPILE.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ initparser.cpp

$(TARGETDIR_runningram)/Init_client.o: $(TARGETDIR_runningram) Init_client.cpp
	$(COMPILE.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ Init_client.cpp



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_runningram)/runningram \
		$(TARGETDIR_runningram)/sdltest.o \
		$(TARGETDIR_runningram)/initparser.o \
		$(TARGETDIR_runningram)/Init_client.o
	$(CCADMIN)
	rm -f -r $(TARGETDIR_runningram)


# Create the target directory (if needed)
$(TARGETDIR_runningram):
	mkdir -p $(TARGETDIR_runningram)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-i386-Linux

