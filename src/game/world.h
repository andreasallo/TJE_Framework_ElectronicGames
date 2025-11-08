#pragma once

#include "framework/includes.h"
#include "framework/input.h"
#include "framework/camera.h"
#include "game.h"
#include "framework/utils.h"

class World {
public:
	static World* instance;

	Entity* root;
	EntityMesh* skybox = nullptr;

	World();

	void render(Camera* camera);
	void update(double seconds_elapsed, Camera* camera);

	static World* getInstance() { return instance; };
};