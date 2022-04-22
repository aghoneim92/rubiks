#include <rubikscube.h>
#include <cfloat>

void RubiksCube::glowRimsFront(int index, int nFrames, short direction)
{
	int i, j;
	vec3Int indexSet;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
		{
			indexSet = indices[i][j][index];
			cubelets[indexSet.x][indexSet.y][indexSet.z].glowRims(nFrames, direction);
		}
}

void RubiksCube::resetRimsFront(int index)
{
	int i, j;
	vec3Int indexSet;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
		{
			indexSet = indices[i][j][index];
			cubelets[indexSet.x][indexSet.y][indexSet.z].resetRims();
		}
}

void RubiksCube::glowRimsHorizontal(int index, int nFrames, short direction)
{
	int i, k;
	vec3Int indexSet;
	for (i = 0; i < 3; i++)
		for (k = 0; k < 3; k++)
		{
			indexSet = indices[i][index][k];
			cubelets[indexSet.x][indexSet.y][indexSet.z].glowRims(nFrames, direction);
		}
}

void RubiksCube::resetRimsHorizontal(int index)
{
	int i, k;
	vec3Int indexSet;
	for (i = 0; i < 3; i++)
		for (k = 0; k < 3; k++)
		{
			indexSet = indices[i][index][k];
			cubelets[indexSet.x][indexSet.y][indexSet.z].resetRims();
		}
}

void RubiksCube::glowRimsSide(int index, int nFrames, short direction)
{
	int j, k;
	vec3Int indexSet;
	for (j = 0; j < 3; j++)
		for (k = 0; k < 3; k++)
		{
			indexSet = indices[index][j][k];
			cubelets[indexSet.x][indexSet.y][indexSet.z].glowRims(nFrames, direction);
		}
}

void RubiksCube::resetRimsSide(int index)
{
	int j, k;
	vec3Int indexSet;
	for (j = 0; j < 3; j++)
		for (k = 0; k < 3; k++)
		{
			indexSet = indices[index][j][k];
			cubelets[indexSet.x][indexSet.y][indexSet.z].resetRims();
		}
}

void RubiksCube::rotateHorizontal(int index, float angle)
{
	int i, k;
	vec3Int indexSet;
	for (i = 0; i < 3; i++)
		for (k = 0; k < 3; k++)
		{
			indexSet = indices[i][index][k];
			cubelets[indexSet.x][indexSet.y][indexSet.z].rotateAfter(vec3(0, angle, 0));
		}
}

void RubiksCube::rotateFront(int index, float angle)
{
	int i, j;
	vec3Int indexSet;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
		{
			indexSet = indices[i][j][index];
			cubelets[indexSet.x][indexSet.y][indexSet.z].rotateAfter(vec3(0, 0, angle));
		}
}

void RubiksCube::finalizeRotationCommon(vec3Int indexSet)
{
	cubelets[indexSet.x][indexSet.y][indexSet.z].rotateAfter(-cubelets[indexSet.x][indexSet.y][indexSet.z].getRotationAfter());
	int i;
	for (i = 0; i < _size; i++)
	{
		children[i]->calculateNormals();
		children[i]->update();
	}
}

void RubiksCube::finalizeRotationFront(int index, short direction)
{
	int i, j;
	vec3Int indexSet;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
		{
			indexSet = indices[i][j][index];
			cubelets[indexSet.x][indexSet.y][indexSet.z].rotatePermanentZ(direction * M_PI / 2);
			finalizeRotationCommon(indexSet);
		}
}

void RubiksCube::finalizeRotationSide(int index, short direction)
{
	int j, k;
	vec3Int indexSet;
	for (j = 0; j < 3; j++)
		for (k = 0; k < 3; k++)
		{
			indexSet = indices[index][j][k];
			cubelets[indexSet.x][indexSet.y][indexSet.z].rotatePermanentX(direction * M_PI / 2);
			finalizeRotationCommon(indexSet);
		}
}

void RubiksCube::finalizeRotationHorizontal(int index, short direction)
{
	int i, k;
	vec3Int indexSet;
	for (i = 0; i < 3; i++)
		for (k = 0; k < 3; k++)
		{
			indexSet = indices[i][index][k];
			cubelets[indexSet.x][indexSet.y][indexSet.z].rotatePermanentY(direction * M_PI / 2);
			finalizeRotationCommon(indexSet);
		}
}

