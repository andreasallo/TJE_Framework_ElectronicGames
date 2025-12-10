#include "chunkGenerator.h"
#include "framework/framework.h"
#include <cmath>
#include <algorithm>
#include "framework/world.h"
#include <iostream>

void ChunkGenerator::init(float safeZone, float chunkLen, float totalLen) {
    safeZoneInitial = safeZone;
    chunkLength = chunkLen;
    totalLevelLength = totalLen;
	rng.seed(42); //semilla fixa per reproducibilitat (he ficat 42 pero pot ser qualsevol num), la secuencia de chunks serà sempre la mateixa
	dist = std::uniform_real_distribution<float>(0.0f, 1.0f);//rang 0-1. agafem qualsevol num aleatori i el transformem a aquest rang, aixi es mes facil escalar-lo després
	nextChunkZ = safeZoneInitial; //generem despres de la zona segura, es per evitar que apareguin asteroides a sobre del jugador al iniciar
}

void ChunkGenerator::update(float dt, float playerZ) {
    float lookahead = 1200.0f; //Ajustat per generar abans i evitar buits. AIXI TMB GENERES MES CHUNKS Q NO NOMES L'ACTUAL, SINO ELS METEORITOS ET MENJEN.
    float targetZ = playerZ + lookahead;
    while (nextChunkZ < targetZ && nextChunkZ < totalLevelLength) {
        generateChunk(nextChunkZ);
        nextChunkZ += chunkLength;
    }
}

void ChunkGenerator::generateChunk(float baseZ) {
    //% del process completat 
    float progress = (baseZ - safeZoneInitial) / (totalLevelLength - safeZoneInitial);

    AsteroidControl& ast = World::instance->asteroidControl;
    RingControl& ring = World::instance->ringControl;

	//augmentar dificultat, no nomes amb els patrons sino amb la velocitat
    float speedMin = 18.0f + progress * 10.0f;   //MOLT MÉS BAIXA al principi ( es veia mas rapid tot)
    float speedMax = 30.0f + progress * 30.0f;

    //debug
    std::cout << "Chunk at Z=" << baseZ << " Progress=" << progress << std::endl;

    if (progress < 0.2f) { // 0-20%: Pluja suau
        std::cout << "  Patró: Pluja suau" << std::endl;
        for (int i = 0; i < 6; ++i) { // +2 per més densitat
            float relZ = (i / 9.0f) * 0.9f; 
            float x = -12.0f + frand() * 24.0f;
            float y = frand() * 24.0f;
            ast.spawnAsteroidAt(x, y, baseZ + relZ * chunkLength, speedMin);
        }
        if (frand() < 0.5f) {
            float rx = -6.0f + frand() * 12.0f;
            float ry = 8.0f + frand() * 12.0f;
            ring.spawnRingAt(rx, ry, baseZ + 0.7f * chunkLength, speedMin);
        }
    }
    else if (progress < 0.5f) { // 20-50%: Mur amb forat 
        std::cout << "  Patró: Mur amb forat" << std::endl;
        float wallZ = baseZ + 0.5f * chunkLength;
        float gapX = -6.0f + frand() * 12.0f;
        float gapY = 6.0f + frand() * 12.0f;

        //Graella amb pas més ample (5u) per visibilitat
        for (float x = -12.0f; x <= 12.0f; x += 5.0f) {
            for (float y = 0.0f; y <= 24.0f; y += 5.0f) {
                if (fabsf(x - gapX) < 6.0f && fabsf(y - gapY) < 6.0f) continue; // Forat 12x12u
                ast.spawnAsteroidAt(x, y, wallZ, speedMax);
            }
        }
        ring.spawnRingAt(gapX, gapY, wallZ + 30.0f, speedMax);
    }
    else if (progress < 0.8f) { // 50-80%: Pluja intensa
        std::cout << "  Patró: Pluja intensa" << std::endl;
        for (int i = 0; i < 18; ++i) { // +4 per densitat alta
            float relZ = (i / 23.0f) * 0.9f;
            float x = -12.0f + frand() * 24.0f;
            float y = frand() * 24.0f;
            ast.spawnAsteroidAt(x, y, baseZ + relZ * chunkLength, speedMax * (1.0f + frand() * 0.3f));
        }
        if (frand() < 0.3f) {
            float rx = -7.0f + frand() * 14.0f;
            float ry = 8.0f + frand() * 12.0f;
            ring.spawnRingAt(rx, ry, baseZ + 0.8f * chunkLength, speedMax * (1.0f + frand() * 0.3f));
        }
    }
    else if (progress < 0.95f) { // 80-95%: Onades/zigzag 
        std::cout << "  Patró: zigzag" << std::endl;
        float startZ = baseZ + 0.2f * chunkLength;
        float gapX = -5.0f + frand() * 10.0f;

        // Línies diagonals/zigzag en 4 onades
        for (int wave = 0; wave < 4; ++wave) {
            float waveZ = startZ + (wave / 3.0f) * 0.6f * chunkLength;
            float offsetX = sin(waveZ * 0.1f) * 8.0f; // Moviment sinuós
            for (int i = 0; i < 8; ++i) {
                float x = -12.0f + (i / 7.0f) * 24.0f + offsetX;
				if (fabsf(x - gapX) < 5.0f) continue;  //fabsf es per valor absolut de float, evitar problemes amb negatius
                float y = 6.0f + cos(waveZ * 0.1f + i * 0.5f) * 8.0f; 
                ast.spawnAsteroidAt(x, y, waveZ, speedMax * 1.6f);
            }
        }
        ring.spawnRingAt(gapX, 12.0f, startZ + 0.6f * chunkLength, speedMax * 1.6f);
    }
    else { // 95-100%: ANIMACIO planeta 
        if (!World::instance->level_finished) {
            World::instance->level_finished = true;
            World::instance->final_sequence_timer = 0.0f;
            World::instance->final_collided = false;

            float startZ = totalLevelLength + 900.0f; // desde on començo a veurel, si vull q aparegui quan encara estic mes lluny fico un numero mes gran.

            //Guardem la posició inicial perquè l'animació la faci World cada frame
            World::instance->planet_initial_z = startZ;

            World::instance->end_planet->model.setIdentity();
            World::instance->end_planet->model.setTranslation(0.0f, 0.0f, startZ);
            World::instance->end_planet->model.scale(4.0f, 4.0f, 4.0f); // petit al fons

            std::cout << "--- PLANETA INICIAT (Z = " << startZ << ") ---" << std::endl;
        }
    }
}
        