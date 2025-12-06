#pragma once
#include "framework/entities/entityCollider.h"

class Projectile : public EntityCollider
{
public:
    Vector3 direction;
    float speed = 70.0f;
    float lifeTime = 1.5f;     // duració màxima
    float radius = mesh->box.halfsize.length();   // radi de col·lisió

    
    Projectile();                           
    void init(const Vector3& origin, const Vector3& dir);

    void update(float dt) override;
    void render(Camera* camera) override;
};
