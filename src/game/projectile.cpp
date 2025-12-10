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
    //NORMALITZAR LA DIRECCIO SEMPRE!!-> REMEMBER
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

    Vector3 pos = model.getTranslation();
    pos = pos + direction * speed * dt;
    model.setTranslation(pos);

    lifeTime -= dt;
    if (lifeTime <= 0.0f)
    {
        toDelete = true;
        return;
    }

    std::vector<Asteroid*>& asteroids = World::instance->asteroidControl.asteroids;

    for (Asteroid* a : asteroids)
    {
        if (!a || a->toDelete) continue;

        float dist = pos.distance(a->getGlobalMatrix().getTranslation());
        float combinedRadius = this->collision_radius + a->collision_radius;

        if (dist < combinedRadius)
        {
            //EXPLOSIÓ
            World::instance->spawnExplosion(
                a->getGlobalMatrix().getTranslation()
            );
            a->toDelete = true;
            World::instance->destroyEntity(a);
            this->toDelete = true;

            // Moneda
            if (World::instance->player)
                World::instance->player->coins_collected += 50;
            Audio::Play("data/explosion-01.wav", 1.0f, BASS_SAMPLE_MONO);

            std::cout << "Impacto confirmado!" << std::endl;

            return; 
        }
    }
}




void Projectile::render(Camera* camera)
{

    EntityCollider::render(camera);

    //BLOQUE DEBUG
    if (Game::IsDebugMode())
    {
        Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
        Mesh* mesh = Mesh::Get("data/meshes/sphere.obj");

        shader->enable();

        Matrix44 m;
        m.setTranslation(getGlobalMatrix().getTranslation());
        m.scale(collision_radius, collision_radius, collision_radius);

        shader->setUniform("u_color", Vector4(0.0f, 1.0f, 1.0f, 1.0f));
        shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
        shader->setUniform("u_model", m);

        mesh->render(GL_LINES);

        shader->disable();
    }
}