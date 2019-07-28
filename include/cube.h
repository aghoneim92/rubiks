#ifndef __CUBE_H__
#define __CUBE_H__

#include "drawable.h"
#include <vector>

class Cube : public Drawable {
    public:
        //constructors
        Cube(vec4 center, float width ,float hight, float depth);

        //methods
        virtual void render();
        virtual void calculateNormals();

        //destructor
        ~Cube();
};

class FilledCube : public Drawable {
    public:
        //constructors
        FilledCube(vec4 center, float width ,float hight, float depth);
		FilledCube(){}
        virtual void render();
        virtual void calculateNormals();

        //destructor
        ~FilledCube();
};

#endif
