#include "game/asteroid.h"
#include "game/game.h"
#include "graphics/shader.h"
#include "graphics/mesh.h"
#include <cmath>
#include "framework/world.h"
#include "game/player.h"
#include "game/ring.h"
#include "game/ringControl.h"

#define RENDER_DEBUG

void Asteroid::update(float dt)
{
    
    bool turbo = World::instance->player->turbo;
    float mult = turbo ? 2.0f : 1.0f;
    //els meteorits es mouen cap al jugador → direcció -Z
    model.translate(0, 0, -speed * dt);

    //si passen del jugador → destruir
    if (model.getTranslation().z < -200.0f || model.getTranslation().z > 200.0f) {
        
        toDelete = true;
        World::instance->destroyEntity(this);
    }
}


void Asteroid::render(Camera* camera)
{
    // Render normal
    EntityMesh::render(camera);

    if (Game::IsDebugMode()) {
        // Debug: dibuixa l’esfera de col·lisió
        Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
        Mesh* mesh = Mesh::Get("data/meshes/sphere.obj");

        shader->enable();

        Matrix44 m;
        m.setTranslation(getGlobalMatrix().getTranslation());
        m.scale(collision_radius, collision_radius, collision_radius);

        shader->setUniform("u_color", Vector4(1.0f, 0.0f, 0.0f, 1.0f));  // <- vermell per enemic
        shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
        shader->setUniform("u_model", m);

        mesh->render(GL_LINES);

        shader->disable();

    }

}
