#pragma once
#include "stdafx.h"

namespace mcpp {
    class tessellator {
    public:
        static constexpr int MEMORY_USE = 4 * 1024 * 1024;
        static constexpr int BUFFER_SIZE = MEMORY_USE / sizeof(float);
    private:
        bool hasColor, hasTexture;
        unsigned int stride, pos, vertexCount;
        float x, y, z;
        float r, g, b;
        float u, v;
        std::array<float, BUFFER_SIZE> buffer;
        std::vector<unsigned int> indices;

        tessellator();
        tessellator(const tessellator&) = delete;
    public:
        static tessellator& getInstance();

        void begin();
        tessellator& vertex(float x, float y, float z);
        tessellator& color(float r, float g, float b);
        tessellator& texCoord(float u, float v);
        tessellator& index(unsigned int numIndices, unsigned int...);
        void emit();
        void end(unsigned int vao, unsigned int vbo, unsigned int ebo,
            bool* vaVertex, bool* vaColor, bool* vaTexCoord,
            size_t* bufferSize, size_t* indicesSize);
    };
}
