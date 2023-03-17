#pragma once

#include "GLObject.h"
#include "IBinder.h"
#include "Renderer.h"

class VertexBuffer : public IBinder, GLObject
{
public:
	VertexBuffer(const void* data, unsigned int size, GLenum type);
	~VertexBuffer();

	void Bind() const override;
	void Unbind() const override;
};