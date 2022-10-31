#pragma once

namespace mcpp {
    namespace texture {
        void atlasUV(int x, int y, int w, int h,
            float* u0, float* v0, float* u1, float* v1,
            int maxW = 256, int maxH = 256);

        void bind2D(unsigned int unit, unsigned int texture);
    }
}
