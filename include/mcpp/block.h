#pragma once
#include "mcpp/math_util.h"
#include "mcpp/tessellator.h"

namespace mcpp {
    class block {
    public:
        virtual unsigned char getTexture(direction face);
        virtual void renderFace(tessellator& t, direction face, int x, int y, int z);
        virtual void render(tessellator& t, int x, int y, int z);
    };

    class air_block : public block {
        virtual void render(tessellator& t, int x, int y, int z) override;
    };

    class grass_block : public block {
        virtual unsigned char getTexture(direction face) override;
    };

    namespace blocks {
        static auto AIR = air_block();
        static auto GRASS_BLOCK = grass_block();

        static block blockIds[256] = {
            AIR, GRASS_BLOCK
        };
    }
}
