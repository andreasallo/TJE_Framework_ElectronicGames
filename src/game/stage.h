#pragma once

#include "game/game.h"
#include "framework/camera.h"
#include "framework/world.h"
#include "graphics/texture.h"
#include "framework/entities/entityUI.h"


class Game;
class World;
class EntityUI;

enum eStage {
	MAIN_MENU,
	PLAY_STAGE,
};

class Stage {
public:

	//Game* instance = Game::instance;

	virtual void render(Camera* camera) {} // o = 0, si vols que les altres classes stage facin render si o si
	virtual void update(double seconds_elapsed, Camera* camera) {}

	/* -------------- AIXÒ ÉS NOU --------------*/
	// he canviat els mètodes a static per poder crider-los des de game.cpp
	static void onEnter(eStage next_stage);
	static void onExit(eStage last_stage);

};

class MenuStage : public Stage {
public:
	eStage type = MAIN_MENU;

	//EntityUI* start_button;

	int selected_option = 0;
	SDL_Event event;
	Image background;
	Texture* background_tex = nullptr;

	MenuStage();

	void render(Camera* camera);
	void update(double seconds_elapsed, Camera* camera);

};

class PlayStage : public Stage {
public:
	eStage type = PLAY_STAGE;

	/*
	Vector3 camera_current_eye;
	Vector3 camera_current_center;
	bool first_frame = true;
	*/

	class World* world = nullptr;

	EntityUI* health_bar = nullptr;

	PlayStage();

	void render(Camera* camera);
	void update(double seconds_elapsed, Camera* camera);
};