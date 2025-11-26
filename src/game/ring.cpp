#include "ring.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "framework/world.h" 
#include "framework/utils.h"
#include "game/player.h"


#define RENDER_DEBUG

void Ring::render(Camera* camera)
{
    EntityMesh::render(camera);
    


    if (Game::IsDebugMode() ){
        Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
        Mesh* mesh = Mesh::Get("data/meshes/sphere.obj");

        shader->enable();

        Matrix44 m;
        m.setTranslation(getGlobalMatrix().getTranslation());
        m.scale(collision_radius, collision_radius, collision_radius);

        shader->setUniform("u_color", Vector4(1, 1, 0, 1)); // groc
        shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
        shader->setUniform("u_model", m);

        mesh->render(GL_LINES);

        shader->disable();
    }
}

void Ring::update(float dt)
{
    // Aceleración de los aros durante el turbo del jugador
    float current_speed = speed;
    if (World::instance && World::instance->player && World::instance->player->turbo) {
        current_speed *= 4.0f; // Multiplicador de velocidad (sincronizado con meteoritos)
    }

    // Lógica de movimiento: el anillo se mueve hacia el jugador.
    model.translate(0, 0, -current_speed * dt);

    // Lógica para borrar si el anillo ha pasado al jugador.
    if (model.getTranslation().z < -20.0f) { // Ajusta el límite de Z si es necesario
        toDelete = true;
    }
    EntityCollider::update(dt);
}