void RubiksCube::rotateSide(int index, float angle)
{
	int j, k;
	vec3Int indexSet;
	for (j = 0; j < 3; j++)
		for (k = 0; k < 3; k++)
		{
			indexSet = indices[index][j][k];
			cubelets[indexSet.x][indexSet.y][indexSet.z].rotateAfter(vec3(angle, 0, 0));
		}
}

const int indexRange[] = {-3, -2, 0, 2, 3};

void RubiksCube::updateIndicesSide(int index, short direction)
{
	int j, k;
	vec3Int toRotate;
	vec3Int oldIndices[3][3][3];
	memcpy(oldIndices, indices, sizeof(vec3Int) * 27);
	for (j = 0; j < 3; j++)
		for (k = 0; k < 3; k++)
		{
			toRotate = vec3Int(index - 1, j - 1, k - 1);
			toRotate.rotateX(direction);
			indices[toRotate.x + 1][toRotate.y + 1][toRotate.z + 1] = oldIndices[index][j][k];
		}

	//Front and Top
	vec3Int center = vec3Int((index - 1) * 2, 0, 0);
	vec3Int oldColorIndices[7][7][7];
	memcpy(oldColorIndices, colorIndices, sizeof(vec3Int) * 7 * 7 * 7);
	for (j = 0; j < 5; j++)
	{
		for (k = 0; k < 5; k++)
		{
			toRotate = vec3Int(center.x, indexRange[j], indexRange[k]);
			toRotate.rotateX(direction);
			colorIndices[toRotate.x + 3][toRotate.y + 3][toRotate.z + 3] = oldColorIndices[center.x + 3][indexRange[j] + 3][indexRange[k] + 3];
		}
	}

	//Side
	if (index != 1)
	{
		center = vec3Int((index - 1) * 3, 0, 0);
		for (j = 1; j < 4; j++)
		{
			for (k = 1; k < 4; k++)
			{
				toRotate = vec3Int(center.x, indexRange[j], indexRange[k]);
				toRotate.rotateX(direction);
				colorIndices[toRotate.x + 3][toRotate.y + 3][toRotate.z + 3] = oldColorIndices[center.x + 3][indexRange[j] + 3][indexRange[k] + 3];
			}
		}
	}
}

