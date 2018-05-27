#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "include/drawable.h"

class Rectangle : public Drawable {
    public:
        //constructors
        Rectangle(vec4 center, float width ,float height);
		Rectangle();
        //methods
        virtual void render();
		virtual void calculateNormals();
        //destructor
        ~Rectangle();
};

class FilledRectangle : public Rectangle {
    public:
        //constructors
        FilledRectangle(vec4 center, float width ,float height);
		FilledRectangle(vec4 _points[4]);
        virtual void render();
		virtual void calculateNormals();
};

#endif
