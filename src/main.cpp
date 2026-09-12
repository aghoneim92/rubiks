#include "animations.h"
#include "globalvariables.h"
#include "initfunctions.h"
#include "inputfunctions.h"
#include "picking.h"
#include "platform.h"
#include "renderer.h"

#include <cstdlib>
#include <cstring>
#include <ctime>

namespace
{

void onResize(int width, int height)
{
	WindowWidth = (GLfloat)width;
	WindowHeight = (GLfloat)height;
	update_projection();
	picking::resize(width, height);
}

} // namespace

int main(int argc, char **argv)
{
	srand((unsigned int)time(NULL));

	for (int i = 1; i < argc; i++)
		if (std::strcmp(argv[i], "--fullscreen") == 0)
			fullScreen = true;

	platform::WindowConfig window;
	window.title = "Rubik's Cube";
	window.width = (int)WindowWidth;
	window.height = (int)WindowHeight;
	window.fullscreen = fullScreen;
	platform::createWindow(window);

	WindowWidth = (GLfloat)platform::drawableWidth();
	WindowHeight = (GLfloat)platform::drawableHeight();
	glViewport(0, 0, platform::drawableWidth(), platform::drawableHeight());

	init();

	platform::Callbacks callbacks;
	callbacks.frame = renderer::drawFrame;
	callbacks.key = keyboard;
	callbacks.special = special;
	callbacks.mouse = mouse;
	callbacks.resize = onResize;

	platform::setTimer(10, floatView, 0);
	platform::run(callbacks);

	cleanup();
	return 0;
}
