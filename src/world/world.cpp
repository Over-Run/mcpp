#include "stdafx.h"
#include "mcpp/world/world.h"

mcpp::World::World(int _width, int _height, int _depth) :
    width(_width),
    height(_height),
    depth(_depth),
    p_blocks(new Blocks::T_blockId[_width * _height * _depth]),
    listeners(std::vector<IWorldListener*>()) {
}

mcpp::World::~World() {
    delete[] p_blocks;
    p_blocks = nullptr;
}

std::vector<mcpp::AABBox*>& mcpp::World::getCubes(const AABBox& box, std::vector<AABBox*>& dest) {
    int x0 = (int)box.minX;
    int x1 = (int)(box.maxX + 1.0f);
    int y0 = (int)box.minY;
    int y1 = (int)(box.maxY + 1.0f);
    int z0 = (int)box.minZ;
    int z1 = (int)(box.maxZ + 1.0f);

    if (x0 < 0) {
        x0 = 0;
    }
    if (y0 < 0) {
        y0 = 0;
    }
    if (z0 < 0) {
        z0 = 0;
    }

    if (x1 > width) {
        x1 = width;
    }
    if (y1 > height) {
        y1 = height;
    }
    if (z1 > depth) {
        z1 = depth;
    }

    for (int x = x0; x < x1; ++x) {
        for (int y = y0; y < y1; ++y) {
            for (int z = z0; z < z1; ++z) {
                Block* block = getBlock(x, y, z);
                AABBox* box = new AABBox();
                block->getCollisionShape(*box);
                if (!box->isInvalid)
                {
                    box->move((float)x, (float)y, (float)z);
                    dest.push_back(box);
                }
                else { delete box; }
            }
        }
    }

    return dest;
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
