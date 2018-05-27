#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H
#include "cubelet.h"
#include "vecInt.h"

class RubiksCube:public DrawableGroup{
public:
	RubiksCube();
	bool checkWin();
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
	void finalizeRotationCommon(vec3Int indexSet);

	void glowRimsFront(int index, int nFrames, short direction);
	void resetRimsFront(int index);

	void glowRimsHorizontal(int index, int nFrames, short direction);
	void resetRimsHorizontal(int index);

	void glowRimsSide(int index, int nFrames, short direction);
	void resetRimsSide(int index);

	void translateCubelet(int i,int j, int k, vec3 translation);
	void translateCubeletNormal(int i, int j, int k, vec3 translation);
	void scaleCubelet(int i, int j, int k, vec3 scale);
	void rotateCubelet(int i, int j, int k, vec3 rotation);
private:
	Cubelet cubelets[3][3][3];
	vec3Int indices[3][3][3];
	vec3Int colorIndices[7][7][7];
	char colors[7][7][7];
};

#endif