void RubiksCube::updateIndicesFront(int index, short direction)
{
	int i, j;
	vec3Int toRotate;
	vec3Int oldIndices[3][3][3];
	memcpy(oldIndices, indices, sizeof(vec3Int) * 27);
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
		{
			toRotate = vec3Int(i - 1, j - 1, index - 1);
			toRotate.rotateZ(direction);
			indices[toRotate.x + 1][toRotate.y + 1][toRotate.z + 1] = oldIndices[i][j][index];
		}

	//Side and Top
	vec3Int center = vec3Int(0, 0, (index - 1) * 2);
	vec3Int oldColorIndices[7][7][7];
	memcpy(oldColorIndices, colorIndices, sizeof(vec3Int) * 7 * 7 * 7);
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			toRotate = vec3Int(indexRange[i], indexRange[j], center.z);
			toRotate.rotateZ(direction);
			colorIndices[toRotate.x + 3][toRotate.y + 3][toRotate.z + 3] = oldColorIndices[indexRange[i] + 3][indexRange[j] + 3][center.z + 3];
		}
	}

	//Front
	if (index != 1)
	{
		center = vec3Int(0, 0, (index - 1) * 3);
		for (i = 1; i < 4; i++)
		{
			for (j = 1; j < 4; j++)
			{
				toRotate = vec3Int(indexRange[i], indexRange[j], center.z);
				toRotate.rotateZ(direction);
				colorIndices[toRotate.x + 3][toRotate.y + 3][toRotate.z + 3] = oldColorIndices[indexRange[i] + 3][indexRange[j] + 3][center.z + 3];
			}
		}
	}
	return;
}
bool RubiksCube::checkWin()
{
	int k = 3, i, j;
	vec3Int colorIndex = colorIndices[1][1][6];
	char lastColor = colors[colorIndex.x][colorIndex.y][colorIndex.z];
	for (i = -2; i <= 2; i += 2)
	{
		for (j = -2; j <= 2; j += 2)
		{
			colorIndex = colorIndices[i + 3][j + 3][k + 3];
			if (colors[colorIndex.x][colorIndex.y][colorIndex.z] != lastColor)
				return false;
			lastColor = colors[colorIndex.x][colorIndex.y][colorIndex.z];
		}
	}
	k = -3;
	colorIndex = colorIndices[1][1][0];
	lastColor = colors[colorIndex.x][colorIndex.y][colorIndex.z];
	for (i = -2; i <= 2; i += 2)
	{
		for (j = -2; j <= 2; j += 2)
		{
			colorIndex = colorIndices[i + 3][j + 3][k + 3];
			if (colors[colorIndex.x][colorIndex.y][colorIndex.z] != lastColor)
				return false;
			lastColor = colors[colorIndex.x][colorIndex.y][colorIndex.z];
		}
	}

	i = 3;
	colorIndex = colorIndices[6][1][1];
	lastColor = colors[colorIndex.x][colorIndex.y][colorIndex.z];
	for (j = -2; j <= 2; j += 2)
	{
		for (k = -2; k <= 2; k += 2)
		{
			colorIndex = colorIndices[i + 3][j + 3][k + 3];
			if (colors[colorIndex.x][colorIndex.y][colorIndex.z] != lastColor)
				return false;
			lastColor = colors[colorIndex.x][colorIndex.y][colorIndex.z];
		}
	}
	i = -3;
	colorIndex = colorIndices[0][1][1];
	lastColor = colors[colorIndex.x][colorIndex.y][colorIndex.z];
	for (j = -2; j <= 2; j += 2)
	{
		for (k = -2; k <= 2; k += 2)
		{
			colorIndex = colorIndices[i + 3][j + 3][k + 3];
			if (colors[colorIndex.x][colorIndex.y][colorIndex.z] != lastColor)
				return false;
			lastColor = colors[colorIndex.x][colorIndex.y][colorIndex.z];
		}
	}
	j = 3;
	colorIndex = colorIndices[1][6][1];
	lastColor = colors[colorIndex.x][colorIndex.y][colorIndex.z];
	for (i = -2; i <= 2; i += 2)
	{
		for (k = -2; k <= 2; k += 2)
		{
			colorIndex = colorIndices[i + 3][j + 3][k + 3];
			if (colors[colorIndex.x][colorIndex.y][colorIndex.z] != lastColor)
				return false;
			lastColor = colors[colorIndex.x][colorIndex.y][colorIndex.z];
		}
	}
	j = -3;
	colorIndex = colorIndices[1][0][1];
	lastColor = colors[colorIndex.x][colorIndex.y][colorIndex.z];
	for (i = -2; i <= 2; i += 2)
	{
		for (k = -2; k <= 2; k += 2)
		{
			colorIndex = colorIndices[i + 3][j + 3][k + 3];
			if (colors[colorIndex.x][colorIndex.y][colorIndex.z] != lastColor)
				return false;
			lastColor = colors[colorIndex.x][colorIndex.y][colorIndex.z];
		}
	}

	return true;
}

void RubiksCube::updateIndicesHorizontal(int index, short direction)
{
	int i, k;
	vec3Int toRotate;
	vec3Int oldIndices[3][3][3];
	memcpy(oldIndices, indices, sizeof(vec3Int) * 27);
	for (i = 0; i < 3; i++)
		for (k = 0; k < 3; k++)
		{
			toRotate = vec3Int(i - 1, index - 1, k - 1);
			toRotate.rotateY(direction);
			indices[toRotate.x + 1][toRotate.y + 1][toRotate.z + 1] = oldIndices[i][index][k];
		}

	//Side and front
	vec3Int center = vec3Int(0, (index - 1) * 2, 0);
	vec3Int oldColorIndices[7][7][7];
	memcpy(oldColorIndices, colorIndices, sizeof(vec3Int) * 7 * 7 * 7);
	for (i = 0; i < 5; i++)
	{
		for (k = 0; k < 5; k++)
		{
			toRotate = vec3Int(indexRange[i], center.y, indexRange[k]);
			toRotate.rotateY(direction);
			colorIndices[toRotate.x + 3][toRotate.y + 3][toRotate.z + 3] = oldColorIndices[indexRange[i] + 3][center.y + 3][indexRange[k] + 3];
		}
	}

	//Top
	if (index != 1)
	{
		center = vec3Int(0, (index - 1) * 3, 0);
		for (i = 1; i < 4; i++)
		{
			for (k = 1; k < 4; k++)
			{
				toRotate = vec3Int(indexRange[i], center.y, indexRange[k]);
				toRotate.rotateY(direction);
				colorIndices[toRotate.x + 3][toRotate.y + 3][toRotate.z + 3] = oldColorIndices[indexRange[i] + 3][center.y + 3][indexRange[k] + 3];
			}
		}
	}
	return;
}

