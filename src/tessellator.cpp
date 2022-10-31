#include "stdafx.h"
#include "mcpp/tessellator.h"

mcpp::tessellator::tessellator() :
    r(0), g(0), b(0),
    hasColor(false), hasTexture(false),
    stride(sizeof(float) * 3), pos(0), vertexCount(0),
    x(0.0f), y(0.0f), z(0.0f),
    u(0.0f), v(0.0f),
    buffer{ 0.0f } {}

mcpp::tessellator& mcpp::tessellator::getInstance() {
    static tessellator t;
    return t;
}

void mcpp::tessellator::begin() {
    pos = 0;
    vertexCount = 0;
    indices.clear();
}

mcpp::tessellator& mcpp::tessellator::vertex(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
    return *this;
}

mcpp::tessellator& mcpp::tessellator::color(float _r, float _g, float _b) {
    r = _r;
    g = _g;
    b = _b;
    if (!hasColor)
    {
        hasColor = true;
        stride += sizeof(float) * 3;
    }
    return *this;
}

mcpp::tessellator& mcpp::tessellator::texCoord(float _u, float _v) {
    u = _u;
    v = _v;
    if (!hasTexture)
    {
        hasTexture = true;
        stride += sizeof(float) * 2;
    }
    return *this;
}

mcpp::tessellator& mcpp::tessellator::index(unsigned int numIndices, unsigned int...) {
    va_list args;
    va_start(args, numIndices);
    for (unsigned int i = 0; i < numIndices; i++)
    {
        unsigned int _index = va_arg(args, unsigned int);
        indices.push_back(vertexCount + _index);
    }
    va_end(args);
    return *this;
}

void mcpp::tessellator::emit() {
    buffer[pos++] = x;
    buffer[pos++] = y;
    buffer[pos++] = z;
    if (hasColor)
    {
        buffer[pos++] = r;
        buffer[pos++] = g;
        buffer[pos++] = b;
    }
    if (hasTexture)
    {
        buffer[pos++] = u;
        buffer[pos++] = v;
    }
    ++vertexCount;
}

/// End tessellation.
/// @param vao[in] the vao to be configured with the vertex attributes and ebo.
/// @param vaVertex[in,out,nullable] the vertex attribute status of vertex
/// @param indicesSize[in,out,notnull] the old indices size
void mcpp::tessellator::end(unsigned int vao, unsigned int vbo, unsigned int ebo, 
    bool* vaVertex, bool* vaColor, bool* vaTexCoord,
    size_t* bufferSize, size_t* indicesSize) {
    bool noVbo = !glIsBuffer(vbo), noEbo = !glIsBuffer(ebo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if (noVbo || (bufferSize == nullptr) || (*bufferSize < pos))
    {
        if (bufferSize) { *bufferSize = pos; }
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pos, buffer.data(), GL_STATIC_DRAW);
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0LL, sizeof(float) * pos, buffer.data());
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    const auto& indexData = indices.data();
    size_t _sz_indices = indices.size();
    if (noEbo || (*indicesSize < _sz_indices))
    {
        *indicesSize = _sz_indices;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * _sz_indices, indexData, GL_STATIC_DRAW);
    }
    else
    {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0LL, sizeof(int) * _sz_indices, indexData);
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    if ((vaVertex && !(*vaVertex)) || (vaVertex == nullptr))
    {
        if (vaVertex) { *vaVertex = true; }
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    }
    // if has color and:
    //  if vaColor is not null and the value is false or:
    //  if vaColor is null
    if (hasColor && ((vaColor && !(*vaColor)) || vaColor == nullptr))
    {
        if (vaColor) { *vaColor = true; }
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));
    }
    if (hasTexture && ((vaTexCoord && !(*vaTexCoord)) || vaTexCoord == nullptr))
    {
        if (vaTexCoord) { *vaTexCoord = true; }
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3 + (hasColor ? sizeof(float) * 3 : 0)));
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
