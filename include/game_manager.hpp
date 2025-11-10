#pragma once
#include "spacecraft.hpp"
#include "alien.hpp"
#include "plasma.hpp"
#include "particle.hpp"
#include "constants.hpp"
#include "mothership.hpp"
#include <vector>

class GameManager {
public:
    Spacecraft spacecraft;
    std::vector<Plasma> plasmas;
    std::vector<Alien> aliens;
    std::vector<Particle> particles;
    std::vector<Mothership> motherships; 
    int wave;
    int score;
    int killCount;
    bool waveActive;
    GameState gameState;
    float stateTimer;

    GameManager();

    void reset();
    void startWave();
    void update(float deltaTime, Vec2 mousePos, bool shooting);
    void checkCollisions();

private:
    Vec2 getSpawnPosition();
    void createAlienExplosion(Vec2 pos, Color baseColor);
    void createPlasmaFlash(Vec2 pos);
    void createShieldImpact(Vec2 pos);
    void createDeathExplosion();
};