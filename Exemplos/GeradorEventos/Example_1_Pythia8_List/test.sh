#!/bin/bash
export PYTHIA8DATA=$PWD/pythia8317/share/Pythia8/xmldoc
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$PWD/pythia8317/lib:$PWD/../Pythia8/LHAPDF/lib/
export LHAPATH=$PWD/../Pythia8/LHAPDF
#setenv LHAPATH /usr/local/share/LHAPDF

./test.exe
