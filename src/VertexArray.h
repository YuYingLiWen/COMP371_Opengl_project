#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "GLObject.h"
#include "IBinder.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

class VertexArray : public IBinder, GLObject
{
public: 
	VertexArray();
	VertexArray(const VertexBuffer& vb);
	VertexArray(const VertexBuffer& vb, const ElementBuffer& eb);

	~VertexArray();

	void Bind() const override;
	void Unbind() const override;

	void SetLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride);
};