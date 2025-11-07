#include "../include/spacecraft.hpp"
#include <algorithm>
#include <cmath>

Spacecraft::Spacecraft()
    : position(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
      velocity(0, 0), rotation(0), shootCooldown(0),
      ammo(30), maxAmmo(30), thrusterPulse(0) {}

void Spacecraft::update(float deltaTime, Vec2 mousePos) {
    position = position + velocity * deltaTime;
    position.x = std::max(SPACECRAFT_RADIUS, std::min(position.x, WINDOW_WIDTH - SPACECRAFT_RADIUS));
    position.y = std::max(SPACECRAFT_RADIUS, std::min(position.y, WINDOW_HEIGHT - SPACECRAFT_RADIUS));

    Vec2 direction = mousePos - position;
    rotation = std::atan2(direction.y, direction.x);

    if (shootCooldown > 0) shootCooldown -= deltaTime;

    shield.update(deltaTime);

    // ANIMATION: Update thruster pulse based on movement
    float speed = velocity.length();
    if (speed > 10.0f) {
        thrusterPulse += deltaTime * 8.0f;
    } else {
        thrusterPulse += deltaTime * 2.0f;
    }
}

bool Spacecraft::canShoot() const {
    return shootCooldown <= 0 && ammo > 0;
}

void Spacecraft::shoot() {
    shootCooldown = SHOOT_COOLDOWN;
    ammo--;
}

void Spacecraft::reload() {
    ammo = maxAmmo;
}

void Spacecraft::takeDamage(float damage) {
    shield.absorbDamage(damage);
}

bool Spacecraft::isAlive() const {
    return shield.getCurrent() > 0;
}

float Spacecraft::getThrusterIntensity() const {
    return 0.6f + 0.4f * std::sin(thrusterPulse);
}
