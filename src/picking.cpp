#include "picking.h"

#include "globalvariables.h"
#include "renderer.h"

namespace
{

GLuint framebuffer = 0;
GLuint colorTexture = 0;
GLuint depthRenderbuffer = 0;
int bufferWidth = 0;
int bufferHeight = 0;

// Mirrors packDepth() in the fragment shader.
float unpackDepth(const GLubyte rgba[4])
{
	return (rgba[0] + (rgba[1] + (rgba[2] + rgba[3] / 255.0f) / 255.0f) / 255.0f) / 255.0f;
}

} // namespace

void picking::resize(int width, int height)
{
	if (width <= 0 || height <= 0 || (width == bufferWidth && height == bufferHeight))
		return;

	bufferWidth = width;
	bufferHeight = height;

	if (framebuffer == 0)
	{
		glGenFramebuffers(1, &framebuffer);
		glGenTextures(1, &colorTexture);
		glGenRenderbuffers(1, &depthRenderbuffer);
	}

	glBindTexture(GL_TEXTURE_2D, colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bufferWidth, bufferHeight, 0, GL_RGBA,
							 GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, bufferWidth, bufferHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
														depthRenderbuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Picking framebuffer is incomplete; clicks will not select a face"
							<< std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

vec3 picking::worldPositionAt(int x, int y)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// GL reads pixels from the bottom-left corner.
	const int pixelY = viewport[3] - y;

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Packed-depth 1.0, so anything the scene misses unprojects onto the far
	// plane exactly as a cleared depth buffer would.
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform1i(depth_pass_loc, GL_TRUE);
	renderer::renderScene();
	glUniform1i(depth_pass_loc, GL_FALSE);

	GLubyte pixel[4] = {255, 0, 0, 0};
	glReadPixels(x, pixelY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // white background, as init() set it

	const float depth = unpackDepth(pixel);

	const vec4 ndc(2.0f * (x - viewport[0]) / viewport[2] - 1.0f,
								 2.0f * (pixelY - viewport[1]) / viewport[3] - 1.0f,
								 2.0f * depth - 1.0f,
								 1.0f);
	const vec4 world = Inverse(mp * mv) * ndc;
	return vec3(world.x / world.w, world.y / world.w, world.z / world.w);
}
