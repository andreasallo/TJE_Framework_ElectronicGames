#pragma once 

#include "framework/entities/entityMesh.h"
#include "framework/input.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "game/game.h"

enum eUIButtonID {
	UI_BUTTON_UNIDENTIFIED,
	UI_BUTTON_PLAY,
	UI_BUTTON_EXIT,
};

class EntityUI : public EntityMesh {
public:
	EntityUI() {};
	EntityUI(Vector2 new_size, const Material& material);
	EntityUI(Vector2 new_pos, Vector2 new_size, const Material& material, eUIButtonID new_button_id);
	
	Mesh* mesh;
	Material material;
	Vector2 size = Vector2(0.0f, 0.0f);
	Vector2 position = Vector2(0.0f, 0.0f);
	eUIButtonID button_id;

	void render(Camera* camera);
	void update(double seconds_elapsed);

};