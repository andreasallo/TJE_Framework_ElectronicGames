#pragma once
#include "framework/entities/entity.h"
#include "game/game.h"
#include <vector>
#include "framework/utils.h"
#include "game/asteroid.h" //asteroid
#include "game/asteroidControl.h"

class Camera;
class Player;
class entityMesh;

//game.cpp: World* world::instance = nullptr;
//Worls::get_instance()->addEntity(child)

class World
{

private:
	//static World* instance;
	Vector3 smoothedTarget;

public:
	static World* instance;

	EntityMesh* skybox = nullptr;

	World();
	static World* getInstance() { return instance; };
	
	
	Vector3 smoothedEye;
	Vector3 smoothedCenter;

	// Per inicialitzar la càmera el primer frame
	bool firstFrame = true;

	Camera* camera = nullptr;
	Entity* root = nullptr;
	//Asteroid* asteroid_root = nullptr;
	//bool has_asteroids = false;
	Player* player = nullptr;

	AsteroidControl asteroidControl;


	float camera_speed = 3.0f;
	float mouse_speed = 10.0f;

	bool free_camera = false;

	Vector3 camera_current_eye;
	Vector3 camera_current_center;
	bool first_frame = true;
	
	//GameMechanic
	//int genRandom();
	//bool genAsteroid();

	void render(Camera* camera);
	void update(float delta_time);

	std::vector<Entity*> entities_to_destroy;

	void addEntity(Entity* entity);
	void destroyEntity(Entity* entity);
	void updateCamera(float dt);

	void entities_to_destroy_clear();

};