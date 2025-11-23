#pragma once
#include "framework/entities/entityCollider.h"

class Asteroid : public EntityCollider {
public:

    float speed = 15.0f; // velocitat base
    float radius = 1.2f; // per a col·lisions senzilles
    bool toDelete = false;
    float collision_radius = 8.0f;

    Asteroid() {}
    Asteroid(Mesh* mesh, const Material& mat, const std::string& name)
        : EntityCollider(mesh, mat, name) {
    }

    void update(float dt) override;
	void render(Camera* camera) override;
};
