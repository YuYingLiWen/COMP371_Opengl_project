#pragma once

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IBinder.h";


class VertexBufferLayout;

class VertexArray : public IBinder
{
private:
	unsigned int render_id;

public:
	VertexArray();
	~VertexArray();

	void Bind() const override;
	void Unbind() const override;

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};


class VertexBufferLayout: public IBinder
{
private:
	unsigned int index;
	unsigned int count;
	GLenum type;
	unsigned int stride;

public:
	VertexBufferLayout() = delete;
	VertexBufferLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride);
	~VertexBufferLayout();

	void Bind() const override;
	void Unbind() const override;

	unsigned int GetStride();
};