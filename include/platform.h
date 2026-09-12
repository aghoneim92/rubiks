#ifndef PLATFORM_H
#define PLATFORM_H

// Thin windowing/input/timer layer so the game code never talks to GLUT (or to
// the browser) directly.  Two backends implement it: platform_glut.cpp for
// desktop builds and platform_web.cpp for the Emscripten/WebGL build.

namespace platform
{

enum class SpecialKey
{
	Up,
	Down,
	Left,
	Right
};

struct Callbacks
{
	void (*frame)() = nullptr;                                  // draw one frame
	void (*key)(unsigned char key) = nullptr;                   // printable key press
	void (*special)(SpecialKey key) = nullptr;                  // arrow key press
	void (*mouse)(bool released, int x, int y) = nullptr;       // left button, pixel coords
	void (*resize)(int width, int height) = nullptr;            // drawable size changed
};

struct WindowConfig
{
	const char *title = "";
	int width = 600;
	int height = 600;
	bool fullscreen = false;
};

// Creates the window plus its GL context.  Must be called before any GL call.
// On return, drawableWidth()/drawableHeight() hold the real framebuffer size.
void createWindow(const WindowConfig &config);

// Size of the framebuffer in device pixels (this is what glViewport wants).
int drawableWidth();
int drawableHeight();

// Runs the callbacks until the program ends.  Returns immediately on the web,
// where the browser owns the event loop.
void run(const Callbacks &callbacks);

// Invokes fn(argument) after roughly delayMs milliseconds.  This is what drives
// every animation in the game.
void setTimer(unsigned delayMs, void (*fn)(int), int argument);

// Requests that the program shut down.  A no-op on the web.
void quit();

} // namespace platform

#endif
