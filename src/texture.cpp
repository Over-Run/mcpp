#include "stdafx.h"
#include "mcpp/texture.h"

std::array<unsigned int, 48> textures2D{ 0 };
unsigned int activeUnit2D = 0;

void mcpp::texture::atlasUV(int x, int y, int w, int h,
    float* u0, float* v0, float* u1, float* v1,
    int maxW, int maxH) {
    if (u0) { *u0 = static_cast<float>(x) / maxW; }
    if (v0) { *v0 = static_cast<float>(y) / maxH; }
    if (u1) { *u1 = (static_cast<float>(x + w)) / maxW; }
    if (v1) { *v1 = (static_cast<float>(y + h)) / maxH; }
}

void mcpp::texture::bind2D(unsigned int unit, unsigned int texture) {
    if (activeUnit2D != unit)
    {
        activeUnit2D = unit;
        glActiveTexture(GL_TEXTURE0 + unit);
    }
    if (textures2D[unit] != texture)
    {
        textures2D[unit] = texture;
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}
