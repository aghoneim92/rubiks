#include "inputfunctions.h"

#include "animations.h"
#include "globalvariables.h"
#include "picking.h"

#include <cfloat>
#include <cmath>

void special(platform::SpecialKey key)
{
	switch (key)
	{
	case platform::SpecialKey::Up:
		newGlowIndex = glowIndex - 1;
		if (newGlowIndex < 0)
			newGlowIndex = 2;
		newGlowFace = glowFace;
		faceChanged = true;
		break;
	case platform::SpecialKey::Down:
		newGlowIndex = (glowIndex + 1) % 3;
		newGlowFace = glowFace;
		faceChanged = true;
		break;
	case platform::SpecialKey::Right:
		newGlowIndex = glowIndex;
		newGlowFace = glowFace - 1;
		if (newGlowFace < 0)
			newGlowFace = 2;
		faceChanged = true;
		break;
	case platform::SpecialKey::Left:
		newGlowIndex = glowIndex;
		newGlowFace = (glowFace + 1) % 3;
		faceChanged = true;
		break;
	}
}

namespace
{

// World-space points where the drag started and ended, and what they imply.
vec3 dragStart, dragEnd, dragDelta, dragMiddle;

} // namespace

void checkSwipe(int x, int y, bool released)
{
	const vec3 position = picking::worldPositionAt(x, y);

	if (!released)
	{
		dragStart = position;
		return;
	}

	bool keepGlowing = false;
	short moveCode = 0;
	dragEnd = position;
	dragDelta = dragEnd - dragStart;
	dragMiddle = dragStart + dragDelta / 2.0f;

	if (std::abs(dragDelta[0]) > 20)
		moveCode |= 1;
	if (std::abs(dragDelta[1]) > 20)
		moveCode |= (1 << 1);
	if (std::abs(dragDelta[2]) > 20)
		moveCode |= (1 << 2);

	switch (moveCode)
	{
	case 1:
		if (std::abs(dragMiddle[2]) <= 72.0)
		{
			moveType = 'f';
			rotDir = (dragDelta[0] > 0 ? -1 : 1);
			if (dragMiddle[2] > -75 && dragMiddle[2] < -12.5)
				faceIndex = 0;
			else if (dragMiddle[2] > -12.5 && dragMiddle[2] < 12.5)
				faceIndex = 1;
			else if (dragMiddle[2] < 75)
				faceIndex = 2;
			rotateFace(0);
			break;
		}
		moveType = 'h';
		rotDir = (dragDelta[0] > 0 ? 1 : -1);
		rotDir *= (dragMiddle[2] > 0 ? 1 : -1);
		if (dragMiddle[1] > -75 && dragMiddle[1] < -12.5)
			faceIndex = 0;
		else if (dragMiddle[1] > -12.5 && dragMiddle[1] < 12.5)
			faceIndex = 1;
		else if (dragMiddle[1] < 75)
			faceIndex = 2;
		rotateFace(0);
		break;
	case 2:
		if (dragMiddle[0] > -74 && dragMiddle[0] < 74)
		{
			moveType = 's';
			rotDir = (dragDelta[1] > 0 ? 1 : -1);
			rotDir *= (dragMiddle[2] > 0 ? -1 : 1);
			if (dragMiddle[0] > -75 && dragMiddle[0] < -12.5)
				faceIndex = 0;
			else if (dragMiddle[0] > -12.5 && dragMiddle[0] < 12.5)
				faceIndex = 1;
			else if (dragMiddle[0] < 75)
				faceIndex = 2;
			rotateFace(0);
		}
		else
		{
			moveType = 'f';
			rotDir = (dragDelta[1] > 0 ? 1 : -1);
			rotDir *= (dragMiddle[0] > 0 ? 1 : -1);
			if (dragMiddle[2] > -75 && dragMiddle[2] < -25)
				faceIndex = 0;
			else if (dragMiddle[2] > -25 && dragMiddle[2] < 25)
				faceIndex = 1;
			else if (dragMiddle[2] < 75)
				faceIndex = 2;
			rotateFace(0);
		}
		break;
	case 4:
		if (dragMiddle[0] < -74 || dragMiddle[0] > 74)
		{
			moveType = 'h';
			rotDir = (dragDelta[2] > 0) ? 1 : -1;
			rotDir *= (dragMiddle[0] > 0) ? -1 : 1;
			if (dragMiddle[1] > -75 && dragMiddle[1] < -12.5)
				faceIndex = 0;
			else if (dragMiddle[1] > -12.5 && dragMiddle[1] < 12.5)
				faceIndex = 1;
			else if (dragMiddle[1] < 75)
				faceIndex = 2;
			rotateFace(0);
		}
		break;
	default:
		keepGlowing = true;
	}

	if (!keepGlowing)
	{
		faceChanged = true;
		newGlowIndex = noGlowIndex;
	}
}

