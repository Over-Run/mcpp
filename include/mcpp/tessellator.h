#pragma once
#include "stdafx.h"

namespace mcpp {
    class Tessellator {
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

        Tessellator();
        Tessellator(const Tessellator&) = delete;
    public:
        static Tessellator& getInstance();

        void begin();
        Tessellator& vertex(float x, float y, float z);
        Tessellator& color(float r, float g, float b);
        Tessellator& texCoord(float u, float v);
        Tessellator& index(unsigned int numIndices, unsigned int...);
        void emit();

        /// End tessellation.
        ///
        /// @param vao[in] the vao to be configured with the vertex attributes and ebo.
        /// @param vaVertex[in,out,nullable] the vertex attribute status of vertex
        /// @param indicesSize[in,out,notnull] the old indices size
        void end(unsigned int vao, unsigned int vbo, unsigned int ebo,
            bool* vaVertex, bool* vaColor, bool* vaTexCoord,
            size_t* bufferSize, size_t* indicesSize);
    };
}
