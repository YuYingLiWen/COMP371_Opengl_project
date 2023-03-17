
#include "VertexArray.h"


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &id);
	Bind();
}

VertexArray::VertexArray(const VertexBuffer& vb)
{
	glGenVertexArrays(1, &id);
	Bind();
	vb.Bind();
}

VertexArray::VertexArray(const VertexBuffer& vb, const ElementBuffer& eb)
{
	glGenVertexArrays(1, &id);
	Bind();
	vb.Bind();
	eb.Bind();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &id);
}

void VertexArray::Bind() const
{
	glBindVertexArray(id);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::SetLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, count, type, GL_FALSE, stride * sizeof(float), 0);
}