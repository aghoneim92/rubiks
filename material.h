#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "include/Angel.h"

// for material values see this link
//  http://devernay.free.fr/cours/opengl/materials.html
class Material {
    public:
        color4 ambientK;
        color4 diffuseK;
        color4 specularK;
        float shininess;

        //default constructor
        Material();

        Material(color4 ambientK, color4 diffuseK, color4 specularK,
                float shininess);

		static Material blackPlastic;
		static Material greenPlastic;
};

#endif
