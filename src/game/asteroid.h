#pragma once

#include "framework/entities/entityCollider.h"

class Asteroid : public EntityCollider {
public:

	float speed = 0;

	Asteroid() {};
	Asteroid(Mesh* mesh, const Material& material, const std::string& name = "")
		: EntityCollider(mesh, material, name) {
	}

	Asteroid* parent = nullptr;
	std::vector<Asteroid*> children;

	void render(Camera* camera);
	void update(double delta_time);

	void addChild(Asteroid* child);
	void removeChild(Asteroid* child);

};