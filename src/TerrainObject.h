#pragma once
#include "SceneObject.h"
#include "MeshGenerator.h"
#include "YuMath.h"
#include <GLEW/glew.h>
#include <vector>
#include "AppTime.h"

class TerrainObject : public SceneObject
{
public:
	TerrainObject(std::shared_ptr<Mesh> mesh, std::vector<glm::vec3>* base_positions)
		:to_mesh(mesh), base_positions(base_positions), SceneObject(mesh->positions.get(), mesh->indexes.get(), mesh->normals.get())
	{

	}

	~TerrainObject()
	{
		base_positions;
	}

	void LerpTo()
	{
		if (t >= 1.0f) UpdatePositions(*to_mesh.get()->positions);

		std::vector<glm::vec3> positions;
		int count = to_mesh.get()->positions->size();

		for (int i = 0; i < count; i++)
		{
			positions.push_back(YuMath::Lerp(base_positions->at(i), to_mesh.get()->positions->at(i), t));
		}

		t = glm::clamp(t + incr * AppTime::DeltaTime(),0.0f, 1.0f);

		UpdatePositions(positions);
	}

	void LerpToBase()
	{
		if (t < 0.0f) UpdatePositions(*base_positions);

		std::vector<glm::vec3> positions;
		int count = to_mesh.get()->positions->size();

		for (int i = 0; i < count; i++)
		{
			positions.push_back(YuMath::Lerp(to_mesh.get()->positions->at(i), base_positions->at(i), t));
		}

		t = glm::clamp(t - incr * AppTime::DeltaTime(), 0.0f , 1.0f);
		
		UpdatePositions(positions);
	}


private:
	float t = 0.0f;
	float incr = 0.2f;

	std::vector<glm::vec3>* base_positions;


	std::shared_ptr<Mesh> to_mesh;
};

