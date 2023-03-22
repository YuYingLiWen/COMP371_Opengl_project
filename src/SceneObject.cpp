#include "SceneObject.h"

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(float* vertexes, unsigned int vbo_size, unsigned int* indexes, unsigned int ebo_size)
    :ebo_count(ebo_size)
{
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vbo_size * sizeof(float), vertexes, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_size * sizeof(unsigned int), indexes, GL_STATIC_DRAW);
}

SceneObject::SceneObject(std::string obj_file)
{
}

void SceneObject::Bind() const
{
    glBindVertexArray(vao);
}

void SceneObject::Unbind() const
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int SceneObject::GetCount() const
{
    return ebo_count;
}

void SceneObject::SetLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, count, type, GL_FALSE, stride * sizeof(float), 0);
}
