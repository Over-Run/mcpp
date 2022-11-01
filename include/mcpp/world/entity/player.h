#pragma once
#include "entity.h"
#include "mcpp/vector.h"

namespace mcpp {
    class Player : public Entity {
    public:
        Player(World* world);

        virtual void tick() override;
    };
}
