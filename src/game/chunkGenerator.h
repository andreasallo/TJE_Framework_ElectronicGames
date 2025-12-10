
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

	//generador de num aleatoris, es millor que el rand. (pero fa el mateix, generar num aleatoris)
    std::mt19937 rng;
	//serveix per definir el rang dels num aleatoris, ja q rng dona num aleatoris molt grans
    std::uniform_real_distribution<float> dist;
	//retorna un num aleatori entre 0 i 1. el motor treu un num amb rng i dist el transforma al rang 0-1.
    float frand() { return dist(rng); }

	static constexpr float PLAYER_MIN_X = -12.0f; //constexpr significa que es una constant en temps de compilació, si no ho fiquem peta el codi. Això és perquè es fa servir en arrays i altres estructures que necessiten saber la mida en temps de compilació.
    static constexpr float PLAYER_MAX_X = 12.0f;
    static constexpr float PLAYER_MIN_Y = 0.0f;
    static constexpr float PLAYER_MAX_Y = 24.0f;
};
