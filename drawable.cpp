#include "include/drawable.h"

GLuint Drawable::shaderProgram = 0;

Drawable::Drawable() {
    normalsCalculated = false;
    wireframe = false;
	scale = vec3(1.0f,1.0f,1.0f);
	translation = vec3(0,0,0);
	rotation = vec3(0,0,0);
	rotationAfter = vec3(0,0,0);
	overlay = false;
}

void Drawable::render() {} 
void Drawable::calculateNormals() {}

void Drawable::init() {
    // Create a vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // BUFFERS
    // Create and initialize a buffer object
    // First vertex buffer
    glGenBuffers( 1, &vbuffer );
    glBindBuffer( GL_ARRAY_BUFFER, vbuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4) * pointsNum, points, GL_STATIC_DRAW );
    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( shaderProgram, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 4, GL_FLOAT, GL_FALSE, 0,
            BUFFER_OFFSET(0));
	//glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbuffer);

    // Second normals buffer
    // Init normals before copying them to buffer
    calculateNormals();
    glGenBuffers( 1, &nbuffer );
    glBindBuffer( GL_ARRAY_BUFFER, nbuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4) * pointsNum, normals, GL_STATIC_DRAW );

    // Initialize the vertex color attribute from the vertex shader
    GLuint loc2 = glGetAttribLocation( shaderProgram, "vNormal" );
    glEnableVertexAttribArray( loc2 );
    glVertexAttribPointer( loc2, 4, GL_FLOAT, GL_FALSE, 0,
            BUFFER_OFFSET(0));

}

void Drawable::update() {
    glBindVertexArray( vao );

    //vbuffer
    glBindBuffer( GL_ARRAY_BUFFER, vbuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4) * pointsNum, points, GL_STATIC_DRAW );

    //nbuffer
    glBindBuffer( GL_ARRAY_BUFFER, nbuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4) * pointsNum, normals, GL_STATIC_DRAW );

}

Drawable::~Drawable (){
    // delete vertex array object
    glDeleteVertexArrays(1,&vao);

    // delete buffers
    glDeleteBuffers(1,&vbuffer);
    glDeleteBuffers(1,&nbuffer);

    // disable position and color attrib arays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // unbind vao and buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}