
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

void AsteroidControl::init() {
    asteroidMesh = Mesh::Get("data/meteorito.obj");
    asteroidMat.diffuse = Texture::Get("data/StoneFloorTexture.png");
    asteroidMat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
 
}

void AsteroidControl::update(float dt)
{
    Player* player = World::instance->player;

    //ACCELERAR TOT EL MON AMB TURBO, OJO
    float worldSpeedFactor = (player && player->turbo) ? 2.0f : 1.0f;

    //movement
    for (Asteroid* a : asteroids) {
        if (!a || a->toDelete) continue;
        float originalSpeed = a->speed;
        a->speed *= worldSpeedFactor;
        a->update(dt);
        a->speed = originalSpeed;
    }

    //colisions
    if (player) {
        for (Asteroid* a : asteroids) {
            if (!a || a->toDelete) continue;

            std::vector<sCollisionData> results;
            bool hit = Collision::TestEntitySphere(a, player->collision_radius, player->getCollisionCenter(), results, eCollisionFilter::ENEMY);

            if (hit) {
                a->toDelete = true;
                World::instance->destroyEntity(a); 

                player->previous_lives = player->lives;
                player->lives -= 1; //std::max(0, player->lives - 1);
                player->health_bar->mask = (float)player->lives / player->max_lives;
                World::instance->chromatic_aberration_timer = 1.0f;
                Audio::Play("data/crash.wav", 1.0f, BASS_SAMPLE_MONO);

                if (player->lives <= -1) { //-1
                    /*NOU NOU NOU*/
                    Game* instance = Game::instance;

                    player->isDestroyed = true;
                    World::instance->level_finished = true;
                    std::cout << "GAME OVER" << std::endl;
					Audio::Play("data/8-bit-game-over-sound-effect-331435.wav", 1.0f, BASS_SAMPLE_MONO);
                    instance->setStage(END_STAGE, PLAY_STAGE);
                }
            }
        }
    }

    // Solo quitamos del vector los que están marcados (NO los borramos otra vez)
    asteroids.erase(
        std::remove_if(asteroids.begin(), asteroids.end(),
            [](Asteroid* a) { return a == nullptr || a->toDelete; }),
        asteroids.end()
    );
}

//Aquesta funcio s'utilitza a chunkGenerator per spawnar meteorits a posicions concretes
void AsteroidControl::spawnAsteroidAt(float x, float y, float z, float speed) {
    Asteroid* a = new Asteroid(asteroidMesh, asteroidMat, "asteroid");
    a->model.setTranslation(x, y, z);
    a->speed = speed;
    a->layer = eCollisionFilter::ENEMY;
    World::instance->addEntity(a);          
    asteroids.push_back(a);
}