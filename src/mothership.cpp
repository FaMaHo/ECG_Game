#include "../include/mothership.hpp"
#include <cmath>

Mothership::Mothership(Vec2 pos, int alienCount)
    : position(pos), velocity(60, 0), size(35.0f), active(true),  // Smaller size (35 instead of 60)
    animationTime(0), spawnTimer(0), spawnInterval(1.5f), aliensToSpawn(alienCount) {
}

void Mothership::update(float deltaTime) {
    animationTime += deltaTime;

    // Move horizontally with slight vertical bobbing
    velocity.x = 60.0f;  // Constant horizontal speed
    velocity.y = std::sin(animationTime * 2.0f) * 15.0f;  // Small vertical bobbing

    position = position + velocity * deltaTime;

    // Wrap around horizontally
    if (position.x > WINDOW_WIDTH + 100) {
        position.x = -100;
    }

    // Keep in UPPER portion of screen (high Y values = top of screen)
    if (position.y > WINDOW_HEIGHT - 40) position.y = WINDOW_HEIGHT - 40;  // Stay below top edge
    if (position.y < WINDOW_HEIGHT - 150) position.y = WINDOW_HEIGHT - 150;  // Don't go too low

    // Update spawn timer
    if (aliensToSpawn > 0) {
        spawnTimer += deltaTime;
    }
}

bool Mothership::shouldSpawnAlien() {
    if (aliensToSpawn > 0 && spawnTimer >= spawnInterval) {
        spawnTimer = 0;
        aliensToSpawn--;
        return true;
    }
    return false;
}

Vec2 Mothership::getSpawnPosition() {
    // Spawn from bottom of mothership
    return Vec2(position.x, position.y - size * 0.8f);
}

bool Mothership::hasFinishedSpawning() const {
    return aliensToSpawn <= 0;
}