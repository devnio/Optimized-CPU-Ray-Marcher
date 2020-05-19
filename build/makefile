IDIR =../include
CC=gcc
CFLAGS= -O3 -I$(IDIR) -mavx -g
export F

ODIR=obj
LDIR =../lib

LIBS=-lm


_DEPS = simd.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS)) # e.g.: DEPS = ../include/lodepng.h

_OBJ = simd.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ)) # e.g.: OBJ = obj/lodepng.o

$(ODIR)/%.o: ../src/%.c $(DEPS) # take files in obj/ that end in .o and 
	$(CC) -c -o $@ $< $(F) $(CFLAGS)  

raymarcher: $(OBJ)
	$(CC) -o $@ $^ $(F) $(CFLAGS)  $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -f $(ODIR)/geometry/*.o *~ core $(INCDIR)/*~ 
