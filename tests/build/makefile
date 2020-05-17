IDIR =../include
OUT_IDIR =../../include
CC=gcc
CFLAGS= -O3 -I$(IDIR) -I$(OUT_IDIR) -g -mavx #-Wall #-pedantic # -Werror 

ODIR=obj
LDIR =../lib

LIBS=-lm

# ======================================
# OUTSIDE FILES [change if you need files outside]
# ======================================
# Define path of dependencies from outside
_OUT_DEPS = lodepng.h vec3.h utility.h
OUT_DEPS = $(patsubst %,$(OUT_IDIR)/%,$(_OUT_DEPS))

# Create objects for files from outside
$(ODIR)/%.o: ../../src/%.c $(OUT_DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

# ======================================
# INSIDE FILES  [change if you need files inside tests]
# ======================================
# Define path of dependencies from inside
_DEPS = vec3_test.h baseline_test.h approx_baseline_test.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# Create objects for files from test
$(ODIR)/%.o: ../test/%.c $(DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

# ======================================
# Run program with create objects [Add new objects also here]
# ======================================
_OBJ = lodepng.o utility.o vec3.o vec3_test.o baseline_test.o approx_baseline_test.o tests.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ)) 

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 