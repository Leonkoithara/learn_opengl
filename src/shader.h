#ifndef SHADER_H
#define SHADER_H

class Shader
{
private:
	unsigned int _program_id;
public:
	Shader(const char *vertex, const char *fragment);

	void use();

	void seti(const char *uniform_name, int value);
};

#endif
