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


void RingControl::init() {
    ringMesh = Mesh::Get("data/final_ring.obj");
    ringMaterial.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    ringMaterial.diffuse = Texture::Get("data/01tizeta_floor_e.png");
    ringMaterial.color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
    spawnTimer = 0.0f;
}

void RingControl::update(float dt)
{
    Player* player = World::instance->player;
    float worldSpeedFactor = (player && player->turbo) ? 2.0f : 1.0f;

    for (Ring* r : rings) {
        if (!r || r->toDelete) continue;
        float orig = r->speed;
        r->speed *= worldSpeedFactor;
        r->update(dt);
        r->speed = orig;
    }

    if (player) {
        for (Ring* r : rings) {
            if (!r || r->toDelete) continue;
            Vector3 pc = player->getCollisionCenter();
            Vector3 rc = r->model.getTranslation();
            float dist = pc.distance(rc);

			if (dist < r->collision_radius - player->collision_radius) { //col·lisió. NO UTILITZEM LA FUNCIO D SPHERE COLLISION PERQUÈ VOLEM UNA DISTÀNCIA MENYS RESTRICTIVA. HA DE DETECTAR EL CENTRE DEL RING I NO LA ESFERA COMPLETA.
                r->toDelete = true;
                World::instance->destroyEntity(r);  

                std::cout << "RING! +3s turbo" << std::endl;
                Audio::Play("data/coin.wav", 1.0f, BASS_SAMPLE_MONO);
            
                player->coins_collected++;
                player->turbo_timer = player->turbo_duration;
                player->turbo_bar->mask = 1.0f;
            }
        }
    }

    rings.erase(
        std::remove_if(rings.begin(), rings.end(),
            [](Ring* r) { return !r || r->toDelete; }),
        rings.end()
    );
}

void RingControl::spawnRingAt(float x, float y, float z, float speed) {
    Ring* r = new Ring(ringMesh, ringMaterial, "ring");
    r->model.setTranslation(x, y, z);
	r->speed = speed;
    r->layer = eCollisionFilter::RING;
    World::instance->addEntity(r);
    rings.push_back(r);
}