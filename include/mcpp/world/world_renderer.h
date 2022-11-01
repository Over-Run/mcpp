#pragma once

namespace mcpp {
    class World;
    class ClientChunk;

    class WorldRenderer {
    private:
        int xChunks, yChunks, zChunks;
        size_t chunksSize;
        World* world;
        ClientChunk** chunks;
    public:
        WorldRenderer(World* world);
        ~WorldRenderer();

        void updateDirtyChunks();
        void render();
    };
}
