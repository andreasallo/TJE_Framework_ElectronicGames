#include "ring.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "framework/world.h" 
#include "framework/utils.h"


#define RENDER_DEBUG

void Ring::render(Camera* camera)
{
    EntityMesh::render(camera);
    


#ifdef RENDER_DEBUG
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
#endif
}

void Ring::update(float dt)
{
    // Lógica de movimiento: el anillo se mueve hacia el jugador.
    // Asumimos que el "frente" del juego es -Z, como en asteroid.cpp.
    model.translate(0, 0, -speed * dt);

    // Lógica para borrar si el anillo ha pasado al jugador.
    if (model.getTranslation().z < -20.0f) { // Ajusta el límite de Z si es necesario
        toDelete = true;
        // World::instance->destroyEntity(this); // Esto se encarga RingControl.cpp
    }

    // Llama al update de la clase base (si tienes alguna lógica específica ahí)
    EntityCollider::update(dt);
}