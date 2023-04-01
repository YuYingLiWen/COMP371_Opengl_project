#pragma once


#include <vector>
#include <string>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Transform.h"
#include "ShaderProgram.h"

class SceneObject
{
private:
	unsigned int vao = -1, positions_vbo = -1, ibo = -1, normals_vbo = -1;
	unsigned int index_count = -1;
	Transform transform{};
	ShaderProgram* shader = nullptr;

public: 
	SceneObject(); // Empty Scene Object
	~SceneObject();

	Transform& Transform();

	glm::mat4 GetModel();

	SceneObject(std::vector<glm::vec3>* vertexes, std::vector<unsigned int>* indexes);
	SceneObject(std::vector<glm::vec3>* vertexes, std::vector<unsigned int>* indexes, std::vector<glm::vec3>* normals);

	void Populate(std::vector<glm::vec3>* positions, std::vector<unsigned int>* indexes, std::vector<glm::vec3>* normals = nullptr);
	
	SceneObject(std::string obj_file);

	ShaderProgram* GetShader();

	void SetPVM();

	void Attach(ShaderProgram& program);
	void Bind();
	void Unbind() const;
	unsigned int GetCount() const;

private:
	void SetLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride);
};

