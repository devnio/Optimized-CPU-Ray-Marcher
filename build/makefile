IDIR =../include
CC=gcc
CFLAGS= -O3 -I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = lodepng.h vec3.h mat4.h camera.h utility.h geometry/sphere.h light.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS)) # e.g.: DEPS = ../include/lodepng.h

_OBJ = raymarcher.o lodepng.o vec3.o mat4.o camera.o utility.o geometry/sphere.o light.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ)) # e.g.: OBJ = obj/lodepng.o

$(ODIR)/%.o: ../src/%.c $(DEPS) # take files in obj/ that end in .o and 
	$(CC) -c -o $@ $< $(CFLAGS)

raymarcher: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
