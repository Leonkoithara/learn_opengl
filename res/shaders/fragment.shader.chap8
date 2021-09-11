#version 330 core

in vec3 vcolor;
in vec2 vtex;

uniform sampler2D u_btexture;
uniform sampler2D u_gtexture;
uniform float u_trans;

out vec4 color;

void main()
{
	color = mix(texture(u_btexture, vtex), texture(u_gtexture, vec2(2*(1-vtex.x), 2*(1-vtex.y))), u_trans) * vec4(vcolor , 1.0f);
}