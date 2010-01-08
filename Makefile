## -*- Makefile -*-
##
## User: necto
## Time: Jan 7, 2010 9:50:40 PM
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
CPPFLAGS_runningram = \
	-Ih
OBJS_runningram =  \
	$(TARGETDIR_runningram)/main.o \
	$(TARGETDIR_runningram)/Game_manager.o \
	$(TARGETDIR_runningram)/Battlefield.o \
	$(TARGETDIR_runningram)/Camera.o \
	$(TARGETDIR_runningram)/Graphic_subsystem.o \
	$(TARGETDIR_runningram)/Eventman.o \
	$(TARGETDIR_runningram)/initparser.o
SYSLIBS_runningram = -lm
USERLIBS_runningram = $(SYSLIBS_runningram)
DEPLIBS_runningram =
LDLIBS_runningram = $(USERLIBS_runningram)


# Link or archive
$(TARGETDIR_runningram)/runningram: $(TARGETDIR_runningram) $(OBJS_runningram) $(DEPLIBS_runningram)
	$(LINK.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ $(OBJS_runningram) $(LDLIBS_runningram)


# Compile source files into .o files
$(TARGETDIR_runningram)/main.o: $(TARGETDIR_runningram) main.cpp
	$(COMPILE.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ main.cpp

$(TARGETDIR_runningram)/Game_manager.o: $(TARGETDIR_runningram) src/Game_manager.cpp
	$(COMPILE.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ src/Game_manager.cpp

$(TARGETDIR_runningram)/Battlefield.o: $(TARGETDIR_runningram) src/Battlefield.cpp
	$(COMPILE.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ src/Battlefield.cpp

$(TARGETDIR_runningram)/Camera.o: $(TARGETDIR_runningram) src/Camera.cpp
	$(COMPILE.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ src/Camera.cpp

$(TARGETDIR_runningram)/Graphic_subsystem.o: $(TARGETDIR_runningram) src/Graphic_subsystem.cpp
	$(COMPILE.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ src/Graphic_subsystem.cpp

$(TARGETDIR_runningram)/Eventman.o: $(TARGETDIR_runningram) src/Eventman.cpp
	$(COMPILE.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ src/Eventman.cpp

$(TARGETDIR_runningram)/initparser.o: $(TARGETDIR_runningram) src/initparser.cpp
	$(COMPILE.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ src/initparser.cpp



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_runningram)/runningram \
		$(TARGETDIR_runningram)/main.o \
		$(TARGETDIR_runningram)/Game_manager.o \
		$(TARGETDIR_runningram)/Battlefield.o \
		$(TARGETDIR_runningram)/Graphic_subsystem.o \
		$(TARGETDIR_runningram)/Eventman.o \
		$(TARGETDIR_runningram)/initparser.o
	$(CCADMIN)
	rm -f -r $(TARGETDIR_runningram)


# Create the target directory (if needed)
$(TARGETDIR_runningram):
	mkdir -p $(TARGETDIR_runningram)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-i386-Linux

