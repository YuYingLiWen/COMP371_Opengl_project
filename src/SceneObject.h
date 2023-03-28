#pragma once


#include <vector>
#include <string>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class SceneObject
{
private:
	unsigned int vao = -1, vbo = -1, ebo = -1;
	unsigned int ebo_count = -1;

public: 
	SceneObject(); // Empty Scene Object

	SceneObject(std::vector<float>* vertexes, std::vector<unsigned int>* indexes);
	SceneObject(std::vector<glm::vec3>* vertexes, std::vector<unsigned int>* indexes);
	SceneObject(std::string obj_file);

	void Bind() const;
	void Unbind() const;
	unsigned int GetCount() const;
	void SetLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride);
};

