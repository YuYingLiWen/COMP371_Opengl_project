#pragma once
#include "SceneObject.h"
#include "MeshGenerator.h"

#include <vector>

class TerrainObject : public SceneObject
{
public:
	TerrainObject(std::shared_ptr<Mesh> mesh, std::vector<glm::vec3>* base_positions);
	~TerrainObject();
	void Lerp(float t);

private:

	std::vector<glm::vec3>* base_positions;
	std::shared_ptr<Mesh> to_mesh;
};

