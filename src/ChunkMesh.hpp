#ifndef CHUNKMESH_HPP
#define CHUNKMESH_HPP

#include <vector>

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "ChunkData.hpp"

const int MAX_QUADS = 8000;
const int MAX_VERTS = MAX_QUADS * 4;
const int MAX_INDICES = MAX_QUADS * 6;

struct ChunkVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 atlas_offset;
};

struct ChunkFaceMask
{
    Block block;
    int normal;

    bool operator==(const ChunkFaceMask& other) const
    {
        return block == other.block && normal == other.normal;
    }

    bool operator!=(const ChunkFaceMask& other) const
    {
        return block != other.block || normal != other.normal;
    }
};

class ChunkMesh
{
public:
    ChunkMesh();
    ~ChunkMesh();

    ChunkMesh(const ChunkMesh& other) = delete;
    ChunkMesh& operator=(const ChunkMesh& other) = delete;

    ChunkMesh(ChunkMesh&& other);
    ChunkMesh& operator=(ChunkMesh&& other);

    void Mesh(ChunkData& chunk);
    void Draw();

private:
    void AppendQuad(glm::vec3 bl, glm::vec3 tl, glm::vec3 tr, glm::vec3 br, ChunkFaceMask mask, glm::ivec3 axis_mask);
    glm::vec2 GetAtlasOffset(glm::ivec3 axis_mask, ChunkFaceMask mask);

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;

    std::vector<ChunkVertex> m_vertices;
    std::vector<unsigned int> m_indices;
};

#endif // CHUNKMESH_HPP