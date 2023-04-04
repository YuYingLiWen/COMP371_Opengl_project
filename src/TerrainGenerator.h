#pragma once

#include <memory>

#include "MeshGenerator.h"
#include "CustomRandom.h"

enum TerrainType
{
	LINES, BUMP, PERLIN
};

class PerlinGrid
{
public:

	void Generate(unsigned int x, unsigned int z, float amplitude);

	unsigned int GetX();
	unsigned int GetZ();
	std::vector<glm::vec2>& GetGrid();

private:
	std::vector<glm::vec2> grid;
	unsigned int x;
	unsigned int z;
};

class TerrainGenerator : public MeshGenerator
{
public:
	TerrainGenerator();
	~TerrainGenerator();

	std::shared_ptr<Mesh> Generate(TerrainType type, glm::i32vec2 dimensions, float freq);
	std::shared_ptr<Mesh> Generate(glm::i32vec2 dimensions, glm::vec2 center, float radius, float freq);
	std::shared_ptr<Mesh> Generate(TerrainType type, unsigned int x, unsigned int z, float freq);
	std::shared_ptr<Mesh> GeneratePerlinTerrain(unsigned int x, unsigned int z, unsigned int iter, float amplitude, unsigned int split);

private:
	std::vector<double>* GenerateHeights(TerrainType type, unsigned int x, unsigned int z, float freq);
	std::vector<double>* GenerateHeights(TerrainType type, glm::i32vec2 dimensions, float freq);
	std::vector<double>* GenerateCircularHeights(glm::i32vec2 dimensions, glm::vec2 center, float radius, float freq);

private:
	double Noise(double x_off);
	float PerlinNoise(double pos_x, double pos_z);
	double CirclePattern(double x_off, double z_off, double center_x, double center_z);
	double CirclePattern(double center_x, double center_z);
	double LinePattern(double x_off, double z_off);
	double BumpPattern(double x_off, double z_off);


private:
	PerlinGrid grid;
};
