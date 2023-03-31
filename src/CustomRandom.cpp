#include "CustomRandom.h"
#include "glm/glm.hpp"

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

double CustomRandom::Generate()
{
	return (float)rand() / (float)RAND_MAX;
}

double CustomRandom::Generate(double num)
{
	return ((float)rand() / (float)RAND_MAX) * num * 2.0f - num;
}

double CustomRandom::Noise(double x_off)
{
	return std::sin(Deg2Rad * x_off);
}

double CustomRandom::Noise(double x_off, double z_off)
{
	return std::sin(Deg2Rad * x_off);
}

double CustomRandom::CircleNoise(double x_off, double z_off)
{
	return CircleNoise(1.0f, 1.0f, 0.0f, 0.0f);
}

double CustomRandom::CircleNoise(double x_off, double z_off, double center_x, double center_z)
{
	return std::sin(Deg2Rad * std::sqrt(std::pow(x_off - center_x, 2.0f) + std::pow(z_off - center_z, 2.0f)));
}

double CustomRandom::LineNoise(double x_off, double z_off)
{
	return std::sin(Deg2Rad * (x_off + z_off));
}

double CustomRandom::BumpNoise(double x_off, double z_off)
{
	return std::sin(Deg2Rad * x_off) + std::sin(Deg2Rad * z_off);
}