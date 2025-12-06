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


// ----------------- Constructor (World::World) -----------------
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

	// POSAR-L'HO LLUNY I ESCALA 0 -> invisible al principi
	end_planet->model.setTranslation(0.0f, 0.0f, -20); // fora de l'àrea de joc
	end_planet->model.setScale(0.001f, 0.001f, 0.001f);     // pràcticament invisible
	end_planet->culling = false;
	root->addChild(end_planet);

	// Chunk generator
	chunkGen.init(2000.0f, 120.0f);
	lastSpawnedChunkZ = -10000.0f;

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
			"data/space_cube/px.png",
			"data/space_cube/nx.png",
			"data/space_cube/ny.png",
			"data/space_cube/py.png",
			"data/space_cube/pz.png",
			"data/space_cube/nz.png"
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
		explosion_emitter->setTexture("data/fire.png"); // exemple
		explosion_emitter->setTextureGridSize(4);        // si la textura és 4x4
		explosion_emitter->setAdditiveBlendingEnabled(true);
		explosion_emitter->setEmissionEnabled(false);    // només quan hi hagi explosió
		explosion_emitter->setEmitRate(0.001f);
		explosion_emitter->setMaxTimeAlive(0.6f);
		explosion_emitter->setRandomFactor(0.8f);
		explosion_emitter->setSizesCurve({ 3.5f, 2.0f, 0.0f }); // creix i desapareix
		explosion_emitter->setColorsCurve({
	Vector4(1.0f, 1.0f, 0.8f, 1.0f), // Inicio: Muy caliente (casi blanco)
	Vector4(1.0f, 0.4f, 0.0f, 0.8f), // Medio: Fuego naranja
	Vector4(0.2f, 0.2f, 0.2f, 0.0f)  // Final: Se desvanece
			});

		transparent_entities.push_back(explosion_emitter);
		root->addChild(explosion_emitter);   // perquè es renderitzi


		//SMOKE
		smoke_emitter = new ParticleEmitter();
		smoke_emitter->setTexture("data/smoke.png");   // usa la teva textura de fum
		smoke_emitter->setTextureGridSize(2);        // 2x2 frames
		smoke_emitter->setEmissionEnabled(false);
		smoke_emitter->setAdditiveBlendingEnabled(false); // fum -> blending normal
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

		// Afegir a l'escena
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

	drawGrid();
	//Asteroid
	//asteroid_root->render(camera);
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



/*
void World::shootLaser()
{
	
	Vector3 front = player->model.frontVector().normalize();
	Vector3 right = player->model.rightVector().normalize(); // Usar el right vector del jugador
	Vector3 up = player->model.topVector().normalize();
	Vector3 dir = front;

	Vector3 origin = player->model.getTranslation() + (dir * 20.0f); //afegir 20 pk sino surt desde el cul d la nau

	float rayLength = 40.0f;
	float laserRadius = 1.0f;
	

	// ---- DETECTAR COL·LISIÓ AMB RAY ----
	sCollisionData hit;
	hit.distance = rayLength;

	for (Asteroid* a : asteroidControl.asteroids)
	{
		if (!a || a->toDelete) continue;

		Collision::TestEntityRay(a, origin, dir, hit,
			eCollisionFilter::ENEMY,
			true,
			rayLength);
	}

	if (hit.collider)
	{
		Asteroid* a = (Asteroid*)hit.collider;
		a->toDelete = true;
		destroyEntity(a);

		//coins sum
		player->coins_collected++;
		std::cout << "Asteroid destroyed! Coins: " << player->coins_collected << std::endl;
	}

	// ---- CREAR EL LASER VISUAL ----
	Mesh* m = new Mesh();
	m->createCube(); // cub base

	Material mat;
	mat.diffuse = Texture::Get("data/textures/texture.tga"); 
	mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	mat.color = Vector4(0.0f, 1.0f, 1.0f, 1.0f); // cian brillant

	laserBeam = new EntityMesh(m, mat, "laser");
	laserBeam->layer = eCollisionFilter::NONE;

	// Escala.
	float laserThickness = 0.4f;
	Matrix44 model;

	
	model.setTranslation(origin + dir * (rayLength * 0.5f));
	model.scale(laserThickness, laserThickness, rayLength);

	Vector3 right = dir.cross(Vector3(0, 1, 0)).normalize();
	Vector3 up = right.cross(dir).normalize();
	model.setUpAndOrthonormalize(up);
	model.setFrontAndOrthonormalize(dir);

	model.setTranslation(origin + dir * (rayLength * 0.5f));
	model.scale(laserThickness, laserThickness, rayLength);

	laserBeam->model = model;

	root->addChild(laserBeam);

	laserTimer = 0.08f; // dura 1–2 frames
}*/




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



