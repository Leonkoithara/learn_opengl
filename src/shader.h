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
	void setf(const char *uniform_name, float value);
	void setmat4(const char *uniform_name, const float *data);
};

#endif
