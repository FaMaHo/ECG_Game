#include "../include/alien.hpp"
#include <algorithm>

Alien::Alien(Vec2 pos, AlienType t, int wave)
    : position(pos), velocity(0, 0), type(t), active(true),
    spawnAnimation(0), animationTime(0) {


    float waveSpeedMultiplier = 1.0f + (wave - 1) * 0.08f;

    switch (type) {
    case AlienType::SCOUT:
        health = 35;
        speed = ALIEN_SPEED * 1.5f * waveSpeedMultiplier;
        break;
    case AlienType::HUNTER:
        health = 25;
        speed = ALIEN_SPEED * 2.0f * waveSpeedMultiplier;
        break;
    case AlienType::BRUTE:
        health = 70;
        speed = ALIEN_SPEED * 1.0f * waveSpeedMultiplier;
        break;
    }

    switch(type) {
        case AlienType::SCOUT: health = 35; speed = ALIEN_SPEED * 6.0f; break;
        case AlienType::HUNTER: health = 25; speed = ALIEN_SPEED * 8.0f; break;
        case AlienType::BRUTE: health = 70; speed = ALIEN_SPEED * 5.0f; break;
    }

    // ANIMATION: Initialize tentacles with different angles
    int numTentacles = (type == AlienType::BRUTE) ? 6 : 4;
    for (int i = 0; i < numTentacles; i++) {
        float angle = (2.0f * PI * i / numTentacles) + PI * 0.5f;
        float length = ALIEN_RADIUS * 0.8f;
        Tentacle tent(angle, length);
        tent.phase = i * PI * 0.5f; // Different phase for each tentacle
        tentacles.push_back(tent);
    }
}

void Alien::update(float deltaTime, Vec2 playerPos) {
    if (spawnAnimation < 1.0f) {
        spawnAnimation += deltaTime * 3.0f;
        spawnAnimation = std::min(1.0f, spawnAnimation);
    }

    // ANIMATION: Update tentacle animation time
    animationTime += deltaTime;

    Vec2 direction = (playerPos - position).normalized();
    float accel = 120.0f * deltaTime;
    velocity = velocity + direction * accel;
    if (velocity.length() > speed) {
        velocity = velocity.normalized() * speed;
    }
    position = position + velocity * deltaTime;
}

void Alien::takeDamage(float damage) {
    health -= damage;
    if (health <= 0) active = false;
}

float Alien::getSize() const {
    return ALIEN_RADIUS * spawnAnimation;
}