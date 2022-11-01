#pragma once
#include "entity.h"

namespace mcpp {
    class Player : public Entity {
    public:
        Player(World* world);

        virtual void tick() override;
    };
}
