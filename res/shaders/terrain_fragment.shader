#version 460 core

out vec4 FragColor;

in vec3 aNormal;
in vec3 aFragPos;

uniform bool u_use_wiremesh;
uniform vec4 u_color;
uniform vec3 u_light;



void main()
{
	if (!u_use_wiremesh)
	{
		vec3 to_light = normalize(aFragPos - u_light);

		vec3 ambient = (aFragPos.y + 1.01) * u_color.xyz;

		FragColor = vec4(ambient,1.0);


		//float cos_angle = dot(aNormal, normalize(-u_light));

		//FragColor = (u_color + u_color) * max(cos_angle, 0.0);
	}
	else // Wiremesh enabled
	{
		FragColor = u_color;
	}
};