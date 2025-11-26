#pragma once
#include <vector>
#include "game/asteroid.h"
#include "ringControl.h"

class AsteroidControl
{
public:
    std::vector<Asteroid*> asteroids;

    float spawnTimer = 0.0f;
    float spawnInterval = 2.4f; // cada x seg apareix un meteorit


    eSpawnPattern currentPattern = PATTERN_MID;

    Mesh* asteroidMesh=nullptr;
    Material asteroidMat;

    void init();
    void update(float dt);
    void spawnAsteroid();
};

