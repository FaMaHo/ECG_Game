#pragma once
#include "vec2.hpp"
#include "constants.hpp"

class Plasma {
public:
    Vec2 position;
    Vec2 velocity;
    bool active;
    float lifetime;

    Plasma(Vec2 pos, Vec2 vel);
    void update(float deltaTime);
};
