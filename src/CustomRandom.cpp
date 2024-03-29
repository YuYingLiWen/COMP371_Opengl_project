#include "CustomRandom.h"

#include <random>
#include <ctime>

#define PI 3.14159265359f
#define Deg2Rad 0.01745329251994f //(PI / 180.0f) from the above pi

CustomRandom::CustomRandom()
{
	std::srand((unsigned int)std::time(nullptr)); // seeds the RNG
}

CustomRandom& CustomRandom::GetInstance()
{
	static CustomRandom instance;

	return instance;
}

float CustomRandom::Generate()
{
	return (float)rand() / (float)RAND_MAX;
}

float CustomRandom::Generate(float num)
{
	return Generate() * num * 2.0f - num;
}

glm::vec2 CustomRandom::RandomCircle()
{
	//float theta = Generate() * 360.0f;

	//float x = glm::sin(glm::radians(theta));
	//float y = glm::cos(glm::radians(theta));

	float x = Generate(1.0f);
	float y = Generate(1.0f);

	return glm::normalize(glm::vec2(x,y));
}

// Returns: angle in radian
float CustomRandom::GenerateAngle(double angle)
{
	// Goes from 0 to angle.
	return Generate() * angle * Deg2Rad;
}

glm::vec3 CustomRandom::RandomSphere()
{
	double tetha = GenerateAngle(360.0f);
	double phi = GenerateAngle(360.0f);

	glm::vec3 rand_vector(
		std::sin(tetha) * std::cos(phi),
		std::sin(tetha) * std::sin(phi),
		std::cos(tetha)
	);
	return rand_vector;
}

