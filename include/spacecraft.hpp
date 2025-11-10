#pragma once
#include "vec2.hpp"
#include "shield.hpp"
#include "constants.hpp"

class Spacecraft {
public:
    Vec2 position;
    Vec2 velocity;
    float rotation;
    ShieldSystem shield;
    float shootCooldown;
    int ammo;
    int maxAmmo;
    float thrusterPulse; 

    Spacecraft();

    void update(float deltaTime, Vec2 mousePos);
    bool canShoot() const;
    void shoot();
    void reload(int& score);
    void takeDamage(float damage);
    bool isAlive() const;
    float getThrusterIntensity() const;
};

