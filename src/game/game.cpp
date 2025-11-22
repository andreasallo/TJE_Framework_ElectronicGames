#include "game.h"
#include "framework/utils.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/fbo.h"
#include "graphics/shader.h"
#include "framework/input.h"
#include "scene_parser.h"
#include "framework/entities/entityMesh.h"
#include "framework/entities/entity.h"

#include "framework/audio.h"

#include <cmath>
#include <iostream>

//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;

Game* Game::instance = NULL;
World* World::instance = NULL;

MenuStage* menu_stage;
PlayStage* play_stage;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	// OpenGL flags
	glEnable(GL_CULL_FACE); //render both sides of every triangle
	glEnable(GL_DEPTH_TEST); //check the occlusions using the Z buffer

	// Create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 1.f, 1.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	//world = new World();

	menu_stage = new MenuStage();
	play_stage = new PlayStage();

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

	{
		/*Material material;
		material.diffuse= Texture::Get("data/textures/texture.tga");
		EntityMesh* entity = new EntityMesh(Mesh::Get("data/meshes/box.ASE"), material, "box");
		root->addChild(entity);
	}*/

	current_stage = play_stage;
	
	/*
	Camera camera2D;
	camera2D.view_matrix = Matrix44(); // Set View to identity
	camera2D.setOrthographic(0, window_width, window_height, 0, -1, 1);
	shader->setUniform("u_viewprojection", camera2D.viewprojection_matrix);
	*/

	Audio::Init();

	audio = new Audio();
	//audio->Play("sounds/coin.wav");


	// Example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	// Hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	current_stage->render(camera);


	// Swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	current_stage->update(seconds_elapsed, camera);
}

void Game::setStage(int new_stage) {
	if (new_stage == MAIN_MENU) {
		current_stage = menu_stage;
	}
	else if (new_stage == PLAY_STAGE) {
		current_stage = play_stage;
	}
}


//Keyboard event handler (sync input)
void Game::onKeyDown(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_ESCAPE:
		must_exit = true; break; //ESC key, kill the app
	case SDLK_F1:
		Shader::ReloadAll(); break;
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{

}

void Game::onMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
		SDL_SetRelativeMouseMode((SDL_bool)(mouse_locked));
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{

}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1f : 0.9f;
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onResize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport(0, 0, width, height);
	camera->aspect = width / (float)height;
	window_width = width;
	window_height = height;

}

void Game::setMouseLocked(bool must_lock)
{
	SDL_ShowCursor(!must_lock);

	SDL_SetRelativeMouseMode((SDL_bool)must_lock);

	mouse_locked = must_lock;
}