#include "stdafx.h"
#include "mcpp/texture.h"
#include "mcpp/world/block.h"
#include "mcpp/world/world.h" 

mcpp::Block::Block(mcpp::Blocks::T_blockId _id) : id(_id) {
}

unsigned char mcpp::Block::getTexture(Direction face) {
    return id;
}

bool mcpp::Block::hasSideTransparency(Direction face) {
    return false;
}

bool mcpp::Block::shouldRenderFace(World* world, Direction face, int x, int y, int z) {
    return world->getBlock(x + dirOffsetX(face), y + dirOffsetY(face), z + dirOffsetZ(face))->hasSideTransparency(dirOpposite(face));
}

void mcpp::Block::renderFace(Tessellator& t, World* world, Direction face, int x, int y, int z) {
    float x0 = x + 0.0f, y0 = y + 0.0f, z0 = z + 0.0f;
    float x1 = x + 1.0f, y1 = y + 1.0f, z1 = z + 1.0f;
    float u0 = 0.0f, v0 = 0.0f, u1 = 0.0f, v1 = 0.0f;
    unsigned char texture = getTexture(face);
    texture::atlasUV((texture % 16) * 16, (texture / 16) * 16, 16, 16, &u0, &v0, &u1, &v1);
    t.index(6, 0, 1, 2, 2, 3, 0);
    switch (face)
    {
    case mcpp::Direction::WEST:
        t.vertex(x0, y1, z0).texCoord(u0, v0).emit();
        t.vertex(x0, y0, z0).texCoord(u0, v1).emit();
        t.vertex(x0, y0, z1).texCoord(u1, v1).emit();
        t.vertex(x0, y1, z1).texCoord(u1, v0).emit();
        break;
    case mcpp::Direction::EAST:
        t.vertex(x1, y1, z1).texCoord(u0, v0).emit();
        t.vertex(x1, y0, z1).texCoord(u0, v1).emit();
        t.vertex(x1, y0, z0).texCoord(u1, v1).emit();
        t.vertex(x1, y1, z0).texCoord(u1, v0).emit();
        break;
    case mcpp::Direction::DOWN:
        t.vertex(x0, y0, z1).texCoord(u0, v0).emit();
        t.vertex(x0, y0, z0).texCoord(u0, v1).emit();
        t.vertex(x1, y0, z0).texCoord(u1, v1).emit();
        t.vertex(x1, y0, z1).texCoord(u1, v0).emit();
        break;
    case mcpp::Direction::UP:
        t.vertex(x0, y1, z0).texCoord(u0, v0).emit();
        t.vertex(x0, y1, z1).texCoord(u0, v1).emit();
        t.vertex(x1, y1, z1).texCoord(u1, v1).emit();
        t.vertex(x1, y1, z0).texCoord(u1, v0).emit();
        break;
    case mcpp::Direction::NORTH:
        t.vertex(x1, y1, z0).texCoord(u0, v0).emit();
        t.vertex(x1, y0, z0).texCoord(u0, v1).emit();
        t.vertex(x0, y0, z0).texCoord(u1, v1).emit();
        t.vertex(x0, y1, z0).texCoord(u1, v0).emit();
        break;
    case mcpp::Direction::SOUTH:
        t.vertex(x0, y1, z1).texCoord(u0, v0).emit();
        t.vertex(x0, y0, z1).texCoord(u0, v1).emit();
        t.vertex(x1, y0, z1).texCoord(u1, v1).emit();
        t.vertex(x1, y1, z1).texCoord(u1, v0).emit();
        break;
    default:
        break;
    }
}

void mcpp::Block::render(Tessellator& t, World* world, int x, int y, int z) {
    if (shouldRenderFace(world, Direction::WEST, x, y, z)) {
        renderFace(t, world, Direction::WEST, x, y, z);
    }
    if (shouldRenderFace(world, Direction::EAST, x, y, z)) {
        renderFace(t, world, Direction::EAST, x, y, z);
    }
    if (shouldRenderFace(world, Direction::DOWN, x, y, z)) {
        renderFace(t, world, Direction::DOWN, x, y, z);
    }
    if (shouldRenderFace(world, Direction::UP, x, y, z)) {
        renderFace(t, world, Direction::UP, x, y, z);
    }
    if (shouldRenderFace(world, Direction::NORTH, x, y, z)) {
        renderFace(t, world, Direction::NORTH, x, y, z);
    }
    if (shouldRenderFace(world, Direction::SOUTH, x, y, z)) {
        renderFace(t, world, Direction::SOUTH, x, y, z);
    }
}

mcpp::Blocks::T_blockId mcpp::Block::getId() const {
    return id;
}

mcpp::AirBlock::AirBlock(mcpp::Blocks::T_blockId _id) : Block(_id) {
}

bool mcpp::AirBlock::hasSideTransparency(Direction face) {
    return true;
}

void mcpp::AirBlock::render(Tessellator& t, World* world, int x, int y, int z) {
}

mcpp::GrassBlock::GrassBlock(mcpp::Blocks::T_blockId _id) : Block(_id) {
}

unsigned char mcpp::GrassBlock::getTexture(Direction face) {
    switch (face)
    {
    case mcpp::Direction::DOWN:
        return 3;
    case mcpp::Direction::UP:
        return 0;
    case mcpp::Direction::WEST:
    case mcpp::Direction::EAST:
    case mcpp::Direction::NORTH:
    case mcpp::Direction::SOUTH:
    default:
        return 1;
    }
}
