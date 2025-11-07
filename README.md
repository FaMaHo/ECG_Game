# XENOSTRIKE: Last Defense of Sector 7

## 🚀 Game Story

**Year 2347 - Deep Space Station Osiris**

You are Commander Alex Chen, piloting the experimental fighter craft **SF-NOVA** on a routine patrol around Deep Space Station Osiris in Sector 7. What starts as a standard shift quickly turns into humanity's greatest nightmare.

Without warning, a massive wormhole tears open near the station. From its swirling depths emerge the **Xentari** - a hostile alien species never encountered before. These biomechanical horrors possess writhing tentacles that can drain energy from any technology they touch, and they're hungry for the station's fusion core.

### The Invasion

The Xentari pour through the wormhole in waves, each more aggressive than the last. Their organic exoskeletons allow them to survive in the vacuum of space, and their glowing red cores pulse with an eerie bio-luminescence. Station Osiris's automated defenses are quickly overwhelmed - you are the last line of defense.

### Your Mission

**Primary Objective**: Defend Station Osiris from the Xentari invasion waves
- Destroy as many aliens as possible before they reach the station
- Manage your ship's energy shields and plasma ammunition
- Survive increasingly difficult waves of alien attackers

**Secondary Objectives**:
- Achieve the highest elimination count possible
- Conserve ammunition by making precise shots
- Master the SF-NOVA's advanced shield regeneration system

### The SF-NOVA Spacecraft

Your ship is equipped with:
- **Plasma Cannon**: Fires high-velocity energy bolts
- **Shield Generator**: Absorbs alien attacks, regenerates over time
- **Thruster Array**: Pulsing ion thrusters for rapid maneuvering
- **Emergency Systems**: Visual warnings when shields or ammo are critically low

### The Xentari

These alien creatures exhibit:
- **Bio-luminescent cores** that pulse with life
- **Animated tentacles** that writhe as they move
- **Hive intelligence** that makes each wave more coordinated
- **Energy-draining attacks** that damage your shields on contact

### Game Mechanics Preview (2D Teaser)

This 2D version demonstrates core mechanics that will be expanded in the full 3D game:

1. **Combat System**: Aim and shoot plasma bolts at incoming aliens
2. **Wave-based Spawning**: Endless waves of increasingly difficult enemies
3. **Energy Management**: Balance between shields and ammunition
4. **Collision Physics**: Advanced detection with shield absorption calculations
5. **Dynamic Animations**: Living aliens and reactive spacecraft systems

### The Stakes

If Station Osiris falls, the wormhole will remain open, allowing unlimited Xentari forces to pour into human space. The fate of Sector 7 - and potentially all of humanity - rests in your hands.

**Will you hold the line?**

---

## 🎮 How to Play

**Controls**:
- **WASD** or **Arrow Keys**: Move your spacecraft
- **Mouse**: Aim your plasma cannon
- **Left Click**: Fire plasma bolts
- **ESC**: Pause/Menu
- **R**: Restart (when game over)

**Survival Tips**:
- Keep moving to avoid alien swarms
- Aim carefully to conserve ammunition
- Let your shields regenerate when possible
- Watch for visual warnings (red screen = low health, yellow = low ammo)

---

## 🛠️ Technical Features

### Animations (2 types):
1. **Alien Tentacle Animation**: Procedural sine-wave movement of alien appendages
2. **Spacecraft Thruster Pulse**: Dynamic thruster glow effect based on movement

### Algorithms (2 original):
1. **Enhanced Collision Detection**: Circle-based collision with penetration depth calculation and response vectors
2. **Shield Absorption System**: Energy-based damage calculation with regeneration curves and absorption efficiency

### Complex Shapes (primitives):
1. **Alien Entity**: Multi-part body with head, core, body segments, and 4 animated tentacles
2. **SF-NOVA Spacecraft**: Detailed ship with cockpit, wings, weapon mounts, and thruster arrays

### User Input:
- Keyboard-based movement (WASD + Arrow keys)
- Mouse-based aiming system
- Click-to-shoot mechanics

---

## 📦 Building the Project

```bash
# Compile (example for Linux/Mac)
g++ -std=c++11 main.cpp shader.cpp -o app -lGL -lGLEW -lglfw -lm

# Run
./app
```

---

## 🎯 Project Information

This game is developed as part of a Computer Graphics project, demonstrating:
- 2D graphics rendering with OpenGL
- Real-time animation systems
- Physics and collision algorithms
- Game state management
- Particle effects systems

**Future 3D Version**: This 2D teaser will be expanded into a full 3D space combat experience with volumetric aliens, 3D spacecraft models, and immersive spatial audio.

---

**Defend. Survive. Fight Back.**

*XENOSTRIKE - Coming to full 3D*
