#include "circle.h"

FilledCircle::FilledCircle(vec4 center, GLfloat radius, int segments)
{
	points.reserve(segments + 1); // segments + the centre point
	points.push_back(center);
	for (int i = 0; i < segments; i++)
	{
		const double angle = 360.0 / (segments - 1) * i;
		const double radians = angle / 180.0 * 3.1415;
		points.push_back(center + vec4(radius * cos(radians), radius * sin(radians), 0.0, 0.0));
	}
	init();
}

void FilledCircle::render()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(points.size()));
}

void FilledCircle::calculateNormals()
{
	const vec4 l0 = points[points.size() / 2] - points[0];
	const vec4 l1 = points[1] - points[0];
	const vec4 normal = vec4(cross(l1, l0), 0);
	for (vec4 &n : normals)
		n = normal;
}
