#include "Blocks.hpp"
#include <unordered_map>

std::unordered_map<Block, BlockProperties> block_properties;

void InitBlockProperties()
{
    block_properties[Block::AIR] = 
    {
        .atlas_coord_front = glm::vec2(0, 0),
        .atlas_coord_back  = glm::vec2(0, 0),
        .atlas_coord_up    = glm::vec2(0, 0),
        .atlas_coord_down  = glm::vec2(0, 0),
        .atlas_coord_left  = glm::vec2(0, 0),
        .atlas_coord_right = glm::vec2(0, 0),
    };

    block_properties[Block::DIRT] = 
    {
        .atlas_coord_front = glm::vec2(0, 0),
        .atlas_coord_back  = glm::vec2(0, 0),
        .atlas_coord_up    = glm::vec2(0, 0),
        .atlas_coord_down  = glm::vec2(0, 0),
        .atlas_coord_left  = glm::vec2(0, 0),
        .atlas_coord_right = glm::vec2(0, 0),
    };

    block_properties[Block::STONE] = 
    {
        .atlas_coord_front = glm::vec2(1, 0),
        .atlas_coord_back  = glm::vec2(1, 0),
        .atlas_coord_up    = glm::vec2(1, 0),
        .atlas_coord_down  = glm::vec2(1, 0),
        .atlas_coord_left  = glm::vec2(1, 0),
        .atlas_coord_right = glm::vec2(1, 0),
    };

    block_properties[Block::GRASS] = 
    {
        .atlas_coord_front = glm::vec2(2, 0),
        .atlas_coord_back  = glm::vec2(2, 0),
        .atlas_coord_up    = glm::vec2(3, 0),
        .atlas_coord_down  = glm::vec2(0, 0),
        .atlas_coord_left  = glm::vec2(2, 0),
        .atlas_coord_right = glm::vec2(2, 0),
    };
}

BlockProperties& GetBlockProperties(Block block)
{
    return block_properties.at(block);
}