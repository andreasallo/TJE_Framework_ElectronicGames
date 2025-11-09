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

//aqui se carga un avion 
//ir hacia la z, ir hacia delante
//else{}
//root->update(delta_time);
//player->model.translate(0, 0, -41 * delta_time);


World* World::instance = nullptr;

void World::init() {
	if (!root) root = new Entity();

	Material player_material;
	player_material.diffuse = Texture::Get("data/textures/texture.tga");
	player_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	player = new Player(Mesh::Get("data/model.obj"), player_material, "player");
	addEntity(player);
}


void World::update(float delta_time) {
	if (free_camera) {
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
		*/
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
	}

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

