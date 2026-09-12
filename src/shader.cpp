#include "shader.h"

#include <cstdlib>
#include <string>
#include <vector>

namespace
{

std::string infoLog(GLuint object, void (*getLength)(GLuint, GLenum, GLint *),
										void (*getLog)(GLuint, GLsizei, GLsizei *, GLchar *))
{
	GLint length = 0;
	getLength(object, GL_INFO_LOG_LENGTH, &length);
	if (length <= 0)
		return {};
	std::vector<GLchar> buffer(static_cast<size_t>(length));
	getLog(object, length, nullptr, buffer.data());
	return std::string(buffer.data());
}

GLuint compile(GLenum type, const char *source, const char *label)
{
	const GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint compiled = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		std::cerr << label << " failed to compile:" << std::endl
							<< infoLog(shader, glGetShaderiv, glGetShaderInfoLog) << std::endl;
		std::exit(EXIT_FAILURE);
	}
	return shader;
}

} // namespace

GLuint buildShaderProgram(const char *vertexSource, const char *fragmentSource)
{
	const GLuint program = glCreateProgram();
	const GLuint vertexShader = compile(GL_VERTEX_SHADER, vertexSource, "vertex shader");
	const GLuint fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentSource, "fragment shader");

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	GLint linked = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		std::cerr << "Shader program failed to link:" << std::endl
							<< infoLog(program, glGetProgramiv, glGetProgramInfoLog) << std::endl;
		std::exit(EXIT_FAILURE);
	}

	// The program holds the only references it needs once it is linked.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(program);
	return program;
}
