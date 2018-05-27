#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "include/drawable.h"
class FilledCircle : public Drawable {
    public:

        //constructors
        FilledCircle (vec4 center,GLfloat radius,int np);

        //methods
        virtual void render();
		virtual void calculateNormals();
        //constructor
        ~FilledCircle();
};

#endif
