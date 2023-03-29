#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <vector>


struct TerrainData 
{
	std::shared_ptr<std::vector<glm::vec3>> positions;
	std::shared_ptr<std::vector<unsigned int>> indexes;
	std::shared_ptr<std::vector<glm::vec3>> normals;
};

class TerrainGenerator
{
public:
	TerrainGenerator();
	~TerrainGenerator();

	TerrainData Generate(unsigned int square_size);
	TerrainData Generate(unsigned int x_count, unsigned int z_count);

private: 
};

