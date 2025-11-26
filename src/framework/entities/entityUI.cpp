#include "framework/entities/entityUI.h"

EntityUI::EntityUI(Vector2 new_size, const Material& material) {
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

	/* -------------- AIXÒ ÉS NOU --------------*/
	// el que fallava era que no feia shader->enable i shader->disable oops
	material.shader->enable();

	material.shader->setUniform("u_model", model);
	material.shader->setUniform("u_viewprojection", viewproj); //error
	material.shader->setUniform("u_color", material.color);
	material.shader->setUniform("u_mask", mask);

	if (material.diffuse) {
		material.shader->setUniform("u_texture", material.diffuse, 0);
	}

	if (is3D) {
		Mesh quad;
		quad.createQuad(position.x, position.y, size.x, size.y, true);
		quad.render(GL_TRIANGLES);

	}
	else {
		mesh->render(GL_TRIANGLES);
	}

	/* -------------- AIXÒ ÉS NOU --------------*/
	material.shader->disable();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	Entity::render(camera);
}

void EntityUI::update(double seconds_elapsed) {
	Vector2 mouse_pos = Input::mouse_position;

	if (button_id != UI_BUTTON_UNIDENTIFIED &&
		mouse_pos.x > (position.x - size.x * 0.5f) &&
		mouse_pos.x < (position.x + size.x * 0.5f) &&
		mouse_pos.y >(position.y - size.y * 0.5f) &&
		mouse_pos.y < (position.y + size.y * 0.5f)) {

		material.color = Vector4(1.25);

		if (Input::wasMouseReleased(SDL_BUTTON_LEFT)) {
			switch (button_id)
			{
			case UI_BUTTON_PLAY:
				/* -------------- AIXÒ ÉS NOU --------------*/
				// setStage passa a ser un mètode amb dues entrades
				Game::instance->setStage(PLAY_STAGE, MAIN_MENU);
				break;
			case UI_BUTTON_EXIT:
				break;
			defalut:
				break;
			}
		}
		else if (Input::isMousePressed(SDL_BUTTON_LEFT)) {
			material.color = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
		}
	}
	else {
		/* -------------- AIXÒ ÉS NOU --------------*/
		//Ho he canviat a verd per què la barra de vida em sortia blanca sempre si ho deixava blanc, cal afegir una opció per a la barra de vida a aquest métode en un futur, crec 
		material.color = Vector4::GREEN;
	}
}