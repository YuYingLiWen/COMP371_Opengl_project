
#include "YuMath.h"

namespace YuMath
{
	float Discriminant(float a, float b, float c) { return b * b - 4.0f * a * c; }

	std::shared_ptr<Tuple> Quadratic(float a, float b, float c)
	{
		float disc = Discriminant(a, b, c);

		if (disc < 0) return nullptr;
		if (a < 0) return nullptr;

		float root_disc = std::sqrt(disc);

		auto tuple = std::make_shared<Tuple>();
		tuple->b_pos = (-b + root_disc) / (2.0f * a);
		tuple->b_neg = (-b - root_disc) / (2.0f * a);

		return tuple;
	}

	std::shared_ptr<Tuple> Quadratic(float a, float b, float c, float discriminant)
	{
		if (discriminant < 0) return nullptr;
		if (a < 0) return nullptr;

		float root_disc = std::sqrt(discriminant);

		auto tuple = std::make_shared<Tuple>();
		tuple->b_pos = (-b + root_disc) / (2.0f * a);
		tuple->b_neg = (-b - root_disc) / (2.0f * a);

		return tuple;
	}

	unsigned int HitResultsNum(float a, float b, float c)
	{
		float dis = Discriminant(a, b, c);
		if (dis > 0) return 2;
		if (dis == 0) return 1;
		if (dis < 0) return 0;

		return -1; // Something went wrong
	}

	float TriangleArea(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
	{
		return glm::length(glm::cross(p1 - p2, p3 - p2)) * 0.5f;
	}

}
