#pragma once
#include "vec2.hpp"
#include "color.hpp"

class Particle {
public:
    Vec2 position;
    Vec2 velocity;
    float lifetime;
    float maxLifetime;
    Color color;

    Particle(Vec2 pos, Vec2 vel, float life, Color col);

    void update(float deltaTime);
    bool isAlive() const;
    float getAlpha() const;
};
