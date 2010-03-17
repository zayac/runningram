## -*- Makefile -*-
##
## User: necto
## Time: Jan 7, 2010 9:50:40 PM
## Makefile created by Sun Studio.
##
## This file is generated automatically, but edited manually.
##


#### Compiler and tool definitions shared by all build targets #####
CCC = g++
CXX = g++
BASICOPTS = -g
CCFLAGS = $(BASICOPTS) `sdl-config --cflags --libs` -lSDL_ttf -lSDL_gfx
CXXFLAGS = $(BASICOPTS) `sdl-config --cflags --libs` -lSDL_ttf -lSDL_gfx
CCADMIN =

# Define a lookup dir

vpath %.cpp .:src

# Define the target directories.
TARGETDIR=GNU-i386-Linux
## Target: runningram
CPPFLAGS_runningram = -Ih
OBJS =  $(patsubst src/%.cpp,$(TARGETDIR)/%.o,$(wildcard src/*.cpp)) \
        $(patsubst %.cpp, $(TARGETDIR)/%.o, $(wildcard *.cpp))

SYSLIBS_runningram = -lm
USERLIBS_runningram = $(SYSLIBS_runningram)
DEPLIBS_runningram =
LDLIBS_runningram = $(USERLIBS_runningram)


install: all
	cp $(TARGETDIR)/runningram .
run: install
	./runningram

all: $(TARGETDIR)/runningram


# Link
$(TARGETDIR)/runningram: $(TARGETDIR) $(OBJS) $(DEPLIBS_runningram)
	$(LINK.cc) $(CCFLAGS_runningram) $(CPPFLAGS_runningram) -o $@ $(OBJS) $(LDLIBS_runningram)


# Compile source files into .o files

$(TARGETDIR)/%.o: %.cpp
	$(COMPILE.cc) -MMD $(CCFLAGS_runningram) $(CPPFLAGS_runningram) $< -o $@

include $(wildcard $(TARGETDIR)/*.d)

#### Clean target deletes all generated files ####
clean: clean_obj
	rm -f runningram $(TARGETDIR)/runningram
	$(CCADMIN)
	rm -rf $(TARGETDIR)

clean_obj:
	rm -f $(OBJS)

# Create the target directory (if needed)
$(TARGETDIR):
	mkdir -p $(TARGETDIR)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-i386-Linux