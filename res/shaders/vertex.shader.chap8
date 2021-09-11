#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 tex;

uniform mat4 u_scale_mat;

out vec3 vcolor;
out vec2 vtex;

void main()
{
	gl_Position = u_scale_mat * vec4(pos.x, pos.y, pos.z, 1.0);
	vcolor = col;
	vtex = tex;
}