#include "TerrainGenerator.h"

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


std::shared_ptr<Mesh> TerrainGenerator::GeneratePerlinTerrain(unsigned int x, unsigned int z)
{
    auto basic_mesh = MeshGenerator::Generate(x, z);

    unsigned int px = 10;
    unsigned int pz = 10;


    GeneratePerlinNoiseGrid(px, pz);
    

    for (int i = 0; i < 1; i++)
    {
        for (auto& position : *basic_mesh->positions)
        {
            // Maps mesh to perlin grid

            double x_ratio = ((double)(grid.GetX()) / (double)(x + 1u));
            double z_ratio = ((double)(grid.GetZ()) / (double)(z + 1u));
            double u = position.x * x_ratio;
            double v = position.z * z_ratio;

            // Do calculations
            position.y += (PerlinNoise(u, v) );
        }
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

void TerrainGenerator::GeneratePerlinNoiseGrid(unsigned int grid_x, unsigned int grid_z)
{
    grid.Generate(grid_x, grid_z);
}

float TerrainGenerator::PerlinNoise(double u, double v)
{
    /// Grid might be null. Check before calling this function.
    auto& latice = grid.GetGrid();
    
    // Maps mesh position to grid position

    unsigned int cell_u = (unsigned int)glm::floor(u);
    unsigned int cell_v = (unsigned int)glm::floor(v);

    glm::vec2 point = glm::vec2(u, v);

    glm::vec2 grid_bl = glm::vec2(cell_u, cell_v);
    glm::vec2 grid_tl = glm::vec2(cell_u, cell_v + 1);
    glm::vec2 grid_br = glm::vec2(cell_u + 1, cell_v);
    glm::vec2 grid_tr = glm::vec2(cell_u + 1, cell_v + 1);

    glm::vec2 to_bot_left =  point - grid_bl;
    glm::vec2 to_top_left =  point - grid_tl;
    glm::vec2 to_bot_right = point - grid_br;
    glm::vec2 to_top_right = point - grid_tr;

    int i_bl = cell_u +     cell_v       * (grid.GetX() + 1);
    int i_br = cell_u + 1 + cell_v       * (grid.GetX() + 1);
    int i_tl = cell_u +     (cell_v + 1) * (grid.GetX() + 1);
    int i_tr = cell_u + 1 + (cell_v + 1) * (grid.GetX() + 1);

    glm::vec2 grad_bot_left = latice.at(i_bl);
    glm::vec2 grad_bot_right = latice.at(i_br);
    glm::vec2 grad_top_left = latice.at(i_tl);
    glm::vec2 grad_top_right = latice.at(i_tr);

    double dot_tr = glm::dot(grad_top_right, to_top_right);
    double dot_tl = glm::dot(grad_top_left , to_top_left );
    double dot_br = glm::dot(grad_bot_right, to_bot_right);
    double dot_bl = glm::dot(grad_bot_left , to_bot_left );
    
    // Normalize to cell
    double ut = u - (double)cell_u;
    double vt = v - (double)cell_v;

    double l1 = YuMath::Lerp(dot_bl, dot_br, ut);
    double l2 = YuMath::Lerp(dot_tl, dot_tr, ut);

    return YuMath::Lerp(l1, l2, vt);
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



//// Perlin Grid

void PerlinGrid::Generate(unsigned int x, unsigned int z)
{
    this->x = x;
    this->z = z;

    grid.clear();

    for (size_t z = 0; z < (PerlinGrid::z + 1ull); z++) // Populate gradient field
    {
        for (size_t x = 0; x < (PerlinGrid::x + 1ull); x++)
        {
            grid.push_back(CustomRandom::GetInstance().RandomCircle() * 50.0f);
        }
    }
}

unsigned int PerlinGrid::GetX()
{
    return x;
}

unsigned int PerlinGrid::GetZ()
{
    return z;
}

std::vector<glm::vec2>& PerlinGrid::GetGrid() { return grid; }
