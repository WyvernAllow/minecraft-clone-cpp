#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "ChunkData.hpp"
#include "ChunkMesh.hpp"

class Chunk
{
public:
    Chunk(int x, int y);

    Block GetBlock(int x, int y, int z) const;
    void SetBlock(int x, int y, int z, Block block);

    void Update();
    void Draw();

private:
    ChunkData m_data;
    ChunkMesh m_mesh;

    bool m_is_dirty;
    int m_x;
    int m_y;
};

#endif // CHUNK_HPP