RubiksCube::RubiksCube()
{
	int i, j, k;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
			{
				cubelets[i][j][k] = Cubelet(vec3((i - 1) * 100.0f, (j - 1) * 100.0f, (k - 1) * 100.0f));
				indices[i][j][k] = vec3Int(i, j, k);
			}

	_size = 27 * cubelets[0][0][0].size();
	children = new Drawable *[_size];
	int offset = 0;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
			{
				memcpy(children + offset, cubelets[i][j][k].getChildren(), sizeof(Drawable *) * cubelets[0][0][0].size());
				offset += cubelets[0][0][0].size();
			}

	//Front Faces
	vec3Int center = vec3Int(0, 0, 3);
	for (i = -2; i <= 2; i += 2)
	{
		for (j = -2; j <= 2; j += 2)
		{
			colors[i + 3][j + 3][center.z + 3] = 'r';
			colorIndices[i + 3][j + 3][center.z + 3] = vec3Int(i + 3, j + 3, center.z + 3);
		}
	}

	center = vec3Int(0, 0, -3);
	for (i = -2; i <= 2; i += 2)
	{
		for (j = -2; j <= 2; j += 2)
		{
			colors[i + 3][j + 3][center.z + 3] = 'o';
			colorIndices[i + 3][j + 3][center.z + 3] = vec3Int(i + 3, j + 3, center.z + 3);
		}
	}

	//Side Faces
	center = vec3Int(-3, 0, 0);
	for (j = -2; j <= 2; j += 2)
	{
		for (k = -2; k <= 2; k += 2)
		{
			colors[center.x + 3][j + 3][k + 3] = 'b';
			colorIndices[center.x + 3][j + 3][k + 3] = vec3Int(center.x + 3, j + 3, k + 3);
		}
	}

	center = vec3Int(3, 0, 0);
	for (j = -2; j <= 2; j += 2)
	{
		for (k = -2; k <= 2; k += 2)
		{
			colors[center.x + 3][j + 3][k + 3] = 'g';
			colorIndices[center.x + 3][j + 3][k + 3] = vec3Int(center.x + 3, j + 3, k + 3);
		}
	}

	//Top Faces
	center = vec3Int(0, 3, 0);
	for (i = -2; i <= 2; i += 2)
	{
		for (k = -2; k <= 2; k += 2)
		{
			colors[i + 3][center.y + 3][k + 3] = 'y';
			colorIndices[i + 3][center.y + 3][k + 3] = vec3Int(i + 3, center.y + 3, k + 3);
		}
	}

	center = vec3Int(0, -3, 0);
	for (i = -2; i <= 2; i += 2)
	{
		for (k = -2; k <= 2; k += 2)
		{
			colors[i + 3][center.y + 3][k + 3] = 'w';
			colorIndices[i + 3][center.y + 3][k + 3] = vec3Int(i + 3, center.y + 3, k + 3);
		}
	}
}

bool RubiksCube::assemble()
{
	int i;
	bool ret = true;
	for (i = 0; i < _size; i++)
	{
		if (abs(children[i]->translation.x) > FLT_EPSILON)
		{
			ret = false;
			children[i]->translation.x += children[i]->translation.x > 0 ? -1 : 1;
		}
		if (abs(children[i]->translation.y) > FLT_EPSILON)
		{
			ret = false;
			children[i]->translation.y += children[i]->translation.y > 0 ? -1 : 1;
		}
		if (abs(children[i]->translation.z) > FLT_EPSILON)
		{
			ret = false;
			children[i]->translation.z += children[i]->translation.z > 0 ? -1 : 1;
		}
	}
	return ret;
}

void RubiksCube::translateCubelet(int i, int j, int k, vec3 translation)
{
	cubelets[i][j][k].translate(vec3(cubelets[i][j][k].offset.x * translation.x,
																	 cubelets[i][j][k].offset.y * translation.y,
																	 cubelets[i][j][k].offset.z * translation.z));
	//cubelets[i][j][k].translate(translation);
}

void RubiksCube::translateCubeletNormal(int i, int j, int k, vec3 translation)
{
	cubelets[i][j][k].translate(translation);
}

void RubiksCube::scaleCubelet(int i, int j, int k, vec3 scale)
{
	cubelets[i][j][k].scale(scale);
}

void RubiksCube::rotateCubelet(int i, int j, int k, vec3 rotation)
{
	cubelets[i][j][k].rotate(rotation);
}
