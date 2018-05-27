CC=g++
CFLAGS= -lGL -lGLU -lGLEW -lglut -Wall -Wno-unused-function -g
#=========================================================

DEPS = drawable.h  \
	   cube.h sphere.h \
	   material.h light_source.h

OBJ = drawable.o \
	  cube.o sphere.o \
	  material.o light_source.o \
	  lab7.o InitShader.o 

#=========================================================

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all : ex

ex : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *o ex
