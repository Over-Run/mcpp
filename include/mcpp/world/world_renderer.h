#pragma once
#include "world.h"

namespace mcpp {
    class ClientChunk;

    class WorldRenderer : public IWorldListener {
    private:
        int xChunks, yChunks, zChunks;
        size_t chunksSize;
        World* world;
        ClientChunk** chunks;

        void getDirtyChunks(std::vector<mcpp::ClientChunk*>& dirtyChunks, size_t* dirtySize);
        void markDirty(int x0, int y0, int z0, int x1, int y1, int z1);
    public:
        WorldRenderer(World* world);
        ~WorldRenderer();

        void pick();
        void updateDirtyChunks();
        void render();

        virtual void onBlockChanged(int x, int y, int z) override;
        virtual void allChanged() override;
    };
}
