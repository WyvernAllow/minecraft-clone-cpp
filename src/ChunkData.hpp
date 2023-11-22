#ifndef CHUNKDATA_HPP
#define CHUNKDATA_HPP

#include <stdint.h>
#include "Blocks.hpp"

const int CHUNK_SIZE = 16;

class ChunkData
{
public:
    Block GetBlock(int x, int y, int z) const;
    void SetBlock(int x, int y, int z, Block block);
    
private:
    Block m_blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
};

#endif // CHUNKDATA_HPP