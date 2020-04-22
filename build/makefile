IDIR =../include
CC=gcc
CFLAGS= -O3 -I$(IDIR) -g

ODIR=obj
LDIR =../lib

LIBS=-lm


_DEPS = lodepng.h vec3.h mat4.h camera.h utility.h light.h geometry/scene.h geometry/plane.h geometry/sphere.h geometry/box.h geometry/cone.h geometry/octahedron.h geometry/torus.h scene_loader.h geometry/transform.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS)) # e.g.: DEPS = ../include/lodepng.h

_OBJ = raymarcher.o lodepng.o vec3.o mat4.o camera.o utility.o light.o geometry/scene.o geometry/plane.o geometry/sphere.o geometry/box.o geometry/cone.o geometry/octahedron.o geometry/torus.o geometry/transform.o scene_loader.o material.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ)) # e.g.: OBJ = obj/lodepng.o

$(ODIR)/%.o: ../src/%.c $(DEPS) # take files in obj/ that end in .o and 
	$(CC) -c -o $@ $< $(CFLAGS)

raymarcher: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
