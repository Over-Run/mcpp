#pragma once
#include "mcpp/math/aabb.h"
#include "mcpp/world/block.h"

namespace mcpp {
    class World {
    private:
        const int width, height, depth;
        Blocks::T_blockId* p_blocks;
    public:
        World(int width, int height, int depth);
        ~World();

        /// <summary>
        /// Get cubes by the given bouding box area. The pointers in the vector must be explicitly deleted.
        /// </summary>
        /// <param name="box">The area</param>
        /// <param name="dest">The vector to store the return values</param>
        /// <returns>dest</returns>
        std::vector<AABBox*> getCubes(const AABBox& box, std::vector<AABBox*>& dest);

        bool setBlock(const Block* block, int x, int y, int z);
        Block* getBlock(int x, int y, int z);
        int getWidth() const;
        int getHeight() const;
        int getDepth() const;
    };
}
