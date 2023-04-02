#include "SceneObject.h"

#include "Camera.h"

extern Camera camera;

SceneObject::SceneObject() // Empty Object
{
}

SceneObject::~SceneObject()
{
    glDeleteBuffers(1, &positions_vbo);
    glDeleteBuffers(1, &normals_vbo);
    glDeleteBuffers(1, &ibo);

    glDeleteVertexArrays(1, &vao);
}

glm::mat4 SceneObject::GetModel()
{
    glm::mat4 mat = glm::mat4(1.0f);

    mat = glm::translate(mat, transform.Position());

    mat = glm::rotate(mat, glm::radians(transform.Rotation().x), RIGHT);
    mat = glm::rotate(mat, glm::radians(transform.Rotation().y), UP);
    mat = glm::rotate(mat, glm::radians(transform.Rotation().z), FORWARD);

    mat = glm::scale(mat, transform.Scale());

    return mat;
}

SceneObject::SceneObject(std::vector<glm::vec3>* vertexes, std::vector<unsigned int>* indexes)
	:ibo_size((unsigned int)indexes->size())
{
	Populate(vertexes, indexes);
}

SceneObject::SceneObject(std::vector<glm::vec3>* vertexes, std::vector<unsigned int>* indexes, std::vector<glm::vec3>* normals)
	:ibo_size((unsigned int)indexes->size())
{
	Populate(vertexes, indexes, normals);
}

void SceneObject::Populate(std::vector<glm::vec3>* positions, std::vector<unsigned int>* indexes, std::vector<glm::vec3>* normals)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &positions_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
	glBufferData(GL_ARRAY_BUFFER, positions->size() * sizeof(glm::vec3), &(*positions)[0], GL_STATIC_DRAW);
	SetLayout(0, 3, GL_FLOAT, sizeof(glm::vec3));

	if (normals != nullptr)
	{
		glGenBuffers(1, &normals_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
		glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(glm::vec3), &(*normals)[0], GL_STATIC_DRAW);
		SetLayout(1, 3, GL_FLOAT, sizeof(glm::vec3));
	}

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes->size() * sizeof(unsigned int), &(*indexes)[0], GL_STATIC_DRAW);

}

void SceneObject::UpdatePositions(std::vector<glm::vec3> positions)
{
	glBindVertexArray(vao);

	glDeleteBuffers(1, &positions_vbo);
	
	glGenBuffers(1, &positions_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &(positions)[0], GL_STATIC_DRAW);
	SetLayout(0, 3, GL_FLOAT, sizeof(glm::vec3));
}

SceneObject::SceneObject(std::string obj_file)
{
}

std::shared_ptr<glm::vec3[]> SceneObject::GetPositions()
{
	std::shared_ptr<glm::vec3[]> size(new glm::vec3[positions_vbo_size]);
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, positions_vbo_size, size.get());
	return size;
}

Transform& SceneObject::Transform() { return transform; }

void SceneObject::Bind() 
{
	/*if (shader)
	{
		shader->Bind();
		SetPVM();
	}*/
		

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
    return ibo_size;
}

void SceneObject::SetLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, count, type, GL_FALSE, stride, 0);
}


void SceneObject::Attach(ShaderProgram& program)
{
	shader = &program;
}

ShaderProgram* SceneObject::GetShader()
{
	return shader;
}

void SceneObject::SetPVM()
{
	shader->SetPVM(camera.GetProjection(), camera.GetView(), GetModel());
}
