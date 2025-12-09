
/*
// Forward declarations
class AsteroidControl;
class RingControl;


enum eChunkType {
    CHUNK_MIXED = 0
};

struct sAsteroidSpawn {
    float x, y, z;
    float speed;
    float scale; // Añadido para variedad visual explícita si se necesita
};

struct sRingSpawn {
    float x, y, z;
};

struct sChunk {
    float startZ;
    float length;
    eChunkType type;
    std::vector<sAsteroidSpawn> asteroids;
    std::vector<sRingSpawn> rings;
};

class ChunkGenerator {
public:
    ChunkGenerator();
    ~ChunkGenerator();

    float levelLength;
    float chunkLength;

    void init(float levelLength = 2500.0f, float chunkLength = 120.0f);
    void update(float dt, const Vector3& playerPos);
    const std::vector<sChunk>& getChunks() const { return chunks; }
    void reset();

private:
    std::vector<sChunk> chunks;

    float nextChunkZ;
    float spawnPadding;

    void generateChunkAt(float startZ);
    void fillPattern(sChunk& chunk);

    // PATRONES
    void createLanePattern(sChunk& chunk, int difficulty);
    void createZigZagPattern(sChunk& chunk, int difficulty);
    void createWallPattern(sChunk& chunk, int difficulty);
    void createStormPattern(sChunk& chunk, int difficulty);
    void createFinalStormPattern(sChunk& chunk); // Lluvia final masiva
};


// chunkGenerator.h
#pragma once
#include <vector>
#include <string>

enum SpawnType { TYPE_ASTEROID, TYPE_RING };

struct sSpawn {
    SpawnType type;
    float x, y, z;
    float speedMult;
};

struct sPattern {
    std::string name;
    std::vector<sSpawn> spawns;
};

class ChunkGenerator {
public:
    float currentSpawnZ = 0.f;
    float chunkLength = 120.f;
    float levelLength = 2000.f;
    float renderDistance = 900.f;

    // Patrons
    sPattern softRain;
    sPattern hardRain;
    sPattern wallGap;
    sPattern snakeWall;

    void init(float startZ, float chunkLen, float totalLen);
    void createPatterns();
    void update(float dt, float playerZ);

private:
    void spawnChunk(const sPattern& p, float startZ);

    // helpers
    float randX();
    float randY();
};*/

// chunkGenerator.h
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
