#include "framework/world.h"
#include "framework/framework.h"
#include "framework/entities/entity.h"
#include "framework/entities/entityMesh.h"
#include "framework/utils.h"
#include "framework/camera.h"
#include "framework/input.h"
#include "game/game.h"
#include "game/player.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "scene_parser.h"

//aqui se carga un avion 
//ir hacia la z, ir hacia delante
//else{}
//root->update(delta_time);
//player->model.translate(0, 0, -41 * delta_time);



World::World() {
	instance = this;

	//camera = new Camera();
	camera = Game::instance->camera;

	//LOAD SCENE	
	root = new Entity();
	root->name = "root";

	Material player_material;
	player_material.diffuse = Texture::Get("data/textures/texture.tga");
	//player_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	//
	//player = new Player(Mesh::Get("data/nau_bl.obj"),player_material, "player");
	//player->model.setTranslation(0.0f, 40.0f, 0.0f);
	//player->model.scale(10.0f, 90.0f, 90.0f);
	Mesh* test_mesh = new Mesh();
	test_mesh->createCube(); // Crea un cubo perfecto por código
	player = new Player(test_mesh, player_material, "player");
	addEntity(player);

	SceneParser parser;
	parser.parse("data/myscene.scene", root);

	/*
	//LOAD SKYBOX
	{
		Texture* cube_texture = new Texture();
		cube_texture->loadCubemap("ProbaCubeMap", {
			"data/Standard_cube/px.png",
			"data/Standard_cube/nx.png",
			"data/Standard_cube/ny.png",
			"data/Standard_cube/py.png",
			"data/Standard_cube/pz.png",
			"data/Standard_cube/nz.png"
			});

		Material cubemap_material;
		cubemap_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/cubemap.fs");
		cubemap_material.diffuse = cube_texture;

		skybox = new EntityMesh(Mesh::Get("data/cubemap.ASE"), cubemap_material);
		//skybox->culling = false;

	}*/

	/*
	if (!root) root = new Entity();

	Material player_material;
	player_material.diffuse = Texture::Get("data/textures/texture.tga");
	player_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	//player = new Player(Mesh::Get("data/nau_bl.obj"),player_material, "player");
	//player->model.setTranslation(0.0f, 40.0f, 0.0f);
	//player->model.scale(10.0f, 10.0f, 10.0f);
	Mesh* test_mesh = new Mesh();
	test_mesh->createCube(); // Crea un cubo perfecto por código
	player = new Player(test_mesh, player_material, "player");
	addEntity(player);*/
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

	player->render(camera);
	// Set flags //ESTO LO GESTIONARA ENTITY MESH RENDER
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	root->render(camera);
	// Draw the floor grid
	drawGrid();

	// Render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}


void World::update(float delta_time) {
	/*if (free_camera) {
		float speed = delta_time * camera_speed;

		if (Input::isMousePressed(SDL_BUTTON_LEFT) || Game::instance->mouse_locked) {
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}

		//Async input to move the camera around
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10;
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(camera->getLocalVector(Vector3(0.0f, 0.0f, -1.0f)) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(camera->getLocalVector(Vector3(0.0f, 0.0f, 1.0f)) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(camera->getLocalVector(Vector3(1.0f, 0.0f, 0.0f)) * speed);

	}
	else {

		//UPDATE CAMERA CONTROLLER 1st person
		float mouse_factor = 0.005f;

		camera->yaw -= Input::mouse_delta.x * delta_time * mouse_speed;
		camera->pitch -= Input::mouse_delta.y * delta_time * mouse_speed;

		float limitAngle = M_PI * 0.4;
		camera->pitch = clamp(camera->pitch, -limitAngle, limitAngle); //clamp restringe el valor entre dos valores

		Matrix44 mYaw;
		mYaw.setRotation(camera->yaw, Vector3(0, 1, 0));
		Matrix44 mPitch;
		mPitch.setRotation(camera->pitch, Vector3(-1, 0, 0));
		Matrix44 mRotation = mPitch * mYaw;

		Vector3 front = mRotation.frontVector().normalize();

		//calcular front
		/*Vector3 front = mRotation.frontVector().normalize();
		Vector3 eye = player->model.getTranslation() + front * 0.1f;
		Vector3 center = eye + front * 2.0f;
		
		//tercera persona

		float orbit_distance = 1.0f;
		Vector3 player_position = player->model.getTranslation() + Vector3(0, 0.4, 0); //altura dels ulls

		//persona
		//Vector3 center = player_position;
		//Vector3 eye = center - front * orbit_distance;
		
		//AVIO
		Vector3 eye = player->model * Vector3(0, 5, 10);
		Vector3 center = player->model.getTranslation() - player->model.frontVector() * 10.0f;

		//camera->lookAt(eye, center, Vector3(0, 1, 0));

		//CONTROLADOR AVIO
		//eye = player->model * (0, 5, 10);
		//center = player->model.getTranslation() - player->model.frontVector() * 10.0f;

		root->update(delta_time);
		player->update(delta_time);
	}*/

	root->update(delta_time);
	for (auto e : entities_to_destroy) {
		if (e->parent) {
			e->parent->removeChild(e);
		}
		delete e;
	}
	entities_to_destroy_clear();		 
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

