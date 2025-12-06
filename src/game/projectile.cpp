#include "projectile.h"
#include "framework/world.h"
#include "game/asteroid.h"
#include "game/asteroidControl.h"
#include "framework/collision.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "game/player.h"
#include "framework/framework.h"
#include "framework/audio.h"

Projectile::Projectile()
    : EntityCollider(Mesh::Get("data/meshes/sphere.obj"), Material(), "projectile")
{
}

void Projectile::init(const Vector3& origin, const Vector3& dir)
{
    // --- Dirección normalizada ---
    direction = Vector3(dir).normalize();
    model.setTranslation(origin);
    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/01tizeta_floor_e.png");//white texture
    material.color = Vector4(1, 0.2f, 0.2f, 1);  // color vermell
    collision_radius = radius;
    layer = eCollisionFilter::PROJECTILE;
}

void Projectile::update(float dt)
{
    if (toDelete) return;

    // --- MOVIMIENTO ---
    Vector3 pos = model.getTranslation();
    pos = pos + direction * speed * dt;
    model.setTranslation(pos);

    // --- TIEMPO DE VIDA ---
    lifeTime -= dt;
    if (lifeTime <= 0.0f)
    {
        toDelete = true;
        return;
    }

    auto& asteroids = World::instance->asteroidControl.asteroids;

    // --- COLISIÓN ESFERA VS ESFERA ---
    for (Asteroid* a : asteroids)
    {
        if (!a || a->toDelete) continue;

        float dist = pos.distance(a->getGlobalMatrix().getTranslation());
        float combinedRadius = this->collision_radius + a->collision_radius;

        if (dist < combinedRadius)
        {
            // 🔥 EXPLOSIÓ
            World::instance->spawnExplosion(
                a->getGlobalMatrix().getTranslation()
            );

            // 1. Marcar el meteorito para que AsteroidControl lo saque de su lista
            a->toDelete = true;

            // 2. IMPORTANTE: Decirle al World que lo borre de la escena y libere memoria
            World::instance->destroyEntity(a);

            // 3. Marcar el proyectil para borrarse
            this->toDelete = true;
            // (El proyectil se gestiona en World::update, pero por seguridad puedes añadir destroyEntity(this) aquí también, aunque la lógica actual de World ya lo hace)

            // Moneda
            if (World::instance->player)
                World::instance->player->coins_collected++;

            // So
            Audio::Play("data/explosion.wav", 1.0f, BASS_SAMPLE_MONO);

            std::cout << "Impacto confirmado!" << std::endl;

            return; // IMPORTANT: sortir del bucle
        }
    }
}




void Projectile::render(Camera* camera)
{
    // Render normal (la malla de la esfera visual)
    EntityCollider::render(camera);

    // --- BLOQUE DEBUG: DIBUJAR ESFERA DE COLISIÓN (CIAN) ---
    if (Game::IsDebugMode())
    {
        Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
        Mesh* mesh = Mesh::Get("data/meshes/sphere.obj");

        shader->enable();

        Matrix44 m;
        // Usamos la posición global real
        m.setTranslation(getGlobalMatrix().getTranslation());
        // Escalamos según el radio de colisión que usa la lógica
        m.scale(collision_radius, collision_radius, collision_radius);

        // Color Cian brillante para diferenciarlo del jugador (verde) y enemigos (rojo)
        shader->setUniform("u_color", Vector4(0.0f, 1.0f, 1.0f, 1.0f));
        shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
        shader->setUniform("u_model", m);

        mesh->render(GL_LINES); // Dibujar solo líneas (alambre)

        shader->disable();
    }
}