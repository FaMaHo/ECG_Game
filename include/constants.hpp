#pragma once

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const float SPACECRAFT_SPEED = 3.5f;
const float PLASMA_SPEED = 9.0f;
const float ALIEN_SPEED = 3.5f;
const float SPACECRAFT_RADIUS = 22.0f;
const float ALIEN_RADIUS = 20.0f;
const float PLASMA_RADIUS = 6.0f;
const float SHOOT_COOLDOWN = 0.25f;
const int ALIENS_PER_WAVE = 5;
const int CIRCLE_SEGMENTS = 30;
const float PI = 3.14159f;

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER_SHIELD,
    GAME_OVER_AMMO
};
