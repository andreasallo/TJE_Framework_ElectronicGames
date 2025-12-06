#pragma once
#include "framework/entities/entity.h"
#include "game/game.h"
#include <vector>
#include "framework/utils.h"
#include "game/asteroid.h"
#include "game/asteroidControl.h"
#include "game/ring.h"
#include "game/ringControl.h"
#include "game/chunkGenerator.h"
#include "game/projectile.h"
#include "graphics/particles.h"

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

	EntityMesh* end_planet = nullptr;


	AsteroidControl asteroidControl;
	RingControl ringControl;
	ChunkGenerator chunkGen;
	float lastSpawnedChunkZ = -9999.0f;

	//particles
	ParticleEmitter* explosion_emitter = nullptr;
	void spawnExplosion(const Vector3& pos);
	float explosion_timer = 0.0f;
	std::vector<Entity*> transparent_entities;
	ParticleEmitter* smoke_emitter = nullptr;
	float smoke_timer = 0.0f;
	ParticleEmitter* engine_fire_emitter = nullptr;






	float camera_speed = 3.0f;
	float mouse_speed = 10.0f;

	bool free_camera = false;

	Vector3 camera_current_eye;
	Vector3 camera_current_center;
	bool first_frame = true;
	std::vector<float> spawnedChunks;

	bool level_finished = false;
	float final_sequence_timer = 0.0f;
	Vector3 planet_target_pos = Vector3(0.0f, 0.0f, 0.0f);
	float planet_current_scale = 1.0f;
	bool final_collided = false;
	float planet_target_scale = 200.0f;  // quan arribi a això → omple pantalla
	float planet_grow_speed = 8.0f;      // com de ràpid creix

	EntityMesh* laserBeam = nullptr;
	float laserTimer = 0.0f;

	std::vector<Projectile*> projectiles;
	void requestShoot();



	void shootLaser();

	
	//GameMechanic
	//int genRandom();
	//bool genAsteroid();

	void render(Camera* camera);
	void update(float delta_time);

	std::vector<Entity*> entities_to_destroy;

	bool hasSpawnedChunk(float z);
	void markChunkSpawned(float z);
	void shootRay();

	void addEntity(Entity* entity);
	void destroyEntity(Entity* entity);
	void updateCamera(float dt);

	void entities_to_destroy_clear();

};