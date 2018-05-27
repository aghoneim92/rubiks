#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "include/Angel.h"
#include "material.h"
#include "light_source.h"
#include <vector>

class Drawable {
    public:
        static GLuint shaderProgram;

		vec3 scale;
		vec3 translation;
		vec3 rotation;
		vec3 rotationAfter;

        int pointsNum;
        vec4* points;
        vec4* normals;
		//vec3 rotation;
        Material material;

		bool overlay;

        Drawable();

        //methods
        // copy points and colors back to opengl buffers
        void update();
        //calculate a unit normal vector for each vertex
        virtual void calculateNormals();

        virtual void render();

        //destructor
        ~Drawable();

    protected:

        bool normalsCalculated;
        bool wireframe;

        //vertex array object
        GLuint vao;

        //vertex buffer
        GLuint vbuffer;

        //color buffer
        GLuint nbuffer;

        void init();
};

#endif
