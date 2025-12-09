#include "chunkGenerator.h"
#include "framework/framework.h"
#include <cmath>
#include <algorithm>

/*

const float GEN_MIN_X = -12.0f;
const float GEN_MAX_X = 12.0f;
const float GEN_MIN_Y = -6.0f;
const float GEN_MAX_Y = 20.0f;

ChunkGenerator::ChunkGenerator()
// CAMBIO CLAVE: spawnPadding a 1200.
// ens marca la longitud abans del jugador on es generen chunks nous.
    : levelLength(2500.f), chunkLength(120.f), nextChunkZ(0.f), spawnPadding(1200.f)
{
}

ChunkGenerator::~ChunkGenerator() {}

void ChunkGenerator::init(float _levelLength, float _chunkLength)
{
    levelLength = _levelLength;
    chunkLength = _chunkLength;
    chunks.clear();
    nextChunkZ = 40.0f;
}

void ChunkGenerator::reset() {
    init(levelLength, chunkLength);
}

void ChunkGenerator::update(float dt, const Vector3& playerPos)
{
	// Generar terreno dinámicamente con suficiente antelación-> AIXO ES LO DEL SPAWN PADDING
    while (nextChunkZ < playerPos.z + spawnPadding && nextChunkZ < levelLength) {
        generateChunkAt(nextChunkZ);
        nextChunkZ += chunkLength;
    }
}

void ChunkGenerator::generateChunkAt(float startZ)
{
    sChunk c;
    c.startZ = startZ;
    c.length = chunkLength;
    c.type = CHUNK_MIXED;

    fillPattern(c);
    chunks.push_back(c);
}

void ChunkGenerator::fillPattern(sChunk& chunk)
{
    float progress = chunk.startZ / levelLength;
    int difficulty = (int)(progress * 10.0f);

    // --- FASE 4:(90%+) ---
    if (progress > 0.90f) {
        createFinalStormPattern(chunk);
        return;
    }

    // --- FASE 3: ZONA DIFÍCIL (60% - 90%) ---
    if (progress > 0.60f) {
        int r = rand() % 100;
        if (r < 40) createWallPattern(chunk, difficulty);
        else if (r < 70) createStormPattern(chunk, difficulty);
        else createLanePattern(chunk, difficulty);
        return;
    }

    // --- FASE 2: ZONA MEDIA (30% - 60%) ---
    if (progress > 0.30f) {
        int r = rand() % 100;
        if (r < 50) createLanePattern(chunk, difficulty);
        else if (r < 80) createZigZagPattern(chunk, difficulty);
        else createStormPattern(chunk, difficulty);
        return;
    }

    // --- FASE 1: INICIO (0% - 30%) ---
    // Aquí forzamos que haya variedad y que no esté vacío el centro.
    // Damos más peso a LanePattern que llena mejor el espacio.
    if (rand() % 100 < 70) createLanePattern(chunk, difficulty);
    else                   createZigZagPattern(chunk, difficulty);
}

// --------------------------------------------------------
// PATRÓN 1: CARRILES (Optimizado para llenar el centro)
// --------------------------------------------------------
void ChunkGenerator::createLanePattern(sChunk& chunk, int difficulty)
{
    float lanesX[] = { -9.0f, 0.0f, 9.0f }; // Izquierda, Centro, Derecha
    float lanesY[] = { -2.0f, 6.0f, 13.0f }; // Abajo, Medio, Arriba

    // Número de filas de obstáculos en este chunk
    int numRows = 5 + (difficulty / 2);
    float zStep = chunk.length / (float)numRows;

    for (int i = 0; i < numRows; i++) {

   
        // Esto garantiza distribución uniforme.
        int lx = rand() % 3;
        int ly = rand() % 3;

        sAsteroidSpawn a;
        a.x = lanesX[lx];
        a.y = lanesY[ly];
        a.z = chunk.startZ + (i * zStep);
        // Velocidad base + pequeña variación
        a.speed = 18.0f + (difficulty * 1.5f) + random(2.0f, -1.0f);
        chunk.asteroids.push_back(a);

        // Doble obstáculo (mayor dificultad)
        if (difficulty > 3 && rand() % 3 == 0) {
            int lx2 = (lx + 1) % 3; // Otro carril
            sAsteroidSpawn a2 = a;
            a2.x = lanesX[lx2];
            chunk.asteroids.push_back(a2);
        }

        // Aro
        if (rand() % 6 == 0) {
            int rx = (lx + 2) % 3;
            sRingSpawn r;
            r.x = lanesX[rx]; r.y = lanesY[ly]; r.z = a.z;
            chunk.rings.push_back(r);
        }
    }
}

void ChunkGenerator::createZigZagPattern(sChunk& chunk, int difficulty)
{
    int numObjects = 5;
    float zStep = chunk.length / (float)numObjects;
    float currentZ = chunk.startZ + 10.0f;

    // ZigZag empieza aleatoriamente
    float side = (rand() % 2 == 0) ? -1.0f : 1.0f;
    float xOffset = 9.0f;

    for (int i = 0; i < numObjects; i++) {
        sAsteroidSpawn a;
        a.x = side * xOffset;
        a.y = 6.0f + sin(currentZ * 0.2f) * 8.0f;
        a.z = currentZ;
        a.speed = 18.0f + (difficulty * 1.5f);
        chunk.asteroids.push_back(a);

        // A veces poner algo en el centro para que no sea tan vacío
        if (i % 2 == 0 && rand() % 2 == 0) {
            sAsteroidSpawn centerA = a;
            centerA.x = 0.0f; // Bloqueo central
            centerA.y = a.y - 5.0f; // Diferente altura
            chunk.asteroids.push_back(centerA);
        }

        if (i == 2) { // Aro en medio del patrón
            sRingSpawn r;
            r.x = 0.0f; r.y = 6.0f; r.z = currentZ + (zStep * 0.5f);
            chunk.rings.push_back(r);
        }
        side *= -1.0f;
        currentZ += zStep;
    }
}

void ChunkGenerator::createWallPattern(sChunk& chunk, int difficulty)
{
    float zPos = chunk.startZ + (chunk.length * 0.5f);
    int cols = 4;
    int rows = 4;
    float startX = -10.0f; float stepX = 6.6f;
    float startY = -3.0f;  float stepY = 6.0f;
    int gapCol = rand() % cols;
    int gapRow = rand() % rows;

    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            if (c == gapCol && r == gapRow) {
                sRingSpawn rng;
                rng.x = startX + (c * stepX);
                rng.y = startY + (r * stepY);
                rng.z = zPos;
                chunk.rings.push_back(rng);
            }
            else {
                sAsteroidSpawn a;
                a.x = startX + (c * stepX) + random(1.0f, -0.5f);
                a.y = startY + (r * stepY) + random(1.0f, -0.5f);
                a.z = zPos;
                a.speed = 12.0f + (difficulty * 0.5f);
                chunk.asteroids.push_back(a);
            }
        }
    }
}

void ChunkGenerator::createStormPattern(sChunk& chunk, int difficulty)
{
    int numAsteroids = 10 + (difficulty / 2);

    for (int i = 0; i < numAsteroids; i++) {
        sAsteroidSpawn a;
        // Rango completo
        a.x = random(GEN_MAX_X - GEN_MIN_X, GEN_MIN_X);
        a.y = random(GEN_MAX_Y - GEN_MIN_Y, GEN_MIN_Y);

        float segment = chunk.length / numAsteroids;
        a.z = chunk.startZ + (i * segment) + random(10.0f, -5.0f);

        a.speed = 22.0f + (difficulty * 1.5f) + random(5.0f, -2.5f);
        chunk.asteroids.push_back(a);
    }
}

void ChunkGenerator::createFinalStormPattern(sChunk& chunk)
{
    int numAsteroids = 30;
    for (int i = 0; i < numAsteroids; i++) {
        sAsteroidSpawn a;
        a.x = random(GEN_MAX_X - GEN_MIN_X, GEN_MIN_X);
        a.y = random(GEN_MAX_Y - GEN_MIN_Y, GEN_MIN_Y);
        a.z = chunk.startZ + random(chunk.length, 0.0f);
        a.speed = random(20.0f, 40.0f);
        chunk.asteroids.push_back(a);
    }
}

#include "chunkGenerator.h"
#include "framework/world.h"
#include <cmath>

float ChunkGenerator::randX() { return random(-12.f, 12.f); }
float ChunkGenerator::randY() { return random(0.f, 24.f); }

void ChunkGenerator::init(float startZ, float chunkLen, float totalLen) {
    currentSpawnZ = startZ;
    chunkLength = chunkLen;
    levelLength = totalLen;
    createPatterns();
}

void ChunkGenerator::createPatterns() {

    // ------------------------
    // 1) SOFT RAIN
    // ------------------------
    softRain.name = "Soft Rain";
    for (int i = 0; i < 8; ++i) {
        softRain.spawns.push_back({
            TYPE_ASTEROID,
            randX(),
            randY(),
            random(10.f, 110.f),  // posició dins chunk
            random(0.8f, 1.3f)
            });
    }
    // 1 ring opcional
    softRain.spawns.push_back({
        TYPE_RING,
        randX(),
        random(5.f, 18.f),
        60.f,
        0.f
        });

    // ------------------------
    // 2) HARD RAIN
    // ------------------------
    hardRain.name = "Hard Rain";
    for (int i = 0; i < 18; ++i) {
        hardRain.spawns.push_back({
            TYPE_ASTEROID,
            randX(),
            randY(),
            random(10.f, 110.f),
            random(1.0f, 1.6f)
            });
    }
    // ring ocasional
    hardRain.spawns.push_back({
        TYPE_RING,
        randX(),
        random(5.f, 16.f),
        50.f,
        0.f
        });

    // ------------------------
    // 3) WALL GAP
    // ------------------------
    wallGap.name = "Wall Gap";
    float gapX = random(-8.f, 8.f);

    for (float x = -12.f; x <= 12.f; x += 3.f) {
        if (fabs(x - gapX) < 3.5f) continue; // forat ample i jugable

        for (float y = 2.f; y <= 22.f; y += 5.f) {
            wallGap.spawns.push_back({
                TYPE_ASTEROID,
                x,
                y,
                50.f,      // un mur, no 40 murs
                1.2f
                });
        }
    }

    // ring al forat
    wallGap.spawns.push_back({
        TYPE_RING,
        gapX,
        10.f,
        50.f,
        0.f
        });

    // ------------------------
    // 4) SNAKE WALL (zig-zag)
    // ------------------------
    snakeWall.name = "Snake Wall";
    float sx = -10.f;
    int dir = 1;

    for (int i = 0; i < 6; i++) {
        for (float y = 2.f; y <= 22.f; y += 5.f) {
            snakeWall.spawns.push_back({
                TYPE_ASTEROID,
                sx,
                y,
                15.f + i * 18.f, // progressió slalom
                1.1f
                });
        }

        sx += dir * 6.f;
        if (sx < -10.f || sx > 10.f) {
            dir *= -1;
            sx += dir * 6.f;
        }
    }
}

void ChunkGenerator::update(float dt, float playerZ) {

    while (currentSpawnZ < playerZ + renderDistance &&
        currentSpawnZ < levelLength)
    {
        float progress = playerZ / levelLength;

        const sPattern* p = nullptr;

        if (progress < 0.25f)
            p = &softRain;
        else if (progress < 0.55f)
            p = (rand() % 2 == 0 ? &softRain : &hardRain);
        else if (progress < 0.80f)
            p = &hardRain;
        else
            p = (rand() % 2 == 0 ? &snakeWall : &wallGap);

        spawnChunk(*p, currentSpawnZ);

        currentSpawnZ += chunkLength;
    }
}

void ChunkGenerator::spawnChunk(const sPattern& p, float startZ) {
    for (auto& s : p.spawns) {
        float fz = startZ + s.z;

        if (s.type == TYPE_ASTEROID)
            World::instance->asteroidControl.spawnAsteroidAt(s.x, s.y, fz,
                20.f * s.speedMult);

        else if (s.type == TYPE_RING)
            World::instance->ringControl.spawnRingAt(s.x, s.y, fz);
    }
}*/
// chunkGenerator.cpp (pega esto completo)
// chunkGenerator.cpp  → VERSIÓN ÉPICA Y ESTABLE (2025 edition)

