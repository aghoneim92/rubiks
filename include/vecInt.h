#ifndef VECINT_H
#define VECINT_H

// Integer lattice coordinate, used to track which cubelet and which sticker sits
// where after a face turns.
struct vec3Int
{
	int x;
	int y;
	int z;

	vec3Int(int x, int y, int z) : x(x), y(y), z(z) {}
	vec3Int(int c = 0) : x(c), y(c), z(c) {}

	void rotateZ(short direction)
	{
		const int oldX = x;
		x = -direction * y;
		y = direction * oldX;
	}

	void rotateY(short direction)
	{
		const int oldX = x;
		x = direction * z;
		z = -direction * oldX;
	}

	void rotateX(short direction)
	{
		const int oldY = y;
		y = -direction * z;
		z = direction * oldY;
	}
};

#endif
