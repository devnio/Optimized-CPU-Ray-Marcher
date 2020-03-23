IDIR =../include
CC=gcc
CFLAGS= -O3 -I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = lodepng.h vec3.h mat4.h camera.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = raymarcher.o lodepng.o vec3.o mat4.o camera.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

raymarcher: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
