
# GNU makefile for RooAverageTools
# S. Kluth 9/2012

LD = $(CXX)
CXXFLAGS = -Wall

LIBFILES = AverageDataParser.cc ClsqAverage.cc
LIB = libRooAverageTools.so
TESTFILE = testAverageDataParser.cc testClsqAverage.cc
TESTEXE = $(basename $(TESTFILE) )
LIBOBJS = $(LIBFILES:.cc=.o)
DEPS = $(LIBFILES:.cc=.d) $(TESTFILE:.cc=.d)
ROOTINC = $(ROOTSYS)/include
ROOTLIBS = $(ROOTSYS)/lib
PROJECTPATH = $(shell echo $${PWD%/*} )
PCKGS = $(PROJECTPATH)/INIParser
CPPFLAGS = -I $(ROOTINC) $(addprefix -I , $(PCKGS) )
LDFLAGS = -L $(ROOTLIBS) $(addprefix -L , $(PCKGS) )
LDLIBS = -lMatrix -lINIParser
LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):$(PCKGS)

all: $(TESTEXE)

$(DEPS): %.d: %.cc
	$(CXX) $(CPPFLAGS) -MM $< -MF $@
-include $(DEPS)

$(LIB): $(LIBOBJS)
	$(LD) -shared -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(TESTEXE): %: %.o $(LIB)
	$(LD) -o $@ $^ -lboost_unit_test_framework $(LDFLAGS) $(LDLIBS)
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) ./$@ --log_level=message

clean:
	rm -f *.o $(DEPS) $(TESTEXE) $(LIB)

