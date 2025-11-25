#include "asteroidControl.h"
#include "game/asteroid.h"
#include "framework/world.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "framework/utils.h"
#include "framework/collision.h"
#include "game/player.h"
#include "framework/audio.h"

// Define a static timer for pattern cycling, in sync with rings
static float patternTimer = 0.0f;
const float patternCycleTime = 6.0f;

void AsteroidControl::init()
{
    asteroidMesh = Mesh::Get("data/meteorito.obj");
    asteroidMat.diffuse = Texture::Get("data/textures/texture.tga");
    spawnTimer = spawnInterval;
}

void AsteroidControl::update(float dt)
{

    // Sync pattern with rings (but keep a separate timer logic for modularity)
    patternTimer += dt;
    if (patternTimer >= patternCycleTime)
    {
        patternTimer = 0.0f;
        currentPattern = (eSpawnPattern)((currentPattern + 1) % PATTERN_TOTAL);
        // Debug: std::cout << "Asteroid pattern changed to: " << currentPattern << std::endl;
    }

    Player* player = World::instance->player;
    float worldSpeedFactor = player && player->turbo ? 2.0f : 1.0f;

    // Spawn amb turbo una mica més ràpid
    spawnTimer += dt;
    float currentSpawnInterval = player->turbo ? spawnInterval * 0.6f : spawnInterval;

    spawnTimer += dt;
    if (spawnTimer >= currentSpawnInterval) {
        spawnTimer = 0.0f;
        spawnAsteroid();
    }

    for (Asteroid* a : asteroids) {
        if (!a || a->toDelete) continue;

        // augmentar la velocitat real
        float originalSpeed = a->speed;

        if (player->turbo)
            a->speed = originalSpeed * 2.0f;   // o 1.5 / 2.5 etc segons gust
        else
            a->speed = originalSpeed;

        a->update(dt);

        // restaurar velocitat original (no la volem guardar modificada)
        a->speed = originalSpeed;
    }


    if (player) {
        for (Asteroid* a : asteroids) {
            if (!a || a->toDelete) continue;

            a->layer = eCollisionFilter::ENEMY;
            std::vector<sCollisionData> results;
            //Fem la esfera amb el centre del player i el radi del player.
            bool hit = Collision::TestEntitySphere(a,player->collision_radius,player->getCollisionCenter(),results,eCollisionFilter::ENEMY);

            if (hit) {
                // impacto!
                a->toDelete = true;
                World::getInstance()->destroyEntity(a);

                // resta vida i reacciona el player
                player->lives = std::max(0, player->lives - 1);

                Audio::Play("data/crash.wav", 1.0f, BASS_SAMPLE_MONO);
                std::cout << "Player hit! lives: " << player->lives << std::endl;

                //Play sound, spawn particle, camera shake...
            }
        }
    }
    // Neteja vector d'asteroids (els marcats per eliminar)
    asteroids.erase(
        std::remove_if(
            asteroids.begin(),
            asteroids.end(),
            [](Asteroid* a) { return a == nullptr || a->toDelete; }
        ),
        asteroids.end()
    );
}

void AsteroidControl::spawnAsteroid()
{
    Asteroid* a = new Asteroid(asteroidMesh, asteroidMat, "asteroid");
    Player* player = World::instance->player;
    Vector3 p = player->model.getTranslation();

    // Spawn lluny, dins l'espai del túnel però fora de la zona del jugador.
    float zSpawn = 120.0f; // davant del jugador (ajusta segons sistema)
    

    // zona prohibida: utilitza els limits del player si són públics
    float pMinX = -12.0f, pMaxX = 12.0f;
    float pMinY = -6.0f, pMaxY = 20.0f;

    float minY = -10.0f, maxY = 10.0f;

    // Genera fora del rectangle que ocupa el jugador
    /*do {
        x = random(-18.0f, 18.0f);
        y = random(-10.0f, 25.0f);

    } while (x > pMinX && x < pMaxX && y > pMinY && y < pMaxY);*/

    float x = random(-20.f, 7.f);     // dins del túnel
    //float y = random(-10.f, 10.f);       // centrat amb el player
    float y = 0.0f;
    // Complementary vertical spawning based on pattern to encourage movement to the ring lane
    switch (currentPattern) {
    case PATTERN_LOW:
        // Ring is LOW [-6, 4] -> Asteroid avoids it, targeting MID/HIGH [4, 10]
        y = random(4.0f, maxY);
        break;
    case PATTERN_MID:
        // Ring is MID [4, 14] -> Asteroid avoids it, targeting LOW [-10, 4]
        y = random(minY, 4.0f);
        break;
    case PATTERN_HIGH:
        // Ring is HIGH [14, 20] -> Asteroid avoids it, targeting MID/LOW [-10, 10]
        // We use a broader range since asteroid max Y is 10.
        if (random(0.0f, 1.0f) > 0.5f)
            y = random(minY, 0.0f); // Low zone [-10, 0]
        else
            y = random(0.0f, maxY); // Mid zone [0, 10]
        break;
    default:
        y = random(minY, maxY);
        break;
    }
    
    float z = p.z + 80.0f;

   

    a->model.setTranslation(x, y, z);

    a->speed = random(2.0f, 6.0f);         // més lent
	a->collision_radius = 2.0f; //RADI PERFECTE PER METEORITS ACTUALS
    a->layer = eCollisionFilter::ENEMY;

    World::instance->addEntity(a);
    asteroids.push_back(a);
}
