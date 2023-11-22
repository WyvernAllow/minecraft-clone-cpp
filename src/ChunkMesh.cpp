#include "ChunkMesh.hpp"

ChunkMesh::ChunkMesh()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ChunkVertex) * MAX_VERTS, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * MAX_INDICES, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (const void*)(offsetof(ChunkVertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (const void*)(offsetof(ChunkVertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (const void*)(offsetof(ChunkVertex, atlas_offset)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ChunkMesh::~ChunkMesh()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

ChunkMesh::ChunkMesh(ChunkMesh&& other)
{
    m_vao = other.m_vao;
    m_vbo = other.m_vbo;
    m_ebo = other.m_ebo;

    other.m_vao = 0;
    other.m_vbo = 0;
    other.m_ebo = 0;
}

ChunkMesh& ChunkMesh::operator=(ChunkMesh&& other)
{
    if (this != &other)
    {
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
        glDeleteVertexArrays(1, &m_vao);

        m_vao = other.m_vao;
        m_vbo = other.m_vbo;
        m_ebo = other.m_ebo;

        other.m_vao = 0;
        other.m_vbo = 0;
        other.m_ebo = 0;
    }

    return *this;
}

void ChunkMesh::Draw()
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

glm::vec2 ChunkMesh::GetAtlasOffset(glm::ivec3 axis_mask, ChunkFaceMask mask)
{
    BlockProperties properties = GetBlockProperties(mask.block);

    if (axis_mask.x > 0.0f)
    {
        return mask.normal > 0.0f ? properties.atlas_coord_right : properties.atlas_coord_left;
    }
    else if (axis_mask.y > 0.0f)
    {
        return mask.normal > 0.0f ? properties.atlas_coord_up : properties.atlas_coord_down;
    }
    else if (axis_mask.z > 0.0f)
    {
        return mask.normal > 0.0f ? properties.atlas_coord_front : properties.atlas_coord_back;
    }

    return properties.atlas_coord_down;
}

void ChunkMesh::AppendQuad(glm::vec3 bl, glm::vec3 tl, glm::vec3 tr, glm::vec3 br, ChunkFaceMask mask, glm::ivec3 axis_mask)
{
    glm::vec3 normal = axis_mask * mask.normal;

    glm::vec2 atlas_offset = GetAtlasOffset(axis_mask, mask);

    m_vertices.push_back({.position = bl, .normal = normal, .atlas_offset = atlas_offset});
    m_vertices.push_back({.position = br, .normal = normal, .atlas_offset = atlas_offset});
    m_vertices.push_back({.position = tl, .normal = normal, .atlas_offset = atlas_offset});
    m_vertices.push_back({.position = tr, .normal = normal, .atlas_offset = atlas_offset});

    if(mask.normal > 0)
    {
        m_indices.emplace_back(m_vertices.size() - 4);
        m_indices.emplace_back(m_vertices.size() - 2);
        m_indices.emplace_back(m_vertices.size() - 3);

        m_indices.emplace_back(m_vertices.size() - 3);
        m_indices.emplace_back(m_vertices.size() - 2);
        m_indices.emplace_back(m_vertices.size() - 1);
    }
    else
    {
        m_indices.emplace_back(m_vertices.size() - 4);
        m_indices.emplace_back(m_vertices.size() - 3);
        m_indices.emplace_back(m_vertices.size() - 2);

        m_indices.emplace_back(m_vertices.size() - 2);
        m_indices.emplace_back(m_vertices.size() - 3);
        m_indices.emplace_back(m_vertices.size() - 1);
    }
}

void ChunkMesh::Mesh(ChunkData& chunk)
{
    m_vertices.clear();
    m_indices.clear();

    for(int axis = 0; axis < 3; axis++)
    {
        int i, j, k, l, w, h;

        int u = (axis + 1) % 3;
        int v = (axis + 2) % 3;

        glm::ivec3 x = glm::ivec3(0);
        glm::ivec3 axis_mask = glm::ivec3(0);
        axis_mask[axis] = 1;

        ChunkFaceMask mask[CHUNK_SIZE * CHUNK_SIZE];

        // Check each slice of the chunk one at a time.
        for(x[axis] = -1; x[axis] < CHUNK_SIZE;)
        {
            // Compute the mask.
            int n = 0;
            for (x[v] = 0; x[v] < CHUNK_SIZE; x[v]++)
            {
                for (x[u] = 0; x[u] < CHUNK_SIZE; x[u]++)
                {
                    Block current_block = chunk.GetBlock(x[0], x[1], x[2]);
                    Block compare_block = chunk.GetBlock(x[0] + axis_mask[0], x[1] + axis_mask[1], x[2] + axis_mask[2]);

                    bool current_opaque = current_block != Block::AIR;
                    bool compare_opaque = compare_block != Block::AIR;

                    if(current_opaque == compare_opaque)
                    {
                        mask[n++] = {Block::AIR, 0};
                    }
                    else if(current_opaque)
                    {
                        mask[n++] = {current_block,  1};
                    }
                    else
                    {
                        mask[n++] = {compare_block, -1};
                    }
                }
            }

            x[axis]++;
            n = 0;

            // Generate the mesh from the mask.
            for (j = 0; j < CHUNK_SIZE; ++j)
            {
                for (i = 0; i < CHUNK_SIZE;)
                {
                    if (mask[n].normal != 0)
                    {
                        ChunkFaceMask current_mask = mask[n];

                        // Compute the width and height of the quad.
                        for (w = 1; i + w < CHUNK_SIZE && mask[n + w] == current_mask; w++) { }

                        bool done = false;
                        for (h = 1; j + h < CHUNK_SIZE; h++)
                        {
                            for (k = 0; k < w; k++)
                            {
                                if (mask[n + k + h * CHUNK_SIZE] != current_mask)
                                {
                                    done = true;
                                    break;
                                }
                            }

                            if (done)
                            {
                                break;
                            }
                        }

                        x[u] = i;
                        x[v] = j;

                        glm::ivec3 du = glm::ivec3(0);
                        glm::ivec3 dv = glm::ivec3(0);

                        du[u] = w;
                        dv[v] = h;

                        glm::vec3 bl = {x[0], x[1], x[2]};
                        glm::vec3 tl = {x[0] + du[0], x[1] + du[1], x[2] + du[2]};
                        glm::vec3 tr = {x[0] + du[0] + dv[0], x[1] + du[1] + dv[1], x[2] + du[2] + dv[2]};
                        glm::vec3 br = {x[0] + dv[0], x[1] + dv[1],x[2] + dv[2]};

                        AppendQuad(bl, tl, tr, br, current_mask, glm::ivec3(axis_mask[0], axis_mask[1], axis_mask[2]));

                        // Clear this part of the mask.
                        for (l = 0; l < h; ++l)
                        {
                            for (k = 0; k < w; ++k)
                            {
                                mask[n + k + l * CHUNK_SIZE] = {Block::AIR, 0};
                            }
                        }

                        i += w;
                        n += w;
                    }
                    else
                    {
                        i++;
                        n++;
                    }
                }
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ChunkVertex) * MAX_VERTS, m_vertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * MAX_INDICES, m_indices.data());
}