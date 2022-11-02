#include "stdafx.h"
#include "mcpp/world/chunk.h"
#include "mcpp/world/world.h"
#include "mcpp/world/world_renderer.h"

mcpp::WorldRenderer::WorldRenderer(World* _world) :
    xChunks(_world->getWidth() / CHUNK_SIZE),
    yChunks(_world->getHeight() / CHUNK_SIZE),
    zChunks(_world->getDepth() / CHUNK_SIZE),
    chunksSize((size_t)xChunks* yChunks* zChunks),
    world(_world),
    chunks(new ClientChunk* [xChunks * yChunks * zChunks]) {
    for (int y = 0; y < yChunks; y++)
    {
        for (int x = 0; x < xChunks; x++)
        {
            for (int z = 0; z < zChunks; z++)
            {
                chunks[x + (y * zChunks + z) * xChunks] = new ClientChunk(
                    _world,
                    x * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE,
                    (x + 1) * CHUNK_SIZE, (y + 1) * CHUNK_SIZE, (z + 1) * CHUNK_SIZE
                );
            }
        }
    }
}

mcpp::WorldRenderer::~WorldRenderer() {
    for (size_t i = 0; i < chunksSize; i++) {
        delete chunks[i];
    }
    delete[] chunks;
}

void mcpp::WorldRenderer::pick() {

}

void mcpp::WorldRenderer::getDirtyChunks(std::vector<mcpp::ClientChunk*>& dirtyChunks, size_t* dirtySize) {
    for (size_t i = 0; i < chunksSize; i++)
    {
        auto& chunk = chunks[i];
        if (chunk->isDirty())
        {
            dirtyChunks.push_back(chunk);
            ++(*dirtySize);
        }
    }
}

void mcpp::WorldRenderer::markDirty(int x0, int y0, int z0, int x1, int y1, int z1) {
    x0 /= CHUNK_SIZE;
    x1 /= CHUNK_SIZE;
    y0 /= CHUNK_SIZE;
    y1 /= CHUNK_SIZE;
    z0 /= CHUNK_SIZE;
    z1 /= CHUNK_SIZE;

    if (x0 < 0) {
        x0 = 0;
    }
    if (y0 < 0) {
        y0 = 0;
    }
    if (z0 < 0) {
        z0 = 0;
    }
    if (x1 >= xChunks) {
        x1 = xChunks - 1;
    }
    if (y1 >= yChunks) {
        y1 = yChunks - 1;
    }
    if (z1 >= zChunks) {
        z1 = zChunks - 1;
    }

    for (int x = x0; x <= x1; ++x) {
        for (int y = y0; y <= y1; ++y) {
            for (int z = z0; z <= z1; ++z) {
                chunks[(x + y * xChunks) * zChunks + z]->markDirty();
            }
        }
    }
}

void mcpp::WorldRenderer::updateDirtyChunks() {
    std::vector<ClientChunk*> dirty = std::vector<ClientChunk*>();
    size_t dirtySize = 0;
    getDirtyChunks(dirty, &dirtySize);
    for (size_t i = 0; i < mcpp::MAX_REBUILDS_PER_FRAME && i < dirtySize; i++)
    {
        dirty[i]->rebuild();
    }
}

void mcpp::WorldRenderer::render() {
    for (size_t i = 0; i < chunksSize; i++) {
        chunks[i]->render();
    }
}

void mcpp::WorldRenderer::onBlockChanged(int x, int y, int z) {
    markDirty(x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
}

void mcpp::WorldRenderer::allChanged() {
    for (size_t i = 0; i < chunksSize; i++) {
        chunks[i]->markDirty();
    }
}
