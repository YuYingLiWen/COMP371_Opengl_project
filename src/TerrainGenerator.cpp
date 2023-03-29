#include "TerrainGenerator.h"

#include "CustomRandom.h"


TerrainGenerator::TerrainGenerator()
{
}

TerrainGenerator::~TerrainGenerator()
{
}

TerrainData* TerrainGenerator::Generate(glm::vec2 dimensions)
{
    return Generate(dimensions.x, dimensions.y);
}

TerrainData* TerrainGenerator::Generate(unsigned int square_size)
{
    return Generate(square_size, square_size);
}

TerrainData* TerrainGenerator::Generate(unsigned int x_count, unsigned int z_count)
{
    if (x_count <= 0 || z_count <= 0) return nullptr;

    unsigned int size_x = x_count + 1;
    unsigned int size_z = z_count + 1;

    /// Generate Positions
    auto positions = std::make_shared<std::vector<glm::vec3>>();

    float half_x = size_x * 0.5f;
    float half_z = size_z * 0.5f;

    for (size_t z = 0; z < size_z; z++)
    {
        for (size_t x = 0; x < size_x; x++)
        {
            positions->push_back(
                glm::vec3(
                    x - half_x, 
                    CustomRandom::GetInstance().Generate(2.0f), 
                    z - half_z)
            );
        }
    }

    /// Generate Indexes
    auto indexes = std::make_shared<std::vector<unsigned int>>();

    for (unsigned int z = 0; z < size_z - 1; z++)
    {
        for (unsigned int x = 0; x < size_x - 1; x++)
        {
            //// A rectangle formed by 2 triangles
            // Triangle 1
            indexes->push_back(x + z * size_x);           // P0
            indexes->push_back(x + z * size_x + 1);       // P1
            indexes->push_back(x + (z + 1) * size_x + 1); // P2

            // Triangle 2
            indexes->push_back(x + z * size_x);           // P0
            indexes->push_back(x + (z + 1) * size_x + 1); // P1
            indexes->push_back(x + (z + 1) * size_x);     // P2
        }
    }
    
    /// Generate Normals
    auto normals = std::make_shared<std::vector<glm::vec3>>();

    for (size_t i = 0; i < indexes.get()->size(); i += 6)
    {
        //// A rectangle formed by 2 triangles
        // Triangle 1
        glm::vec3 p0 = (*positions)[(*indexes)[i]];
        glm::vec3 p1 = (*positions)[(*indexes)[i + 1]];
        glm::vec3 p2 = (*positions)[(*indexes)[i + 2]];

        normals.get()->push_back(glm::cross(p2 - p1, p0 -p1));

        // Triangle 2
        p0 = (*positions)[(*indexes)[i + 3]];
        p1 = (*positions)[(*indexes)[i + 4]];
        p2 = (*positions)[(*indexes)[i + 5]];

        normals.get()->push_back(glm::cross(p2 - p1, p0 - p1));
    }

    return new TerrainData { positions, indexes, normals };
}
