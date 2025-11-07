#pragma once
#include "vec2.hpp"
#include "constants.hpp"
#include <cmath>

class Tentacle {
private:
    float baseAngle;
    float length;
    int segments;

public:
    float phase;

    Tentacle(float angle, float len);

    Vec2 getSegmentPosition(int segment, float animTime) const;
    int getSegmentCount() const { return segments; }
};
