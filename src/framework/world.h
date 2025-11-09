#pragma once
#include "framework/entities/entity.h"
#include <vector>

class Camera;
class Player;
class entityMesh;

//game.cpp: World* world::instance = nullptr;
//Worls::get_instance()->addEntity(child)

class World
{

private:
	static World* instance;

	World() {
		instance = this;
	}

public:
	static World* getInstance() {
		return instance;
	}

	void init();
	

	Camera* camera = nullptr;
	Entity* root = nullptr;
	entityMesh* skybox = nullptr;
	Player* player = nullptr;

	float camera_speed = 10.0f;
	float mouse_speed = 10.0f;

	bool free_camera = true;

	void render();
	void update(float delta_time);

	std::vector<Entity*> entities_to_destroy;

	void addEntity(Entity* entity);
	void destroyEntity(Entity* entity);

	void entities_to_destroy_clear();
	
};







