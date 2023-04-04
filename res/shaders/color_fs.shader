#version 460 core

layout(location = 1) in vec3 normal;

out vec4 FragColor;

uniform vec4 u_color_begin;
uniform vec4 u_color_end;
uniform float u_t;

//uniform vec3 u_camera_pos;

void main()
{
	//if(u_camera_pos - position <= )

	FragColor = u_color_end + u_t * (u_color_begin - u_color_end);

	
};