#pragma once
#include "framework/entities/entity.h"
#include "graphics/material.h"
#include "framework/collision.h"


class Mesh;
class Camera;


class EntityMesh : public Entity {

public:

	EntityMesh() {};
	EntityMesh(Mesh* mesh, const Material& material, const std::string& name = "");

	// Attributes of the derived class  
	Mesh* mesh = nullptr;

	// Check your framework material class!
	Material material;

	//culling
	bool culling = true;

	int layer = eCollisionFilter::NONE;
	//Instancing

	bool isInstanced = false;
	std::vector<Matrix44> models;

	// Methods overwritten from base class
	void render(Camera* camera);
	void update(float elapsed_time);
};


