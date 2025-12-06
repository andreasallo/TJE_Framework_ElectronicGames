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



void AsteroidControl::init()
{
    asteroidMesh = Mesh::Get("data/meteorito.obj");
    asteroidMat.diffuse = Texture::Get("data/StoneFloorTexture.png");
	asteroidMat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Asteroid* new_asteroid = new Asteroid(asteroidMesh, asteroidMat, "asteroid");
    spawnTimer = 0.0f;
   
}

void AsteroidControl::update(float dt)
{

    Player* player = World::instance->player;
    float worldSpeedFactor = (player && player->turbo) ? 2.0f : 1.0f;

    for (Asteroid* a : asteroids) {
        if (!a || a->toDelete) continue;

        // augmentar la velocitat real
        float originalSpeed = a->speed;
        a->speed = originalSpeed * worldSpeedFactor;
        a->update(dt);
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

                /*----------------Això és nou----------------*/
                //actualitza la mask de la barra de vida quan la nau rep un cop
                player->health_bar->mask = (float)(player->lives / player->max_lives);

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

void AsteroidControl::spawnAsteroidAt(float x, float y, float z, float speed)
{
    Asteroid* a = new Asteroid(asteroidMesh, asteroidMat, "asteroid");
    a->model.setTranslation(Vector3(x, y, z));
    a->speed = speed;
    a->layer = eCollisionFilter::ENEMY;
    World::instance->root->addChild(a);
    asteroids.push_back(a);
}

void AsteroidControl::spawnAsteroid()
{
    // fallback: random spawn ahead of player
    Player* p = World::instance->player;
    Vector3 pos = p->model.getTranslation();
    float x = random(-8.0f, 8.0f);
    float y = random(-5.0f, 10.0f);
    float z = pos.z + random(80.0f, 140.0f);
    float s = random(2.0f, 6.0f);
    spawnAsteroidAt(x, y, z, s);
}
