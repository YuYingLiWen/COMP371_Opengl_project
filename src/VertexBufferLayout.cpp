#include "VertexBufferLayout.h"


VertexBufferLayout::VertexBufferLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, count, type, GL_FALSE, stride * sizeof(float), 0);
}

VertexBufferLayout::~VertexBufferLayout() {}