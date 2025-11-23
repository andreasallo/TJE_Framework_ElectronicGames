#include "asteroidControl.h"
#include "game/asteroid.h"
#include "framework/world.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "framework/utils.h"
#include "framework/collision.h"
#include "game/player.h"


void AsteroidControl::init()
{
    asteroidMesh = Mesh::Get("data/meteorito.obj");
    asteroidMat.diffuse = Texture::Get("data/textures/texture.tga");
    spawnTimer = spawnInterval;
}

void AsteroidControl::update(float dt)
{

    Player* player = World::instance->player;

    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;
        spawnAsteroid();
    }

    // Update de cada meteorit
    for (Asteroid* a : asteroids) {
        if (a && !a->toDelete) {
            a->update(dt);
        }
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

    // Genera fora del rectangle que ocupa el jugador
    /*do {
        x = random(-18.0f, 18.0f);
        y = random(-10.0f, 25.0f);

    } while (x > pMinX && x < pMaxX && y > pMinY && y < pMaxY);*/

    float x = random(-20.f, 7.f);     // dins del túnel
    float y = random(-10.f, 10.f);       // centrat amb el player
    float z = p.z + 80.0f;

    a->model.setTranslation(x, y, z);

    a->speed = random(2.0f, 6.0f);         // més lent
	a->collision_radius = 2.0f; //RADI PERFECTE PER METEORITS ACTUALS
    a->layer = eCollisionFilter::ENEMY;

    World::instance->addEntity(a);
    asteroids.push_back(a);
}
