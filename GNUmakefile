
# GNU makefile for RooAverageTools
# S. Kluth 9/2012

# CXX = g++-4.7
LD = $(CXX)
CXXFLAGS = -Wall -fPIC

 LIBFILES = minuitSolver.cc AverageDataParser.cc ClsqAverage.cc 
#LIBFILES = AverageDataParser.cc ClsqAverage.cc 
LIB = libRooAverageTools.so
 TESTFILE = testminuitSolver.cc testAverageDataParser.cc testClsqAverage.cc
#TESTFILE = testAverageDataParser.cc testClsqAverage.cc
TESTEXE = $(basename $(TESTFILE) )
LIBOBJS = $(LIBFILES:.cc=.o)
DEPS = $(LIBFILES:.cc=.d) $(TESTFILE:.cc=.d)
PROJECTPATH = $(shell echo $${PWD%/*} )
CPPFLAGS = -I $(PROJECTPATH)/INIParser
LDFLAGS = -L $(PROJECTPATH)/INIParser
LDLIBS = -lINIParser -lMatrix 
# LDLIBS = -lMatrix 
ifdef HEPROOT
CPPFLAGS += -I $(HEPROOT)/include -I $(HEPROOT)/include/boost-1_46/
LDFLAGS += -L $(HEPROOT)/lib64
LDLIBS += -lboost_unit_test_framework-gcc46-mt-1_46
else
CPPFLAGS += -I $(ROOTSYS)/include
LDFLAGS += -L $(ROOTSYS)/lib 
LDLIBS += -lboost_unit_test_framework
endif
LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):$(PROJECTPATH)/INIParser

.INTERMEDIATE: $(LIBOBJS) $(TESTFILE:.cc=.o)

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
	rm -f $(DEPS) $(TESTEXE) $(LIB)
