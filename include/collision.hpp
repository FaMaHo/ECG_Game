#pragma once
#include "vec2.hpp"

struct CollisionInfo {
    bool hasCollision;
    float penetrationDepth;
    Vec2 collisionNormal;
    Vec2 contactPoint;

    CollisionInfo()
        : hasCollision(false), penetrationDepth(0),
          collisionNormal(0, 0), contactPoint(0, 0) {}
};

CollisionInfo detectCollision(Vec2 pos1, float radius1, Vec2 pos2, float radius2);
