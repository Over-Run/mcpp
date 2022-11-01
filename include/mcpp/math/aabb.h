#pragma once

namespace mcpp {
    struct AABBox {
    private:
        static constexpr float EPSILON = 0.0f;
    public:
        bool isInvalid;
        float minX, minY, minZ;
        float maxX, maxY, maxZ;

        AABBox& set(float minX, float minY, float minZ,
            float maxX, float maxY, float maxZ);

        AABBox& expand(float x, float y, float z, AABBox& dest) const;
        AABBox& grow(float x, float y, float z, AABBox& dest) const;

        /// Clip the X collide.
        ///
        /// @param c The moving object.
        float clipXCollide(const AABBox& c, float x) const;
        float clipYCollide(const AABBox& c, float y) const;
        float clipZCollide(const AABBox& c, float z) const;

        bool intersects(const AABBox& c) const;
        void move(float x, float y, float z);
    };
}
