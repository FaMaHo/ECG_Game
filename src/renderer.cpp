#include "../include/renderer.hpp"
#include "../shader.hpp"
#include "../include/mothership.hpp"
#include <cstdlib>
#include <cmath>
#include <GLFW/glfw3.h>

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

void Renderer::drawGameOverScreen(GameState state, int wave, int score) {
    // Dark overlay
    drawRectangle(Vec2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2),
                 Vec2(WINDOW_WIDTH, WINDOW_HEIGHT), 0, Color(0, 0, 0, 0.85f));

    Vec2 center(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    // Simple "GAME OVER" visual
    drawCircle(center, 100, Color(0.9f, 0.2f, 0.2f, 0.2f));
    drawCircle(center, 70, Color(1.0f, 0.3f, 0.3f, 0.4f));
    drawCircle(center, 40, Color(1.0f, 0.4f, 0.4f, 0.6f));

    // Wave count - simple bar visualization  
    Vec2 wavePos = center + Vec2(-150, 100);
    int waveBars = std::min(wave, 20);
    for (int i = 0; i < waveBars; i++) {
        float x = wavePos.x + i * 16;
        float height = 10 + i * 2;
        drawRectangle(Vec2(x, wavePos.y), Vec2(12, height), 0, 
                     Color(1.0f, 0.8f, 0.3f, 0.8f));
    }
    // Label icon for waves
    drawCircle(wavePos + Vec2(-30, 0), 12, Color(1.0f, 0.8f, 0.2f, 0.5f));

    // Score - simple dot visualization
    Vec2 scorePos = center + Vec2(-150, -100);
    int scoreDots = std::min(score / 50, 40);
    for (int i = 0; i < scoreDots; i++) {
        int row = i / 10;
        int col = i % 10;
        Vec2 dotPos = scorePos + Vec2(col * 16, row * 16);
        drawCircle(dotPos, 5, Color(0.3f, 1.0f, 0.8f, 0.9f));
    }
    // Label icon for score
    drawCircle(scorePos + Vec2(-30, 8), 12, Color(0.3f, 0.9f, 0.7f, 0.5f));

    // SPACE to restart hint - simple and clear
    Vec2 hintPos = center + Vec2(0, -200);
    float pulseTime = glfwGetTime();
    float pulse = 0.6f + 0.4f * std::sin(pulseTime * 3.0f);
    drawRectangle(hintPos, Vec2(200, 50), 0, Color(0.3f, 1.0f, 0.3f, 0.3f * pulse));
    drawRectangle(hintPos, Vec2(180, 35), 0, Color(0.2f, 0.8f, 0.2f, 0.5f));
    // SPACE bar icon
    drawRectangle(hintPos, Vec2(120, 20), 0, Color(0.9f, 0.9f, 0.9f, pulse));
}

