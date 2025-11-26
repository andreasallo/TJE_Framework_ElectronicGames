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

    //sincronitzar amb els rings
    patternTimer += dt;
    if (patternTimer >= patternCycleTime)
    {
        patternTimer = 0.0f;
        currentPattern = (eSpawnPattern)((currentPattern + 1) % PATTERN_TOTAL);
    }

    Player* player = World::instance->player;
    float worldSpeedFactor = player && player->turbo ? 2.0f : 1.0f;

	//Spawn // turbo afecta spawn rate
    spawnTimer += dt;
	float currentSpawnInterval = player->turbo ? spawnInterval * 0.6f : spawnInterval; // per el turbo, fem que apareguin més sovint, un 40% més ràpid

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
            a->speed = originalSpeed * 4.0f; 
        else
            a->speed = originalSpeed;

        a->update(dt);

        //restaurar velocitat original (no la volem guardar modificada)
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
               
                a->toDelete = true;
                World::getInstance()->destroyEntity(a);

                //resta vida 
				//AQUI PODREIEM AFEGIR UNES PARTICULES O EFECTE DE CRASH AL METORIT
                player->lives = std::max(0, player->lives - 1);

                Audio::Play("data/crash.wav", 1.0f, BASS_SAMPLE_MONO);
                std::cout << "Player hit! lives: " << player->lives << std::endl;

            }
        }
    }
    //Neteja vector d'asteroids (els marcats per eliminar a asteroid)
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

   
    float zSpawn = 220.0f; 
    
    const float tunnelMinY = -6.0f;
    const float tunnelMaxY = 20.0f;
    const float ringLowLimit = 0.0f;
    const float ringMidLimit = 10.0f;
    const float ringHighLimit = 16.0f;

    

    float minY = -10.0f, maxY = 10.0f;

    // Genera fora del rectangle que ocupa el jugador
    /*do {
        x = random(-18.0f, 18.0f);
        y = random(-10.0f, 25.0f);

    } while (x > pMinX && x < pMaxX && y > pMinY && y < pMaxY);*/

    float x = random(-20.f, 7.f);     // dins del túnel
    //float y = random(-10.f, 10.f);       // centrat amb el player
    float y = 0.0f;
   

    switch (currentPattern) {
    case PATTERN_LOW:
        // Ring is LOW [-6, 4] -> Asteroid avoids it, targeting MID/HIGH [4, 10]
        y = random(ringLowLimit, tunnelMaxY);
        break;
    case PATTERN_MID:
        if (random(0.0f, 1.0f) > 0.5f) // Randomly choose low or high complementary area
            y = random(tunnelMinY, ringLowLimit); // Low area
        else
            y = random(ringMidLimit, tunnelMaxY); // High area
        break;
    case PATTERN_HIGH:
        // Ring is HIGH [14, 20] -> Asteroid avoids it, targeting MID/LOW [-10, 10]
        
        if (random(0.0f, 1.0f) > 0.5f)
            y = random(tunnelMinY, ringLowLimit); // Low zone 
        else
            y = random(ringMidLimit, tunnelMaxY); // Mid zone 
        break;
    default:
        y = random(tunnelMinY,tunnelMaxY);
        break;
    }
    
    float z = p.z + 80.0f;
    //float z = player->model.getTranslation().z + zSpawn;
   

    a->model.setTranslation(x, y, z);

    a->speed = random(2.0f, 6.0f);//més lent
	a->collision_radius = 2.0f; //RADI PERFECTE PER METEORITS ACTUALS, SI CANVIEM EL MESH CALDRIA TOCAR AQUI
    a->layer = eCollisionFilter::ENEMY;

    World::instance->addEntity(a);
    asteroids.push_back(a);
}
