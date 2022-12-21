#include "stdafx.h"
#include "mcpp/math/rayaab_intersection.h"

mcpp::RayAabIntersection::RayAabIntersection(float originX, float originY, float originZ, float dirX, float dirY, float dirZ) {
    set(originX, originY, originZ, dirX, dirY, dirZ);
}

mcpp::RayAabIntersection::RayAabIntersection(const Vector3f& origin, const Vector3f& dir) :
    RayAabIntersection(origin.x, origin.y, origin.z, dir.x, dir.y, dir.z) {
}

void mcpp::RayAabIntersection::set(float _originX, float _originY, float _originZ, float _dirX, float _dirY, float _dirZ) {
    originX = _originX;
    originY = _originY;
    originZ = _originZ;
    dirX = _dirX;
    dirY = _dirY;
    dirZ = _dirZ;
    precomputeSlope();
}

bool mcpp::RayAabIntersection::test(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
    // tableswitch with dense and consecutive cases (will be a simple jump based on the switch argument)
    switch (classification) {
    case 0: // 0b000000: // MMM
        return MMM(minX, minY, minZ, maxX, maxY, maxZ);
    case 1: // 0b000001: // OMM
        return OMM(minX, minY, minZ, maxX, maxY, maxZ);
    case 2: // 0b000010: // PMM
        return PMM(minX, minY, minZ, maxX, maxY, maxZ);
    case 3: // 0b000011: // not used
        return false;
    case 4: // 0b000100: // MOM 
        return MOM(minX, minY, minZ, maxX, maxY, maxZ);
    case 5: // 0b000101: // OOM
        return OOM(minX, minY, minZ, maxX, maxY);
    case 6: // 0b000110: // POM
        return POM(minX, minY, minZ, maxX, maxY, maxZ);
    case 7: // 0b000111: // not used
        return false;
    case 8: // 0b001000: // MPM
        return MPM(minX, minY, minZ, maxX, maxY, maxZ);
    case 9: // 0b001001: // OPM
        return OPM(minX, minY, minZ, maxX, maxY, maxZ);
    case 10: // 0b001010: // PPM
        return PPM(minX, minY, minZ, maxX, maxY, maxZ);
    case 11: // 0b001011: // not used
    case 12: // 0b001100: // not used
    case 13: // 0b001101: // not used
    case 14: // 0b001110: // not used
    case 15: // 0b001111: // not used
        return false;
    case 16: // 0b010000: // MMO
        return MMO(minX, minY, minZ, maxX, maxY, maxZ);
    case 17: // 0b010001: // OMO
        return OMO(minX, minY, minZ, maxX, maxZ);
    case 18: // 0b010010: // PMO
        return PMO(minX, minY, minZ, maxX, maxY, maxZ);
    case 19: // 0b010011: // not used
        return false;
    case 20: // 0b010100: // MOO
        return MOO(minX, minY, minZ, maxY, maxZ);
    case 21: // 0b010101: // OOO
        return false; // <- degenerate case
    case 22: // 0b010110: // POO
        return POO(minY, minZ, maxX, maxY, maxZ);
    case 23: // 0b010111: // not used
        return false;
    case 24: // 0b011000: // MPO
        return MPO(minX, minY, minZ, maxX, maxY, maxZ);
    case 25: // 0b011001: // OPO
        return OPO(minX, minZ, maxX, maxY, maxZ);
    case 26: // 0b011010: // PPO
        return PPO(minX, minY, minZ, maxX, maxY, maxZ);
    case 27: // 0b011011: // not used
    case 28: // 0b011100: // not used
    case 29: // 0b011101: // not used
    case 30: // 0b011110: // not used
    case 31: // 0b011111: // not used
        return false;
    case 32: // 0b100000: // MMP
        return MMP(minX, minY, minZ, maxX, maxY, maxZ);
    case 33: // 0b100001: // OMP
        return OMP(minX, minY, minZ, maxX, maxY, maxZ);
    case 34: // 0b100010: // PMP
        return PMP(minX, minY, minZ, maxX, maxY, maxZ);
    case 35: // 0b100011: // not used
        return false;
    case 36: // 0b100100: // MOP
        return MOP(minX, minY, minZ, maxX, maxY, maxZ);
    case 37: // 0b100101: // OOP
        return OOP(minX, minY, maxX, maxY, maxZ);
    case 38: // 0b100110: // POP
        return POP(minX, minY, minZ, maxX, maxY, maxZ);
    case 39: // 0b100111: // not used
        return false;
    case 40: // 0b101000: // MPP
        return MPP(minX, minY, minZ, maxX, maxY, maxZ);
    case 41: // 0b101001: // OPP
        return OPP(minX, minY, minZ, maxX, maxY, maxZ);
    case 42: // 0b101010: // PPP
        return PPP(minX, minY, minZ, maxX, maxY, maxZ);
    default:
        return false;
    }
}

bool mcpp::RayAabIntersection::test(const AABBox& box) {
    return test(box.minX, box.minY, box.minZ, box.maxX, box.maxY, box.maxZ);
}