void Renderer::drawMenuScreen(float pulseTime) {
    Vec2 center(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    float pulse = 0.7f + 0.3f * std::sin(pulseTime * 2.5f);

    // Title - spaceship icon
    Vec2 titlePos = center + Vec2(0, 120);
    drawCircle(titlePos, 50 * pulse, Color(0.3f, 0.6f, 1.0f, 0.3f * pulse));
    drawCircle(titlePos, 35, Color(0.4f, 0.7f, 1.0f, 0.6f));
    drawTriangle(titlePos + Vec2(-20, 15), titlePos + Vec2(0, -25), titlePos + Vec2(20, 15),
                Color(0.5f, 0.8f, 1.0f, 1.0f));

    // Simple alien icons
    Vec2 alien1Pos = center + Vec2(-80, 120);
    drawCircle(alien1Pos, 18, Color(0.3f, 0.9f, 0.3f, 0.7f));
    drawCircle(alien1Pos + Vec2(-5, 4), 3, Color(1.0f, 0.0f, 0.0f, 1.0f));
    drawCircle(alien1Pos + Vec2(5, 4), 3, Color(1.0f, 0.0f, 0.0f, 1.0f));

    Vec2 alien2Pos = center + Vec2(80, 120);
    drawCircle(alien2Pos, 18, Color(0.9f, 0.3f, 0.9f, 0.7f));
    drawCircle(alien2Pos + Vec2(-5, 4), 3, Color(1.0f, 0.0f, 0.0f, 1.0f));
    drawCircle(alien2Pos + Vec2(5, 4), 3, Color(1.0f, 0.0f, 0.0f, 1.0f));

    // Start button - pulsing SPACE bar
    Vec2 buttonPos = center + Vec2(0, -20);
    drawCircle(buttonPos, 70 * pulse, Color(0.3f, 1.0f, 0.3f, 0.2f * pulse));
    drawRectangle(buttonPos, Vec2(140, 50), 0, Color(0.3f, 0.8f, 0.3f, 0.6f));
    drawRectangle(buttonPos, Vec2(120, 35), 0, Color(0.9f, 0.9f, 0.9f, pulse));

    // Control hints - minimal icons
    Vec2 controlsY = center + Vec2(0, -140);
    
    // WASD movement
    drawCircle(controlsY + Vec2(-100, 0), 25, Color(0.4f, 0.4f, 0.4f, 0.5f));
    drawRectangle(controlsY + Vec2(-100, 0), Vec2(12, 12), 0, Color(0.7f, 0.7f, 0.7f, 0.8f));
    drawTriangle(controlsY + Vec2(-100, -8), controlsY + Vec2(-100, -15), 
                controlsY + Vec2(-95, -8), Color(0.9f, 0.9f, 0.9f, 0.9f));
    
    // Mouse
    drawCircle(controlsY + Vec2(0, 0), 16, Color(0.5f, 0.5f, 0.5f, 0.5f));
    drawCircle(controlsY + Vec2(0, 0), 10, Color(0.3f, 0.3f, 0.3f, 0.7f));
    
    // Click
    drawCircle(controlsY + Vec2(100, 0), 16, Color(1.0f, 0.5f, 0.2f, 0.5f));
    drawCircle(controlsY + Vec2(100, 0), 7, Color(1.0f, 0.6f, 0.3f, 0.9f));
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
    switch (alien.type) {
    case AlienType::SCOUT: bodyColor = Color(0.3f, 0.9f, 0.3f, alpha); break;
    case AlienType::HUNTER: bodyColor = Color(0.9f, 0.3f, 0.9f, alpha); break;
    case AlienType::BRUTE: bodyColor = Color(0.9f, 0.2f, 0.2f, alpha); break;
    }

    float size = alien.getSize();

    // Tentacles (keep them for some alien feel)
    for (const auto& tentacle : alien.tentacles) {
        Vec2 prevPos = alien.position;
        for (int i = 0; i < tentacle.getSegmentCount(); i++) {
            Vec2 offset = tentacle.getSegmentPosition(i, alien.animationTime);
            Vec2 segPos = alien.position + offset * alien.spawnAnimation * 0.3f;  // Shorter tentacles

            float segWidth = size * 0.08f * (1.0f - (float)i / tentacle.getSegmentCount());
            drawLine(prevPos, segPos, Color(bodyColor.r * 0.7f, bodyColor.g * 0.7f,
                bodyColor.b * 0.7f, alpha * 0.6f));
            drawCircle(segPos, segWidth, Color(bodyColor.r * 0.8f, bodyColor.g * 0.8f,
                bodyColor.b * 0.8f, alpha * 0.7f));
            prevPos = segPos;
        }
    }

    // Main head - larger oval/egg shape (wider at top)
    // Draw multiple circles to create oval shape
    drawCircle(alien.position, size * 1.1f, bodyColor);  // Top wider part
    drawCircle(alien.position + Vec2(0, -size * 0.3f), size * 0.9f, bodyColor);  // Middle
    drawCircle(alien.position + Vec2(0, -size * 0.5f), size * 0.7f, bodyColor);  // Bottom (chin)

    // Subtle glow/highlight on top of head
    drawCircle(alien.position + Vec2(-size * 0.3f, size * 0.3f), size * 0.3f,
        Color(bodyColor.r * 1.3f, bodyColor.g * 1.3f, bodyColor.b * 1.3f, alpha * 0.4f));

    // Large alien eyes - black and slanted
    float eyeWidth = size * 0.35f;
    float eyeHeight = size * 0.55f;

    // Move eyes farther apart
    Vec2 leftEyeCenter = alien.position + Vec2(-size * 0.50f, size * 0.15f);
    Vec2 rightEyeCenter = alien.position + Vec2(size * 0.50f, size * 0.15f);

    // Make them more tilted (increase the vertical slant factor)
    float slantFactor = 0.55f;  // was 0.3f before, now more vertical

    // Left eye: outer corner (left side) up
    for (int i = 0; i < 5; i++) {
        float t = i / 4.0f;
        float offsetX = (t - 0.5f) * eyeWidth * 1.5f;
        float offsetY = -(t - 0.5f) * eyeHeight * slantFactor;  // steeper upward tilt
        float circleSize = eyeHeight * 0.5f * (1.0f - std::abs(t - 0.5f) * 0.4f);
        drawCircle(leftEyeCenter + Vec2(offsetX, offsetY), circleSize,
            Color(0.0f, 0.0f, 0.0f, alpha));
    }

    // Right eye: outer corner (right side) up
    for (int i = 0; i < 5; i++) {
        float t = i / 4.0f;
        float offsetX = (t - 0.5f) * eyeWidth * 1.5f;
        float offsetY = (t - 0.5f) * eyeHeight * slantFactor;  // steeper upward tilt
        float circleSize = eyeHeight * 0.5f * (1.0f - std::abs(t - 0.5f) * 0.4f);
        drawCircle(rightEyeCenter + Vec2(offsetX, offsetY), circleSize,
            Color(0.0f, 0.0f, 0.0f, alpha));
    }
    // Tiny white reflections in eyes
    drawCircle(leftEyeCenter + Vec2(-eyeWidth * 0.3f, eyeHeight * 0.2f), size * 0.08f,
        Color(1.0f, 1.0f, 1.0f, alpha * 0.8f));
    drawCircle(rightEyeCenter + Vec2(eyeWidth * 0.3f, eyeHeight * 0.2f), size * 0.08f,
        Color(1.0f, 1.0f, 1.0f, alpha * 0.8f));

    // Small mouth/nose slits
    Vec2 mouthPos = alien.position + Vec2(0, -size * 0.35f);
    drawCircle(mouthPos + Vec2(-size * 0.1f, 0), size * 0.06f,
        Color(0.0f, 0.0f, 0.0f, alpha * 0.5f));
    drawCircle(mouthPos + Vec2(size * 0.1f, 0), size * 0.06f,
        Color(0.0f, 0.0f, 0.0f, alpha * 0.5f));

    // Center nose slit
    Vec2 mouthCenter = alien.position + Vec2(0, -size * 0.5f);
    float mouthRadius = size * 0.25f;
    int mouthSegments = 12;

    // WeÃ¯Â¿Â½ll draw short arc of circles forming an upward curve
    for (int i = 0; i <= mouthSegments; ++i) {
        float t = (float)i / mouthSegments;
        // angle range creates a shallow upward curve
        float angle = 3.6f + t * (5.8f - 3.6f); // radians ~206Ã¯Â¿Â½ to 332Ã¯Â¿Â½
        Vec2 pos = mouthCenter + Vec2(cos(angle), sin(angle)) * mouthRadius;
        drawCircle(pos, size * 0.04f, Color(0.0f, 0.0f, 0.0f, alpha * 0.7f));
    }
}

void Renderer::drawMothership(const Mothership& mothership) {
    if (!mothership.active) return;

    float pulse = 0.9f + 0.1f * std::sin(mothership.animationTime * 3.0f);

    // Main body - triangle shape (smaller)
    Vec2 top = mothership.position + Vec2(0, mothership.size * 0.5f);
    Vec2 left = mothership.position + Vec2(-mothership.size * 0.8f, -mothership.size * 0.3f);
    Vec2 right = mothership.position + Vec2(mothership.size * 0.8f, -mothership.size * 0.3f);

    drawTriangle(top, left, right, Color(0.2f, 0.2f, 0.3f, 0.9f));

    // Glowing core
    drawCircle(mothership.position, mothership.size * 0.3f * pulse,
        Color(0.8f, 0.2f, 0.2f, 0.6f));
    drawCircle(mothership.position, mothership.size * 0.18f,
        Color(1.0f, 0.3f, 0.3f, 0.9f));

    // Bottom opening (where aliens spawn)
    Vec2 bottom = mothership.position + Vec2(0, -mothership.size * 0.4f);
    drawCircle(bottom, mothership.size * 0.25f, Color(0.1f, 0.1f, 0.2f, 0.8f));
    drawCircle(bottom, mothership.size * 0.15f, Color(0.9f, 0.4f, 0.1f, 0.5f * pulse));

    // Side lights (fewer for smaller size)
    for (int i = 0; i < 2; i++) {
        float angle = mothership.animationTime * 2.0f + i * PI;
        float lightPulse = 0.5f + 0.5f * std::sin(angle);
        Vec2 lightPos = mothership.position + Vec2(
            std::cos(angle) * mothership.size * 0.5f,
            std::sin(angle) * mothership.size * 0.2f
        );
        drawCircle(lightPos, 3.0f, Color(0.2f, 0.8f, 1.0f, lightPulse));
    }
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

    // In-game UI - only essential info
    drawShieldBar(game.spacecraft);
    drawAmmoCounter(game.spacecraft);

    // Show "press SPACE to start wave" indicator only between waves
    if (!game.waveActive && game.wave >= 0) {
        Vec2 center(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
        float pulse = 0.6f + 0.4f * std::sin(game.stateTimer * 3.5f);
        
        // Pulsing circle
        drawCircle(center, 70 * pulse, Color(0.3f, 1.0f, 0.3f, 0.2f * pulse));
        
        // SPACE bar representation
        drawRectangle(center, Vec2(100, 40), 0, Color(0.3f, 0.9f, 0.3f, 0.5f));
        drawRectangle(center, Vec2(80, 28), 0, Color(0.9f, 0.9f, 0.9f, pulse));
    }
}

void Renderer::cleanup() {
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (shaderProgram) glDeleteProgram(shaderProgram);
}