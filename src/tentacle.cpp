#include "../include/tentacle.hpp"

Tentacle::Tentacle(float angle, float len)
    : baseAngle(angle), length(len), segments(4), phase(0) {}

Vec2 Tentacle::getSegmentPosition(int segment, float animTime) const {
    float t = (float)segment / segments;
    float angle = baseAngle + std::sin(animTime * 3.0f + phase + t * PI) * 0.3f;
    float segLength = length / segments;
    return Vec2(std::cos(angle) * segLength * (segment + 1),
                std::sin(angle) * segLength * (segment + 1));
}
