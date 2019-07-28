CC=g++
CFLAGS=\
-std=c++2a \
-I include \
-I /usr/local/include \
--debug

LINKERFLAGS=\
-L: /usr/local/lib/libglfw3.a \
-lstdc++ \
-framework Cocoa \
-framework CoreVideo \
-framework IOKit \
-framework OpenGL \
-framework Glut
#=========================================================

# DEPS = drawable.h  \
# 	   cube.h sphere.h \
# 	   material.h light_source.h

OBJ=\
animations.o \
circle.o \
cube.o \
cubelet.o \
drawable.o \
drawablegroup.o \
globalvariables.o \
initfunctions.o \
InitShader.o \
inputfunctions.o \
light_source.o \
source.o \
material.o \
rectangle.o \
rubikscube.o

%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@  

all: cube

cube: $(OBJ)
	$(CC) $(CFLAGS) $(LINKERFLAGS) -o $@ $^ 

clean:
	rm -rf *o cube
