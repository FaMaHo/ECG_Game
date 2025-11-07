#pragma once
#include <algorithm>

class ShieldSystem {
private:
    float maxEnergy;
    float currentEnergy;
    float regenRate;
    float regenDelay;
    float timeSinceLastHit;
    float absorptionEfficiency;

public:
    ShieldSystem();

    void update(float deltaTime);
    float absorbDamage(float incomingDamage);

    float getPercentage() const { return currentEnergy / maxEnergy; }
    bool isDepleted() const { return currentEnergy <= 0; }
    float getCurrent() const { return currentEnergy; }
};
