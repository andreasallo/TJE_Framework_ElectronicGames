#include "framework/world.h"
#include "framework/framework.h"
#include "framework/entities/entity.h"
#include "framework/entities/entityMesh.h"
#include "framework/entities/entityCollider.h"
#include "framework/utils.h"
#include "framework/camera.h"
#include "framework/input.h"
#include "game/game.h"
#include "game/player.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "scene_parser.h"
#include "framework/audio.h"
#include "game/asteroidControl.h"
#include "game/ringControl.h"
#include "game/chunkGenerator.h"
#include "framework/includes.h"
#include "game/projectile.h"

#include <random>

World::World() {
	instance = this;
	Audio::Init();
	camera = Game::instance->camera;

	//LOAD SCENE
	root = new Entity();
	root->name = "root";

	asteroidControl.init();
	ringControl.init();

	// --- END PLANET (CREAR PERÒ NO POSAR A L'ESCENA VISIBLE AIXÍ) ---
	Material planet_material;
	planet_material.diffuse = Texture::Get("data/planeta/RP-0002-N.png");
	planet_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	end_planet = new EntityMesh(Mesh::Get("data/planeta/RP-0002-N.obj"), planet_material, "EndPlanet");
	end_planet->model.setTranslation(0.0f, 0.0f, -20);
	end_planet->model.setScale(0.001f, 0.001f, 0.001f);
	end_planet->culling = false;
	root->addChild(end_planet);

	// Chunk generator
	// init( ZonaSeguraInicial, LongitudDelPatron, LongitudTotalDelNivel )
	chunkGen.init(300.0f, 120.0f, 3000.0f);
	lastSpawnedChunkZ = chunkGen.safeZoneInitial - chunkGen.chunkLength;  // Preparar primer spawn

	// Player
	Material player_material;
	player_material.diffuse = Texture::Get("data/Red_Camo.png");
	player_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	player = new Player(Mesh::Get("data/nau30.obj"), player_material, "player");
	player->layer = eCollisionFilter::PLAYER;
	addEntity(player);

	if (player) {
		smoothedTarget = player->model.getTranslation();
	}

	// Skybox (idem)
	{
		Texture* cube_texture = new Texture();
		cube_texture->loadCubemap("ProbaCubeMap", {
			"data/space_cube_new/px.png",
			"data/space_cube_new/nx.png",
			"data/space_cube_new/ny.png",
			"data/space_cube_new/py.png",
			"data/space_cube_new/pz.png",
			"data/space_cube_new/nz.png"
			});

		Material cubemap_material;
		cubemap_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/cubemap.fs");
		cubemap_material.diffuse = cube_texture;

		skybox = new EntityMesh(Mesh::Get("data/cubemap.ASE"), cubemap_material);
	}


	//PARTICULAS
	{
		//fire
		explosion_emitter = new ParticleEmitter();
		explosion_emitter->setTexture("data/fire.png");
		explosion_emitter->setTextureGridSize(4);        
		explosion_emitter->setAdditiveBlendingEnabled(true);
		explosion_emitter->setEmissionEnabled(false);    
		explosion_emitter->setEmitRate(0.001f);
		explosion_emitter->setMaxTimeAlive(0.6f);
		explosion_emitter->setRandomFactor(0.8f);
		explosion_emitter->setSizesCurve({ 3.5f, 2.0f, 0.0f }); // creix i desapareix
		explosion_emitter->setColorsCurve({
	Vector4(1.0f, 1.0f, 0.8f, 1.0f), // Inicio: (casi blanco)
	Vector4(1.0f, 0.4f, 0.0f, 0.8f), // Medio: Fuego naranja
	Vector4(0.2f, 0.2f, 0.2f, 0.0f)  // Final: Se desvanece
			});

		transparent_entities.push_back(explosion_emitter);
		root->addChild(explosion_emitter);   


		//SMOKE
		smoke_emitter = new ParticleEmitter();
		smoke_emitter->setTexture("data/smoke.png");  
		smoke_emitter->setTextureGridSize(2);       
		smoke_emitter->setEmissionEnabled(false);
		smoke_emitter->setAdditiveBlendingEnabled(false); 
		smoke_emitter->setEmitRate(0.01f);            // surt un núvol seguit però no molt dens
		smoke_emitter->setMaxTimeAlive(1.7f);         // dura més que el foc
		smoke_emitter->setRandomFactor(2.5f);         // lleugera dispersió

		smoke_emitter->setSizesCurve({1.0f, 2.3f, 3.0f });
		// comença petit → creix → es redueix

		smoke_emitter->setColorsCurve({
	Vector4(0.6f, 0.6f, 0.6f, 0.0f),  // Gris medio, bastante visible al inicio
	Vector4(0.5f, 0.5f, 0.5f, 0.15f),  // Se oscurece (se enfría)
	Vector4(0.0f, 0.0f, 0.0f, 0.0f)   // Negro transparente al final
			});

		// IMPORTANT: afegir al render de transparències
		transparent_entities.push_back(smoke_emitter);

		root->addChild(smoke_emitter);

		//ENGINE FIRE
		// --- ENGINE FIRE EMITTER (foc continu al motor) ---
		engine_fire_emitter = new ParticleEmitter();
		engine_fire_emitter->setTexture("data/fire.png");
		engine_fire_emitter->setTextureGridSize(4);
		engine_fire_emitter->setAdditiveBlendingEnabled(true);

		engine_fire_emitter->setEmissionEnabled(true);   // sempre ON
		engine_fire_emitter->setEmitRate(0.01f);         // poc foc
		engine_fire_emitter->setMaxTimeAlive(0.6f);      // foc curt
		engine_fire_emitter->setRandomFactor(0.15f);
		engine_fire_emitter->setSizesCurve({ 0.3f, 0.8f, 0.0f });
		engine_fire_emitter->setColorsCurve({
			Vector4(1, 0.6f, 0.1f, 1),     // taronja
			Vector4(1, 0.2f, 0.0f, 0.6f),  // vermell
			Vector4(0.1f, 0.0f, 0.0f, 0.0f)
			});

		// Transparent entities
		transparent_entities.push_back(engine_fire_emitter);
		root->addChild(engine_fire_emitter);


	}
}


