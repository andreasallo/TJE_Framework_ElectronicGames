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


	// Constructor
	EntityCollider() : EntityMesh() {}
	EntityCollider(Mesh* mesh, const Material& material, const std::string& name = "")
		: EntityMesh(mesh, material, name) {
	}
	

};

// El 'if (entity->layer & SCENARIO)' que tenías
// debe ir DENTRO de una función que haga colisiones,
// por ejemplo, en player.cpp::canMove