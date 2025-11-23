#pragma once
#include "framework/entities/entityMesh.h"
#include "framework/collision.h"
#include "graphics/mesh.h"



enum eColliderType {
	COLLIDER_NONE = 0,
	COLLIDER_SPHERE,
	COLLIDER_BOX
};

class EntityCollider : public EntityMesh {

public:
	bool is_dynamic = false;
	int layer = eCollisionFilter::NONE; 
	eColliderType collider_type = COLLIDER_SPHERE;

	float collision_radius = 1.0f; // per a esfera
	bool toDelete = false;

	// Constructor
	EntityCollider() : EntityMesh() {}
	EntityCollider(Mesh* mesh, const Material& material, const std::string& name = "")
		: EntityMesh(mesh, material, name) {
	}

	Vector3 getColliderCenter() {
		Vector3 center = model.getTranslation();
		return center;
	}
	

};

// El 'if (entity->layer & SCENARIO)' que tenías
// debe ir DENTRO de una función que haga colisiones,
// por ejemplo, en player.cpp::canMove