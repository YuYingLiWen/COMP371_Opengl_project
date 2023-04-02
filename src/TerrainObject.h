#pragma once
#include "SceneObject.h"
#include "MeshGenerator.h"
#include "YuMath.h"

#include <vector>
class TerrainObject : public SceneObject
{
	float t = 0.0f;
	float incr = 0.05f;

	std::vector<glm::vec3> LerpTo()
	{
		if (t >= 1.0f) return *to_mesh.get()->positions;

		std::vector<glm::vec3> positions;
		int count = to_mesh.get()->positions->size();

		for (int i = 0; i < count; i++)
		{
			positions.push_back(YuMath::Lerp(base_positions[i], to_mesh.get()->positions->at(i), t));
		}


		t = glm::max(t + incr, 1.0f);

		return positions;
	}

	std::vector<glm::vec3> LerpToBase()
	{
		if (t < 0.0f) return base_positions;

		std::vector<glm::vec3> positions;
		int count = to_mesh.get()->positions->size();

		for (int i = 0; i < count; i++)
		{
			positions.push_back(YuMath::Lerp(to_mesh.get()->positions->at(i), base_positions[i], t));
		}

		t = glm::min(t - incr, 0.0f);
		
		return positions;
	}


private:
	std::vector<glm::vec3> base_positions;


	std::shared_ptr<Mesh> to_mesh;
};

