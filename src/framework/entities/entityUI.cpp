
#include "framework/entities/entityUI.h"

EntityUI::EntityUI(Vector2 new_size, const Material & material) {
	size = new_size;

	this->material = material;

	if (!this->material.shader) {
		this->material.shader = Shader::Get("data/shaders/basic.vs", material.diffuse ? "data/shaders/fragment.fs" : nullptr);
	}
}

EntityUI::EntityUI(Vector2 new_pos, Vector2 new_size, const Material& material, eUIButtonID new_button_id) {
	position = new_pos;
	size = new_size;
	button_id = new_button_id;

	mesh = new Mesh();
	mesh->createQuad(position.x, position.y, size.x, size.y, true);
	this->material = material;

	if (!this->material.shader) {
		this->material.shader = Shader::Get("data/shaders/basic.vs", material.diffuse ? "data/shaders/fragment.fs" : nullptr);
	}
}

void EntityUI::render(Camera* camera) {
	Matrix44 viewproj = camera->viewprojection_matrix;

	/*----------------Això és nou----------------*/
	//Això ho tenia posat al Stage render, ara aquí dins
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	material.shader->enable();

	material.shader->setUniform("u_model", model);
	material.shader->setUniform("u_viewprojection", viewproj);
	material.shader->setUniform("u_color", material.color);
	material.shader->setUniform("u_mask", mask);

	if (material.diffuse) {
		/*----------------Això és nou----------------*/
		//SetTexture enlloc de setUniform
		material.shader->setTexture("u_texture", material.diffuse, 0);
	}

	if (is3D) {
		Mesh quad;
		quad.createQuad(position.x, position.y, size.x, size.y, true);
		quad.render(GL_TRIANGLES);

	}
	else {
		mesh->render(GL_TRIANGLES);
	}

	material.shader->disable();

	/*----------------Això és nou----------------*/
	//Això ho tenia posat al Stage render, ara aquí dins, s'ha de desactivar
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	Entity::render(camera);
}

void EntityUI::update(double seconds_elapsed) {
	Vector2 mouse_pos = Input::mouse_position;
	/*----------------Això és nou----------------*/
	//instancies de game i player
	Game* instance = Game::instance;
	Player* player = Player::instance;

	/*----------------Això és nou----------------*/
	//he  posat que només entri si el tipus de EntityUI és un tipus 'button'
	if ((button_id == UI_BUTTON_PLAY || button_id == UI_BUTTON_EXIT || button_id == UI_BUTTON_RESUME) &&
		mouse_pos.x > (position.x - size.x * 0.5f) &&
		mouse_pos.x < (position.x + size.x * 0.5f) &&
		mouse_pos.y >(position.y - size.y * 0.5f) &&
		mouse_pos.y < (position.y + size.y * 0.5f)) {

		material.color = Vector4(1.25);

		if (Input::wasMouseReleased(SDL_BUTTON_LEFT)) {
			switch (button_id)
			{
			case UI_BUTTON_PLAY:
				/*----------------Això és nou----------------*/
				//que vagi del menu al stage nou de Lore i Tutorial
				instance->setStage(TUTORIAL_STAGE, MAIN_MENU);
				break;
				/*----------------Això és nou----------------*/
				//el case del nou botó de la pantalla de pausa
			case UI_BUTTON_RESUME:
				instance->isPaused = false;
				break;
			case UI_BUTTON_EXIT:
				/*----------------Això és nou----------------*/
				//fa que si cliques el botó de exit, tanca el programa
				instance->must_exit = true;
				break;
				/*----------------Això és nou----------------*/
				//default estava malament escrit
			default:
				break;
			}
		}
		else if (Input::isMousePressed(SDL_BUTTON_LEFT)) {
			/*----------------Això és nou----------------*/
			//h canviat llegerament el color
			material.color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
		}
	}
	/* -------------- AIXÒ ÉS NOU --------------*/
	//però no fa res lol
	else if (button_id == UI_BUTTON_EXIT || button_id == UI_BUTTON_PLAY) {
		material.color = Vector4::WHITE;
	}
	else if (button_id == UI_HEALTHBAR) {
	}
	else if (button_id == UI_TURBO) {
	}
}