// ----------------- Update (World::update) -----------------
void World::update(float delta_time)
{
	
	player->update(delta_time);
	updateCamera(delta_time);

	//logica normal
	if (!level_finished)
	{
		// Actualizar obstáculos
		asteroidControl.update(delta_time);
		ringControl.update(delta_time);
		chunkGen.update(delta_time, player->model.getTranslation());

		// Generar chunks
		for (const sChunk& c : chunkGen.getChunks())
		{
			if (!hasSpawnedChunk(c.startZ))
			{
				for (const sAsteroidSpawn& a : c.asteroids)
					asteroidControl.spawnAsteroidAt(a.x, a.y, a.z, a.speed);

				for (const sRingSpawn& r : c.rings)
					ringControl.spawnRingAt(r.x, r.y, r.z);

				markChunkSpawned(c.startZ);
			}
		}

		// DETECTAR EL FINAL DEL NIVEL
		// Si el jugador supera la longitud del nivel
		if (player->model.getTranslation().z >= chunkGen.levelLength)
		{
			level_finished = true;
			final_sequence_timer = 0.0f; //animacio
			final_collided = false;

			
			//long + distncia inicial
			float startZ = chunkGen.levelLength + 400.0f;

			end_planet->model.setIdentity();
			end_planet->model.setTranslation(0.0f, 0.0f, startZ);
			end_planet->model.scale(10.0f, 10.0f, 10.0f); // Tamaño inicial visible

			std::cout << "--- FINAL DEL NIVEL ALCANZADO ---" << std::endl;
		}
	}
	// --- SECUENCIA FINAL (PLANETA ACERCÁNDOSE) ---
	else
	{
		// Aumentamos el temporizador
		final_sequence_timer += delta_time;

		// PARÁMETROS DE LA ANIMACIÓN
		float approachSpeed = 80.0f;  // Velocidad a la que se acerca el planeta (similar a meteoritos)
		float growthSpeed = 30.0f;    // Cuánto crece por segundo
		float rotationSpeed = 0.1f;   // Rotación lenta estética

		// 1. Calcular dónde debería estar el planeta ahora
		float startZ = chunkGen.levelLength;
		float currentZ = startZ - (approachSpeed * final_sequence_timer);

		//Calcular escala actual
		float startScale = 10.0f;
		float currentScale = startScale + (growthSpeed * final_sequence_timer);

		// ONSTRUIR LA MATRIZ (Orden: T * R * S)
		// Usamos una matriz limpia cada frame para evitar deformaciones
		Matrix44 m;
		m.setIdentity();
		m.setTranslation(0.0f, 0.0f, currentZ); // Primero Traslación
		m.rotate(final_sequence_timer * rotationSpeed, Vector3(0, 1, 0)); // Luego Rotación
		m.scale(currentScale, currentScale, currentScale); // Finalmente Escala

	
		end_planet->model = m;

		// DETECTAR COLISIÓN FINAL
		
		if (!final_collided && currentZ < player->model.getTranslation().z + 20.0f)
		{
			final_collided = true;
			std::cout << "COLISION CON PLANETA. ENDSTAGE." << std::endl;
		}
	}

	// --- GESTIÓN DE PROYECTILES (NUEVO SISTEMA) ---

	// 1. Update de todos los proyectiles vivos
	for (Projectile* p : projectiles) {
		if (p && !p->toDelete) {
			p->update(delta_time);
		}
	}

	// 2. Usar remove_if para limpiar el vector y mandar a destruir las entidades
	// 1. Update de proyectiles
	for (Projectile* p : projectiles) {
		if (p && !p->toDelete) {
			p->update(delta_time);
		}
	}

	// 2. Limpieza segura (Aquí es donde ocurre la magia para no crashear)
	auto iterator = std::remove_if(projectiles.begin(), projectiles.end(), [](Projectile* p) {
		if (p->toDelete) {
			// El World ve la bandera y dice: "Vale, hora de borrarte de verdad"
			World::instance->destroyEntity(p);
			return true; // Sácalo de la lista de proyectiles
		}
		return false;
		});
	// Borramos del vector los que hemos sacado
	projectiles.erase(iterator, projectiles.end());

	//PROJECTILS
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




	// 3) Limpieza de entidades
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

	// MOOTHING
	//smoothedTarget = smoothedTarget * 0.9f + planePos * 0.1f;

	//POSICIÓ CÀMERA
	//Vector3 cam_offset=Vector3(0.0f, 0.3f, -10.2f);

	//Vector3 eye =smoothedTarget +front * cam_offset.z +Vector3(0.0f, cam_offset.y, 0.0f);

	//Vector3 center = smoothedTarget;

	//Vector3 rotatedUp = player->model.rotateVector(Vector3(0, 1, 0));
	//camera->lookAt(eye, center, rotatedUp);
	Vector3 rotatedUp = player->model.rotateVector(Vector3(0, 1, 0));
	Vector3 cam_offset(0.0f, 4.5f, -20.2f); //para poner la camara mas atras usamos el valor negativo en z y 0.3 
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



