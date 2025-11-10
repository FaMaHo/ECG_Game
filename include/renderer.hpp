#pragma once
#include "spacecraft.hpp"
#include "alien.hpp"
#include "plasma.hpp"
#include "particle.hpp"
#include "game_manager.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "mothership.hpp"

class Renderer {
private:
    GLuint shaderProgram;
    GLuint VAO, VBO;
    std::vector<float> vertices;

    void drawLine(Vec2 start, Vec2 end, Color color);
    void drawCircle(Vec2 center, float radius, Color color);
    void drawRectangle(Vec2 center, Vec2 size, float rotation, Color color);
    void drawTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Color color);

    // Visual UI elements (no text!)
    void drawGameOverScreen(GameState state, int wave, int score);
    void drawMenuScreen(float pulseTime);

public:
    Renderer();

    bool initialize();
    void beginFrame();
    void drawStarfield();
    void drawSpacecraft(const Spacecraft& ship);
    void drawAlien(const Alien& alien);
    void drawPlasma(const Plasma& plasma);
    void drawParticle(const Particle& particle);
    void drawMothership(const Mothership& mothership);
    void drawShieldBar(const Spacecraft& ship);
    void drawAmmoCounter(const Spacecraft& ship);
    void drawUI(const GameManager& game);
    void cleanup();
};