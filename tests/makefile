IDIR =../include
CC=gcc
CFLAGS= -O3 -I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

#TODO as soon as you add new tests you can add the header file here in order to be compiled
_DEPS = greatest.h vec3.h
#lodepng.h vec3.h mat4.h camera.h utility.h sphere.h light.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ =  vec3.o
OBJ = $(patsubst %,../build/$(ODIR)/%,$(_OBJ)) # e.g.: OBJ = obj/lodepng.o

TDEPS = vec3_test.h test_example.h
TDIR = ../tests
TFLAGS =  -I$(IDIR) -g -Wall -pedantic -Werror #-fprofile-arcs -ftest-coverage
#-fprofile-arcs -ftest-coverage required for coverage testing
TODIR = ../build/obj
_TOBJ = tests.o 
TOBJ = $(patsubst %,$(TODIR)/%,$(_TOBJ))

#TODO improve the makefile.
$(TODIR)/%.o: %.c $(DEPS) $(TDEPS) 
	$(CC) -c -o $@ $< $(TFLAGS)

test: $(TOBJ) $(OBJ)
	$(CC) -o $@ $^ $(TFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
