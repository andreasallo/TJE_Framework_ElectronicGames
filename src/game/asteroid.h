#pragma once
#include "framework/entities/entityCollider.h"

class Asteroid : public EntityCollider {
public:

    float speed = 30.0f; // velocitat base
    float radius = 1.2f; // per a col·lisions senzilles
    bool toDelete = false;

    Vector3 rotation_axis;
    float rotation_speed;

    Asteroid() {}
    Asteroid(Mesh* mesh, const Material& mat, const std::string& name)
        : EntityCollider(mesh, mat, name) {
        if (mesh) {
            collision_radius = mesh->box.halfsize.length()*0.6;
        }
    }

    void update(float dt) override;
	void render(Camera* camera) override;
};
