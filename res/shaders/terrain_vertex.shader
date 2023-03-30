
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;

out vec3 aNormal;
out vec3 aFragPos;

void main()
{
	gl_Position =  u_projection * u_view * u_model * vec4(position, 1.0f);

	aNormal = normal;
	aFragPos = vec3(u_model * vec4(position, 1.0));
};

