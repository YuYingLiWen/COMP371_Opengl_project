
#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;

out vec4 vColor;

void main()
{
	gl_Position =  u_projection * u_view * u_model * vec4(position, 1.0f);

	vColor = vec4(1, 0, 1, 1);
};

