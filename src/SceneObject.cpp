#include "SceneObject.h"

SceneObject::SceneObject() // Empty Object
{
}

SceneObject::SceneObject(std::vector<float>* vertexes, std::vector<unsigned int>* indexes)
    :ebo_count(indexes->size())
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexes->size() * sizeof(float), &(*vertexes)[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes->size() * sizeof(unsigned int), &(*indexes)[0], GL_STATIC_DRAW);
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
