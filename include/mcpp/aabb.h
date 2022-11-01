#pragma once

namespace mcpp {
    struct AABBox {
    private:
        static constexpr float EPSILON = 0.0f;
    public:
        float minX, minY, minZ;
        float maxX, maxY, maxZ;

        AABBox& expand(float x, float y, float z, AABBox& dest);
        AABBox& grow(float x, float y, float z, AABBox& dest);

        /// Clip the X collide.
        ///
        /// @param c The moving object.
        float clipXCollide(const AABBox& c, float x);
        float clipYCollide(const AABBox& c, float y);
        float clipZCollide(const AABBox& c, float z);

        bool intersects(const AABBox& c);
        void move(float x, float y, float z);
    };
}