void World::render(Camera* camera) {
	// Set the clear color (the background color)
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the camera as default
	camera->enable();

	if (skybox) {
		skybox->model.setTranslation(camera->eye);

		glDisable(GL_DEPTH_TEST);
		skybox->render(camera);
		glEnable(GL_DEPTH_TEST);
	}

	
	// Set flags //ESTO LO GESTIONARA ENTITY MESH RENDER
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	player->render(camera);
	root->render(camera);

	//drawGrid();
	
	for (Entity* e : transparent_entities)
	{
		if (e)
			e->render(camera);
	}

	// Draw the floor grid
	

	// Render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}

bool World::hasSpawnedChunk(float z) {
	for (float val : spawnedChunks)
		if (fabs(val - z) < 0.001f) // compara floats
			return true;
	return false;
}
void World::markChunkSpawned(float z) {
	spawnedChunks.push_back(z);
}

void World::spawnExplosion(const Vector3& pos)
{
	explosion_emitter->setEmitPosition(pos);
	//explosion_emitter->clearParticles();        // reset
	explosion_emitter->setEmissionEnabled(true);
	explosion_timer = 0.1f;

	// --- FUM ---
	smoke_emitter->setEmitPosition(pos);
	//smoke_emitter->clearParticles();
	smoke_emitter->setEmissionEnabled(true);
	smoke_timer = 0.5f;

}




void World::requestShoot()
{
	Vector3 origin = player->model.getTranslation() +
		player->model.frontVector().normalize() * 2.0f;

	Vector3 dir = player->model.frontVector().normalize();

	Projectile* p = new Projectile();
	p->init(origin, dir);

	addEntity(p);
	projectiles.push_back(p);
}



void World::update(float delta_time)
{

	player->update(delta_time);
	updateCamera(delta_time);
	chunkGen.update(delta_time, player->model.getTranslation().z);

	// Actualizar obstáculos
	asteroidControl.update(delta_time);
	ringControl.update(delta_time);

	if (player)
	{
		float playerZ = player->model.getTranslation().z;
		float progress = (playerZ - chunkGen.safeZoneInitial) /
			(chunkGen.totalLevelLength - chunkGen.safeZoneInitial);

		if (progress >= 0.95f)
		{
			player->turbo = false;                  
			player->forwardSpeed = 5.0f;             // velocitat molt lenta
			Audio::Play("data/level-win-6416.wav", 0.6f); 
		}
	}

	//logica normal
	if (level_finished) {  
		final_sequence_timer += delta_time;

		float worldSpeedFactor = (player && player->turbo) ? 2.0f : 1.0f;
		float approachSpeed = 80.0f * worldSpeedFactor;
		float growthSpeed = 30.0f;
		float rotationSpeed = 0.1f;

		float currentZ = planet_initial_z - (approachSpeed * final_sequence_timer);
		float currentScale = 10.0f + (growthSpeed * final_sequence_timer);

		Matrix44 m;
		m.setIdentity();
		m.setTranslation(0.0f, 0.0f, currentZ);
		m.rotate(final_sequence_timer * rotationSpeed, Vector3(0, 1, 0));
		m.scale(currentScale, currentScale, currentScale);

		end_planet->model = m;

		float planetBaseRadius = end_planet->mesh->box.halfsize.x;

		float planetRealRadius = planetBaseRadius * currentScale * 0.95f;
		float distance = end_planet->model.getTranslation().distance(player->model.getTranslation());

		if (!final_collided && distance < (planetRealRadius + player->collision_radius)) {
			final_collided = true;
			std::cout << "COLISION CON PLANETA (SUPERFICIE). ENDSTAGE." << std::endl;
			Game::instance->setStage(END_STAGE, PLAY_STAGE);
		}
	}

	//PROJEECTILS CONTROL
	
	for (Projectile* p : projectiles) {
		if (p && !p->toDelete) {
			p->update(delta_time);
		}
	}

	// NETEJA SEGURA, MOLT CUIDADO SINO CRUSH
	auto iterator = std::remove_if(projectiles.begin(), projectiles.end(), [](Projectile* p) {
		if (p->toDelete) {
			World::instance->destroyEntity(p);
			return true; 
		}
		return false;
		});
	projectiles.erase(iterator, projectiles.end());

	explosion_emitter->update(delta_time);

	if (explosion_timer > 0.0f)
	{
		explosion_timer -= delta_time;
		if (explosion_timer <= 0.0f)
			explosion_emitter->setEmissionEnabled(false);
	}

	smoke_emitter->update(delta_time);
	engine_fire_emitter->update(delta_time);

	if (smoke_timer > 0.0f)
	{
		smoke_timer -= delta_time;

		if (smoke_timer <= 0.0f)
			smoke_emitter->setEmissionEnabled(false);
	}

	if (chromatic_aberration_timer > 0.0f)
	{
		chromatic_aberration_timer -= delta_time;

		if (chromatic_aberration_timer < 0.0f)
			chromatic_aberration_timer = 0.0f;
	}

	


	//Limpieza de entidades
	for (auto e : entities_to_destroy)
	{
		if (e->parent) e->parent->removeChild(e);
		delete e;
	}
	entities_to_destroy_clear();
}




void World::updateCamera(float dt)
{
	Vector3 planePos = player->model.getTranslation();
	Vector3 front = player->model.frontVector().normalize();

	Vector3 rotatedUp = player->model.rotateVector(Vector3(0, 1, 0));
	Vector3 cam_offset(0.0f, 4.5f, -20.2f); //para poner la camara mas atras usamos el valor negativo en z 20
	Vector3 new_eye = planePos
		+ front * cam_offset.z
		+ Vector3(0, cam_offset.y, 0);
	Vector3 new_center = planePos;
	float k = explerpFactor(dt, 4.0f);
	if (firstFrame)
	{
		smoothedEye = new_eye;
		smoothedCenter = new_center;
		firstFrame = false;
	}

	smoothedEye = lerp(smoothedEye, new_eye, k);
	smoothedCenter = lerp(smoothedCenter, new_center, k);
	


	camera->lookAt(smoothedEye, smoothedCenter, rotatedUp);
}


void World::addEntity(Entity* entity) {
	root->addChild(entity);
}

void World::destroyEntity(Entity* entity) {
	entities_to_destroy.push_back(entity);
}

void World::entities_to_destroy_clear() {
	entities_to_destroy.clear();
}



