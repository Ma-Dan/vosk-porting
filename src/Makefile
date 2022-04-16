OPENFST_ROOT = openfst
OPENBLAS_ROOT = OpenBLAS

CXX = g++
CXXFLAGS = -std=c++11 -DHAVE_CLAPACK
INCLUDE = -I/usr/local/include -I./vosk -I./kaldi -I$(OPENFST_ROOT)/src/include
LDFLAGS = -std=c++11
LDLIBS	= -lpthread -lm
EXECUTABLE = vosk_test
STATICLIBRARY = vosk_lib.a
SOURCES = $(shell find . -name "*.cc")
OBJECTS = $(patsubst %.cc, %.o, $(SOURCES))

LIBS = \
	$(OPENBLAS_ROOT)/libopenblas.a \
	$(OPENFST_ROOT)/src/lib/.libs/libfst.a \
	$(OPENFST_ROOT)/src/extensions/ngram/.libs/libfstngram.a \
	/usr/local/Cellar/gcc/11.2.0/lib/gcc/11/libgfortran.dylib

BASE = $(USER)

all: $(EXECUTABLE) $(STATICLIBRARY)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS) $(LDLIBS)

$(STATICLIBRARY): $(OBJECTS)
	ar rcs $@ $^

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< -o $@

clean:
	rm $(OBJECTS)
	rm $(EXECUTABLE)
	rm $(STATICLIBRARY)
