#include "stdafx.h"
#include "mcpp/math/intersection.h"

bool mcpp::intersection::intersectRayAab(
    float originX, float originY, float originZ, float dirX, float dirY, float dirZ,
    float minX, float minY, float minZ, float maxX, float maxY, float maxZ, float* _near, float* _far) {
    float invDirX = 1.0f / dirX, invDirY = 1.0f / dirY, invDirZ = 1.0f / dirZ;
    float tNear, tFar, tymin, tymax, tzmin, tzmax;
    if (invDirX >= 0.0f) {
        tNear = (minX - originX) * invDirX;
        tFar = (maxX - originX) * invDirX;
    }
    else {
        tNear = (maxX - originX) * invDirX;
        tFar = (minX - originX) * invDirX;
    }
    if (invDirY >= 0.0f) {
        tymin = (minY - originY) * invDirY;
        tymax = (maxY - originY) * invDirY;
    }
    else {
        tymin = (maxY - originY) * invDirY;
        tymax = (minY - originY) * invDirY;
    }
    if (tNear > tymax || tymin > tFar)
        return false;
    if (invDirZ >= 0.0f) {
        tzmin = (minZ - originZ) * invDirZ;
        tzmax = (maxZ - originZ) * invDirZ;
    }
    else {
        tzmin = (maxZ - originZ) * invDirZ;
        tzmax = (minZ - originZ) * invDirZ;
    }
    if (tNear > tzmax || tzmin > tFar)
        return false;
    tNear = tymin > tNear || isnan(tNear) ? tymin : tNear;
    tFar = tymax < tFar || isnan(tFar) ? tymax : tFar;
    tNear = tzmin > tNear ? tzmin : tNear;
    tFar = tzmax < tFar ? tzmax : tFar;
    if (tNear < tFar && tFar >= 0.0f) {
        if (_near != nullptr) { *_near = tNear; }
        if (_far != nullptr) { *_far = tFar; }
        return true;
    }
    return false;
}
