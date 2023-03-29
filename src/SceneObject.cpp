#include "SceneObject.h"

SceneObject::SceneObject() // Empty Object
{
}

SceneObject::~SceneObject()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vbo2);
    glDeleteBuffers(1, &ebo);

    glDeleteVertexArrays(1, &vao);
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

void SceneObject::SetLayout(unsigned int index, unsigned int count_per_vertex, GLenum type, unsigned int stride)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, count_per_vertex, type, GL_FALSE, stride * sizeof(float), 0);
}
