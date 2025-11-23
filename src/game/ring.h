#pragma once
#include "framework/entities/entityCollider.h"
#include "graphics/mesh.h"
#include "framework/camera.h"

#pragma once
#include "framework/entities/entityCollider.h"

class Ring : public EntityCollider {
public:
    float speed = 20.0f;     // Velocitat cap al jugador
    float radius = 3.0f;     // Radi de col·lisió
    bool consumed = false;   // Per evitar doble detecció
    bool toDelete = false;

    Ring(Mesh* mesh, const Material& material, const std::string& name = "")
        : EntityCollider(mesh, material, name)
    {
        layer = eCollisionFilter::ENEMY;  //perquè el player el pugui detectar
        collider_type = COLLIDER_SPHERE;
    }

    void update(float dt) override;
    void render(Camera* camera) override;
};
