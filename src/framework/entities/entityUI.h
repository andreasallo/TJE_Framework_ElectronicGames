#pragma once 

#include "framework/entities/entityMesh.h"
#include "framework/input.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "game/game.h"
/*----------------Això és nou----------------*/
//player
#include "game/player.h"

enum eUIButtonID {
	UI_BUTTON_UNIDENTIFIED,
	UI_BUTTON_PLAY,
	UI_BUTTON_EXIT,
	/* -------------- AIXÒ ÉS NOU --------------*/
	//nous tipus de botó
	UI_HEALTHBAR,
	UI_TURBO,
	UI_TIMETOEND,
	UI_TURBO_YN,
	UI_BACKGROUND,
	UI_BUTTON_RESUME,
};

class EntityUI : public EntityMesh {
public:
	EntityUI() {};
	EntityUI(Vector2 new_size, const Material& material);
	EntityUI(Vector2 new_pos, Vector2 new_size, const Material& material, eUIButtonID new_button_id);

	/*----------------Això és nou----------------*/
	//he eliminat mesh i material

	Vector2 size = Vector2(0.0f, 0.0f);
	Vector2 position = Vector2(0.0f, 0.0f);
	eUIButtonID button_id = UI_BUTTON_UNIDENTIFIED;
	bool is3D = false;
	/*----------------Això és nou----------------*/
	//mask és un float ara, i és 1
	float mask = 1.0f;

	void render(Camera* camera);
	void update(double seconds_elapsed);

};

