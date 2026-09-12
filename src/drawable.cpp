#include "drawable.h"

GLuint Drawable::shaderProgram = 0;

void Drawable::init()
{
	normals.resize(points.size());

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * points.size(), points.data(), GL_STATIC_DRAW);

	const GLint positionLoc = glGetAttribLocation(shaderProgram, "vPosition");
	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// Normals have to exist before they can be copied to their buffer.
	calculateNormals();
	glGenBuffers(1, &nbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, nbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * normals.size(), normals.data(), GL_STATIC_DRAW);

	const GLint normalLoc = glGetAttribLocation(shaderProgram, "vNormal");
	glEnableVertexAttribArray(normalLoc);
	glVertexAttribPointer(normalLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindVertexArray(0);
}

void Drawable::update()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * points.size(), points.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * normals.size(), normals.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Drawable::~Drawable()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbuffer);
	glDeleteBuffers(1, &nbuffer);
}
