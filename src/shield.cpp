#include "../include/shield.hpp"
#include <cmath>

ShieldSystem::ShieldSystem()
    : maxEnergy(100.0f), currentEnergy(100.0f), regenRate(15.0f),
      regenDelay(2.0f), timeSinceLastHit(0), absorptionEfficiency(0.85f) {}

void ShieldSystem::update(float deltaTime) {
    timeSinceLastHit += deltaTime;

    // Shield regeneration with exponential curve
    if (timeSinceLastHit > regenDelay && currentEnergy < maxEnergy) {
        float regenAmount = regenRate * deltaTime;
        // Exponential regeneration curve (slower as shield approaches max)
        float regenMultiplier = 1.0f - (currentEnergy / maxEnergy);
        regenMultiplier = regenMultiplier * regenMultiplier; // Square for exponential effect
        currentEnergy += regenAmount * (0.5f + regenMultiplier);
        currentEnergy = std::min(currentEnergy, maxEnergy);
    }
}

float ShieldSystem::absorbDamage(float incomingDamage) {
    timeSinceLastHit = 0;

    // Calculate absorbed damage based on efficiency
    float absorbedDamage = incomingDamage * absorptionEfficiency;
    float actualDamage = incomingDamage * (1.0f - absorptionEfficiency);

    // Apply to shield
    currentEnergy -= absorbedDamage;

    // If shield depleted, remaining damage bleeds through
    if (currentEnergy < 0) {
        actualDamage += std::abs(currentEnergy);
        currentEnergy = 0;
    }

    return actualDamage;
}
