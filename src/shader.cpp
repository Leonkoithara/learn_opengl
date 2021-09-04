#include "shader.h"

#include <GL/glew.h>

#include <fstream>
#include <iostream>

Shader::Shader(const char *vertex, const char *fragment) : _program_id(0)
{
	int success;
	char infolog[512];
	
	_program_id = glCreateProgram();

	std::ifstream vertex_file(vertex);
	std::ifstream fragment_file(fragment);
	std::string vertex_src((std::istreambuf_iterator<char>(vertex_file)), (std::istreambuf_iterator<char>()));
	std::string frag_src((std::istreambuf_iterator<char>(fragment_file)), (std::istreambuf_iterator<char>()));

	const char *vsrc = vertex_src.c_str();
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsrc, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, infolog);
		std::cout << "Vertex shader error: " << infolog << std::endl;
	}

	const char *fsrc = frag_src.c_str();
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsrc, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, infolog);
		std::cout << "Fragment shader error: " << infolog << std::endl;
	}

	glAttachShader(_program_id, vs);
	glAttachShader(_program_id, fs);
	
	glLinkProgram(_program_id);
	glGetShaderiv(_program_id, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(_program_id, 512, NULL, infolog);
		std::cout << "Shader link error: " << infolog << std::endl;
	}

	glValidateProgram(_program_id);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void Shader::use()
{
	glUseProgram(_program_id);
}

void Shader::seti(const char *uniform_name, int value)
{
	int location = glGetUniformLocation(_program_id, uniform_name);
	glUniform1i(location, value);
}
void Shader::setf(const char *uniform_name, float value)
{
	int location = glGetUniformLocation(_program_id, uniform_name);
	glUniform1f(location, value);
}
