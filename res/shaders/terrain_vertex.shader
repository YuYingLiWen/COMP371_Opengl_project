
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;
uniform float u_time;

out vec3 aNormal;
out vec3 aFragPos;

void main()
{
	//vec3 height = vec3(0,position.y + sin(u_time),0);

	vec3 height = vec3(0);
	gl_Position =  u_projection * u_view * u_model * vec4(position + height, 1.0f);

	aNormal = normal;
	aFragPos = vec3(u_model * vec4(position, 1.0));
};

