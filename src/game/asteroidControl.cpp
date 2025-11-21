#include "asteroidControl.h"
#include "game/asteroid.h"
#include "framework/world.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "framework/utils.h"


void AsteroidControl::init()
{
    asteroidMesh = Mesh::Get("data/meteorito.obj");
    asteroidMat.diffuse = Texture::Get("data/textures/Atlas.png");
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
    for (auto* a : asteroids)
        a->update(dt);
	// Netejar els meteorits marcats per eliminar
    asteroids.erase(std::remove_if(asteroids.begin(),asteroids.end(),[](Asteroid* a) { 
        return a == nullptr || a->toDelete; 
        }
        ),
        asteroids.end()
    );
}

void AsteroidControl::spawnAsteroid()
{
    Asteroid* a = new Asteroid(asteroidMesh, asteroidMat, "asteroid");

    // Posició aleatòria dins del túnel
    float x = random(-12.f, 12.f);
    float y = random(-6.f, 20.f);
    float z = 80.0f; // lluny endavant

    a->model.setTranslation(x, y, z);

    // Velocitat aleatòria
    a->speed = random(20.f, 45.f);

    World::instance->addEntity(a);
    asteroids.push_back(a);
}
