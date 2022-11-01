#include "stdafx.h"
#include "mcpp/math/aabb.h"

mcpp::AABBox& mcpp::AABBox::set(float _minX, float _minY, float _minZ,
    float _maxX, float _maxY, float _maxZ) {
    minX = _minX;
    minY = _minY;
    minZ = _minZ;
    maxX = _maxX;
    maxY = _maxY;
    maxZ = _maxZ;
    return *this;
}

mcpp::AABBox& mcpp::AABBox::expand(float x, float y, float z, AABBox& dest) const {
    float _x0 = minX;
    float _y0 = minY;
    float _z0 = minZ;
    float _x1 = maxX;
    float _y1 = maxY;
    float _z1 = maxZ;
    if (x < 0) {
        _x0 += x;
    }
    if (x > 0) {
        _x1 += x;
    }
    if (y < 0) {
        _y0 += y;
    }
    if (y > 0) {
        _y1 += y;
    }
    if (z < 0) {
        _z0 += z;
    }
    if (z > 0) {
        _z1 += z;
    }
    dest.minX = _x0;
    dest.minY = _y0;
    dest.minZ = _z0;
    dest.maxX = _x1;
    dest.maxY = _y1;
    dest.maxZ = _z1;
    return dest;
}

mcpp::AABBox& mcpp::AABBox::grow(float x, float y, float z, AABBox& dest) const {
    float _x0 = minX - x;
    float _y0 = minY - y;
    float _z0 = minZ - z;
    float _x1 = maxX + x;
    float _y1 = maxY + y;
    float _z1 = maxZ + z;
    dest.minX = _x0;
    dest.minY = _y0;
    dest.minZ = _z0;
    dest.maxX = _x1;
    dest.maxY = _y1;
    dest.maxZ = _z1;
    return dest;
}

float mcpp::AABBox::clipXCollide(const AABBox& c, float x) const {
    if (c.maxY <= minY || c.minY >= maxY || c.maxZ <= minZ || c.minZ >= maxZ) {
        return x;
    }
    float max;
    if (x > 0 && c.maxX <= minX) {
        max = minX - c.maxX - EPSILON;
        if (max < x) {
            x = max;
        }
    }
    if (x < 0 && c.minX >= maxX) {
        max = maxX - c.minX + EPSILON;
        if (max > x) {
            x = max;
        }
    }
    return x;
}

float mcpp::AABBox::clipYCollide(const AABBox& c, float y) const {
    if (c.maxX <= minX || c.minX >= maxX || c.maxZ <= minZ || c.minZ >= maxZ) {
        return y;
    }
    float max;
    if (y > 0 && c.maxY <= minY) {
        max = minY - c.maxY - EPSILON;
        if (max < y) {
            y = max;
        }
    }
    if (y < 0 && c.minY >= maxY) {
        max = maxY - c.minY + EPSILON;
        if (max > y) {
            y = max;
        }
    }
    return y;
}

float mcpp::AABBox::clipZCollide(const AABBox& c, float z) const {
    if (c.maxX <= minX || c.minX >= maxX || c.maxY <= minY || c.minY >= maxY) {
        return z;
    }
    float max;
    if (z > 0 && c.maxZ <= minZ) {
        max = minZ - c.maxZ - EPSILON;
        if (max < z) {
            z = max;
        }
    }
    if (z < 0 && c.minZ >= maxZ) {
        max = maxZ - c.minZ + EPSILON;
        if (max > z) {
            z = max;
        }
    }
    return z;
}

bool mcpp::AABBox::intersects(const AABBox& c) const {
    return c.maxX > minX && c.minX < maxX
        && c.maxY > minY && c.minY < maxY
        && c.maxZ > minZ && c.minZ < maxZ;
}

void mcpp::AABBox::move(float x, float y, float z) {
    minX += x;
    minY += y;
    minZ += z;
    maxX += x;
    maxY += y;
    maxZ += z;
}
