#pragma once
#include "mcpp/world/block.h"

namespace mcpp {
    class World {
    private:
        const int width, height, depth;
        Blocks::T_blockId* p_blocks;
    public:
        World(int width, int height, int depth);
        ~World();

        bool setBlock(const Block* block, int x, int y, int z);
        Block* getBlock(int x, int y, int z);
        int getWidth() const;
        int getHeight() const;
        int getDepth() const;
    };
}
