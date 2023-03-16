

#include "VertexArray.h"

// Vertex Array

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &render_id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &render_id);
}

void VertexArray::Bind() const
{
	glBindVertexArray(render_id);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	layout.Bind();
	vb.Bind();

}




// Vertex Buffer Layout

VertexBufferLayout::VertexBufferLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride)
	:index(index), count(count), type(type), stride(stride)
{
	//glEnableVertexAttribArray(index); // Enable's the buffer
	//glVertexAttribPointer(index, count, type, GL_FALSE, stride * sizeof(float), 0);

}

VertexBufferLayout::~VertexBufferLayout()
{
}

void VertexBufferLayout::Bind() const
{
	glEnableVertexAttribArray(index); // Enable's the buffer
	glVertexAttribPointer(index, count, type, GL_FALSE, stride * sizeof(float), 0);
}

void VertexBufferLayout::Unbind() const
{
	glDisableVertexAttribArray(index); // Enable's the buffer
}

unsigned int VertexBufferLayout::GetStride()
{
	return stride;
}
