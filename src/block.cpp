#include "stdafx.h"
#include "mcpp/block.h"
#include "mcpp/texture.h"

unsigned char mcpp::block::getTexture(direction face) {
    return 0;
}

void mcpp::block::renderFace(tessellator& t, direction face, int x, int y, int z) {
    float x0 = x + 0.0f, y0 = y + 0.0f, z0 = z + 0.0f;
    float x1 = x + 1.0f, y1 = y + 1.0f, z1 = z + 1.0f;
    float u0 = 0.0f, v0 = 0.0f, u1 = 0.0f, v1 = 0.0f;
    unsigned char texture = getTexture(face);
    texture::atlasUV((texture % 16) * 16, (texture / 16) * 16, 16, 16, &u0, &v0, &u1, &v1);
    t.index(6, 0, 1, 2, 2, 3, 0);
    switch (face)
    {
    case mcpp::direction::WEST:
        t.vertex(x0, y1, z0).texCoord(u0, v0).emit();
        t.vertex(x0, y0, z0).texCoord(u0, v1).emit();
        t.vertex(x0, y0, z1).texCoord(u1, v1).emit();
        t.vertex(x0, y1, z1).texCoord(u1, v0).emit();
        break;
    case mcpp::direction::EAST:
        t.vertex(x1, y1, z1).texCoord(u0, v0).emit();
        t.vertex(x1, y0, z1).texCoord(u0, v1).emit();
        t.vertex(x1, y0, z0).texCoord(u1, v1).emit();
        t.vertex(x1, y1, z0).texCoord(u1, v0).emit();
        break;
    case mcpp::direction::DOWN:
        t.vertex(x0, y0, z1).texCoord(u0, v0).emit();
        t.vertex(x0, y0, z0).texCoord(u0, v1).emit();
        t.vertex(x1, y0, z0).texCoord(u1, v1).emit();
        t.vertex(x1, y0, z1).texCoord(u1, v0).emit();
        break;
    case mcpp::direction::UP:
        t.vertex(x0, y1, z0).texCoord(u0, v0).emit();
        t.vertex(x0, y1, z1).texCoord(u0, v1).emit();
        t.vertex(x1, y1, z1).texCoord(u1, v1).emit();
        t.vertex(x1, y1, z0).texCoord(u1, v0).emit();
        break;
    case mcpp::direction::NORTH:
        t.vertex(x1, y1, z0).texCoord(u0, v0).emit();
        t.vertex(x1, y0, z0).texCoord(u0, v1).emit();
        t.vertex(x0, y0, z0).texCoord(u1, v1).emit();
        t.vertex(x0, y1, z0).texCoord(u1, v0).emit();
        break;
    case mcpp::direction::SOUTH:
        t.vertex(x0, y1, z1).texCoord(u0, v0).emit();
        t.vertex(x0, y0, z1).texCoord(u0, v1).emit();
        t.vertex(x1, y0, z1).texCoord(u1, v1).emit();
        t.vertex(x1, y1, z1).texCoord(u1, v0).emit();
        break;
    default:
        break;
    }
}

void mcpp::block::render(tessellator& t, int x, int y, int z) {
    renderFace(t, mcpp::direction::WEST, x, y, z);
    renderFace(t, mcpp::direction::EAST, x, y, z);
    renderFace(t, mcpp::direction::DOWN, x, y, z);
    renderFace(t, mcpp::direction::UP, x, y, z);
    renderFace(t, mcpp::direction::NORTH, x, y, z);
    renderFace(t, mcpp::direction::SOUTH, x, y, z);
}

void mcpp::air_block::render(tessellator& t, int x, int y, int z) {}

unsigned char mcpp::grass_block::getTexture(direction face) {
    switch (face)
    {
    case mcpp::direction::DOWN:
        return 3;
        break;
    case mcpp::direction::UP:
        return 0;
        break;
    case mcpp::direction::WEST:
    case mcpp::direction::EAST:
    case mcpp::direction::NORTH:
    case mcpp::direction::SOUTH:
    default:
        return 1;
    }
}
