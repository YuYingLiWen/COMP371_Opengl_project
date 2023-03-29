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

glm::mat4 SceneObject::GetModel()
{
    glm::mat4 mat = glm::mat4(1.0f);

    mat = glm::translate(mat, transform.Position());

    mat = glm::rotate(mat, glm::radians(transform.Rotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(transform.Rotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(transform.Rotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

    mat = glm::scale(mat, transform.Scale());

    return mat;
}


SceneObject::SceneObject(std::string obj_file)
{
}

Transform& SceneObject::Transform() { return transform; }

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
