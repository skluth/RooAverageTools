
# GNU makefile for RooAverageTools
# S. Kluth 9/2012

LD = $(CXX)
CXXFLAGS = -Wall -fPIC

LIBFILES = AverageDataParser.cc ClsqAverage.cc
LIB = libRooAverageTools.so
TESTFILE = testAverageDataParser.cc testClsqAverage.cc
TESTEXE = $(basename $(TESTFILE) )
LIBOBJS = $(LIBFILES:.cc=.o)
DEPS = $(LIBFILES:.cc=.d) $(TESTFILE:.cc=.d)
PROJECTPATH = $(shell echo $${PWD%/*} )
CPPFLAGS = -I $(ROOTSYS)/include -I $(PROJECTPATH)/INIParser
LDLIBS = -lMatrix -lINIParser
ifdef HEPROOT
CPPFLAGS += -I $(HEPROOT)/include/boost-1_48/
LDFLAGS = -L $(PROJECTPATH)/INIParser -L $(HEPROOT)/lib64
LDLIBS += -lboost_unit_test_framework-gcc46-mt-1_48
else
LDFLAGS = -L $(PROJECTPATH)/INIParser -L $(ROOTSYS)/lib 
LDLIBS += -lboost_unit_test_framework
endif
LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):$(PROJECTPATH)/INIParser

all: $(TESTEXE)

$(DEPS): %.d: %.cc
	$(CXX) $(CPPFLAGS) -MM $< -MF $@
-include $(DEPS)

$(LIB): $(LIBOBJS)
	$(LD) -shared -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(TESTEXE): %: %.o $(LIB)
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH): ./$@ --log_level=message

clean:
	rm -f *.o $(DEPS) $(TESTEXE) $(LIB)

