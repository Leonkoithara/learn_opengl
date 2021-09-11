#version 330 core

in vec3 vcolor;
in vec2 vtex;

uniform sampler2D u_btexture;

out vec4 color;

void main()
{
	color = texture(u_btexture, vtex);
}