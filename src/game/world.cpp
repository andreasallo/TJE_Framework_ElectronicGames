#include "world.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"
#include "scene_parser.h"


World::World() {
	instance = this;

	//LOAD SCENE	
	root = new Entity();
	root->name = "root";
	SceneParser parser;
	parser.parse("data/myscene.scene", root);

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

	}

	/*//CREATE HEIGHTMAP
	{
		float size = 50.0f;

		Mesh* heightmap_mesh = new Mesh();
		heightmap_mesh->createSubdividedPlane(size);

		Material heightmap_material;
		heightmap_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		heightmap_material.diffuse = Texture::Get("data/textures/heightmap.png");
		heightmap_material.color = Vector4(1.0, 1.0, 1.0, 1.0);

		EntityMesh* heightmap = new EntityMesh(heightmap_mesh, heightmap_material);
		heightmap->culling = false;
		heightmap->model.translate(-size * 0.5f, 0.0f, -size * 0.5f);
		root->addChild(heightmap);
	}
	*/
	/*
	{
		Material material;
		material.diffuse= Texture::Get("data/textures/texture.tga");
		EntityMesh* entity = new EntityMesh(Mesh::Get("data/meshes/box.ASE"), material, "box");
		root->addChild(entity);
	}
	*/
}

void World::render(Camera* camera) {
	// Set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

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
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	root->render(camera);
	// Draw the floor grid
	drawGrid();

	// Render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}

void World::update(double seconds_elapsed, Camera* camera) {
	Game* instance = Game::getInstance();

	float speed = seconds_elapsed * instance->mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	// Example
	instance->angle += (float)seconds_elapsed * 10.0f;

	// Mouse input to rotate the cam
	if (Input::isMousePressed(SDL_BUTTON_LEFT) || instance->mouse_locked) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}

	// Async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
}