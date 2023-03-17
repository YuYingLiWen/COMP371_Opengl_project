#pragma once

#include "GLObject.h"
#include "IBinder.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

class VertexBuffer : public IBinder, GLObject
{
public:
	VertexBuffer(const void* data, unsigned int size, GLenum type);
	~VertexBuffer();

	void Bind() const override;
	void Unbind() const override;
};