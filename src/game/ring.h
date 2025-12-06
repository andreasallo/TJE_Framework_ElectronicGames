#pragma once
#include "framework/entities/entityCollider.h"
#include "graphics/mesh.h"
#include "framework/camera.h"

#pragma once
#include "framework/entities/entityCollider.h"

class Ring : public EntityCollider {
public:
    float speed = 30.0f; //Velocitat cap al jugador
    //float radius = 3.0f;//Radi de col·lisió
    //float inner_radius = 3.0f;

    bool consumed = false;//Per evitar doble detecció
    bool toDelete = false;

    Ring(Mesh* mesh, const Material& material, const std::string& name = "")
        : EntityCollider(mesh, material, name)
    {
		layer = eCollisionFilter::ENEMY;  //perquè el player el pugui detectar sha de marcar com a ENEMY
        collider_type = COLLIDER_SPHERE;
		collision_radius = mesh->box.halfsize.length()*0.8;
    }

    void update(float dt) override;
    void render(Camera* camera) override;
};
