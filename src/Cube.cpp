
#include <vector>
#include "glm/glm.hpp"

std::vector<glm::vec3> cube_pos = {
	// Front face
	glm::vec3(-0.5f, -0.5f, 0.5f),   // 0 Bot Left
	glm::vec3(0.5f, -0.5f, 0.5f),   // 1 Bot Right
	glm::vec3(0.5f,  0.5f, 0.5f),   // 2 Top Right
	glm::vec3(-0.5f,  0.5f, 0.5f),   // 3 Top Left

	// Back face
	glm::vec3(-0.5f, -0.5f, -0.5f),  // 4 Bot Left
	glm::vec3(0.5f, -0.5f, -0.5f),  // 5 Bot Right
	glm::vec3(0.5f,  0.5f, -0.5f),  // 6 Top Right
	glm::vec3(-0.5f,  0.5f, -0.5f),  // 7 Top Left
};

std::vector<unsigned int> cube_indexes = {
	// front face
	0, 1, 2,
	0, 2, 3,

	//Back face
	4,5,6,
	4,6,7,

	// Top face
	3,2,6,
	3,6,7,

	// Bottom face
	0,1,5,
	0,5,4,

	//Left face
	1,5,6,
	1,6,2,

	//Right face
	4,0,3,
	4,3,7
};