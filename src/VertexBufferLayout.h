#pragma once

#include "Renderer.h"

class VertexBufferLayout
{
public:
	VertexBufferLayout() = delete;
	VertexBufferLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride);
	~VertexBufferLayout();
};