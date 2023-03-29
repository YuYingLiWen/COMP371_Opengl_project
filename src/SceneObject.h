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
	unsigned int vao = -1, vbo = -1, ebo = -1, vbo2 = -1;
	unsigned int ebo_count = -1;

public: 
	SceneObject(); // Empty Scene Object
	~SceneObject();

	template <typename T> SceneObject(std::vector<T>* vertexes, std::vector<unsigned int>* indexes)
		:ebo_count(indexes->size())
	{
		Populate(vertexes, indexes);
	}

	template <typename T> SceneObject(std::vector<T>* vertexes, std::vector<unsigned int>* indexes, std::vector<glm::vec3>* normals)
		:ebo_count(indexes->size())
	{
		Populate(vertexes, indexes, normals);
	}

	template <typename T> void Populate(std::vector<T>* vertexes, std::vector<unsigned int>* indexes, std::vector<glm::vec3>* normals = nullptr)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexes->size() * sizeof(T), &(*vertexes)[0], GL_STATIC_DRAW);

		if (normals != nullptr)
		{
			/*glGenBuffers(1, &vbo2);
			glBindBuffer(GL_ARRAY_BUFFER, vbo2);
			glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(glm::vec3), &(*normals)[0], GL_STATIC_DRAW);*/
		}

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes->size() * sizeof(unsigned int), &(*indexes)[0], GL_STATIC_DRAW);
	}

	SceneObject(std::string obj_file);

	void Bind() const;
	void Unbind() const;
	unsigned int GetCount() const;
	void SetLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride);

};

