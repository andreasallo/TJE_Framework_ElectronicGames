#include "chunkGenerator.h"
#include "framework/framework.h"
#include <cmath>
#include <algorithm>

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