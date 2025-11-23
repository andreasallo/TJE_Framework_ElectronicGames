#pragma once

#include <vector>
#include "game/ring.h"

class RingControl {

public:
    std::vector<Ring*> rings;

    float spawnInterval = 3.0f; //cada quant apareix un arc
    float spawnTimer = 0.0f;

    float minX = -12.0f, maxX = 12.0f;
    float minY = -6.0f, maxY = 20.0f;

    Mesh* ringMesh = nullptr;
    Material ringMaterial;

    void init();
    void update(float dt);
    void spawnRing();
};
