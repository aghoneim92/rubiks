// Desktop backend for the platform layer, built on GLUT.

#include "platform.h"

#include "Angel.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <cstdlib>

namespace platform
{
namespace
{

Callbacks callbacks;
int windowWidth = 0;
int windowHeight = 0;
// Framebuffer pixels per window point.  1 unless the window sits on a HiDPI
// display with a backing store larger than its point size.
double pixelScale = 1.0;

// GLUT reports window geometry in points.  Work out once how many framebuffer
// pixels a point is worth by comparing against the viewport GLUT set up for the
// freshly created window.
void measurePixelScale()
{
	const int pointWidth = glutGet(GLUT_WINDOW_WIDTH);

	GLint viewport[4] = {0, 0, pointWidth, glutGet(GLUT_WINDOW_HEIGHT)};
	glGetIntegerv(GL_VIEWPORT, viewport);

	pixelScale = (pointWidth > 0 && viewport[2] > 0)
			? static_cast<double>(viewport[2]) / pointWidth
			: 1.0;
	windowWidth = viewport[2];
	windowHeight = viewport[3];
}

void onDisplay()
{
	if (callbacks.frame)
		callbacks.frame();
	glutSwapBuffers();
}

void onRedisplayTimer(int)
{
	glutPostRedisplay();
	glutTimerFunc(16, onRedisplayTimer, 0);
}

void onKeyboard(unsigned char key, int, int)
{
	if (callbacks.key)
		callbacks.key(key);
}

void onSpecial(int key, int, int)
{
	if (!callbacks.special)
		return;
	switch (key)
	{
	case GLUT_KEY_UP:
		callbacks.special(SpecialKey::Up);
		break;
	case GLUT_KEY_DOWN:
		callbacks.special(SpecialKey::Down);
		break;
	case GLUT_KEY_LEFT:
		callbacks.special(SpecialKey::Left);
		break;
	case GLUT_KEY_RIGHT:
		callbacks.special(SpecialKey::Right);
		break;
	default:
		break;
	}
}

void onMouse(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON || !callbacks.mouse)
		return;
	callbacks.mouse(state == GLUT_UP,
									static_cast<int>(x * pixelScale),
									static_cast<int>(y * pixelScale));
}

void onReshape(int pointWidth, int pointHeight)
{
	windowWidth = static_cast<int>(pointWidth * pixelScale);
	windowHeight = static_cast<int>(pointHeight * pixelScale);
	glViewport(0, 0, windowWidth, windowHeight);
	if (callbacks.resize)
		callbacks.resize(windowWidth, windowHeight);
}

} // namespace

void createWindow(const WindowConfig &config)
{
	int argc = 1;
	char programName[] = "Cube";
	char *argv[] = {programName, nullptr};
	glutInit(&argc, argv);

	int width = config.width;
	int height = config.height;
	if (config.fullscreen)
	{
		width = glutGet(GLUT_SCREEN_WIDTH);
		height = glutGet(GLUT_SCREEN_HEIGHT);
	}

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(width, height);
	glutCreateWindow(config.title);

	if (config.fullscreen)
		glutFullScreen();

#if !defined(__APPLE__)
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialise GLEW" << std::endl;
		std::exit(EXIT_FAILURE);
	}
#endif

	windowWidth = width;
	windowHeight = height;
	measurePixelScale();
}

int drawableWidth() { return windowWidth; }
int drawableHeight() { return windowHeight; }

void run(const Callbacks &newCallbacks)
{
	callbacks = newCallbacks;

	glutDisplayFunc(onDisplay);
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(onSpecial);
	glutMouseFunc(onMouse);
	glutReshapeFunc(onReshape);
	glutTimerFunc(16, onRedisplayTimer, 0);

	glutMainLoop();
}

void setTimer(unsigned delayMs, void (*fn)(int), int argument)
{
	glutTimerFunc(delayMs, fn, argument);
}

void quit()
{
	std::exit(EXIT_SUCCESS);
}

} // namespace platform
