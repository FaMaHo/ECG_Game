#pragma once
#include "vec2.hpp"
#include "color.hpp"
#include "constants.hpp"

class Mothership {
public:
    Vec2 position;
    Vec2 velocity;
    float size;
    bool active;
    float animationTime;
    float spawnTimer;
    float spawnInterval;
    int aliensToSpawn;

    Mothership(Vec2 pos, int alienCount);

    void update(float deltaTime);
    bool shouldSpawnAlien();
    Vec2 getSpawnPosition();
    bool hasFinishedSpawning() const;
};