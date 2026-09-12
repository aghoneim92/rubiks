#include "rectangle.h"

namespace
{

void buildCorners(std::vector<vec4> &points, vec4 center, float width, float height)
{
	const vec4 dx = vec4(width / 2, 0.0, 0.0, 0.0);
	const vec4 dy = vec4(0.0, height / 2, 0.0, 0.0);

	points = {center - dx - dy, center - dx + dy, center + dx - dy, center + dx + dy};
}

} // namespace

Rectangle::Rectangle(vec4 center, float width, float height)
{
	buildCorners(points, center, width, height);
	init();
}

void Rectangle::render()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(points.size()));
}

//-------------
// FILLED Rectangle CLASS

FilledRectangle::FilledRectangle(vec4 center, float width, float height)
{
	buildCorners(points, center, width, height);
	init();
}

FilledRectangle::FilledRectangle(const vec4 corners[4])
{
	points.assign(corners, corners + 4);
	init();
}

void FilledRectangle::render()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(points.size()));
}

void FilledRectangle::calculateNormals()
{
	const vec4 normal = vec4(normalize(cross(points[2] - points[0], points[1] - points[0])), 0);
	for (vec4 &n : normals)
		n = normal;
}
