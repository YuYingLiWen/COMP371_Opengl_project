#pragma once

#include <string>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>



class SceneObject
{
private:
	unsigned int vao = -1, vbo = -1, ebo = -1;
	unsigned int ebo_count = -1;

public: 
	SceneObject(); // Empty Scene Object
	SceneObject(float* vertexes, unsigned int vbo_size, unsigned int* indexes, unsigned int ebo_size);
	SceneObject(std::string obj_file);

	void Bind() const;
	void Unbind() const;
	unsigned int GetCount() const;
	void SetLayout(unsigned int index, unsigned int count, GLenum type, unsigned int stride);
};

