
# GNU makefile for RooAverageTools
# S. Kluth 9/2012

LD = $(CXX)
CXXFLAGS = -Wall

LIBFILES = AverageDataParser.cc
LIB = libRooAverageTools.so
TESTFILE = testAverageDataParser.cc
TESTEXE = $(basename $(TESTFILE) )
LIBOBJS = $(LIBFILES:.cc=.o)
DEPS = $(LIBFILES:.cc=.d) $(TESTFILE:.cc=.d)
ROOTINC = $(ROOTSYS)/include
ROOTLIBS = $(ROOTSYS)/lib
PROJECTPATH = $(shell echo $${PWD%/*} )
CPPFLAGS = -I $(ROOTINC) -I $(PROJECTPATH)/INIParser
LDFLAGS = -L $(ROOTLIBS) -L $(PROJECTPATH)/INIParser
LDLIBS = -lMatrix -lINIParser


all: $(TESTEXE)

$(DEPS): %.d: %.cc
	$(CXX) $(CPPFLAGS) -MM $< -MF $@
-include $(DEPS)

$(LIB): $(LIBOBJS)
	$(LD) -shared -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(TESTEXE): $(TESTFILE:.cc=.o) $(LIB)
	$(LD) -o $@ $^ -lboost_unit_test_framework $(LDFLAGS) $(LDLIBS)
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(PROJECTPATH)/INIParser ./$@ --log_level=message

clean:
	rm -f *.o $(DEPS) $(TESTEXE) $(LIB)

