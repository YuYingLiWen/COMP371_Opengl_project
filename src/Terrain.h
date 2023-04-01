#pragma once

#include <memory>

#include "MeshGenerator.h"
#include "CustomRandom.h"

enum TerrainType
{
	LINES, BUMP, PERLIN
};

class TerrainGenerator : public MeshGenerator
{
public:
	TerrainGenerator();
	~TerrainGenerator();

	std::shared_ptr<Mesh> Generate(TerrainType type, glm::i32vec2 dimensions, float freq);
	std::shared_ptr<Mesh> Generate(glm::i32vec2 dimensions, glm::vec2 center, float radius, float freq);
	std::shared_ptr<Mesh> Generate(TerrainType type, unsigned int x, unsigned int z, float freq);
	std::shared_ptr<Mesh> GeneratePerlinTerrain(double x, double z);


private:
	std::vector<double>* GenerateHeights(TerrainType type, unsigned int x, unsigned int z, float freq);
	std::vector<double>* GenerateHeights(TerrainType type, glm::i32vec2 dimensions, float freq);
	std::vector<double>* GenerateCircularHeights(glm::i32vec2 dimensions, glm::vec2 center, float radius, float freq);

private:
	double Noise(double x_off);
	std::vector<float> PerlinNoise(double x_off, double z_off);
	double CirclePattern(double x_off, double z_off, double center_x, double center_z);
	double CirclePattern(double center_x, double center_z);
	double LinePattern(double x_off, double z_off);
	double BumpPattern(double x_off, double z_off);
};
