#include "stdafx.h"
#include "mcpp/world/chunk.h"
#include "mcpp/world/world.h"

mcpp::ClientChunk::ClientChunk(
    World* _world,
    int _x0, int _y0, int _z0,
    int _x1, int _y1, int _z1) :
    dirty(true),
    vaVertex(false), vaColor(false), vaTexCoord(false),
    vao(0), vbo(0), ebo(0),
    x0(_x0), y0(_y0), z0(_z0),
    x1(_x1), y1(_y1), z1(_z1),
    bufferSize(0), indicesSize(0),
    position(Vector3f(
        math::lerp(_x0, _x1, 0.5f),
        math::lerp(_y0, _y1, 0.5f),
        math::lerp(_z0, _z1, 0.5f)
    )),
    world(_world) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

mcpp::ClientChunk::~ClientChunk() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void mcpp::ClientChunk::rebuild() {
    dirty = false;
    auto& t = Tessellator::getInstance();
    t.begin();
    t.color(1.0f, 1.0f, 1.0f);
    for (int y = y0; y < y1; y++)
    {
        for (int x = x0; x < x1; x++)
        {
            for (int z = z0; z < z1; z++)
            {
                world->getBlock(x, y, z)->render(t, world, x, y, z);
            }
        }
    }
    t.end(vao, vbo, ebo, &vaVertex, &vaColor, &vaTexCoord, &bufferSize, &indicesSize);
}

void mcpp::ClientChunk::render() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)indicesSize, GL_UNSIGNED_INT, (void*)0);
}

bool mcpp::ClientChunk::isDirty() const {
    return dirty;
}

void mcpp::ClientChunk::markDirty() {
    dirty = true;
}
