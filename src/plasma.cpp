#include "../include/plasma.hpp"

Plasma::Plasma(Vec2 pos, Vec2 vel)
    : position(pos), velocity(vel), active(true), lifetime(3.0f) {}

void Plasma::update(float deltaTime) {
    position = position + velocity * deltaTime;
    lifetime -= deltaTime;
    if (position.x < 0 || position.x > WINDOW_WIDTH ||
        position.y < 0 || position.y > WINDOW_HEIGHT || lifetime <= 0) {
        active = false;
    }
}
