#pragma once

#include <vector>
#include "game/ring.h"


class RingControl {

public:
    std::vector<Ring*> rings;

    float spawnInterval = 3.5f; //cada quant apareix un arc
    float spawnTimer = 0.0f;


    Mesh* ringMesh = nullptr;
    Material ringMaterial;

    float minX = -8.0f, maxX = 8.0f;
    float minY = -5.0f, maxY = 10.0f;

    void init();
    void update(float dt);
    void spawnRingAt(float x, float y, float z);
    void spawnRing();
};