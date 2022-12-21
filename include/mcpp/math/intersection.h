#pragma once

namespace mcpp {
    namespace intersection {
        bool intersectRayAab(float originX, float originY, float originZ, float dirX, float dirY, float dirZ,
            float minX, float minY, float minZ, float maxX, float maxY, float maxZ, float* _near, float* _far);
    }
}
