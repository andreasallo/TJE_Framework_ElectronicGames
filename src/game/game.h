#pragma once

#include "framework/includes.h"
#include "framework/camera.h"
#include "framework/utils.h"
#include "framework/entities/entity.h"
#include "framework/entities/entityMesh.h"
#include "framework/world.h"
#include "stage.h"

class Stage;
class World;
class Audio;
/* -------------- AIXÒ ÉS NOU --------------*/
// sinó no funciona la nova declaració de setStage
enum eStage;

class Game
{
public:
	static Game* instance;

	//debug
	static bool debug;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;

	//some globals
	long frame;
	float time;
	float elapsed_time;
	int fps;
	bool must_exit;

	//world
	//World* world;

	//audio
	//Llista de àudios???
	Audio* audio;

	/* -------------- AIXÒ ÉS NOU --------------*/
	//pause menu flag
	bool isPaused = false;

	//stage
	Stage* current_stage;

	//some vars
	Camera* camera; //our global camera
	Camera* camera2d;
	bool mouse_locked; //tells if the mouse is locked (not seen)

	//some more vars (abans globals)
	float angle = 0;
	float mouse_speed = 10.0f;

	Game(int window_width, int window_height, SDL_Window* window);

	//main functions
	void render(void);
	void update(double dt);

	void setMouseLocked(bool must_lock);

	//events
	void onKeyDown(SDL_KeyboardEvent event);
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown(SDL_MouseButtonEvent event);
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);

	//metodes get !!!
	static Game* getInstance() { return instance; };
	static Game* getEvent() {};
	//static EntityMesh* getSkybox() { return instance->skybox; };
	//static Entity* getRoot() { return instance->root; };

	//stage !!!
	void setStage(eStage new_stage, eStage last_stage);

	//debug
	static bool IsDebugMode() { return debug; }
};