// chunkGenerator.cpp → VERSIÓ FINAL DEFINITIVA (copia i pega exactament això)
#include "chunkGenerator.h"
#include "framework/world.h"

// chunkGenerator.cpp → VERSIÓ FINAL AMB MUR SÒLID I FORAT (copia i pega exactament això)
#include "chunkGenerator.h"
#include "framework/world.h"
// chunkGenerator.cpp → VERSIÓ FINAL VARIADA I SENSE REPETICIÓ
#include "chunkGenerator.h"
#include "framework/world.h"
#include <iostream>

void ChunkGenerator::init(float safeZone, float chunkLen, float totalLen) {
    safeZoneInitial = safeZone;
    chunkLength = chunkLen;
    totalLevelLength = totalLen;
    rng.seed(42); // Mateix mapa; comenta per variació
    dist = std::uniform_real_distribution<float>(0.0f, 1.0f);
    nextChunkZ = safeZoneInitial;
}

void ChunkGenerator::update(float dt, float playerZ) {
    float lookahead = 1200.0f; // Ajustat per generar abans i evitar buits
    float targetZ = playerZ + lookahead;
    while (nextChunkZ < targetZ && nextChunkZ < totalLevelLength) {
        generateChunk(nextChunkZ);
        nextChunkZ += chunkLength;
    }
}

