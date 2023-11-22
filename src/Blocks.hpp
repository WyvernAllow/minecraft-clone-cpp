#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include <stdint.h>
#include <glm/glm.hpp>

enum class Block : uint8_t
{
    AIR,
    DIRT,
    STONE,
    GRASS
};

struct BlockProperties
{
    glm::vec2 atlas_coord_front;
    glm::vec2 atlas_coord_back;
    glm::vec2 atlas_coord_up;
    glm::vec2 atlas_coord_down;
    glm::vec2 atlas_coord_left;
    glm::vec2 atlas_coord_right;
};

void InitBlockProperties();
BlockProperties& GetBlockProperties(Block block);

#endif // BLOCKS_HPP