void mcpp::RayAabIntersection::precomputeSlope() {
    float invDirX = 1.0f / dirX;
    float invDirY = 1.0f / dirY;
    float invDirZ = 1.0f / dirZ;
    s_yx = dirX * invDirY;
    s_xy = dirY * invDirX;
    s_zy = dirY * invDirZ;
    s_yz = dirZ * invDirY;
    s_xz = dirZ * invDirX;
    s_zx = dirX * invDirZ;
    c_xy = originY - s_xy * originX;
    c_yx = originX - s_yx * originY;
    c_zy = originY - s_zy * originZ;
    c_yz = originZ - s_yz * originY;
    c_xz = originZ - s_xz * originX; // <- original paper had a bug here. It switched originZ/originX
    c_zx = originX - s_zx * originZ; // <- original paper had a bug here. It switched originZ/originX
    int sgnX = signum(dirX);
    int sgnY = signum(dirY);
    int sgnZ = signum(dirZ);
    classification = (char)((sgnZ + 1) << 4 | (sgnY + 1) << 2 | (sgnX + 1));
}

bool mcpp::RayAabIntersection::MMM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX >= minX && originY >= minY && originZ >= minZ
        && s_xy * minX - maxY + c_xy <= 0.0f
        && s_yx * minY - maxX + c_yx <= 0.0f
        && s_zy * minZ - maxY + c_zy <= 0.0f
        && s_yz * minY - maxZ + c_yz <= 0.0f
        && s_xz * minX - maxZ + c_xz <= 0.0f
        && s_zx * minZ - maxX + c_zx <= 0.0f;
}
bool mcpp::RayAabIntersection::OMM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX >= minX && originX <= maxX && originY >= minY && originZ >= minZ
        && s_zy * minZ - maxY + c_zy <= 0.0f
        && s_yz * minY - maxZ + c_yz <= 0.0f;
}
bool mcpp::RayAabIntersection::PMM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX <= maxX && originY >= minY && originZ >= minZ
        && s_xy * maxX - maxY + c_xy <= 0.0f
        && s_yx * minY - minX + c_yx >= 0.0f
        && s_zy * minZ - maxY + c_zy <= 0.0f
        && s_yz * minY - maxZ + c_yz <= 0.0f
        && s_xz * maxX - maxZ + c_xz <= 0.0f
        && s_zx * minZ - minX + c_zx >= 0.0f;
}
bool mcpp::RayAabIntersection::MOM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originY >= minY && originY <= maxY && originX >= minX && originZ >= minZ
        && s_xz * minX - maxZ + c_xz <= 0.0f
        && s_zx * minZ - maxX + c_zx <= 0.0f;
}
bool mcpp::RayAabIntersection::OOM(float minX, float minY, float minZ, float maxX, float maxY) const {
    return originZ >= minZ && originX >= minX && originX <= maxX && originY >= minY && originY <= maxY;
}
bool mcpp::RayAabIntersection::POM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originY >= minY && originY <= maxY && originX <= maxX && originZ >= minZ
        && s_xz * maxX - maxZ + c_xz <= 0.0f
        && s_zx * minZ - minX + c_zx >= 0.0f;
}
bool mcpp::RayAabIntersection::MPM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX >= minX && originY <= maxY && originZ >= minZ
        && s_xy * minX - minY + c_xy >= 0.0f
        && s_yx * maxY - maxX + c_yx <= 0.0f
        && s_zy * minZ - minY + c_zy >= 0.0f
        && s_yz * maxY - maxZ + c_yz <= 0.0f
        && s_xz * minX - maxZ + c_xz <= 0.0f
        && s_zx * minZ - maxX + c_zx <= 0.0f;
}
bool mcpp::RayAabIntersection::OPM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX >= minX && originX <= maxX && originY <= maxY && originZ >= minZ
        && s_zy * minZ - minY + c_zy >= 0.0f
        && s_yz * maxY - maxZ + c_yz <= 0.0f;
}
bool mcpp::RayAabIntersection::PPM(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX <= maxX && originY <= maxY && originZ >= minZ
        && s_xy * maxX - minY + c_xy >= 0.0f
        && s_yx * maxY - minX + c_yx >= 0.0f
        && s_zy * minZ - minY + c_zy >= 0.0f
        && s_yz * maxY - maxZ + c_yz <= 0.0f
        && s_xz * maxX - maxZ + c_xz <= 0.0f
        && s_zx * minZ - minX + c_zx >= 0.0f;
}
bool mcpp::RayAabIntersection::MMO(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originZ >= minZ && originZ <= maxZ && originX >= minX && originY >= minY
        && s_xy * minX - maxY + c_xy <= 0.0f
        && s_yx * minY - maxX + c_yx <= 0.0f;
}
bool mcpp::RayAabIntersection::OMO(float minX, float minY, float minZ, float maxX, float maxZ) const {
    return originY >= minY && originX >= minX && originX <= maxX && originZ >= minZ && originZ <= maxZ;
}
bool mcpp::RayAabIntersection::PMO(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originZ >= minZ && originZ <= maxZ && originX <= maxX && originY >= minY
        && s_xy * maxX - maxY + c_xy <= 0.0f
        && s_yx * minY - minX + c_yx >= 0.0f;
}
bool mcpp::RayAabIntersection::MOO(float minX, float minY, float minZ, float maxY, float maxZ) const {
    return originX >= minX && originY >= minY && originY <= maxY && originZ >= minZ && originZ <= maxZ;
}
bool mcpp::RayAabIntersection::POO(float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX <= maxX && originY >= minY && originY <= maxY && originZ >= minZ && originZ <= maxZ;
}
bool mcpp::RayAabIntersection::MPO(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originZ >= minZ && originZ <= maxZ && originX >= minX && originY <= maxY
        && s_xy * minX - minY + c_xy >= 0.0f
        && s_yx * maxY - maxX + c_yx <= 0.0f;
}
bool mcpp::RayAabIntersection::OPO(float minX, float minZ, float maxX, float maxY, float maxZ) const {
    return originY <= maxY && originX >= minX && originX <= maxX && originZ >= minZ && originZ <= maxZ;
}
bool mcpp::RayAabIntersection::PPO(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originZ >= minZ && originZ <= maxZ && originX <= maxX && originY <= maxY
        && s_xy * maxX - minY + c_xy >= 0.0f
        && s_yx * maxY - minX + c_yx >= 0.0f;
}
bool mcpp::RayAabIntersection::MMP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX >= minX && originY >= minY && originZ <= maxZ
        && s_xy * minX - maxY + c_xy <= 0.0f
        && s_yx * minY - maxX + c_yx <= 0.0f
        && s_zy * maxZ - maxY + c_zy <= 0.0f
        && s_yz * minY - minZ + c_yz >= 0.0f
        && s_xz * minX - minZ + c_xz >= 0.0f
        && s_zx * maxZ - maxX + c_zx <= 0.0f;
}
bool mcpp::RayAabIntersection::OMP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX >= minX && originX <= maxX && originY >= minY && originZ <= maxZ
        && s_zy * maxZ - maxY + c_zy <= 0.0f
        && s_yz * minY - minZ + c_yz >= 0.0f;
}
bool mcpp::RayAabIntersection::PMP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX <= maxX && originY >= minY && originZ <= maxZ
        && s_xy * maxX - maxY + c_xy <= 0.0f
        && s_yx * minY - minX + c_yx >= 0.0f
        && s_zy * maxZ - maxY + c_zy <= 0.0f
        && s_yz * minY - minZ + c_yz >= 0.0f
        && s_xz * maxX - minZ + c_xz >= 0.0f
        && s_zx * maxZ - minX + c_zx >= 0.0f;
}
bool mcpp::RayAabIntersection::MOP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originY >= minY && originY <= maxY && originX >= minX && originZ <= maxZ
        && s_xz * minX - minZ + c_xz >= 0.0f
        && s_zx * maxZ - maxX + c_zx <= 0.0f;
}
bool mcpp::RayAabIntersection::OOP(float minX, float minY, float maxX, float maxY, float maxZ) const {
    return originZ <= maxZ && originX >= minX && originX <= maxX && originY >= minY && originY <= maxY;
}
bool mcpp::RayAabIntersection::POP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originY >= minY && originY <= maxY && originX <= maxX && originZ <= maxZ
        && s_xz * maxX - minZ + c_xz >= 0.0f
        && s_zx * maxZ - minX + c_zx <= 0.0f;
}
bool mcpp::RayAabIntersection::MPP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX >= minX && originY <= maxY && originZ <= maxZ
        && s_xy * minX - minY + c_xy >= 0.0f
        && s_yx * maxY - maxX + c_yx <= 0.0f
        && s_zy * maxZ - minY + c_zy >= 0.0f
        && s_yz * maxY - minZ + c_yz >= 0.0f
        && s_xz * minX - minZ + c_xz >= 0.0f
        && s_zx * maxZ - maxX + c_zx <= 0.0f;
}
bool mcpp::RayAabIntersection::OPP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX >= minX && originX <= maxX && originY <= maxY && originZ <= maxZ
        && s_zy * maxZ - minY + c_zy <= 0.0f
        && s_yz * maxY - minZ + c_yz <= 0.0f;
}
bool mcpp::RayAabIntersection::PPP(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const {
    return originX <= maxX && originY <= maxY && originZ <= maxZ
        && s_xy * maxX - minY + c_xy >= 0.0f
        && s_yx * maxY - minX + c_yx >= 0.0f
        && s_zy * maxZ - minY + c_zy >= 0.0f
        && s_yz * maxY - minZ + c_yz >= 0.0f
        && s_xz * maxX - minZ + c_xz >= 0.0f
        && s_zx * maxZ - minX + c_zx >= 0.0f;
}
