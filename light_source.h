#ifndef __LIGHT_SOURCE_H__
#define __LIGHT_SOURCE_H__

#include "include/Angel.h"

class LightSource {
    public:
        color4 ambient;
        color4 diffuse;
        color4 specular;

        //a Point (w=1) on which light source is located
        vec4 position;

        LightSource(vec4 position, color4 ambient, color4 diffuse,
                color4 specular);

        vec4 getDirection();
        vec4 getDirectionFromPoint(vec4 point);
};

#endif
