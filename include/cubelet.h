#ifndef CUBELET_H
#define CUBELET_H

#include "cube.h"
#include "drawablegroup.h"
#include "rectangle.h"

#include <memory>

// One of the 27 small cubes: six coloured stickers plus the twelve black rims
// that frame them.  The cubelet owns all of that geometry.
class Cubelet : public DrawableGroup
{
public:
	Cubelet() = default;
	explicit Cubelet(vec3 offset);

	// Steps the rims one frame towards white (direction 1) or back towards black
	// (direction -1), over nFrames frames.
	void glowRims(int nFrames, short direction);
	void resetRims();

	const vec3 &getOffset() const { return offset; }

private:
	vec3 offset = vec3(0.0f, 0.0f, 0.0f);
	std::vector<std::unique_ptr<Drawable>> owned;
	std::vector<Drawable *> blackRims;
};

#endif
