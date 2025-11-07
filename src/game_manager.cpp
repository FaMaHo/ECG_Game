#include "../include/game_manager.hpp"
#include "../include/collision.hpp"
#include <cstdlib>
#include <iostream>
#include <algorithm>

GameManager::GameManager()
    : wave(0), score(0), killCount(0), waveActive(false),
      gameState(GameState::MENU), stateTimer(0) {}

void GameManager::reset() {
    spacecraft = Spacecraft();
    plasmas.clear();
    aliens.clear();
    particles.clear();
    wave = 0;
    score = 0;
    killCount = 0;
    waveActive = false;
    stateTimer = 0;
    gameState = GameState::PLAYING;
}

Vec2 GameManager::getSpawnPosition() {
    int side = rand() % 4;
    float x, y;

    switch(side) {
        case 0: x = rand() % WINDOW_WIDTH; y = -50; break;
        case 1: x = rand() % WINDOW_WIDTH; y = WINDOW_HEIGHT + 50; break;
        case 2: x = -50; y = rand() % WINDOW_HEIGHT; break;
        case 3: x = WINDOW_WIDTH + 50; y = rand() % WINDOW_HEIGHT; break;
    }

    return Vec2(x, y);
}

void GameManager::createAlienExplosion(Vec2 pos, Color baseColor) {
    for (int i = 0; i < 12; i++) {
        float angle = (rand() % 360) * PI / 180.0f;
        float speed = 50.0f + (rand() % 100);
        Vec2 vel(cos(angle) * speed, sin(angle) * speed);
        particles.push_back(Particle(pos, vel, 0.6f, baseColor));
    }
}

void GameManager::createPlasmaFlash(Vec2 pos) {
    for (int i = 0; i < 5; i++) {
        float angle = (rand() % 60 - 30) * PI / 180.0f;
        Vec2 vel(cos(angle) * 200, sin(angle) * 200);
        particles.push_back(Particle(pos, vel, 0.2f, Color(0.3f, 0.9f, 1.0f, 1.0f)));
    }
}

void GameManager::createShieldImpact(Vec2 pos) {
    for (int i = 0; i < 15; i++) {
        float angle = (rand() % 360) * PI / 180.0f;
        float speed = 80.0f + (rand() % 120);
        Vec2 vel(cos(angle) * speed, sin(angle) * speed);
        particles.push_back(Particle(pos, vel, 0.4f, Color(0.2f, 0.8f, 1.0f, 0.8f)));
    }
}

void GameManager::createDeathExplosion() {
    for (int i = 0; i < 40; i++) {
        float angle = (rand() % 360) * PI / 180.0f;
        float speed = 100.0f + (rand() % 250);
        Vec2 vel(cos(angle) * speed, sin(angle) * speed);
        Color col = (rand() % 2 == 0) ? Color(1.0f, 0.3f, 0.0f, 1.0f) : Color(0.2f, 0.6f, 1.0f, 1.0f);
        particles.push_back(Particle(spacecraft.position, vel, 1.5f, col));
    }
}

void GameManager::startWave() {
    wave++;
    waveActive = true;
    int alienCount = ALIENS_PER_WAVE + (wave - 1) * 2;

    std::cout << "Wave " << wave << " - " << alienCount << " aliens incoming!" << std::endl;

    for (int i = 0; i < alienCount; i++) {
        Vec2 spawnPos = getSpawnPosition();

        AlienType type = AlienType::SCOUT;
        if (wave > 2 && (rand() % 100) < 35) type = AlienType::HUNTER;
        if (wave > 4 && (rand() % 100) < 20) type = AlienType::BRUTE;

        aliens.push_back(Alien(spawnPos, type));
    }
}

