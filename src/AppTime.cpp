#include "AppTime.h"

static float last_frame = 0.0f;
static float curr_frame = 0.0f;
static float delta_time = 0.0f;

namespace AppTime
{
	float Time()
	{
		return (float)glfwGetTime();
	}

	void UpdateTime()
	{
		curr_frame = Time();
		delta_time = curr_frame - last_frame;
		last_frame = curr_frame;
	}

	float DeltaTime()
	{
		return delta_time;
	}
}