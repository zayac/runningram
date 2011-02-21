## -*- Makefile -*-
##
## User: necto
## Time: Jan 7, 2010 9:50:40 PM
## Makefile created by Sun Studio.
##
## This file is generated automatically, but edited manually by abovementioned.
##

INTERPRETER_FLAGS = -Dlinux -Wl,--rpath,./lib/
INTERPRETER_LIBS = -L./lib/ -lecl -ldl -lm
LINK_LIBS = $(INTERPRETER_LIBS) `sdl-config --cflags --libs` -lSDL_ttf -lSDL_gfx -lSDL_image

#### Compiler and tool definitions shared by all build targets #####
CCC = g++
CXX = g++
BASICOPTS = -g
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN =

# Define a lookup dir

vpath %.cpp .:src

# Define the target directories.
TARGETDIR=GNU-i386-Linux


CPPFLAGS = -Ih $(INTERPRETER_FLAGS)
OBJS =  $(patsubst src/%.cpp,$(TARGETDIR)/%.o,$(wildcard src/*.cpp)) \
        $(patsubst %.cpp, $(TARGETDIR)/%.o, $(wildcard *.cpp))

DEPLIBS = lib/libecl.so
LDLIBS = -lm $(LINK_LIBS)
PROJ_PATH := $(PWD)


install: runningram

runningram: $(TARGETDIR)/runningram
	cp $(TARGETDIR)/runningram .

run: install
	./runningram


# Link
$(TARGETDIR)/runningram: $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK.cc) $(CCFLAGS) $(CPPFLAGS) -o $@ $(OBJS) $(LDLIBS)

lib/libecl.so: ecl.tar.gz
	echo "================== building the ECL library ======================"
	rm ecl -rf
	tar -xzf ecl.tar.gz
	cd ecl && ./configure '--prefix=$(PROJ_PATH)' '--includedir=$(PROJ_PATH)/h' \
		'--libdir=$(PROJ_PATH)/lib' '--datarootdir=$(PROJ_PATH)/trash' \
		'--localstatedir=$(PROJ_PATH)/trash' -with-cxx
	cd ecl && $(MAKE)
	cd ecl && $(MAKE) install
	cd ecl && $(MAKE) clean
	rm trash/* -r
	echo "================== ECL build finished ======================"


# Compile source files into .o files

$(TARGETDIR)/%.o: %.cpp $(DEPLIBS)
	$(COMPILE.cc) -MMD $(CCFLAGS) $(CPPFLAGS) $< -o $@

include $(wildcard $(TARGETDIR)/*.d)

#### Clean target deletes all generated files ####
clean: clean_obj
	rm -f runningram $(TARGETDIR)/runningram
	$(CCADMIN)
	rm -rf $(TARGETDIR)
	cd ecl && $(MAKE) clean

clean_obj:
	rm -f $(OBJS)

# Create the target directory (if needed)
$(TARGETDIR):
	mkdir -p $(TARGETDIR)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-i386-Linux