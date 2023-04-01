#ifndef RANDOM_H
#define RANDOM_H

#include "glm/glm.hpp"

class CustomRandom
{
private:
	CustomRandom();
public:

	static CustomRandom& GetInstance();

	/// Returns a float between 0 and 1
	float Generate();
	/// Returns a float between -num and num
	float Generate(float num);

	glm::vec2 RandomCircle();
};


#endif // !RANDOM_H
