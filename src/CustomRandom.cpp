#include "CustomRandom.h"
#include "glm/glm.hpp"

#include <random>
#include <ctime>

CustomRandom::CustomRandom()
{
	std::srand((unsigned int)std::time(nullptr)); // seeds the RNG
}

CustomRandom& CustomRandom::GetInstance()
{
	static CustomRandom instance;

	return instance;
}

double CustomRandom::Generate()
{
	return (float)rand() / (float)RAND_MAX;
}

double CustomRandom::Generate(double num)
{
	return ((float)rand() / (float)RAND_MAX) * num * 2.0f - num;
}
