#include <cubelet.h>

Cubelet::Cubelet(vec3 offset)
{
	this->offset = offset;
	int i;
	GLfloat v = 50;
	vec4 points[4] = {vec4(-v, -v, v, 1), vec4(-v, v, v, 1), vec4(v, -v, v, 1), vec4(v, v, v, 1)};
	for (i = 0; i < 4; i++)
	{
		points[i] += offset;
	}
	FilledRectangle *r = new FilledRectangle(points);
	r->material = Material(color4(1.0, 0, 0, 1), color4(1.0, 0, 0, 1), color4(1.0, 0, 0, 1), 50);

	_size = 18;
	int index = 0;
	children = new Drawable *[_size];

	children[index++] = r;

	points[0] = vec4(-v, -v, -v, 1);
	points[1] = vec4(-v, v, -v, 1);
	points[2] = vec4(-v, -v, v, 1);
	points[3] = vec4(-v, v, v, 1);
	for (i = 0; i < 4; i++)
	{
		points[i] += offset;
	}
	r = new FilledRectangle(points);
	r->material = Material(color4(0, 0, 1, 1), color4(0, 0, 1, 1), color4(0, 0, 1, 1), 50);

	children[index++] = r;

	points[0] = vec4(-v, v, v, 1);
	points[1] = vec4(-v, v, -v, 1);
	points[2] = vec4(v, v, v, 1);
	points[3] = vec4(v, v, -v, 1);
	for (i = 0; i < 4; i++)
	{
		points[i] += offset;
	}
	r = new FilledRectangle(points);
	r->material = Material(color4(1, 1, 0, 1), color4(1, 1, 0, 1), color4(1, 1, 0, 1), 50);

	children[index++] = r;

	points[0] = vec4(-v, -v, -v, 1);
	points[1] = vec4(-v, v, -v, 1);
	points[2] = vec4(v, -v, -v, 1);
	points[3] = vec4(v, v, -v, 1);
	for (i = 0; i < 4; i++)
	{
		points[i] += offset;
	}
	r = new FilledRectangle(points);
	r->material = Material(color4(1, 0.2f, 0, 1), color4(1, 0.2f, 0, 1), color4(1, 0.2f, 0, 1), 50);

	children[index++] = r;

	points[0] = vec4(v, -v, v, 1);
	points[1] = vec4(v, v, v, 1);
	points[2] = vec4(v, -v, -v, 1);
	points[3] = vec4(v, v, -v, 1);
	for (i = 0; i < 4; i++)
	{
		points[i] += offset;
	}
	r = new FilledRectangle(points);
	r->material = Material(color4(0, 1, 0, 1), color4(0, 1, 0, 1), color4(0, 1, 0, 1), 50);

	children[index++] = r;

	points[0] = vec4(-v, -v, v, 1);
	points[1] = vec4(-v, -v, -v, 1);
	points[2] = vec4(v, -v, v, 1);
	points[3] = vec4(v, -v, -v, 1);
	for (i = 0; i < 4; i++)
	{
		points[i] += offset;
	}
	r = new FilledRectangle(points);
	r->material = Material(color4(1, 1, 1, 1), color4(1, 1, 1, 1), color4(1, 1, 1, 1), 50);

	children[index++] = r;

	int j, rimIndex = 0;
	for (i = -1; i <= 1; i += 2)
		for (j = -1; j <= 1; j += 2)
		{
			blackRims[rimIndex] = new FilledCube(vec4(0, i * v, j * v, 1) + offset, 2 * v, 5, 5);
			blackRims[rimIndex]->material = Material::blackPlastic;
			children[index++] = blackRims[rimIndex++];
			blackRims[rimIndex] = new FilledCube(vec4(i * v, 0, j * v, 1) + offset, 5, 2 * v, 5);
			blackRims[rimIndex]->material = Material::blackPlastic;
			children[index++] = blackRims[rimIndex++];
			blackRims[rimIndex] = new FilledCube(vec4(i * v, j * v, 0, 1) + offset, 5, 5, 2 * v);
			blackRims[rimIndex]->material = Material::blackPlastic;
			children[index++] = blackRims[rimIndex++];
		}

	translate(offset / 2);
}

void Cubelet::glowRims(int nFrames, short direction)
{
	const Material totalDiff = Material(color4(1, 1, 1, 1) - Material::blackPlastic.ambientK,
																			color4(1, 1, 1, 1) - Material::blackPlastic.diffuseK,
																			color4(1, 1, 1, 1) - Material::blackPlastic.specularK,
																			1 - Material::blackPlastic.shininess);
	int i;
	for (i = 0; i < 12; i++)
		blackRims[i]->material = Material(blackRims[i]->material.ambientK + direction * totalDiff.ambientK / (GLfloat)nFrames,
																			blackRims[i]->material.diffuseK + direction * totalDiff.diffuseK / (GLfloat)nFrames,
																			blackRims[i]->material.specularK + direction * totalDiff.specularK / (GLfloat)nFrames,
																			blackRims[i]->material.shininess + direction * totalDiff.shininess / (GLfloat)nFrames);
}

void Cubelet::resetRims()
{
	int i;
	for (i = 0; i < 12; i++)
		blackRims[i]->material = Material::blackPlastic;
}

Cubelet::Cubelet() {}