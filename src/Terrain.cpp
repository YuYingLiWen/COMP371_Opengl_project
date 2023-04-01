#include "Terrain.h"

#include "YuMath.h"

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
    auto basic_mesh = MeshGenerator::Generate(dimensions.x, dimensions.y);

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
    auto basic_mesh = MeshGenerator::Generate(x, z);

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
            case LINES:
                heights->push_back(LinePattern(off_x, off_z));
                break;
            case BUMP:
                heights->push_back(BumpPattern(off_x, off_z));
                break;
            }
            off_x += freq;
        }
        off_z += freq;
    }

    return heights;
}


std::shared_ptr<Mesh> TerrainGenerator::GeneratePerlinTerrain(double x, double z)
{
    auto basic_mesh = MeshGenerator::Generate(x, z);

    auto heights = PerlinNoise(x, z);

    unsigned int index = 0;
    for (auto& position : *basic_mesh->positions)
    {
        position.y = (heights.at(index));

        index++;
    }

    return basic_mesh;
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
                CirclePattern(
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

double TerrainGenerator::Noise(double x_off)
{
    return glm::sin(glm::radians(x_off));
}

std::vector<float> TerrainGenerator::PerlinNoise(double x_off, double z_off)
{
    auto latice = std::vector<glm::vec2>();
    auto heights = std::vector<float>();

    unsigned int x_size = x_off + 2;
    unsigned int z_size = z_off + 2;

    for (size_t z = 0; z < z_size; z++) // Populate gradient field
    {
        for (size_t x = 0; x < x_size; x++)
        {
            latice.push_back(CustomRandom::GetInstance().RandomCircle());
        }
    }

    for (size_t z = 0; z < z_size - 1; z++) // For each square
    {
        for (size_t x = 0; x < x_size - 1; x++)
        {
            glm::vec2 bot_left  = latice[x +    z    * x_size    ];
            glm::vec2 top_left  = latice[x + (z + 1) * x_size    ];
            glm::vec2 bot_right = latice[x +    z    * x_size + 1];
            glm::vec2 top_right = latice[x + (z + 1) * x_size + 1];

            glm::vec2 grid_point = glm::vec2(glm::length(bot_right - bot_left) * 0.5f, glm::length(top_right - bot_right) * 0.5f);

            glm::vec2 to_bot_left  = bot_left  - grid_point;
            glm::vec2 to_top_left  = top_left  - grid_point;
            glm::vec2 to_bot_right = bot_right - grid_point;
            glm::vec2 to_top_right = top_right - grid_point;

            float dot_tr = glm::dot(top_right, to_top_right);
            float dot_tl = glm::dot(top_left, to_top_left);
            float dot_br = glm::dot(bot_right, to_bot_right);
            float dot_bl = glm::dot(bot_left, to_bot_left);


            float l1 = YuMath::Lerp(dot_bl, dot_tl, 0.5f);
            float l2 = YuMath::Lerp(dot_br, dot_tr, 0.5f);


            heights.push_back(YuMath::Lerp(l1, l2, 0.5f));
        }
    }

    return heights;
}

double TerrainGenerator::CirclePattern(double x_off, double z_off)
{
    return CirclePattern(1.0f, 1.0f, 0.0f, 0.0f);
}

double TerrainGenerator::CirclePattern(double x_off, double z_off, double center_x, double center_z)
{
    double noise = std::sqrt(std::pow(x_off - center_x, 2.0f) + std::pow(z_off - center_z, 2.0f));
    return glm::sin(glm::radians(noise));
}

double TerrainGenerator::LinePattern(double x_off, double z_off)
{
    return glm::sin(glm::radians(x_off + z_off));
}

double TerrainGenerator::BumpPattern(double x_off, double z_off)
{
    return glm::sin(glm::radians(x_off)) + glm::sin(glm::radians(z_off));
}