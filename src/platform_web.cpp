// Web backend for the platform layer, built on Emscripten's HTML5 API and
// WebGL 2.

#include "platform.h"

#include "Angel.h"

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include <cstdlib>
#include <cstring>
#include <vector>

namespace platform
{
namespace
{

const char *const kCanvas = "#canvas";

Callbacks callbacks;
int canvasWidth = 0;
int canvasHeight = 0;
double devicePixelRatio = 1.0;

// Animation timers run on a virtual clock advanced by the render loop rather
// than on setTimeout.  A background tab throttles setTimeout to about two calls
// a second, which would crawl every animation; driving them from
// requestAnimationFrame instead keeps them at the same rate as the desktop
// build and simply pauses them while the tab is hidden.
struct PendingTimer
{
	double dueAt;
	void (*fn)(int);
	int argument;
};

std::vector<PendingTimer> timers;
double clockMs = 0.0;
double lastRealMs = 0.0;
// While a timer callback runs this is its scheduled time, so a self-rescheduling
// animation keeps its cadence instead of drifting by the frame interval.
double schedulingBaseMs = 0.0;

// A frame longer than this (a hidden tab, a stalled main thread) is treated as a
// pause rather than something to fast-forward through.
constexpr double kMaxFrameStepMs = 100.0;

void runDueTimers()
{
	while (true)
	{
		auto next = timers.end();
		for (auto it = timers.begin(); it != timers.end(); ++it)
			if (it->dueAt <= clockMs && (next == timers.end() || it->dueAt < next->dueAt))
				next = it;
		if (next == timers.end())
			return;

		const PendingTimer timer = *next;
		timers.erase(next);
		schedulingBaseMs = timer.dueAt;
		timer.fn(timer.argument);
		schedulingBaseMs = clockMs;
	}
}

// Matches the canvas' backing store to the browser window, in device pixels.
void resizeCanvasToWindow()
{
	devicePixelRatio = emscripten_get_device_pixel_ratio();

	double cssWidth = 0.0;
	double cssHeight = 0.0;
	emscripten_get_element_css_size(kCanvas, &cssWidth, &cssHeight);

	const int width = static_cast<int>(cssWidth * devicePixelRatio);
	const int height = static_cast<int>(cssHeight * devicePixelRatio);
	if (width <= 0 || height <= 0 || (width == canvasWidth && height == canvasHeight))
		return;

	canvasWidth = width;
	canvasHeight = height;
	emscripten_set_canvas_element_size(kCanvas, canvasWidth, canvasHeight);
}

void onFrame()
{
	const double real = emscripten_get_now();
	double elapsed = real - lastRealMs;
	lastRealMs = real;
	if (elapsed > kMaxFrameStepMs)
		elapsed = kMaxFrameStepMs;
	clockMs += elapsed;
	schedulingBaseMs = clockMs;

	runDueTimers();

	if (callbacks.frame)
		callbacks.frame();
}

EM_BOOL onResize(int, const EmscriptenUiEvent *, void *)
{
	resizeCanvasToWindow();
	glViewport(0, 0, canvasWidth, canvasHeight);
	if (callbacks.resize)
		callbacks.resize(canvasWidth, canvasHeight);
	return EM_TRUE;
}

EM_BOOL onMouse(int eventType, const EmscriptenMouseEvent *event, void *)
{
	if (event->button != 0 || !callbacks.mouse)
		return EM_FALSE;
	callbacks.mouse(eventType == EMSCRIPTEN_EVENT_MOUSEUP,
									static_cast<int>(event->targetX * devicePixelRatio),
									static_cast<int>(event->targetY * devicePixelRatio));
	return EM_TRUE;
}

EM_BOOL onKeyPress(int, const EmscriptenKeyboardEvent *event, void *)
{
	// `key` holds the printable character for an ordinary key press; anything
	// longer than one character is a named key we do not care about here.
	if (!callbacks.key || event->key[0] == '\0' || event->key[1] != '\0')
		return EM_FALSE;
	callbacks.key(static_cast<unsigned char>(event->key[0]));
	return EM_TRUE;
}

EM_BOOL onKeyDown(int, const EmscriptenKeyboardEvent *event, void *)
{
	if (!callbacks.special)
		return EM_FALSE;

	const char *key = event->key;
	if (std::strcmp(key, "ArrowUp") == 0)
		callbacks.special(SpecialKey::Up);
	else if (std::strcmp(key, "ArrowDown") == 0)
		callbacks.special(SpecialKey::Down);
	else if (std::strcmp(key, "ArrowLeft") == 0)
		callbacks.special(SpecialKey::Left);
	else if (std::strcmp(key, "ArrowRight") == 0)
		callbacks.special(SpecialKey::Right);
	else
		return EM_FALSE;

	// Keep the arrows (and space) from scrolling the page.
	return EM_TRUE;
}

} // namespace

void createWindow(const WindowConfig &config)
{
	emscripten_set_window_title(config.title);
	resizeCanvasToWindow();

	EmscriptenWebGLContextAttributes attributes;
	emscripten_webgl_init_context_attributes(&attributes);
	attributes.alpha = EM_FALSE;
	attributes.depth = EM_TRUE;
	attributes.stencil = EM_FALSE;
	attributes.antialias = EM_TRUE; // stands in for GLUT_MULTISAMPLE
	attributes.preserveDrawingBuffer = EM_FALSE;
	attributes.majorVersion = 2;
	attributes.minorVersion = 0;

	const EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context =
			emscripten_webgl_create_context(kCanvas, &attributes);
	if (context <= 0)
	{
		std::cerr << "Failed to create a WebGL 2 context" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	emscripten_webgl_make_context_current(context);
}

int drawableWidth() { return canvasWidth; }
int drawableHeight() { return canvasHeight; }

void run(const Callbacks &newCallbacks)
{
	callbacks = newCallbacks;

	emscripten_set_mousedown_callback(kCanvas, nullptr, EM_FALSE, onMouse);
	emscripten_set_mouseup_callback(kCanvas, nullptr, EM_FALSE, onMouse);
	emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, onKeyPress);
	emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, onKeyDown);
	emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, onResize);

	lastRealMs = emscripten_get_now();

	// 0 fps means "drive the loop from requestAnimationFrame"; the final
	// argument makes this behave like glutMainLoop() and never return.
	emscripten_set_main_loop(onFrame, 0, EM_TRUE);
}

void setTimer(unsigned delayMs, void (*fn)(int), int argument)
{
	timers.push_back({schedulingBaseMs + delayMs, fn, argument});
}

void quit()
{
	// A web page has no meaningful "exit"; leave the canvas running.
}

} // namespace platform
