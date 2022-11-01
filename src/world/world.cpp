#include "stdafx.h"
#include "mcpp/world/world.h"

mcpp::World::World(int _width, int _height, int _depth) :
    width(_width),
    height(_height),
    depth(_depth) {
    p_blocks = new Blocks::T_blockId[_width * _height * _depth];
}

mcpp::World::~World() {
    delete[] p_blocks;
    p_blocks = nullptr;
}

bool mcpp::World::setBlock(const Block* block, int x, int y, int z) {
    if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth)
    {
        return false;
    }
    p_blocks[x + (y * depth + z) * width] = block->getId();
    return true;
}

mcpp::Block* mcpp::World::getBlock(int x, int y, int z) {
    if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth)
    {
        return Blocks::AIR;
    }
    return Blocks::blockIds[p_blocks[x + (y * depth + z) * width]];
}

int mcpp::World::getWidth() const {
    return width;
}

int mcpp::World::getHeight() const {
    return height;
}

int mcpp::World::getDepth() const {
    return depth;
}
