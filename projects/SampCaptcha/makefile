# This file demonstrates how to compile the SampCaptcha project on Linux.
#
# To compile SampCaptcha do:
#
# make SampCaptcha 
#

GPP = g++
GCC = gcc
SampCaptcha_OUTFILE = "./SampCaptcha.so"

COMPILE_FLAGS = -std=c++0x -m32 -fPIC -c -I ./ -w -D LINUX -D PROJECT_NAME=\"SampCaptcha\"

SampCaptcha = -D SampCaptcha $(COMPILE_FLAGS)

all: SampCaptcha

clean:
	-rm -f *~ *.o *.so

SampCaptcha: clean
	$(GPP) $(SampCaptcha) *.cpp
	$(GCC) $(SampCaptcha) *.c
	$(GPP) -m32 -fshort-wchar -shared -o $(SampCaptcha_OUTFILE) *.o