void ChunkGenerator::generateChunk(float baseZ) {
    float progress = (baseZ - safeZoneInitial) / (totalLevelLength - safeZoneInitial);

    AsteroidControl& ast = World::instance->asteroidControl;
    RingControl& ring = World::instance->ringControl;

    float speedMin = 18.0f + progress * 20.0f;   // ← MOLT MÉS BAIXA al principi
    float speedMax = 30.0f + progress * 35.0f;

    // Debug per veure quin patró es genera
    std::cout << "Chunk at Z=" << baseZ << " Progress=" << progress << std::endl;

    if (progress < 0.2f) { // 0-20%: Pluja suau
        std::cout << "  Patró: Pluja suau" << std::endl;
        for (int i = 0; i < 10; ++i) { // +2 per més densitat
            float relZ = (i / 9.0f) * 0.9f; // Uniforme per no buits
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
    else if (progress < 0.5f) { // 20-50%: Mur amb forat (graella menys densa)
        std::cout << "  Patró: Mur amb forat" << std::endl;
        float wallZ = baseZ + 0.5f * chunkLength;
        float gapX = -6.0f + frand() * 12.0f;
        float gapY = 6.0f + frand() * 12.0f;

        // Graella amb pas més ample (5u) per visibilitat
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
        for (int i = 0; i < 24; ++i) { // +4 per densitat alta
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
    else if (progress < 0.95f) { // 80-95%: Onades/zigzag (nou, no graella)
        std::cout << "  Patró: Onades zigzag" << std::endl;
        float startZ = baseZ + 0.2f * chunkLength;
        float gapX = -5.0f + frand() * 10.0f;

        // Línies diagonals/zigzag en 4 onades
        for (int wave = 0; wave < 4; ++wave) {
            float waveZ = startZ + (wave / 3.0f) * 0.6f * chunkLength;
            float offsetX = sin(waveZ * 0.1f) * 8.0f; // Moviment sinuós
            for (int i = 0; i < 8; ++i) {
                float x = -12.0f + (i / 7.0f) * 24.0f + offsetX;
                if (fabsf(x - gapX) < 5.0f) continue; // Forat per passar
                float y = 6.0f + cos(waveZ * 0.1f + i * 0.5f) * 8.0f; // Variació en Y
                ast.spawnAsteroidAt(x, y, waveZ, speedMax * 1.6f);
            }
        }
        ring.spawnRingAt(gapX, 12.0f, startZ + 0.6f * chunkLength, speedMax * 1.6f);
    }
    else { // 95-100%: Vacío + planeta (controlat ÚNICAMENTE aquí)
        if (!World::instance->level_finished) {
            World::instance->level_finished = true;
            World::instance->final_sequence_timer = 0.0f;
            World::instance->final_collided = false;

            // Posició inicial: molt lluny per veure'l de fons
            float startZ = totalLevelLength + 900.0f;

            // Guardem la posició inicial perquè l'animació la faci World cada frame
            World::instance->planet_initial_z = startZ;

            World::instance->end_planet->model.setIdentity();
            World::instance->end_planet->model.setTranslation(0.0f, 0.0f, startZ);
            World::instance->end_planet->model.scale(4.0f, 4.0f, 4.0f); // petit al fons

            std::cout << "--- PLANETA INICIAT LLUNY (Z = " << startZ << ") ---" << std::endl;
        }
    }
}
        