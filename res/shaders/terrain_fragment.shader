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
		//vec3 to_light = normalize(aFragPos - u_light);

		//vec3 ambient = vec3(1, 1, 1);//(aFragPos.y + 1.01) * u_color.xyz;

		//FragColor = vec4(ambient,1.0);
		vec4 f_color = vec4(0);

		if (aFragPos.y < 5.0f) f_color = vec4(0, 0, 1, 1);
		else if (aFragPos.y >= 5.5f && aFragPos.y < 20.0f) f_color = vec4(0, 1, 0, 1);
		else if (aFragPos.y > 60.0f) f_color = vec4(1, 1, 1, 1);

		float cos_angle = dot(aNormal, normalize(-u_light));

		FragColor = f_color + ( u_color) * max(cos_angle, 0.0);
	}
	else // Wiremesh enabled
	{
		FragColor = u_color;
	}
};