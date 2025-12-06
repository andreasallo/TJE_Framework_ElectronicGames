#pragma once
#include <vector>
#include "game/asteroid.h"
#include "ringControl.h"

class AsteroidControl
{
public:
    std::vector<Asteroid*> asteroids;

    float spawnTimer = 0.0f;
    float spawnInterval = 1.8f; // cada x seg apareix un meteorit

    Mesh* asteroidMesh=nullptr;
    Material asteroidMat;

    void init();
    void update(float dt);
    void spawnAsteroidAt(float x, float y, float z, float speed);
    void spawnAsteroid();
};

