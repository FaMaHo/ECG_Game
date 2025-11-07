#include "../include/particle.hpp"

Particle::Particle(Vec2 pos, Vec2 vel, float life, Color col)
    : position(pos), velocity(vel), lifetime(life), maxLifetime(life), color(col) {}

void Particle::update(float deltaTime) {
    position = position + velocity * deltaTime;
    lifetime -= deltaTime;
}

bool Particle::isAlive() const {
    return lifetime > 0;
}

float Particle::getAlpha() const {
    return (lifetime / maxLifetime) * color.a;
}
