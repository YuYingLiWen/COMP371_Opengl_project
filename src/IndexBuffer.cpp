
#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count)
    : count(count)
{
    glGenBuffers(1, &renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);

    Unbind();
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &renderer_id);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


unsigned int IndexBuffer::GetCount() { return count; }