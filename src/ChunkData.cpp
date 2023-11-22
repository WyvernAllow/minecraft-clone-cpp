#include "ChunkData.hpp"

Block ChunkData::GetBlock(int x, int y, int z) const
{
    if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE)
    {
        return m_blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE];
    }

    return Block::AIR;
}

void ChunkData::SetBlock(int x, int y, int z, Block block)
{
    if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE)
    {
        m_blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE] =  block;
    }
}