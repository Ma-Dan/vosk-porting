OPENFST_ROOT = openfst
OPENBLAS_ROOT = OpenBLAS

CXX = g++
CXXFLAGS = -std=c++11 --debug -DHAVE_CLAPACK
INCLUDE	= -I/usr/local/include -I./vosk -I./kaldi -I$(OPENFST_ROOT)/include
LDFLAGS = -std=c++11
LDLIBS	= -lpthread -lm
EXECUTABLE = vosk_test
SOURCES = $(shell find . -name "*.cc")
OBJECTS = $(patsubst %.cc, %.o, $(SOURCES))

LIBS= \
	$(OPENBLAS_ROOT)/libopenblas.a \
	$(OPENFST_ROOT)/lib/libfst.a \
	$(OPENFST_ROOT)/lib/libfstngram.a \
	/usr/local/Cellar/gcc/10.2.0_4/lib/gcc/10/libgfortran.dylib

BASE = $(USER)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(LIBS) $(LDLIBS) $^ -o $@

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< -o $@

clean:
	rm $(OBJECTS)
	rm $(EXECUTABLE)