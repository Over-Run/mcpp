#pragma once
#include "entity.h"

namespace mcpp {
    class PlayerEntity : public Entity {
    public:
        PlayerEntity(World* world);

        virtual void tick() override;
    };
}
