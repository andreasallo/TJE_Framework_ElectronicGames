#pragma once

#include "framework/entities/entityMesh.h"

class Player : public EntityMesh {
public:

	void render(Camera* camera);
	void update(Camera* camera);

	//const Vector3& getMovementDirection();
	//isGrounded
};