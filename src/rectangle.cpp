#include <rectangle.h>

Rectangle::Rectangle(){}

Rectangle::Rectangle(vec4 center, float width, float height) {
	pointsNum = 4;
	points = new vec4[pointsNum];
	normals = new vec4[pointsNum];
	vec4 dx = vec4(width/2, 0.0, 0.0, 0.0);
	vec4 dy = vec4(0.0, height/2, 0.0, 0.0);

	points[0] = center - dx - dy;
	points[1] = center - dx + dy;
	points[2] = center + dx - dy;
	points[3] = center + dx + dy;

	this->init();
}

void Rectangle::render() {
	glBindVertexArray( vao );
	glDrawArrays( GL_LINE_LOOP , 0, pointsNum);

}

Rectangle::~Rectangle() {
	delete[] points;
}

void Rectangle::calculateNormals(){
	;
}

//-------------
// FILLED Rectangle CLASS
FilledRectangle::FilledRectangle(vec4 center, float width, float height) {
	pointsNum = 4;
	points = new vec4[pointsNum];
	normals = new vec4[pointsNum];
	vec4 dx = vec4(width/2, 0.0, 0.0, 0.0);
	vec4 dy = vec4(0.0, height/2, 0.0, 0.0);

	points[0] = center - dx - dy;
	points[1] = center - dx + dy;
	points[2] = center + dx - dy;
	points[3] = center + dx + dy;

	this->init();
}

FilledRectangle::FilledRectangle(vec4 _points[4]){
	pointsNum = 4;
	points = new vec4[pointsNum];
	normals = new vec4[pointsNum];
	memcpy(points, _points, sizeof(vec4)*4);
	this->init();
}


void FilledRectangle::render() {
	glBindVertexArray( vao );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, pointsNum);
}

void FilledRectangle::calculateNormals(){
	int i;
	vec4 normal = vec4(normalize(cross(points[2]-points[0],points[1]-points[0])),0);
	for(i=0; i<4; i++)
		normals[i] = normal;
}