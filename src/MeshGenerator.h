#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <vector>


struct Mesh
{
	std::shared_ptr<std::vector<glm::vec3>> positions;
	std::shared_ptr<std::vector<unsigned int>> indexes;
	std::shared_ptr<std::vector<glm::vec3>> normals;
};

class MeshGenerator
{
public:
	MeshGenerator();
	~MeshGenerator();

	std::shared_ptr<Mesh> Generate(glm::i32vec2 dimensions);
	std::shared_ptr<Mesh> Generate(unsigned int square_size);
	std::shared_ptr<Mesh> Generate(unsigned int x_count, unsigned int z_count);
	std::shared_ptr<Mesh> GenerateGrid(unsigned int size, float spread);
};

