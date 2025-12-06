#pragma once
#include <vector>
#include "framework/utils.h"

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