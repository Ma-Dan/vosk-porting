from distutils.core import setup, Extension

import os

os.environ["CC"] = "g++"
#os.environ["CXX"] = "g++"

module1 = Extension('demo',
                    language = "c++",
                    extra_compile_args = ['-std=c++17', '-O2', '-DFST_NO_DYNAMIC_LINKING'],
                    sources = ['demomodule.c', 'vosk/kaldi_recognizer.cc', 'vosk/language_model.cc', 'vosk/model.cc', 'vosk/spk_model.cc', 'vosk/vosk_api.cc'],
                    libraries = ['portaudio'],
                    include_dirs = ['vosk', 'kaldi', 'openfst/src/include'],
                    extra_objects = ['vosk_lib.a',
                                 'openfst/src/lib/.libs/libfst.a',
                                 'openfst/src/extensions/ngram/.libs/libfstngram.a',
                                 '/usr/local/Cellar/gcc/11.2.0/lib/gcc/11/libgfortran.dylib']
                    )

setup (name = 'a demo extension module',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1])
