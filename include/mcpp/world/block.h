#pragma once
#include "mcpp/math/math_util.h"
#include "mcpp/tessellator.h"

namespace mcpp {
    class AABBox;
    class World;

    namespace Blocks {
        using T_blockId = unsigned char;
    }

    class Block {
    protected:
        const Blocks::T_blockId id;
    public:
        Block(Blocks::T_blockId id);
        virtual ~Block() = default;

        virtual unsigned char getTexture(Direction face);

        virtual AABBox& getCollisionShape(AABBox& box);

        virtual bool hasSideTransparency(Direction face);

        bool shouldRenderFace(World* world, Direction face, int x, int y, int z);
        virtual void renderFace(Tessellator& t, World* world, Direction face, int x, int y, int z);
        virtual void render(Tessellator& t, World* world, int x, int y, int z);

        Blocks::T_blockId getId() const;
    };

    class AirBlock : public Block {
    public:
        AirBlock(Blocks::T_blockId id);

        virtual AABBox& getCollisionShape(AABBox& box) override;

        virtual bool hasSideTransparency(Direction face) override;

        virtual void render(Tessellator& t, World* world, int x, int y, int z) override;
    };

    class GrassBlock : public Block {
    public:
        GrassBlock(Blocks::T_blockId id);

        virtual unsigned char getTexture(Direction face) override;
    };

    namespace Blocks {
        static auto AIR = new AirBlock(0);
        static auto GRASS_BLOCK = new GrassBlock(1);
        static auto COBBLESTONE = new Block(2);
        static auto DIRT = new Block(3);

        static Block* blockIds[] = {
            AIR, GRASS_BLOCK, COBBLESTONE, DIRT
        };
    }
}
