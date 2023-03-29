#version 460 core
layout(location = 0) out vec4 color;

in vec3 Normal;

uniform vec4 u_color;

uniform vec3 u_dir_light_dir;



void main()
{

	color = u_color;// *dot(normalize(Normal), u_dir_light_dir);

};