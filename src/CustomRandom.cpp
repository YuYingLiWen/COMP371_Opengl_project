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

	return glm::vec2(x,y);
}

