#pragma once
#include "stdafx.h"
#include "aabb.h"
#include "matrix.h"
#include "vector.h"

namespace mcpp {
    class FrustumIntersection {
    private:
        float nxX, nxY, nxZ, nxW;
        float pxX, pxY, pxZ, pxW;
        float nyX, nyY, nyZ, nyW;
        float pyX, pyY, pyZ, pyW;
        float nzX, nzY, nzZ, nzW;
        float pzX, pzY, pzZ, pzW;
        Vector4f planes[6];
    public:
        FrustumIntersection() :
            nxX(0.0f), nxY(0.0f), nxZ(0.0f), nxW(0.0f),
            pxX(0.0f), pxY(0.0f), pxZ(0.0f), pxW(0.0f),
            nyX(0.0f), nyY(0.0f), nyZ(0.0f), nyW(0.0f),
            pyX(0.0f), pyY(0.0f), pyZ(0.0f), pyW(0.0f),
            nzX(0.0f), nzY(0.0f), nzZ(0.0f), nzW(0.0f),
            pzX(0.0f), pzY(0.0f), pzZ(0.0f), pzW(0.0f) {
            for (size_t i = 0; i < 6; i++)
            {
                planes[i] = Vector4f();
            }
        }

        FrustumIntersection(const Matrix4f& m, bool allowTestSpheres = true) : FrustumIntersection() {
            set(m, allowTestSpheres);
        }

        FrustumIntersection& set(const Matrix4f& m, bool allowTestSpheres = true) {
            float invl;
            nxX = m.m03 + m.m00; nxY = m.m13 + m.m10; nxZ = m.m23 + m.m20; nxW = m.m33 + m.m30;
            if (allowTestSpheres) {
                invl = 1.0f / sqrt(nxX * nxX + nxY * nxY + nxZ * nxZ);
                nxX *= invl; nxY *= invl; nxZ *= invl; nxW *= invl;
            }
            planes[0].set(nxX, nxY, nxZ, nxW);
            pxX = m.m03 - m.m00; pxY = m.m13 - m.m10; pxZ = m.m23 - m.m20; pxW = m.m33 - m.m30;
            if (allowTestSpheres) {
                invl = 1.0f / sqrt(pxX * pxX + pxY * pxY + pxZ * pxZ);
                pxX *= invl; pxY *= invl; pxZ *= invl; pxW *= invl;
            }
            planes[1].set(pxX, pxY, pxZ, pxW);
            nyX = m.m03 + m.m01; nyY = m.m13 + m.m11; nyZ = m.m23 + m.m21; nyW = m.m33 + m.m31;
            if (allowTestSpheres) {
                invl = 1.0f / sqrt(nyX * nyX + nyY * nyY + nyZ * nyZ);
                nyX *= invl; nyY *= invl; nyZ *= invl; nyW *= invl;
            }
            planes[2].set(nyX, nyY, nyZ, nyW);
            pyX = m.m03 - m.m01; pyY = m.m13 - m.m11; pyZ = m.m23 - m.m21; pyW = m.m33 - m.m31;
            if (allowTestSpheres) {
                invl = 1.0f / sqrt(pyX * pyX + pyY * pyY + pyZ * pyZ);
                pyX *= invl; pyY *= invl; pyZ *= invl; pyW *= invl;
            }
            planes[3].set(pyX, pyY, pyZ, pyW);
            nzX = m.m03 + m.m02; nzY = m.m13 + m.m12; nzZ = m.m23 + m.m22; nzW = m.m33 + m.m32;
            if (allowTestSpheres) {
                invl = 1.0f / sqrt(nzX * nzX + nzY * nzY + nzZ * nzZ);
                nzX *= invl; nzY *= invl; nzZ *= invl; nzW *= invl;
            }
            planes[4].set(nzX, nzY, nzZ, nzW);
            pzX = m.m03 - m.m02; pzY = m.m13 - m.m12; pzZ = m.m23 - m.m22; pzW = m.m33 - m.m32;
            if (allowTestSpheres) {
                invl = 1.0f / sqrt(pzX * pzX + pzY * pzY + pzZ * pzZ);
                pzX *= invl; pzY *= invl; pzZ *= invl; pzW *= invl;
            }
            planes[5].set(pzX, pzY, pzZ, pzW);
            return *this;
        }

        bool testAab(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
            /*
             * This is an implementation of the "2.4 Basic intersection test" of the mentioned site.
             * It does not distinguish between partially inside and fully inside, though, so the test with the 'p' vertex is omitted.
             */
            return nxX * (nxX < 0 ? minX : maxX) + nxY * (nxY < 0 ? minY : maxY) + nxZ * (nxZ < 0 ? minZ : maxZ) >= -nxW &&
                pxX * (pxX < 0 ? minX : maxX) + pxY * (pxY < 0 ? minY : maxY) + pxZ * (pxZ < 0 ? minZ : maxZ) >= -pxW &&
                nyX * (nyX < 0 ? minX : maxX) + nyY * (nyY < 0 ? minY : maxY) + nyZ * (nyZ < 0 ? minZ : maxZ) >= -nyW &&
                pyX * (pyX < 0 ? minX : maxX) + pyY * (pyY < 0 ? minY : maxY) + pyZ * (pyZ < 0 ? minZ : maxZ) >= -pyW &&
                nzX * (nzX < 0 ? minX : maxX) + nzY * (nzY < 0 ? minY : maxY) + nzZ * (nzZ < 0 ? minZ : maxZ) >= -nzW &&
                pzX * (pzX < 0 ? minX : maxX) + pzY * (pzY < 0 ? minY : maxY) + pzZ * (pzZ < 0 ? minZ : maxZ) >= -pzW;
        }

        bool testAab(AABBox& box) const {
            return testAab(box.minX, box.minY, box.minZ, box.maxX, box.maxY, box.maxZ);
        }
    };
}
