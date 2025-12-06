#include "RingControl.h"
#include "framework/world.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "framework/utils.h"
#include "framework/collision.h"
#include "framework/entities/entityCollider.h"
#include "game/player.h"
#include "framework/audio.h"

void RingControl::init()
{
    ringMesh = Mesh::Get("data/final_ring.obj");
    ringMaterial.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    ringMaterial.diffuse = Texture::Get("data/01tizeta_floor_e.png");  // ES UNA TEXTURA BLANCA AIXI SI FIQUES UN COLOR PER SOBRE ES VEURA BE, PER DEFECTE SINO FIQUES TEXTURA EL SHADER LA FICA NEGRA.
    ringMaterial.color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
    Ring* new_ring = new Ring(RingControl::ringMesh, RingControl::ringMaterial);
    //ringMaterial.diffuse = Texture::Get("data/textures/"); //OJOOO
    spawnTimer = 0.0f;
}

void RingControl::update(float dt)
{


    Player* player = World::instance->player;
    float worldSpeedFactor = (player && player->turbo) ? 2.0f : 1.0f;

    for (Ring* r : rings)
    {
        if (!r || r->toDelete) continue;
        // augment velocitat segons turbo
        float orig = r->speed;
        r->speed = orig * worldSpeedFactor;
        r->update(dt);
        r->speed = orig;

    }

    // collisions: detectar si el centre del player entra dins de l'inner_radius
    if (player) {
        for (Ring* r : rings) {
            if (!r || r->toDelete) continue;
            Vector3 pc = player->getCollisionCenter();
            Vector3 rc = r->model.getTranslation();
            float dist = pc.distance(rc);

            // considerem "passar pel forat" si centre entra dins inner radius - player radius
            if (dist < r->collision_radius - player->collision_radius) {
                /* -------------- AIXÒ ÉS NOU --------------*/
            // +3 segons de turbo
                std::cout << "HAS AGAFAT UN RING! +1 moneda +3s turbo" << std::endl;
                Audio::Play("data/coin.wav", 1.0f, BASS_SAMPLE_MONO);
                Audio::Play("data/turbo.wav", 2.0f, BASS_SAMPLE_MONO);

                player->coins_collected++;
                /* -------------- AIXÒ ÉS NOU --------------*/
                //he comentat aquesta linia per què no s'activi el turbo quan passes per un anell
                //player->turbo = true;
                player->turbo_timer = player->turbo_duration;
                /*----------------Això és nou----------------*/
                //Emplena la mask de la barra de turbo al complert cada vegada que passes per dins d'un anell
                player->turbo_bar->mask = 1;

                r->toDelete = true;
                World::instance->destroyEntity(r);
            }
        }
    }

    rings.erase(
        std::remove_if(
            rings.begin(), rings.end(),
            [](Ring* r) { return !r || r->toDelete; }),
        rings.end()
    );
}

void RingControl::spawnRingAt(float x, float y, float z)
{
    Ring* r = new Ring(ringMesh, ringMaterial, "ring");
    r->model.setTranslation(Vector3(x, y, z));
    r->layer = eCollisionFilter::ENEMY;
    World::instance->root->addChild(r);
    rings.push_back(r);
}

/*
void RingControl::spawnRing()
{
    Player* p = World::instance->player;
    Vector3 pos = p->model.getTranslation();
    float x = random(minX, maxX);
    float y = random(minY, maxY);
    float z = pos.z + random(60.0f, 120.0f);
    spawnRingAt(x, y, z);

}*/