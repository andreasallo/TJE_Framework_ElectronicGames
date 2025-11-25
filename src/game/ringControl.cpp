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

static float patternTimer = 0.0f;
const float patternCycleTime = 6.0f;

void RingControl::init()
{
    ringMesh = Mesh::Get("data/ringg_scale.obj");
    ringMaterial.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    ringMaterial.diffuse = Texture::Get("data/textures/texture.tga"); //OJOOO
}

void RingControl::update(float dt)
{

    patternTimer += dt;
    if (patternTimer >= patternCycleTime)
    {
        patternTimer = 0.0f;
        currentPattern = (eSpawnPattern)((currentPattern + 1) % PATTERN_TOTAL);
        // Debug: std::cout << "Ring pattern changed to: " << currentPattern << std::endl;
    }

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

        r->update(dt);

        // --- DETECCIÓ DE COLLISIÓ PEL FORAT ---
        Vector3 pc = player->getCollisionCenter();
        Vector3 rc = r->model.getTranslation();

        float dist = pc.distance(rc);

        // Si el jugador passa pel forat
        if (dist < r->inner_radius - player->collision_radius)
        {
            std::cout << "HAS AGAFAT UN RING! +1 moneda" << std::endl;
            Audio::Play("data/coin.wav", 1.0f, BASS_SAMPLE_MONO);
            Audio::Play("data/turbo.wav", 2.0f, BASS_SAMPLE_MONO);

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
    //float y = random(minY, maxY);
    float y = 0.0f;
    // Structured vertical spawning based on pattern
    switch (currentPattern) {
    case PATTERN_LOW:
        y = random(minY, 4.0f); // Lower range [-6.0, 4.0]
        break;
    case PATTERN_MID:
        y = random(4.0f, 14.0f); // Mid range [4.0, 14.0]
        break;
    case PATTERN_HIGH:
        y = random(14.0f, maxY); // High range [14.0, 20.0]
        break;
    default:
        y = random(minY, maxY);
        break;
    }
    float z = random(40.0f, 80.0f);

    r->model.setTranslation(Vector3(x, y, z));

    World::instance->root->addChild(r);
    rings.push_back(r);
}