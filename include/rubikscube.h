#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include "cubelet.h"
#include "vecInt.h"

#include <array>

// The cube itself: 27 cubelets plus the bookkeeping that tracks where each one,
// and each sticker colour, ended up after a sequence of face turns.
class RubiksCube : public DrawableGroup
{
public:
	RubiksCube();

	bool checkWin();
	// Steps the exploded cubelets one frame closer to home; true once assembled.
	bool assemble();

	void rotateHorizontal(int index, float angle);
	void updateIndicesHorizontal(int index, short direction);
	void finalizeRotationHorizontal(int index, short direction);
	void rotateFront(int index, float angle);
	void updateIndicesFront(int index, short direction);
	void finalizeRotationFront(int index, short direction);
	void rotateSide(int index, float angle);
	void updateIndicesSide(int index, short direction);
	void finalizeRotationSide(int index, short direction);

	void glowRimsFront(int index, int nFrames, short direction);
	void resetRimsFront(int index);

	void glowRimsHorizontal(int index, int nFrames, short direction);
	void resetRimsHorizontal(int index);

	void glowRimsSide(int index, int nFrames, short direction);
	void resetRimsSide(int index);

	void translateCubelet(int i, int j, int k, vec3 translation);
	void translateCubeletNormal(int i, int j, int k, vec3 translation);
	void scaleCubelet(int i, int j, int k, vec3 scale);
	void rotateCubelet(int i, int j, int k, vec3 rotation);

private:
	template <typename T, size_t N>
	using Grid = std::array<std::array<std::array<T, N>, N>, N>;

	// Clears the transient rotation of one cubelet once its turn has been baked
	// into the vertex data.
	void clearRotationAfter(vec3Int indexSet);
	// Re-uploads every child's vertices and normals after a turn is baked in.
	void uploadAll();

	Cubelet cubelets[3][3][3];
	Grid<vec3Int, 3> indices;
	Grid<vec3Int, 7> colorIndices;
	Grid<char, 7> colors;
};

#endif
