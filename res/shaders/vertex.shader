
#version 460 core

layout(location = 0) in vec4 position;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	gl_Position = u_projection * u_view * u_model * position;
};

