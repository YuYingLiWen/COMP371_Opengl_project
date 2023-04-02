#include "MeshGenerator.h"

#include "CustomRandom.h"
#include "AppTime.h"

MeshGenerator::MeshGenerator()
{
}

MeshGenerator::~MeshGenerator()
{
}

std::shared_ptr<Mesh> MeshGenerator::Generate(glm::i32vec2 dimensions)
{
    return Generate(dimensions.x, dimensions.y);
}

std::shared_ptr<Mesh> MeshGenerator::Generate(unsigned int square_size)
{
    return Generate(square_size, square_size);
}

std::shared_ptr<Mesh> MeshGenerator::Generate(unsigned int x_count, unsigned int z_count)
{
    if (x_count <= 0 || z_count <= 0) return nullptr;

    unsigned int size_x = x_count + 1;
    unsigned int size_z = z_count + 1;

    /// Generate Positions
    auto positions = std::make_shared<std::vector<glm::vec3>>();



    for (size_t z = 0; z < size_z; z++)
    {
        for (size_t x = 0; x < size_x; x++)
        {
            positions->push_back(
                glm::vec3(
                    (float)x , 
                    0.0f,
                    (float)z )
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
    glm::vec3 p0;
    glm::vec3 p1;
    glm::vec3 p2;
    for (size_t i = 0; i < indexes.get()->size(); i += 6)
    {
        //// A rectangle formed by 2 triangles
        // Triangle 1
        p0 = (*positions)[(*indexes)[i]];
        p1 = (*positions)[(*indexes)[i + 1]];
        p2 = (*positions)[(*indexes)[i + 2]];

        normals.get()->push_back(glm::normalize(glm::cross(p2 - p0, p1 - p0)));

        // Triangle 2
        p0 = (*positions)[(*indexes)[i + 3]];
        p1 = (*positions)[(*indexes)[i + 4]];
        p2 = (*positions)[(*indexes)[i + 5]];

        normals.get()->push_back(glm::normalize(glm::cross(p2 - p0, p1 - p0)));
    }

    return std::make_shared<Mesh>(Mesh { positions, indexes, normals, glm::i32vec2(size_x, size_z)});
}

std::shared_ptr<Mesh> MeshGenerator::GenerateGrid(unsigned int size, float spread)
{
    if (size <= 0) return nullptr;

    unsigned int length = size + 1;

    /// Generate Positions
    auto positions = std::make_shared<std::vector<glm::vec3>>();

    double z_inc = 0.0f;
    for (size_t z = 0; z < length; z++)
    {
        double x_inc = 0.0f;

        for (size_t x = 0; x < length; x++)
        {
            positions->push_back(
                glm::vec3(
                    (float)x_inc, 
                    0.0f,
                    (float)z_inc)
            );
            x_inc += spread;
        }
        z_inc += spread;
    }

    /// Generate Indexes
    auto indexes = std::make_shared<std::vector<unsigned int>>();

    for (unsigned int z = 0; z < length - 1; z++)
    {
        for (unsigned int x = 0; x < length - 1; x++)
        {
            //// A rectangle formed by 4 sides triangles

            //S1
            indexes->push_back(x + z * length);           // P0
            indexes->push_back(x + z * length + 1);       // P1
            
            //S2
            indexes->push_back(x + z * length + 1);       // P1
            indexes->push_back(x + (z + 1) * length + 1); // P2


            //S3
            indexes->push_back(x + (z + 1) * length + 1); // P2
            indexes->push_back(x + (z + 1) * length);     // P3
            
            //S4
            indexes->push_back(x + (z + 1) * length);     // P3
            indexes->push_back(x + z * length);           // P0
        }
    }

    return std::make_shared<Mesh>(Mesh { positions, indexes });
}


