KALDI_ROOT=/Users/dan/Documents/ASR/kaldi

CXX = g++
CXXFLAGS = -std=c++11 --debug
INCLUDE	= -I/usr/local/include -I./ -I$(KALDI_ROOT)/src -I$(KALDI_ROOT)/tools/openfst/include
LDFLAGS = -std=c++11
LDLIBS	= -lpthread -lm
EXECUTABLE = bin/vosk
SOURCES = $(wildcard **/*.cc)
OBJECTS = $(patsubst %.cc, obj/%.o, $(SOURCES))

LIBS= \
	$(KALDI_ROOT)/src/decoder/kaldi-decoder.a \
	$(KALDI_ROOT)/src/ivector/kaldi-ivector.a \
	$(KALDI_ROOT)/src/gmm/kaldi-gmm.a \
	$(KALDI_ROOT)/src/nnet3/kaldi-nnet3.a \
	$(KALDI_ROOT)/src/tree/kaldi-tree.a \
	$(KALDI_ROOT)/src/feat/kaldi-feat.a \
	$(KALDI_ROOT)/src/lat/kaldi-lat.a \
	$(KALDI_ROOT)/src/lm/kaldi-lm.a \
	$(KALDI_ROOT)/src/hmm/kaldi-hmm.a \
	$(KALDI_ROOT)/src/transform/kaldi-transform.a \
	$(KALDI_ROOT)/src/cudamatrix/kaldi-cudamatrix.a \
	$(KALDI_ROOT)/src/matrix/kaldi-matrix.a \
	$(KALDI_ROOT)/src/fstext/kaldi-fstext.a \
	$(KALDI_ROOT)/src/util/kaldi-util.a \
	$(KALDI_ROOT)/src/base/kaldi-base.a \
	$(KALDI_ROOT)/tools/OpenBLAS/libopenblas.a \
	$(KALDI_ROOT)/tools/openfst/lib/libfst.a \
	$(KALDI_ROOT)/tools/openfst/lib/libfstngram.a \
	/usr/local/Cellar/gcc/10.2.0_4/lib/gcc/10/libgfortran.dylib

BASE = $(USER)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(LIBS) $(LDLIBS) $^ -o $@

obj/%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< -o $@

clean:
	rm $(OBJECTS)