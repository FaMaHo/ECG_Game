#include "../include/renderer.hpp"
#include "../shader.hpp"
#include <cstdlib>
#include <cmath>

Renderer::Renderer() : shaderProgram(0), VAO(0), VBO(0) {}

bool Renderer::initialize() {
    shaderProgram = LoadShaders("SimpleVertexShader.vertexshader",
                                "SimpleFragmentShader.fragmentshader");
    if (shaderProgram == 0) return false;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

void Renderer::beginFrame() {
    glClearColor(0.01f, 0.01f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
}

void Renderer::drawStarfield() {
    srand(12345);
    for (int i = 0; i < 100; i++) {
        float x = (rand() % WINDOW_WIDTH);
        float y = (rand() % WINDOW_HEIGHT);
        float brightness = 0.3f + (rand() % 100) / 150.0f;
        drawCircle(Vec2(x, y), 1.0f, Color(brightness, brightness, brightness, 0.8f));
    }
}

void Renderer::drawLine(Vec2 start, Vec2 end, Color color) {
    vertices.clear();
    float x1 = (start.x / WINDOW_WIDTH) * 2.0f - 1.0f;
    float y1 = (start.y / WINDOW_HEIGHT) * 2.0f - 1.0f;
    float x2 = (end.x / WINDOW_WIDTH) * 2.0f - 1.0f;
    float y2 = (end.y / WINDOW_HEIGHT) * 2.0f - 1.0f;

    vertices.push_back(x1); vertices.push_back(y1); vertices.push_back(0.0f);
    vertices.push_back(x2); vertices.push_back(y2); vertices.push_back(0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_DYNAMIC_DRAW);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
    GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glm::mat4 transform = glm::mat4(1.0f);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void Renderer::drawCircle(Vec2 center, float radius, Color color) {
    vertices.clear();
    for (int i = 0; i <= CIRCLE_SEGMENTS; i++) {
        float angle = 2.0f * PI * i / CIRCLE_SEGMENTS;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        float ndcX = (x / WINDOW_WIDTH) * 2.0f - 1.0f;
        float ndcY = (y / WINDOW_HEIGHT) * 2.0f - 1.0f;
        vertices.push_back(ndcX);
        vertices.push_back(ndcY);
        vertices.push_back(0.0f);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_DYNAMIC_DRAW);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
    GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glm::mat4 transform = glm::mat4(1.0f);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_SEGMENTS + 1);
    glBindVertexArray(0);
}

void Renderer::drawRectangle(Vec2 center, Vec2 size, float rotation, Color color) {
    vertices.clear();
    float halfW = size.x / 2.0f;
    float halfH = size.y / 2.0f;
    Vec2 corners[4] = {
        Vec2(-halfW, -halfH), Vec2(halfW, -halfH),
        Vec2(halfW, halfH), Vec2(-halfW, halfH)
    };

    for (int i = 0; i < 4; i++) {
        float x = corners[i].x * cos(rotation) - corners[i].y * sin(rotation);
        float y = corners[i].x * sin(rotation) + corners[i].y * cos(rotation);
        x += center.x;
        y += center.y;
        float ndcX = (x / WINDOW_WIDTH) * 2.0f - 1.0f;
        float ndcY = (y / WINDOW_HEIGHT) * 2.0f - 1.0f;
        vertices.push_back(ndcX);
        vertices.push_back(ndcY);
        vertices.push_back(0.0f);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_DYNAMIC_DRAW);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
    GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glm::mat4 transform = glm::mat4(1.0f);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

void Renderer::drawTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Color color) {
    vertices.clear();
    float x1 = (p1.x / WINDOW_WIDTH) * 2.0f - 1.0f;
    float y1 = (p1.y / WINDOW_HEIGHT) * 2.0f - 1.0f;
    float x2 = (p2.x / WINDOW_WIDTH) * 2.0f - 1.0f;
    float y2 = (p2.y / WINDOW_HEIGHT) * 2.0f - 1.0f;
    float x3 = (p3.x / WINDOW_WIDTH) * 2.0f - 1.0f;
    float y3 = (p3.y / WINDOW_HEIGHT) * 2.0f - 1.0f;

    vertices.push_back(x1); vertices.push_back(y1); vertices.push_back(0.0f);
    vertices.push_back(x2); vertices.push_back(y2); vertices.push_back(0.0f);
    vertices.push_back(x3); vertices.push_back(y3); vertices.push_back(0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_DYNAMIC_DRAW);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
    GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glm::mat4 transform = glm::mat4(1.0f);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

// Visual indicator: Play icon (triangle pointing right)
void Renderer::drawPlayIcon(Vec2 center, float size, Color color) {
    Vec2 p1 = center + Vec2(-size * 0.4f, -size * 0.6f);
    Vec2 p2 = center + Vec2(size * 0.7f, 0);
    Vec2 p3 = center + Vec2(-size * 0.4f, size * 0.6f);
    drawTriangle(p1, p2, p3, color);
}

// Visual wave indicator (circles around a number)
void Renderer::drawWaveIndicator(int wave) {
    Vec2 pos(WINDOW_WIDTH - 80, 60);

    // Draw concentric circles
    drawCircle(pos, 35, Color(1.0f, 0.3f, 0.3f, 0.3f));
    drawCircle(pos, 25, Color(1.0f, 0.4f, 0.4f, 0.5f));

    // Draw dots to represent wave number (max 10 dots)
    int dots = std::min(wave, 10);
    for (int i = 0; i < dots; i++) {
        float angle = (i * 2.0f * PI / 10.0f) - PI / 2.0f;
        Vec2 dotPos = pos + Vec2(cos(angle) * 20, sin(angle) * 20);
        drawCircle(dotPos, 3, Color(1.0f, 0.5f, 0.5f, 1.0f));
    }
}

// Visual alien count indicator
void Renderer::drawAlienCountIndicator(int count) {
    Vec2 pos(WINDOW_WIDTH - 80, 120);

    // Draw container
    drawCircle(pos, 30, Color(0.3f, 0.9f, 0.3f, 0.2f));

    // Draw alien icon (simplified)
    drawCircle(pos, 12, Color(0.3f, 0.9f, 0.3f, 0.8f));
    drawCircle(pos + Vec2(-4, 3), 3, Color(1.0f, 0.0f, 0.0f, 1.0f));
    drawCircle(pos + Vec2(4, 3), 3, Color(1.0f, 0.0f, 0.0f, 1.0f));

    // Draw bars to show count (up to 10 bars)
    int bars = std::min(count / 2, 10);
    for (int i = 0; i < bars; i++) {
        float barHeight = 3 + (i * 0.5f);
        Vec2 barPos = pos + Vec2(-18 + i * 4, -20);
        drawRectangle(barPos, Vec2(3, barHeight), 0, Color(0.5f, 1.0f, 0.5f, 0.8f));
    }
}

void Renderer::drawGameOverScreen(GameState state, int wave, int score) {
    // Dark overlay
    drawRectangle(Vec2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2),
                 Vec2(WINDOW_WIDTH, WINDOW_HEIGHT), 0, Color(0, 0, 0, 0.8f));

    Vec2 center(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    // Big X icon for game over
    float size = 80;
    drawLine(center + Vec2(-size, -size), center + Vec2(size, size), Color(1.0f, 0.2f, 0.2f, 1.0f));
    drawLine(center + Vec2(size, -size), center + Vec2(-size, size), Color(1.0f, 0.2f, 0.2f, 1.0f));
    drawCircle(center, size * 1.3f, Color(1.0f, 0.0f, 0.0f, 0.1f));

    // Show wave survived with circles
    Vec2 wavePos = center + Vec2(0, 150);
    for (int i = 0; i < std::min(wave, 15); i++) {
        float angle = (i * 2.0f * PI / 15.0f);
        Vec2 dotPos = wavePos + Vec2(cos(angle) * 40, sin(angle) * 40);
        drawCircle(dotPos, 4, Color(1.0f, 1.0f, 0.3f, 0.8f));
    }
    drawCircle(wavePos, 30, Color(1.0f, 1.0f, 0.3f, 0.3f));

    // Show score with stars
    Vec2 scorePos = center + Vec2(0, -150);
    int stars = std::min(score / 100, 20);
    for (int i = 0; i < stars; i++) {
        float angle = (i * 2.0f * PI / 20.0f);
        Vec2 starPos = scorePos + Vec2(cos(angle) * 50, sin(angle) * 50);
        drawCircle(starPos, 3, Color(1.0f, 0.8f, 0.2f, 1.0f));
    }
    drawCircle(scorePos, 35, Color(1.0f, 0.8f, 0.0f, 0.3f));

    // Enter key hint (rectangle with arrow pointing down)
    Vec2 keyPos = center + Vec2(0, -250);
    drawRectangle(keyPos, Vec2(80, 40), 0, Color(0.3f, 1.0f, 0.3f, 0.5f));
    drawTriangle(keyPos + Vec2(-15, -5), keyPos + Vec2(0, 10), keyPos + Vec2(15, -5),
                Color(1.0f, 1.0f, 1.0f, 1.0f));
}

void Renderer::drawMenuScreen(float pulseTime) {
    Vec2 center(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    float pulse = 0.7f + 0.3f * std::sin(pulseTime * 3.0f);

    // Title area - spaceship icon
    Vec2 titlePos = center + Vec2(0, 150);
    drawCircle(titlePos, 40, Color(0.3f, 0.6f, 1.0f, 0.5f));
    drawTriangle(titlePos + Vec2(-20, 20), titlePos + Vec2(0, -30), titlePos + Vec2(20, 20),
                Color(0.4f, 0.7f, 1.0f, 1.0f));

    // Alien icon
    Vec2 alienPos = center + Vec2(-100, 150);
    drawCircle(alienPos, 20, Color(0.3f, 0.9f, 0.3f, 0.6f));
    drawCircle(alienPos + Vec2(-6, 5), 4, Color(1.0f, 0.0f, 0.0f, 1.0f));
    drawCircle(alienPos + Vec2(6, 5), 4, Color(1.0f, 0.0f, 0.0f, 1.0f));

    Vec2 alienPos2 = center + Vec2(100, 150);
    drawCircle(alienPos2, 20, Color(0.3f, 0.9f, 0.3f, 0.6f));
    drawCircle(alienPos2 + Vec2(-6, 5), 4, Color(1.0f, 0.0f, 0.0f, 1.0f));
    drawCircle(alienPos2 + Vec2(6, 5), 4, Color(1.0f, 0.0f, 0.0f, 1.0f));

    // Start button - pulsing play icon
    Vec2 buttonPos = center + Vec2(0, -50);
    drawCircle(buttonPos, 60 * pulse, Color(0.3f, 1.0f, 0.3f, 0.3f * pulse));
    drawCircle(buttonPos, 45, Color(0.3f, 0.8f, 0.3f, 0.5f));
    drawPlayIcon(buttonPos, 40, Color(1.0f, 1.0f, 1.0f, pulse));

    // Control hints - visual icons
    Vec2 controlY = center + Vec2(0, -180);

    // WASD keys
    Vec2 keyBase = controlY + Vec2(-120, 0);
    float keySize = 15;
    drawRectangle(keyBase + Vec2(0, -keySize), Vec2(keySize * 2, keySize * 2), 0, Color(0.5f, 0.5f, 0.5f, 0.4f));
    drawTriangle(keyBase + Vec2(-5, -keySize - 5), keyBase + Vec2(0, -keySize - 12), keyBase + Vec2(5, -keySize - 5),
                Color(0.8f, 0.8f, 0.8f, 0.8f));

    // Mouse icon
    Vec2 mouseBase = controlY + Vec2(0, 0);
    drawCircle(mouseBase, 18, Color(0.5f, 0.5f, 0.5f, 0.4f));
    drawCircle(mouseBase, 12, Color(0.3f, 0.3f, 0.3f, 0.6f));
    drawLine(mouseBase + Vec2(0, -12), mouseBase + Vec2(0, 0), Color(0.7f, 0.7f, 0.7f, 0.8f));

    // Click icon
    Vec2 clickBase = controlY + Vec2(120, 0);
    drawCircle(clickBase, 18, Color(1.0f, 0.5f, 0.2f, 0.4f));
    drawCircle(clickBase, 8, Color(1.0f, 0.6f, 0.3f, 0.8f));
}

void Renderer::drawSpacecraft(const Spacecraft& ship) {
    if (!ship.isAlive()) return;

    float thrusterGlow = ship.getThrusterIntensity();

    // Shield glow
    if (ship.shield.getPercentage() > 0.3f) {
        float shieldAlpha = ship.shield.getPercentage() * 0.3f;
        drawCircle(ship.position, SPACECRAFT_RADIUS * 1.4f, Color(0.2f, 0.6f, 1.0f, shieldAlpha));
    }

    // Main body
    drawCircle(ship.position, SPACECRAFT_RADIUS * 0.9f, Color(0.3f, 0.35f, 0.4f, 1.0f));

    // Cockpit
    Vec2 cockpitOffset(SPACECRAFT_RADIUS * 0.3f * cos(ship.rotation),
                      SPACECRAFT_RADIUS * 0.3f * sin(ship.rotation));
    drawCircle(ship.position + cockpitOffset, SPACECRAFT_RADIUS * 0.4f,
              Color(0.2f, 0.5f, 0.7f, 1.0f));

    Vec2 windowOffset(SPACECRAFT_RADIUS * 0.4f * cos(ship.rotation),
                     SPACECRAFT_RADIUS * 0.4f * sin(ship.rotation));
    drawCircle(ship.position + windowOffset, SPACECRAFT_RADIUS * 0.2f,
              Color(0.4f, 0.7f, 1.0f, 0.6f));

    // Wings
    float wingAngle1 = ship.rotation + PI * 0.6f;
    float wingAngle2 = ship.rotation - PI * 0.6f;
    Vec2 wing1Pos = ship.position + Vec2(cos(wingAngle1) * SPACECRAFT_RADIUS * 0.8f,
                                         sin(wingAngle1) * SPACECRAFT_RADIUS * 0.8f);
    Vec2 wing2Pos = ship.position + Vec2(cos(wingAngle2) * SPACECRAFT_RADIUS * 0.8f,
                                         sin(wingAngle2) * SPACECRAFT_RADIUS * 0.8f);

    Vec2 wingTip1 = ship.position + Vec2(cos(wingAngle1) * SPACECRAFT_RADIUS * 1.6f,
                                         sin(wingAngle1) * SPACECRAFT_RADIUS * 1.6f);
    Vec2 wingTip2 = ship.position + Vec2(cos(wingAngle2) * SPACECRAFT_RADIUS * 1.6f,
                                         sin(wingAngle2) * SPACECRAFT_RADIUS * 1.6f);
    Vec2 wingBase1 = ship.position + Vec2(cos(ship.rotation - PI) * SPACECRAFT_RADIUS * 0.3f,
                                          sin(ship.rotation - PI) * SPACECRAFT_RADIUS * 0.3f);

    drawTriangle(wing1Pos, wingTip1, wingBase1, Color(0.25f, 0.3f, 0.35f, 1.0f));
    drawTriangle(wing2Pos, wingTip2, wingBase1, Color(0.25f, 0.3f, 0.35f, 1.0f));

    // Thrusters
    Vec2 thrusterOffset(SPACECRAFT_RADIUS * 0.7f * cos(ship.rotation + PI),
                       SPACECRAFT_RADIUS * 0.7f * sin(ship.rotation + PI));
    Vec2 thruster1 = ship.position + thrusterOffset + Vec2(cos(wingAngle1) * 8, sin(wingAngle1) * 8);
    Vec2 thruster2 = ship.position + thrusterOffset + Vec2(cos(wingAngle2) * 8, sin(wingAngle2) * 8);

    Color thrusterColor(0.3f + thrusterGlow * 0.7f, 0.6f + thrusterGlow * 0.4f, 1.0f, thrusterGlow);
    drawCircle(thruster1, 6.0f * thrusterGlow, thrusterColor);
    drawCircle(thruster2, 6.0f * thrusterGlow, thrusterColor);

    drawCircle(thruster1, 4.0f, Color(0.5f, 0.8f, 1.0f, 1.0f));
    drawCircle(thruster2, 4.0f, Color(0.5f, 0.8f, 1.0f, 1.0f));

    // Weapons
    drawRectangle(wing1Pos, Vec2(8, 6), ship.rotation, Color(0.2f, 0.2f, 0.25f, 1.0f));
    drawRectangle(wing2Pos, Vec2(8, 6), ship.rotation, Color(0.2f, 0.2f, 0.25f, 1.0f));
}

void Renderer::drawAlien(const Alien& alien) {
    if (!alien.active) return;
    float alpha = alien.spawnAnimation;

    Color bodyColor;
    switch(alien.type) {
        case AlienType::SCOUT: bodyColor = Color(0.3f, 0.9f, 0.3f, alpha); break;
        case AlienType::HUNTER: bodyColor = Color(0.9f, 0.3f, 0.9f, alpha); break;
        case AlienType::BRUTE: bodyColor = Color(0.9f, 0.2f, 0.2f, alpha); break;
    }

    float size = alien.getSize();

    // Tentacles
    for (const auto& tentacle : alien.tentacles) {
        Vec2 prevPos = alien.position;
        for (int i = 0; i < tentacle.getSegmentCount(); i++) {
            Vec2 offset = tentacle.getSegmentPosition(i, alien.animationTime);
            Vec2 segPos = alien.position + offset * alien.spawnAnimation;

            float segWidth = size * 0.15f * (1.0f - (float)i / tentacle.getSegmentCount());
            drawLine(prevPos, segPos, Color(bodyColor.r * 0.7f, bodyColor.g * 0.7f,
                                           bodyColor.b * 0.7f, alpha * 0.8f));
            drawCircle(segPos, segWidth, Color(bodyColor.r * 0.8f, bodyColor.g * 0.8f,
                                              bodyColor.b * 0.8f, alpha));
            prevPos = segPos;
        }
    }

    // Body
    drawCircle(alien.position, size, bodyColor);

    // Core
    float corePulse = 0.7f + 0.3f * std::sin(alien.animationTime * 4.0f);
    drawCircle(alien.position, size * 0.6f * corePulse,
              Color(bodyColor.r * 1.2f, bodyColor.g * 1.2f, bodyColor.b * 1.2f, alpha));
    drawCircle(alien.position, size * 0.3f, Color(1.0f, 0.3f, 0.3f, alpha * corePulse));

    // Eyes
    Vec2 eyeOffset1(-size * 0.25f, size * 0.2f);
    Vec2 eyeOffset2(size * 0.25f, size * 0.2f);
    drawCircle(alien.position + eyeOffset1, size * 0.15f, Color(1.0f, 0.0f, 0.0f, alpha));
    drawCircle(alien.position + eyeOffset2, size * 0.15f, Color(1.0f, 0.0f, 0.0f, alpha));
    drawCircle(alien.position + eyeOffset1, size * 0.08f, Color(0.2f, 0.0f, 0.0f, alpha));
    drawCircle(alien.position + eyeOffset2, size * 0.08f, Color(0.2f, 0.0f, 0.0f, alpha));
}

void Renderer::drawPlasma(const Plasma& plasma) {
    if (!plasma.active) return;
    drawCircle(plasma.position, PLASMA_RADIUS * 2.0f, Color(0.3f, 0.8f, 1.0f, 0.4f));
    drawCircle(plasma.position, PLASMA_RADIUS, Color(0.5f, 0.9f, 1.0f, 1.0f));
}

void Renderer::drawParticle(const Particle& particle) {
    if (!particle.isAlive()) return;
    Color col = particle.color;
    col.a = particle.getAlpha();
    drawCircle(particle.position, 3.0f, col);
}

void Renderer::drawShieldBar(const Spacecraft& ship) {
    Vec2 barPos(100, WINDOW_HEIGHT - 40);
    Vec2 barSize(250, 30);

    // Border
    drawRectangle(barPos, Vec2(barSize.x + 6, barSize.y + 6), 0, Color(0.8f, 0.8f, 0.8f, 0.8f));
    drawRectangle(barPos, barSize, 0, Color(0.1f, 0.1f, 0.1f, 0.9f));

    // Shield fill
    float shieldPercent = ship.shield.getPercentage();
    Vec2 shieldSize(barSize.x * shieldPercent, barSize.y);
    Vec2 shieldPos(barPos.x - barSize.x * (1.0f - shieldPercent) / 2.0f, barPos.y);

    Color shieldColor;
    if (shieldPercent > 0.6f) shieldColor = Color(0.2f, 0.8f, 1.0f, 0.9f);
    else if (shieldPercent > 0.3f) shieldColor = Color(1.0f, 0.9f, 0.2f, 0.9f);
    else shieldColor = Color(1.0f, 0.2f, 0.2f, 0.9f);

    drawRectangle(shieldPos, shieldSize, 0, shieldColor);

    // Shield icon
    Vec2 iconPos = barPos + Vec2(-barSize.x/2 - 30, 0);
    drawCircle(iconPos, 15, Color(0.3f, 0.7f, 1.0f, 0.5f));
    drawCircle(iconPos, 10, Color(0.4f, 0.8f, 1.0f, 0.7f));
}

void Renderer::drawAmmoCounter(const Spacecraft& ship) {
    Vec2 barPos(WINDOW_WIDTH - 100, WINDOW_HEIGHT - 40);
    Vec2 barSize(180, 30);

    // Border
    drawRectangle(barPos, Vec2(barSize.x + 6, barSize.y + 6), 0, Color(0.8f, 0.8f, 0.8f, 0.8f));
    drawRectangle(barPos, barSize, 0, Color(0.1f, 0.1f, 0.1f, 0.9f));

    // Ammo fill
    float ammoPercent = (float)ship.ammo / ship.maxAmmo;
    Vec2 ammoBarSize(barSize.x * ammoPercent, barSize.y);
    Vec2 ammoBarPos(barPos.x - barSize.x * (1.0f - ammoPercent) / 2.0f, barPos.y);

    Color ammoColor = ammoPercent > 0.3f ? Color(0.3f, 1.0f, 0.8f, 0.9f) : Color(1.0f, 0.3f, 0.3f, 0.9f);
    drawRectangle(ammoBarPos, ammoBarSize, 0, ammoColor);

    // Ammo icon
    Vec2 iconPos = barPos + Vec2(barSize.x/2 + 25, 0);
    drawCircle(iconPos, 8, Color(0.5f, 0.9f, 1.0f, 0.9f));
    drawCircle(iconPos, 4, Color(0.7f, 1.0f, 1.0f, 1.0f));
}

void Renderer::drawUI(const GameManager& game) {
    if (game.gameState == GameState::GAME_OVER_SHIELD || game.gameState == GameState::GAME_OVER_AMMO) {
        drawGameOverScreen(game.gameState, game.wave, game.score);
        return;
    }

    if (game.gameState == GameState::MENU) {
        drawMenuScreen(game.stateTimer);
        return;
    }

    // In-game UI
    drawShieldBar(game.spacecraft);
    drawAmmoCounter(game.spacecraft);

    if (game.waveActive) {
        drawWaveIndicator(game.wave);
        drawAlienCountIndicator(game.aliens.size());
    } else if (game.wave > 0) {
        // Show "press space to continue" indicator
        Vec2 center(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
        float pulse = 0.6f + 0.4f * std::sin(game.stateTimer * 4.0f);
        drawCircle(center, 60 * pulse, Color(0.3f, 1.0f, 0.3f, 0.2f * pulse));
        drawPlayIcon(center, 50, Color(0.3f, 1.0f, 0.3f, pulse));
    } else {
        // First wave - show start indicator
        Vec2 center(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
        float pulse = 0.6f + 0.4f * std::sin(game.stateTimer * 3.0f);
        drawCircle(center, 80 * pulse, Color(0.3f, 1.0f, 0.3f, 0.15f * pulse));
        drawPlayIcon(center, 70, Color(0.5f, 1.0f, 0.5f, pulse));
    }
}

void Renderer::cleanup() {
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (shaderProgram) glDeleteProgram(shaderProgram);
}