void GameManager::update(float deltaTime, Vec2 mousePos, bool shooting) {
    stateTimer += deltaTime;

    if (gameState != GameState::PLAYING) {
        // Continue particle animations
        for (auto& particle : particles) {
            particle.update(deltaTime);
        }
        for (auto& alien : aliens) {
            if (alien.active) alien.update(deltaTime, spacecraft.position);
        }
        return;
    }

    spacecraft.update(deltaTime, mousePos);

    if (shooting && spacecraft.canShoot() && spacecraft.isAlive()) {
        Vec2 direction = (mousePos - spacecraft.position).normalized();
        Vec2 plasmaVel = direction * PLASMA_SPEED * 60.0f;
        Vec2 muzzlePos = spacecraft.position + direction * SPACECRAFT_RADIUS;
        plasmas.push_back(Plasma(muzzlePos, plasmaVel));
        createPlasmaFlash(muzzlePos);
        spacecraft.shoot();
    }

    // Check for out of ammo game over
    if (spacecraft.ammo == 0 && plasmas.empty() && !aliens.empty() && waveActive) {
        bool alienNearby = false;
        for (auto& alien : aliens) {
            if (alien.active) {
                float dist = (alien.position - spacecraft.position).length();
                if (dist < 150.0f) {
                    alienNearby = true;
                    break;
                }
            }
        }
        if (alienNearby) {
            gameState = GameState::GAME_OVER_AMMO;
            createDeathExplosion();
            std::cout << "GAME OVER - Out of Ammo!" << std::endl;
        }
    }

    for (auto& plasma : plasmas) {
        if (plasma.active) plasma.update(deltaTime);
    }

    for (auto& alien : aliens) {
        if (alien.active) alien.update(deltaTime, spacecraft.position);
    }

    for (auto& particle : particles) {
        particle.update(deltaTime);
    }

    checkCollisions();

    plasmas.erase(std::remove_if(plasmas.begin(), plasmas.end(),
        [](const Plasma& p) { return !p.active; }), plasmas.end());

    aliens.erase(std::remove_if(aliens.begin(), aliens.end(),
        [](const Alien& a) { return !a.active; }), aliens.end());

    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const Particle& p) { return !p.isAlive(); }), particles.end());

    if (waveActive && aliens.empty()) {
        waveActive = false;
        score += wave * 100;
        std::cout << "Wave " << wave << " complete! Score: " << score << std::endl;
        spacecraft.reload();
    }
}

void GameManager::checkCollisions() {
    // Plasma-alien collisions
    for (auto& plasma : plasmas) {
        if (!plasma.active) continue;
        for (auto& alien : aliens) {
            if (!alien.active) continue;

            CollisionInfo collision = detectCollision(plasma.position, PLASMA_RADIUS,
                                                     alien.position, alien.getSize());

            if (collision.hasCollision) {
                plasma.active = false;
                alien.takeDamage(30);

                Color explosionColor;
                switch(alien.type) {
                    case AlienType::SCOUT: explosionColor = Color(0.3f, 0.9f, 0.3f, 1.0f); break;
                    case AlienType::HUNTER: explosionColor = Color(0.9f, 0.3f, 0.9f, 1.0f); break;
                    case AlienType::BRUTE: explosionColor = Color(0.9f, 0.2f, 0.2f, 1.0f); break;
                }

                createAlienExplosion(collision.contactPoint, explosionColor);

                if (!alien.active) {
                    score += 15;
                    killCount++;
                }
                break;
            }
        }
    }

    // Spacecraft-alien collisions
    if (!spacecraft.isAlive()) return;
    for (auto& alien : aliens) {
        if (!alien.active) continue;

        CollisionInfo collision = detectCollision(spacecraft.position, SPACECRAFT_RADIUS,
                                                 alien.position, alien.getSize());

        if (collision.hasCollision) {
            float damage = 0.8f + (collision.penetrationDepth * 0.1f);
            spacecraft.takeDamage(damage);

            Vec2 pushBack = collision.collisionNormal * -1.0f * collision.penetrationDepth;
            alien.position = alien.position + pushBack;

            createShieldImpact(collision.contactPoint);

            if (!spacecraft.isAlive()) {
                gameState = GameState::GAME_OVER_SHIELD;
                createDeathExplosion();
                std::cout << "GAME OVER - Shields Failed!" << std::endl;
            }
        }
    }
}
