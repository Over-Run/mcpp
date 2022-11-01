#pragma once
#include "mcpp/vector.h"

namespace mcpp {
    static constexpr int CHUNK_SIZE = 32;
    static constexpr int MAX_REBUILDS_PER_FRAME = 4;

    class World;

    class ClientChunk {
    private:
        bool dirty;
        bool vaVertex, vaColor, vaTexCoord;
        unsigned int vao, vbo, ebo;
        const int x0, y0, z0;
        const int x1, y1, z1;
        size_t bufferSize, indicesSize;
        const Vector3f position;
        World* const world;
    public:
        ClientChunk(World* world,
            int x0, int y0, int z0,
            int x1, int y1, int z1);
        ~ClientChunk();

        void rebuild();
        void render();

        bool isDirty() const;
        void markDirty();
    };
}
