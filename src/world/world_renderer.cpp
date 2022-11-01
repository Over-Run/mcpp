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

void getDirtyChunks(size_t chunksSize, mcpp::ClientChunk** chunks,
    std::vector<mcpp::ClientChunk*>& dirtyChunks, size_t* dirtySize) {
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

void mcpp::WorldRenderer::updateDirtyChunks() {
    std::vector<ClientChunk*> dirty = std::vector<ClientChunk*>();
    size_t dirtySize = 0;
    getDirtyChunks(chunksSize, chunks, dirty, &dirtySize);
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
