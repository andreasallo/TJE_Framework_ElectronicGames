
#pragma once

#include "game/game.h"
#include "framework/camera.h"
#include "framework/world.h"
#include "graphics/texture.h"
#include "framework/entities/entityUI.h"
#include "graphics/fbo.h"


class Game;
class World;
class EntityUI;
/* -------------- AIXÒ ÉS NOU --------------*/
class Player;

enum eStage {
	MAIN_MENU,
	PLAY_STAGE,
	/* -------------- AIXÒ ÉS NOU --------------*/
	//pel nou stage
	TUTORIAL_STAGE,
	END_STAGE,
};

class Stage {
public:

	//PREPARATIUS PER FER EFECTES AMB SHADERS
	FBO* fbo = nullptr;
	Mesh* screen_quad = nullptr;

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

	

	/* -------------- AIXÒ ÉS NOU --------------*/
	//la referència de e les entities que inicialitzem al constructor
	EntityUI* start_button = nullptr;
	EntityUI* exit_button = nullptr;
	
	//títol
	EntityUI* title1 = nullptr;
	EntityUI* title3 = nullptr;
	EntityUI* title2 = nullptr;
	EntityUI* title4 = nullptr;

	EntityMesh* skybox;

	MenuStage();

	/*----------------Això és nou----------------*/
	//menu stage render
	void render(Camera* camera2d);
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

	float vignette_intensity = 0.0f;

	/*----------------Això és nou----------------*/
	//les flags del menu de pausa
	bool pWasPressed = false;
	bool pIsPressed = false;

	class World* world = nullptr;

	/* -------------- AIXÒ ÉS NOU --------------*/
	//la referència de les entities que inicialitzem al constructor
	//EntityUI* health_bar = nullptr;
	//EntityUI* turbo_bar = nullptr;

	/*----------------Això és nou----------------*/
	//entities UI
	EntityUI* healthbar_background = nullptr;
	EntityUI* level_indicator_bg = nullptr;
	EntityUI* turbo_indicator_bg = nullptr;
	//EntityUI* turbo_bg = nullptr;
	EntityUI* turbo_indicator = nullptr;
	EntityUI* level_indicator = nullptr;

	/*----------------Això és nou----------------*/
	//Pause Menu Entities
	EntityUI* resume_button = nullptr;
	EntityUI* exit_button = nullptr;

	EntityUI* pause_text = nullptr;
	EntityUI* pause_shader = nullptr;


	PlayStage();

	void render(Camera* camera);
	void update(double seconds_elapsed, Camera* camera);
};

/* -------------- AIXÒ ÉS NOU --------------*/
//TutorialStage i EndStage
class TutorialStage : public Stage {
public:
	eStage type = TUTORIAL_STAGE;

	EntityMesh* skybox;

	//Lore
	EntityUI* lore1 = nullptr;
	EntityUI* lore2 = nullptr;
	EntityUI* lore3 = nullptr;
	EntityUI* lore4 = nullptr;
	EntityUI* lore5 = nullptr;
	EntityUI* lore6 = nullptr;
	EntityUI* lore7 = nullptr;
	EntityUI* lore8 = nullptr;	
	EntityUI* lore9 = nullptr;
	EntityUI* lore10 = nullptr;
	EntityUI* lore11 = nullptr;
	EntityUI* lore12 = nullptr;	
	EntityUI* lore13 = nullptr;
	EntityUI* lore14 = nullptr;
	EntityUI* lore15 = nullptr;
	EntityUI* lore16 = nullptr;

	//Tutorial
	EntityUI* title1 = nullptr;
	EntityUI* title2 = nullptr;
	EntityUI* spacebar = nullptr;
	EntityUI* p = nullptr;
	EntityUI* z = nullptr;
	EntityUI* arrows = nullptr;
	EntityUI* play1 = nullptr;
	EntityUI* play2 = nullptr;
	EntityUI* play3 = nullptr;
	EntityUI* info1 = nullptr;
	EntityUI* info2 = nullptr;
	EntityUI* info3 = nullptr;
	EntityUI* info4 = nullptr;


	bool zWasPressed = false;
	int block = 0; //posar a 0 en acabar les proves.
	bool heldDown = false;

	TutorialStage();

	void render(Camera* camera);
	void update(double seconds_elapsed, Camera* camera);
};

class EndStage : public Stage {
public:
	eStage type = END_STAGE;

	EndStage();

	void render(Camera* camera);
	void update(double seconds_elapsed, Camera* camera);
private:
	EntityUI* end_screen = nullptr;
};
