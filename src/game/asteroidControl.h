#pragma once
#include <vector>
#include "game/asteroid.h"

class AsteroidControl
{
public:
    std::vector<Asteroid*> asteroids;

    float spawnTimer = 0.0f;
    float spawnInterval = 1.2f; // cada 1.2s apareix un meteorit

    Mesh* asteroidMesh=nullptr;
    Material asteroidMat;

    void init();
    void update(float dt);
    void spawnAsteroid();
};

