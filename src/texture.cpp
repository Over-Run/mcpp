#include "stdafx.h"
#include "mcpp/texture.h"

std::array<unsigned int, 48> textures2D{ 0 };
std::map<std::string_view, unsigned int> texture2DLoad{ };
unsigned int activeUnit2D = 0;

void mcpp::texture::atlasUV(int x, int y, int w, int h,
    TextureRegion& region,
    int maxW, int maxH) {
    region.u0 = static_cast<float>(x) / maxW;
    region.v0 = static_cast<float>(y) / maxH;
    region.u1 = (static_cast<float>(x + w)) / maxW;
    region.v1 = (static_cast<float>(y + h)) / maxH;
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

void mcpp::texture::putTexture2D(std::string_view name, unsigned int texture) {
    texture2DLoad[name] = texture;
}

void mcpp::texture::findTexture2D(std::string_view name, unsigned int* texture) {
    if (texture2DLoad.count(name) == 0)
    {
        *texture = 0;
        return;
    }
    *texture = texture2DLoad[name];
}

void mcpp::texture::deleteTextures2D() {
    for (auto& p : texture2DLoad) {
        glDeleteTextures(1, &p.second);
    }
}
