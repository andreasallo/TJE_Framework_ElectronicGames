
#pragma once
#include "framework/utils.h"
#include "game/asteroidControl.h"
#include "game/ringControl.h"
#include <random>

#include <vector>

class ChunkGenerator {
public:
    float safeZoneInitial = 300.0f;
    float chunkLength = 120.0f;
    float totalLevelLength = 3000.0f;
    float nextChunkZ = 0.0f;

    void init(float safeZone, float chunkLen, float totalLen);
    void update(float dt, float playerZ);
    void generateChunk(float baseZ);
   
private:
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;

    float frand() { return dist(rng); }

    static constexpr float PLAYER_MIN_X = -12.0f;
    static constexpr float PLAYER_MAX_X = 12.0f;
    static constexpr float PLAYER_MIN_Y = 0.0f;
    static constexpr float PLAYER_MAX_Y = 24.0f;
};
