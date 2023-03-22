#pragma once

#include <GLFW/glfw3.h>

static float last_frame = 0.0f;
static float curr_frame = 0.0f;
static float delta_time = 0.0f;

namespace AppTime
{
	void UpdateTime()
	{
		curr_frame = (float)glfwGetTime();
		delta_time = curr_frame - last_frame;
		last_frame = curr_frame;
	}

	float DeltaTime()
	{
		return delta_time;
	}
}

