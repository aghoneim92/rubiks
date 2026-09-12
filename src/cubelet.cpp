#include "cubelet.h"

namespace
{

constexpr GLfloat kHalfWidth = 50; // half the width of a cubelet
constexpr GLfloat kRimWidth = 5;

// Every cubelet vertex carries w = 2 rather than the usual 1.  That is not a
// typo: the original code offset each vec4 corner by a vec3, which Angel
// silently promotes to vec4(offset, 1), so the w components added up to 2.  The
// perspective divide then renders the whole cube at half size, and the swipe
// thresholds in checkSwipe() are calibrated against that halved world space, so
// the convention is kept deliberately.
constexpr GLfloat kVertexW = 2;

// The six stickers, in the order they are drawn.  Each row lists the four
// corners of the quad (in units of kHalfWidth) followed by its colour.
struct StickerSpec
{
	int corners[4][3];
	color4 color;
};

const StickerSpec kStickers[] = {
		{{{-1, -1, +1}, {-1, +1, +1}, {+1, -1, +1}, {+1, +1, +1}}, color4(1.0f, 0.0f, 0.0f, 1)},
		{{{-1, -1, -1}, {-1, +1, -1}, {-1, -1, +1}, {-1, +1, +1}}, color4(0.0f, 0.0f, 1.0f, 1)},
		{{{-1, +1, +1}, {-1, +1, -1}, {+1, +1, +1}, {+1, +1, -1}}, color4(1.0f, 1.0f, 0.0f, 1)},
		{{{-1, -1, -1}, {-1, +1, -1}, {+1, -1, -1}, {+1, +1, -1}}, color4(1.0f, 0.2f, 0.0f, 1)},
		{{{+1, -1, +1}, {+1, +1, +1}, {+1, -1, -1}, {+1, +1, -1}}, color4(0.0f, 1.0f, 0.0f, 1)},
		{{{-1, -1, +1}, {-1, -1, -1}, {+1, -1, +1}, {+1, -1, -1}}, color4(1.0f, 1.0f, 1.0f, 1)},
};

} // namespace

Cubelet::Cubelet(vec3 offset) : offset(offset)
{
	owned.reserve(18);
	blackRims.reserve(12);

	for (const StickerSpec &sticker : kStickers)
	{
		vec4 corners[4];
		for (int i = 0; i < 4; i++)
			corners[i] = vec4(sticker.corners[i][0] * kHalfWidth + offset.x,
												sticker.corners[i][1] * kHalfWidth + offset.y,
												sticker.corners[i][2] * kHalfWidth + offset.z,
												kVertexW);

		auto quad = std::make_unique<FilledRectangle>(corners);
		quad->material = Material(sticker.color, sticker.color, sticker.color, 50);
		owned.push_back(std::move(quad));
	}

	// The twelve edges of the cubelet, three per (i, j) corner pair.
	for (int i = -1; i <= 1; i += 2)
		for (int j = -1; j <= 1; j += 2)
		{
			const vec4 centers[3] = {
					vec4(offset.x, i * kHalfWidth + offset.y, j * kHalfWidth + offset.z, kVertexW),
					vec4(i * kHalfWidth + offset.x, offset.y, j * kHalfWidth + offset.z, kVertexW),
					vec4(i * kHalfWidth + offset.x, j * kHalfWidth + offset.y, offset.z, kVertexW),
			};
			const vec3 sizes[3] = {
					vec3(2 * kHalfWidth, kRimWidth, kRimWidth),
					vec3(kRimWidth, 2 * kHalfWidth, kRimWidth),
					vec3(kRimWidth, kRimWidth, 2 * kHalfWidth),
			};

			for (int k = 0; k < 3; k++)
			{
				auto rim = std::make_unique<FilledCube>(centers[k], sizes[k].x, sizes[k].y, sizes[k].z);
				rim->material = Material::blackPlastic;
				blackRims.push_back(rim.get());
				owned.push_back(std::move(rim));
			}
		}

	children.reserve(owned.size());
	for (const std::unique_ptr<Drawable> &child : owned)
		children.push_back(child.get());

	translate(offset / 2);
}

void Cubelet::glowRims(int nFrames, short direction)
{
	static const Material totalDiff =
			Material(color4(1, 1, 1, 1) - Material::blackPlastic.ambientK,
							 color4(1, 1, 1, 1) - Material::blackPlastic.diffuseK,
							 color4(1, 1, 1, 1) - Material::blackPlastic.specularK,
							 1 - Material::blackPlastic.shininess);

	const GLfloat step = static_cast<GLfloat>(direction) / static_cast<GLfloat>(nFrames);
	for (Drawable *rim : blackRims)
		rim->material = Material(rim->material.ambientK + totalDiff.ambientK * step,
														 rim->material.diffuseK + totalDiff.diffuseK * step,
														 rim->material.specularK + totalDiff.specularK * step,
														 rim->material.shininess + totalDiff.shininess * step);
}

void Cubelet::resetRims()
{
	for (Drawable *rim : blackRims)
		rim->material = Material::blackPlastic;
}
