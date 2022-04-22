#include <initfunctions.h>

void build_model()
{
	//LIGHTS
	// add a single light
	lights.push_back(new LightSource(
			vec4(0, 100, 200, 1),
			color4(1, 1, 1, 1.0),
			color4(1.0, 1.0, 1.0, 1.0),
			color4(1.0, 1.0, 1.0, 1.0)));

	rubiksCube = new RubiksCube();
	shapes.insert(shapes.end(), rubiksCube->start(), rubiksCube->end());

	int i;
	FilledCircle *c;
	GLfloat x, y;
	for (i = 0; i < 300; i++)
	{
		x = 2.0f * (rand() % 2 - 0.5f) * (rand() % ((int)WindowWidth / 6));
		y = 2.0f * (rand() % 2 - 0.5f) * (rand() % ((int)WindowHeight / 3));
		c = new FilledCircle(vec4(0, 0, -1, 1), 0.5f * (rand() % 3) + 0.5f, 100);
		c->translation = vec3(x, y, 0);
		c->overlay = true;
		c->material = Material(color4(0, 0, 0, 1), color4(0, 0, 0, 1), color4(0, 0, 0, 1), 500.0f);
		c->scale = vec3(0, 0, 0);
		noise.push_back(c);
	}
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

	//eye and at are points
	eye.x = radius * sin(theta) * cos(phi);
	eye.y = radius * sin(theta) * sin(phi);
	eye.z = radius * cos(theta);
	eye.w = 1.0;

	mat4 rotationMatrix = RotateY(-45);
	eye = rotationMatrix * eye;

	rotationMatrix = RotateX(-35);
	eye = rotationMatrix * eye;

	mv = LookAt(eye, at, up);

	float zf = zoomFactor;

	if (viewMode == PERSPECTIVE_WITH_FOV)
		mp = Perspective(fovy, WindowWidth / WindowHeight, zNear, zFar);
	else if (viewMode == PERSPECTIVE_WITH_FRUSTUM)
		mp = Frustum(left / zf, right / zf, bottom / zf, top / zf, zNear, zFar);
	else
		mp = Ortho(left / zf, right / zf, bottom / zf, top / zf, -zNear, -zFar);

	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, mv);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, mp);
}

void init()
{
	// Load shaders and use the resulting shader program
	Drawable::shaderProgram = InitShader("vshader.glsl", "fshader.glsl" /*, "gshader.glsl", "tcshader.glsl", "teshader.glsl"*/);
	glUseProgram(Drawable::shaderProgram);

	//init locations
	model_view_loc = glGetUniformLocation(Drawable::shaderProgram, "model_view");
	projection_loc = glGetUniformLocation(Drawable::shaderProgram, "projection");

	light_position_loc = glGetUniformLocation(Drawable::shaderProgram, "light_position");
	eye_position_loc = glGetUniformLocation(Drawable::shaderProgram, "eye_position");

	ambient_product_loc = glGetUniformLocation(Drawable::shaderProgram, "ambient_product");
	diffuse_product_loc = glGetUniformLocation(Drawable::shaderProgram, "diffuse_product");
	specular_product_loc = glGetUniformLocation(Drawable::shaderProgram, "specular_product");
	shininess_loc = glGetUniformLocation(Drawable::shaderProgram, "shininess");

	translation_loc = glGetUniformLocation(Drawable::shaderProgram, "translation");
	rotation_loc = glGetUniformLocation(Drawable::shaderProgram, "rotation");
	scale_loc = glGetUniformLocation(Drawable::shaderProgram, "scale");
	rotation_after_loc = glGetUniformLocation(Drawable::shaderProgram, "rotationAfter");

	wavetime_loc = glGetUniformLocation(Drawable::shaderProgram, "waveTime");
	wavewidth_loc = glGetUniformLocation(Drawable::shaderProgram, "waveWidth");
	waveheight_loc = glGetUniformLocation(Drawable::shaderProgram, "waveHeight");

	grayscale_loc = glGetUniformLocation(Drawable::shaderProgram, "grayScale");

	water_loc = glGetUniformLocation(Drawable::shaderProgram, "waterEffect");

	dark_loc = glGetUniformLocation(Drawable::shaderProgram, "darkEffect");

	overlay_loc = glGetUniformLocation(Drawable::shaderProgram, "overlay");

	windowwidth_loc = glGetUniformLocation(Drawable::shaderProgram, "windowWidth");
	windowheight_loc = glGetUniformLocation(Drawable::shaderProgram, "windowHeight");

	redchrome_loc = glGetUniformLocation(Drawable::shaderProgram, "redChrome");
	bluechrome_loc = glGetUniformLocation(Drawable::shaderProgram, "blueChrome");
	greenchrome_loc = glGetUniformLocation(Drawable::shaderProgram, "greenChrome");
	left_loc = glGetUniformLocation(Drawable::shaderProgram, "left");
	glUniform1f(windowwidth_loc, WindowWidth);
	glUniform1f(windowheight_loc, WindowHeight);
	glUniform1f(wavetime_loc, waveTime);
	glUniform1f(wavewidth_loc, waveWidth);
	glUniform1f(waveheight_loc, waveHeight);
	glUniform1i(grayscale_loc, grayScale);
	glUniform1i(water_loc, waterEffect);
	glUniform1i(dark_loc, darkEffect);
	glUniform1f(redchrome_loc, redEffect);
	glUniform1f(greenchrome_loc, greenEffect);
	glUniform1f(bluechrome_loc, blueEffect);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	build_model();
	viewMode = PERSPECTIVE_WITH_FOV;
	setup_view();

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

void cleanup()
{
	//destroy shaders
	glUseProgram(0);
	glDeleteProgram(Drawable::shaderProgram);
	//destroy shapes and its vertices
	for (unsigned int i = 0; i < shapes.size(); i++)
	{
		delete shapes[i];
	}
}