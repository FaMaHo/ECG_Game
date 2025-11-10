#pragma once
#include "vec2.hpp"
#include "tentacle.hpp"
#include "constants.hpp"
#include <vector>

enum class AlienType { SCOUT, HUNTER, BRUTE };

class Alien {
public:
    Vec2 position;
    Vec2 velocity;
    AlienType type;
    float health;
    float speed;
    bool active;
    float spawnAnimation;
    float animationTime; 
    std::vector<Tentacle> tentacles;

    Alien(Vec2 pos, AlienType t = AlienType::SCOUT, int wave = 1);

    void update(float deltaTime, Vec2 playerPos);
    void takeDamage(float damage);
    float getSize() const;
};
