
#include "ElementBuffer.h"

#include "Renderer.h"

ElementBuffer::ElementBuffer(unsigned int* data, unsigned int count)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
    glDeleteBuffers(1, &id);
}

void ElementBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void ElementBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
