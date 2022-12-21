#pragma once
#include "stdafx.h"

namespace mcpp {
    struct TextureRegion {
        float u0, v0, u1, v1;
    };

    namespace texture {
        constexpr auto& TERRAIN_ATLAS = "texture/terrain.png";

        void atlasUV(int x, int y, int w, int h,
            TextureRegion& region,
            int maxW = 256, int maxH = 256);

        void bind2D(unsigned int unit, unsigned int texture);

        void putTexture2D(std::string_view name, unsigned int texture);
        void findTexture2D(std::string_view name, unsigned int* texture);

        void deleteTextures2D();
    }
}
