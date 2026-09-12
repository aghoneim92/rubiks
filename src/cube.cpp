#include "cube.h"

namespace
{

/*
 *      7 +--------+ 6
 *       /        /|
 *      /        / |
 *      +--------+ |
 *      |3      2| |
 *      |        | + 5
 *      |        | /
 *      |        |/
 *      +--------+
 *      0        1
 */
void buildCorners(vec4 corners[8], vec4 center, float width, float height, float depth,
									const int dz[8])
{
	static const int dx[] = {-1, +1, +1, -1, -1, +1, +1, -1};
	static const int dy[] = {-1, -1, +1, +1, -1, -1, +1, +1};

	for (int i = 0; i < 8; i++)
	{
		corners[i] = center;
		corners[i].x += dx[i] * width / 2;
		corners[i].y += dy[i] * height / 2;
		corners[i].z += dz[i] * depth / 2;
	}
}

} // namespace

Cube::Cube(vec4 center, float width, float height, float depth)
{
	static const int dz[] = {-1, -1, -1, -1, +1, +1, +1, +1};
	vec4 corners[8];
	buildCorners(corners, center, width, height, depth, dz);

	// NOTE: indexed buffer objects are not used; the index list only keeps the
	// vertex list readable.
	static const unsigned int indices[] = {
			// near plane
			0, 1, 1, 2, 2, 3, 3, 0,
			// far plane
			4, 5, 5, 6, 6, 7, 7, 4,
			// sides
			0, 4, 1, 5, 2, 6, 3, 7};

	points.reserve(24);
	for (unsigned int index : indices)
		points.push_back(corners[index]);

	init();
}

void Cube::render()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(points.size()));
}

//--------------------------------------------------
// FILLED Cube CLASS

FilledCube::FilledCube(vec4 center, float width, float height, float depth)
{
	static const int dz[] = {+1, +1, +1, +1, -1, -1, -1, -1};
	vec4 corners[8];
	buildCorners(corners, center, width, height, depth, dz);

	// Triangles that form the cube faces.  Vertices are repeated so each one can
	// carry its own face normal, and each triangle is wound so that the cross
	// product of two of its edges points outwards.
	static const unsigned int indices[] = {
			// near plane
			0, 1, 2, 0, 2, 3,
			// far plane
			4, 6, 5, 4, 7, 6,
			// up plane
			3, 2, 6, 3, 6, 7,
			// bottom plane
			0, 5, 1, 0, 4, 5,
			// right side
			1, 5, 2, 2, 5, 6,
			// left side
			0, 3, 4, 3, 7, 4};

	points.reserve(36);
	for (unsigned int index : indices)
		points.push_back(corners[index]);

	init();
}

void FilledCube::calculateNormals()
{
	for (size_t i = 0; i < points.size(); i += 3)
	{
		const vec4 normal =
				vec4(normalize(cross(points[i + 1] - points[i], points[i + 2] - points[i])), 0);
		normals[i + 0] = normal;
		normals[i + 1] = normal;
		normals[i + 2] = normal;
	}
}

void FilledCube::render()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(points.size()));
}
