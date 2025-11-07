#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include "include/game_manager.hpp"
#include "include/renderer.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include <glm/glm.hpp>

// Global game objects
GameManager game;
Renderer renderer;
Vec2 mousePosition;
bool mousePressed = false;
bool spacePressed = false;
bool prevSpacePressed = false;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        mousePressed = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    mousePosition.x = static_cast<float>(xpos);
    mousePosition.y = WINDOW_HEIGHT - static_cast<float>(ypos);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE) {
        spacePressed = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        if (game.gameState == GameState::GAME_OVER_SHIELD ||
            game.gameState == GameState::GAME_OVER_AMMO ||
            game.gameState == GameState::MENU) {
            game.reset();
            std::cout << "\n=== NEW GAME STARTED ===" << std::endl;
        }
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS && game.gameState == GameState::PLAYING) {
        game.spacecraft.reload();
        std::cout << "Reloaded!" << std::endl;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main() {
    srand(time(NULL));

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          "XENOSTRIKE - Alien Defense", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetKeyCallback(window, keyCallback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    if (!renderer.initialize()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    std::cout << "=== XENOSTRIKE: ALIEN DEFENSE ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move" << std::endl;
    std::cout << "  Mouse - Aim" << std::endl;
    std::cout << "  Click - Shoot" << std::endl;
    std::cout << "  R - Reload" << std::endl;
    std::cout << "  SPACE - Start Wave" << std::endl;
    std::cout << "  ENTER - Start/Retry" << std::endl;
    std::cout << "  ESC - Quit" << std::endl;
    std::cout << "\nDefend Station Osiris!" << std::endl;
    std::cout << std::endl;

    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        deltaTime = std::min(deltaTime, 0.1f);

        // Handle input
        if (game.gameState == GameState::PLAYING) {
            game.spacecraft.velocity = Vec2(0, 0);
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                game.spacecraft.velocity.y += SPACECRAFT_SPEED * 60.0f;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                game.spacecraft.velocity.y -= SPACECRAFT_SPEED * 60.0f;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                game.spacecraft.velocity.x -= SPACECRAFT_SPEED * 60.0f;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                game.spacecraft.velocity.x += SPACECRAFT_SPEED * 60.0f;

            if (spacePressed && !prevSpacePressed && !game.waveActive) {
                game.startWave();
            }
        }

        prevSpacePressed = spacePressed;

        // Update game logic
        game.update(deltaTime, mousePosition, mousePressed);

        // Render
        renderer.beginFrame();
        renderer.drawStarfield();

        for (const auto& particle : game.particles) {
            renderer.drawParticle(particle);
        }

        for (const auto& alien : game.aliens) {
            renderer.drawAlien(alien);
        }

        for (const auto& plasma : game.plasmas) {
            renderer.drawPlasma(plasma);
        }

        if (game.gameState == GameState::PLAYING) {
            renderer.drawSpacecraft(game.spacecraft);
        }

        renderer.drawUI(game);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    renderer.cleanup();
    glfwTerminate();

    std::cout << "\n=== GAME OVER ===" << std::endl;
    std::cout << "Final Score: " << game.score << std::endl;
    std::cout << "Waves Survived: " << game.wave << std::endl;
    std::cout << "Aliens Eliminated: " << game.killCount << std::endl;

    return 0;
}