void mouse(bool released, int x, int y)
{
	if (dir == 'l')
		return;
	// Releasing before the game has started is ignored, but the press that
	// begins a drag is still recorded.
	if (!start && released)
		return;
	if (shuffled)
		checkSwipe(x, y, released);
}

namespace
{

void anyKey(unsigned char key)
{
	easterEgg.append(1, key);
	bool oneMatchAtLeast = false;

	for (size_t i = 0; i < easterEggs.size(); i++)
	{
		const size_t prefix = std::min(easterEggs[i].size(), easterEgg.size());
		if (easterEggs[i].compare(0, prefix, easterEgg, 0, prefix) != 0)
			continue;

		if (easterEgg.compare(easterEggs[i]) == 0)
		{
			switch (i)
			{
			case 0:
				glUniform1i(grayscale_loc, grayScale = !grayScale);
				for (std::unique_ptr<Drawable> &speck : noise)
					speck->scale = grayScale ? vec3(1.0f, 1.0f, 1.0f) : vec3(0, 0, 0);
				if (grayScale)
					animateNoise(0);
				break;
			case 1:
				glUniform1i(water_loc, waterEffect = !waterEffect);
				if (waterEffect)
					animateWater(0);
				break;
			case 2:
				glUniform1i(dark_loc, darkEffect = !darkEffect);
				break;
			case 3:
				lsdEffect = !lsdEffect;
				animateLSD(0);
				break;
			case 4:
				if (redEffect - 1.0f < FLT_EPSILON)
				{
					glUniform1f(redchrome_loc, redEffect = 2.0f);
					glUniform1i(grayscale_loc, grayScale = true);
				}
				else
				{
					glUniform1f(redchrome_loc, redEffect = 1.0f);
					if (greenEffect - 1.0f < FLT_EPSILON && blueEffect - 1.0f < FLT_EPSILON)
						glUniform1i(grayscale_loc, grayScale = false);
				}
				break;
			case 5:
				if (greenEffect - 1.0f < FLT_EPSILON)
				{
					glUniform1f(greenchrome_loc, greenEffect = 2.0f);
					glUniform1i(grayscale_loc, grayScale = true);
				}
				else
				{
					glUniform1f(greenchrome_loc, greenEffect = 1.0f);
					if (redEffect - 1.0f < FLT_EPSILON && blueEffect - 1.0f < FLT_EPSILON)
						glUniform1i(grayscale_loc, grayScale = false);
				}
				break;
			case 6:
				if (blueEffect - 1.0f < FLT_EPSILON)
				{
					glUniform1f(bluechrome_loc, blueEffect = 2.0f);
					glUniform1i(grayscale_loc, grayScale = true);
				}
				else
				{
					glUniform1f(bluechrome_loc, blueEffect = 1.0f);
					if (redEffect - 1.0f < FLT_EPSILON && greenEffect - 1.0f < FLT_EPSILON)
						glUniform1i(grayscale_loc, grayScale = false);
				}
				break;
			case 7:
				if (harlemShaked)
				{
					for (Drawable *shape : shapes)
					{
						shape->scale = vec3(1.0f, 1.0f, 1.0f);
						shape->rotation = vec3(0, 0, 0);
						shape->translation = vec3(0, 0, 0);
					}
					harlemShaked = false;
				}
				else
				{
					animateHarlemShake1(0);
				}
				break;
			}
			easterEgg.clear();
			return;
		}
		oneMatchAtLeast = true;
	}

	if (!oneMatchAtLeast)
		easterEgg.clear();
}

} // namespace

void keyboard(unsigned char key)
{
	if (dir == 'l')
		return;
	const float deltaAng = 4;
	switch (key)
	{
	case 033: // Escape Key
	case 'q':
	case 'Q':
		platform::quit();
		break;
	case ' ':
		if (!start)
			start = true;
		break;
	case 'a':
		eye = RotateY(deltaAng) * eye;
		break;
	case 'd':
		eye = RotateY(-deltaAng) * eye;
		break;
	case 'w':
		eye = RotateX(deltaAng) * eye;
		break;
	case 's':
		eye = RotateX(-deltaAng) * eye;
		break;
	case '-':
		if (rotDone && glowIndex != noGlowIndex)
		{
			rotDone = false;
			rotDir = -1;
			moveType = moveTypes[(int)glowFace];
			faceIndex = glowIndex;
			rotateFace(0);
		}
		break;
	case '+':
		if (rotDone && glowIndex != noGlowIndex)
		{
			rotDone = false;
			rotDir = 1;
			moveType = moveTypes[(int)glowFace];
			faceIndex = glowIndex;
			rotateFace(0);
		}
		break;
	}
	anyKey(key);
}
