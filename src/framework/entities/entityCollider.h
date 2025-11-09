#pragma once
#include "framework/entities/entityMesh.h"
#include "framework/collision.h" // Asumo que collision.h existe



// Define la CLASE
class EntityCollider : public EntityMesh {

public: // Haz público el layer
	bool is_dynamic = false;
	int layer = eCollisionFilter::NONE; // ¡Sintaxis correcta!

	// Constructor
	EntityCollider() : EntityMesh() {}
	EntityCollider(Mesh* mesh, const Material& material, const std::string& name = "")
		: EntityMesh(mesh, material, name) {
	}
};

// El 'if (entity->layer & SCENARIO)' que tenías
// debe ir DENTRO de una función que haga colisiones,
// por ejemplo, en player.cpp::canMove