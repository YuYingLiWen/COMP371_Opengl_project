#version 460 core

out vec4 FragColor;

in vec3 aNormal;
in vec3 aFragPos;
in vec4 aColor;

uniform vec4 u_color;

uniform vec3 u_light;



void main()
{
	//vec3 to_light = normalize(aFragPos - u_light);
	float cos_angle = dot(aNormal, normalize(-u_light));

	FragColor = (u_color + u_color) * max(cos_angle, 0.0);
};