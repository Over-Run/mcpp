#pragma once
#include "aabb.h"
#include "vector.h"

namespace mcpp {
    class RayAabIntersection {
    private:
        float originX, originY, originZ;
        float dirX, dirY, dirZ;

        /* Needed for ray slope intersection method */
        float c_xy, c_yx, c_zy, c_yz, c_xz, c_zx;
        float s_xy, s_yx, s_zy, s_yz, s_xz, s_zx;
        char classification;

        inline static constexpr int signum(float f) {
            return (f == 0.0f || isnan(f)) ? 0 : (f < 0.0f ? -1 : 1);
        }

        void precomputeSlope();

        bool MMM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool OMM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool PMM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool MOM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool OOM(float minX, float minY, float minZ, float maxX, float maxY) const;
        bool POM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool MPM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool OPM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool PPM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool MMO(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool OMO(float minX, float minY, float minZ, float maxX, float maxZ) const;
        bool PMO(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool MOO(float minX, float minY, float minZ, float maxY, float maxZ) const;
        bool POO(float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool MPO(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool OPO(float minX, float minZ, float maxX, float maxY, float maxZ) const;
        bool PPO(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool MMP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool OMP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool PMP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool MOP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool OOP(float minX, float minY, float maxX, float maxY, float maxZ) const;
        bool POP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool MPP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool OPP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
        bool PPP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
    public:
        RayAabIntersection() = default;
        RayAabIntersection(float originX, float originY, float originZ, float dirX, float dirY, float dirZ);
        RayAabIntersection(const Vector3f& origin, const Vector3f& dir);

        void set(float originX, float originY, float originZ, float dirX, float dirY, float dirZ);
        bool test(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
        bool test(const AABBox& box);
    };
}
