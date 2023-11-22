#include "Chunk.hpp"

Chunk::Chunk(int x, int y)
    : m_is_dirty(true), m_x(x), m_y(y)
{

}

Block Chunk::GetBlock(int x, int y, int z) const
{
    return m_data.GetBlock(x, y, z);
}

void Chunk::SetBlock(int x, int y, int z, Block block)
{
    m_data.SetBlock(x, y, z, block);
    m_is_dirty = true;
}

void Chunk::Update()
{
    if(m_is_dirty)
    {
        m_mesh.Mesh(m_data);
        m_is_dirty = false;
    }
}

void Chunk::Draw()
{
    m_mesh.Draw();
}