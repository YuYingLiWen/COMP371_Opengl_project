#ifndef YUMATH_H
#define YUMATH_H

#include <cmath>
#include <memory>

#include "glm/glm.hpp"

#include "CustomRandom.h"

#define PI 3.14159265359f
#define Deg2Rad 0.01745329251994f //(PI / 180.0f) from the above pi

namespace YuMath
{
	struct Tuple
	{
		float b_pos;
		float b_neg;
	};

	float Discriminant(float a, float b, float c);

	std::shared_ptr<Tuple> Quadratic(float a, float b, float c);
	std::shared_ptr<Tuple> Quadratic(float a, float b, float c, float discriminant);

	unsigned int HitResultsNum(float a, float b, float c);

	float TriangleArea(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

	template<typename T>
	T Lerp(T from, T to, T t) { return from + t * (to - from); }
}

#endif

