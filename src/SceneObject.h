#pragma once


#include <vector>
#include <string>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Transform.h"

class SceneObject
{
private:
	unsigned int vao = -1, positions_vbo = -1, ebo = -1, normals_vbo = -1;
	unsigned int ebo_count = -1;
	Transform transform{};

public: 
	SceneObject(); // Empty Scene Object
	~SceneObject();

	Transform& Transform();

	glm::mat4 GetModel();

	SceneObject(std::vector<glm::vec3>* vertexes, std::vector<unsigned int>* indexes);
	SceneObject(std::vector<glm::vec3>* vertexes, std::vector<unsigned int>* indexes, std::vector<glm::vec3>* normals);

	void Populate(std::vector<glm::vec3>* positions, std::vector<unsigned int>* indexes, std::vector<glm::vec3>* normals = nullptr);
	
	SceneObject(std::string obj_file);

	void Bind() const;
	void Unbind() const;
	unsigned int GetCount() const;

private:
	void SetLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride);
};

