#include "Terrain.h"


TerrainGenerator::TerrainGenerator()
{

}


TerrainGenerator::~TerrainGenerator()
{

}

std::shared_ptr<Mesh> TerrainGenerator::Generate(TerrainType type, glm::i32vec2 dimensions, float freq)
{
	return Generate(type, dimensions.x, dimensions.y, freq);
}

std::shared_ptr<Mesh> TerrainGenerator::Generate(glm::i32vec2 dimensions, glm::vec2 center, float radius, float freq)
{
    auto basic_mesh = generator.Generate(dimensions.x, dimensions.y);

    auto heights = GenerateCircularHeights(dimensions, center, radius, freq);

    unsigned int index = 0;
    for (auto& position : *basic_mesh->positions)
    {
        position.y = (float)(heights->at(index));

        index++;
    }

    delete heights;
    return basic_mesh;
}

std::shared_ptr<Mesh> TerrainGenerator::Generate(TerrainType type, unsigned int x, unsigned int z, float freq)
{
    auto basic_mesh = generator.Generate(x, z);

    auto heights = GenerateHeights(type, x, z, freq);

    unsigned int index = 0;
    for (auto& position : *basic_mesh->positions)
    {
        position.y = (float)(heights->at(index));

        index++;
    }

    delete heights;
	return basic_mesh;
}

std::vector<double>* TerrainGenerator::GenerateHeights(TerrainType type, unsigned int x, unsigned int z, float freq)
{
	return GenerateHeights(type, glm::i32vec2(x, z), freq);
}

std::vector<double>* TerrainGenerator::GenerateHeights(TerrainType type, glm::i32vec2 dimensions, float freq)
{
    auto heights = new std::vector<double>();

    unsigned int z_size = dimensions.y + 1;
    unsigned int x_size = dimensions.x + 1;

    float off_z = 0.0f;
    for (size_t z = 0; z < z_size; z++)
    {
        float off_x = 000.0f;
        for (size_t x = 0; x < x_size; x++)
        {
            switch (type)
            {
            case PERLIN:
                heights->push_back(CustomRandom::GetInstance().Noise(off_x, off_z));
                break;
            case LINES:
                heights->push_back(CustomRandom::GetInstance().LineNoise(off_x, off_z));
                break;
            case BUMP:
                heights->push_back(CustomRandom::GetInstance().BumpNoise(off_x, off_z));
                break;
            }
            off_x += freq;
        }
        off_z += freq;
    }

    return heights;
}


std::vector<double>* TerrainGenerator::GenerateCircularHeights(glm::i32vec2 dimensions, glm::vec2 center, float radius, float freq)
{
    auto heights = new std::vector<double>();

    unsigned int z_size = dimensions.x + 1;
    unsigned int x_size = dimensions.y + 1;

    float off_z = 0.0f;
    for (size_t z = 0; z < z_size; z++)
    {
        float off_x = 0.0f;
        for (size_t x = 0; x < x_size; x++)
        {
            heights->push_back(
                CustomRandom::GetInstance().CircleNoise(
                    off_x, 
                    off_z, 
                    center.x * radius , 
                    center.y * radius 
                )
            );

            off_x += freq;
        }
        off_z += freq;
    }

    return heights;
}