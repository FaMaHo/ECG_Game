#include "../include/collision.hpp"

CollisionInfo detectCollision(Vec2 pos1, float radius1, Vec2 pos2, float radius2) {
    CollisionInfo info;
    Vec2 diff = pos2 - pos1;
    float distance = diff.length();
    float radiiSum = radius1 + radius2;

    if (distance < radiiSum) {
        info.hasCollision = true;
        info.penetrationDepth = radiiSum - distance;

        // Calculate collision normal (from object 1 to object 2)
        if (distance > 0.001f) {
            info.collisionNormal = diff.normalized();
        } else {
            info.collisionNormal = Vec2(1, 0); // Default if objects are exactly overlapping
        }

        // Contact point is at the midpoint of penetration
        info.contactPoint = pos1 + info.collisionNormal * (radius1 - info.penetrationDepth * 0.5f);
    }

    return info;
}
