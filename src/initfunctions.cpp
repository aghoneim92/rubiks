#include "initfunctions.h"

#include "circle.h"
#include "embedded_shaders.h"
#include "globalvariables.h"
#include "picking.h"
#include "shader.h"

#include <cstdlib>

// Scatters one TV-noise speck somewhere across the window.  Each axis draws a
// sign and then a magnitude, in that order; the order matters because it decides
// which random value lands where, and leaving it to the compiler (as a single
// expression would) makes the layout depend on the optimiser.
vec3 randomNoisePosition()
{
	const GLfloat xSign = 2.0f * (rand() % 2 - 0.5f);
	const GLfloat xMagnitude = (GLfloat)(rand() % ((int)WindowWidth / 6));
	const GLfloat ySign = 2.0f * (rand() % 2 - 0.5f);
	const GLfloat yMagnitude = (GLfloat)(rand() % ((int)WindowHeight / 3));
	return vec3(xSign * xMagnitude, ySign * yMagnitude, 0);
}

void build_model()
{
	// LIGHTS
	// add a single light
	lights.push_back(std::make_unique<LightSource>(
			vec4(0, 100, 200, 1),
			color4(1, 1, 1, 1.0),
			color4(1.0, 1.0, 1.0, 1.0),
			color4(1.0, 1.0, 1.0, 1.0)));

	rubiksCube = std::make_unique<RubiksCube>();
	const std::vector<Drawable *> &cubeParts = rubiksCube->getChildren();
	shapes.insert(shapes.end(), cubeParts.begin(), cubeParts.end());

	noise.reserve(300);
	for (int i = 0; i < 300; i++)
	{
		// Position first, then radius: that is the order the random stream was
		// consumed in originally, and the specks are laid out from it.
		const vec3 position = randomNoisePosition();
		auto speck = std::make_unique<FilledCircle>(vec4(0, 0, -1, 1), 0.5f * (rand() % 3) + 0.5f, 100);
		speck->translation = position;
		speck->overlay = true;
		speck->material = Material(color4(0, 0, 0, 1), color4(0, 0, 0, 1), color4(0, 0, 0, 1), 500.0f);
		speck->scale = vec3(0, 0, 0);
		noise.push_back(std::move(speck));
	}
}

void update_projection()
{
	const float zf = zoomFactor;

	if (viewMode == PERSPECTIVE_WITH_FOV)
		mp = Perspective(fovy, WindowWidth / WindowHeight, zNear, zFar);
	else if (viewMode == PERSPECTIVE_WITH_FRUSTUM)
		mp = Frustum(viewLeft / zf, viewRight / zf, viewBottom / zf, viewTop / zf, zNear, zFar);
	else
		mp = Ortho(viewLeft / zf, viewRight / zf, viewBottom / zf, viewTop / zf, -zNear, -zFar);

	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, mp);
	glUniform1f(windowwidth_loc, WindowWidth);
	glUniform1f(windowheight_loc, WindowHeight);
}

void setup_view()
{
	if (viewMode == ORTHO)
	{
		radius = 1.0;
		zoomFactor = 1;
	}
	else if (viewMode == PERSPECTIVE_WITH_FRUSTUM)
	{
		zoomFactor = 100;
	}
	else
	{
		fovy = 60;
	}

	// eye and at are points
	eye.x = radius * sin(theta) * cos(phi);
	eye.y = radius * sin(theta) * sin(phi);
	eye.z = radius * cos(theta);
	eye.w = 1.0;

	eye = RotateY(-45) * eye;
	eye = RotateX(-35) * eye;

	mv = LookAt(eye, at, up);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, mv);

	update_projection();
}

void init()
{
	// Load shaders and use the resulting shader program
	Drawable::shaderProgram =
			buildShaderProgram(embedded_shaders::vertex, embedded_shaders::fragment);
	glUseProgram(Drawable::shaderProgram);

	// init locations
	const GLuint program = Drawable::shaderProgram;
	model_view_loc = glGetUniformLocation(program, "model_view");
	projection_loc = glGetUniformLocation(program, "projection");

	light_position_loc = glGetUniformLocation(program, "light_position");
	eye_position_loc = glGetUniformLocation(program, "eye_position");

	ambient_product_loc = glGetUniformLocation(program, "ambient_product");
	diffuse_product_loc = glGetUniformLocation(program, "diffuse_product");
	specular_product_loc = glGetUniformLocation(program, "specular_product");
	shininess_loc = glGetUniformLocation(program, "shininess");

	translation_loc = glGetUniformLocation(program, "translation");
	rotation_loc = glGetUniformLocation(program, "rotation");
	scale_loc = glGetUniformLocation(program, "scale");
	rotation_after_loc = glGetUniformLocation(program, "rotationAfter");

	wavetime_loc = glGetUniformLocation(program, "waveTime");
	wavewidth_loc = glGetUniformLocation(program, "waveWidth");
	waveheight_loc = glGetUniformLocation(program, "waveHeight");

	grayscale_loc = glGetUniformLocation(program, "grayScale");
	water_loc = glGetUniformLocation(program, "waterEffect");
	dark_loc = glGetUniformLocation(program, "darkEffect");
	overlay_loc = glGetUniformLocation(program, "overlay");

	windowwidth_loc = glGetUniformLocation(program, "windowWidth");
	windowheight_loc = glGetUniformLocation(program, "windowHeight");

	redchrome_loc = glGetUniformLocation(program, "redChrome");
	bluechrome_loc = glGetUniformLocation(program, "blueChrome");
	greenchrome_loc = glGetUniformLocation(program, "greenChrome");

	depth_pass_loc = glGetUniformLocation(program, "depthPass");

	glUniform1f(windowwidth_loc, WindowWidth);
	glUniform1f(windowheight_loc, WindowHeight);
	glUniform1f(wavetime_loc, waveTime);
	glUniform1f(wavewidth_loc, waveWidth);
	glUniform1f(waveheight_loc, waveHeight);
	glUniform1i(grayscale_loc, grayScale);
	glUniform1i(water_loc, waterEffect);
	glUniform1i(dark_loc, darkEffect);
	glUniform1i(depth_pass_loc, GL_FALSE);
	glUniform1f(redchrome_loc, redEffect);
	glUniform1f(greenchrome_loc, greenEffect);
	glUniform1f(bluechrome_loc, blueEffect);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	build_model();
	viewMode = PERSPECTIVE_WITH_FOV;
	setup_view();

	picking::resize((int)WindowWidth, (int)WindowHeight);

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

void cleanup()
{
	shapes.clear();
	noise.clear();
	rubiksCube.reset();

	glUseProgram(0);
	glDeleteProgram(Drawable::shaderProgram);
}
