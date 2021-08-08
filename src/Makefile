OPENFST_ROOT = openfst

CXX = g++
CXXFLAGS = -std=c++11 -DHAVE_ATLAS
INCLUDE = -I/usr/local/include -I./vosk -I./kaldi -I$(OPENFST_ROOT)/src/include
LDFLAGS = -std=c++11
LDLIBS  = -lpthread -lm -ldl
EXECUTABLE = vosk_test
SOURCES = $(shell find . -name "*.cc")
OBJECTS = $(patsubst %.cc, %.o, $(SOURCES))

LIBS = \
	/usr/local/lib/libfst.so \
	/usr/local/lib/libfstngram.so \
	/usr/lib64/atlas/libsatlas.so

BASE = $(USER)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(LIBS) $(LDLIBS) $^ -o $@

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< -o $@

clean:
	rm $(OBJECTS)
	rm $(EXECUTABLE)
