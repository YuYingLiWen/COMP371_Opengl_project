#include "TerrainObject.h"

#include "YuMath.h"
#include <GLEW/glew.h>

TerrainObject::TerrainObject(std::shared_ptr<Mesh> mesh, std::vector<glm::vec3>* base_positions)
	:to_mesh(mesh), base_positions(base_positions), SceneObject(base_positions, mesh->indexes.get(), mesh->normals.get())
{

}

TerrainObject::~TerrainObject()
{
}

void TerrainObject::Lerp(float t)
{
	if (t >= 1.0f) UpdatePositions(*to_mesh.get()->positions);
	else if (t <= 0.0) UpdatePositions(*base_positions);

	std::vector<glm::vec3> positions;
	int count = to_mesh.get()->positions->size();

	for (int i = 0; i < count; i++)
	{
		positions.push_back(YuMath::Lerp((*base_positions)[i], (*to_mesh.get()->positions)[i], t));
	}

	UpdatePositions(positions);
}
