#include "RingControl.h"
#include "framework/world.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "framework/utils.h"
#include "framework/collision.h"
#include "framework/entities/entityCollider.h"
#include "game/player.h"

void RingControl::init()
{
    ringMesh = Mesh::Get("data/ring.obj");
    ringMaterial.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    ringMaterial.diffuse = Texture::Get("data/textures/texture.tga"); //OJOOO
}

void RingControl::update(float dt)
{
    spawnTimer += dt;
    if (spawnTimer >= spawnInterval)
    {
        spawnTimer = 0.0f;
        spawnRing();
    }
    Player* player = World::instance->player;
    for (Ring* r : rings)
    {
        if (!r || r->toDelete) continue;

        std::vector<sCollisionData> hits;
        bool col = Collision::TestEntitySphere(
            r,
            player->collision_radius,
            player->getCollisionCenter(),
            hits,
            eCollisionFilter::ENEMY
        );

        if (col)
        {
            std::cout << "HAS AGAFAT UN RING! +1 moneda" << std::endl;
            player->coins_collected++;
            player->turbo = true;
            player->turbo_timer = player->turbo_duration;

            r->toDelete = true;
            World::instance->destroyEntity(r);
        }
    }

    rings.erase(
        std::remove_if(
            rings.begin(), rings.end(),
            [](Ring* r) { return !r || r->toDelete; }),
        rings.end()
    );
}

void RingControl::spawnRing()
{
    Ring* r = new Ring(ringMesh, ringMaterial, "ring");

    float x = random(minX, maxX);
    float y = random(minY, maxY);
    float z = random(80.0f, 120.0f);

    r->model.setTranslation(Vector3(x, y, z));

    World::instance->root->addChild(r);
    rings.push_back(r);
}