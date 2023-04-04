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
	unsigned int vao = -1;
	Transform transform{};
	ShaderProgram* shader = nullptr;

private:
	unsigned int positions_vbo = -1;
	unsigned int positions_vbo_size = 0;

	unsigned int normals_vbo = -1;
	unsigned int normals_vbo_size = 0;

	unsigned int ibo = -1;
	unsigned int ibo_size = 0;

public: 
	SceneObject();
	virtual ~SceneObject();

	Transform& Transform();

	glm::mat4 GetModel();

	SceneObject(std::vector<glm::vec3>* vertexes, std::vector<unsigned int>* indexes);
	SceneObject(std::vector<glm::vec3>* vertexes, std::vector<unsigned int>* indexes, std::vector<glm::vec3>* normals);

	
	SceneObject(std::string obj_file);

	void UpdatePositions(std::vector<glm::vec3> vertexes);
	std::shared_ptr<glm::vec3[]> GetPositions();

	ShaderProgram* GetShader();

	void SetPVM();



	void Attach(ShaderProgram& program);
	void Bind();
	void Unbind() const;
	unsigned int GetIndexCount() const;

private:
	void Populate(std::vector<glm::vec3>* positions, std::vector<unsigned int>* indexes, std::vector<glm::vec3>* normals = nullptr);

	void SetLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride);
};

