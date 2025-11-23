#include "framework/entities/entityUI.h"

EntityUI::EntityUI(	Vector2 new_size, const Material& material) {
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
	Game* instance = Game::instance;
	Camera* camera2d = new Camera();
	camera2d->view_matrix = Matrix44();
	camera2d->setOrthographic(0, instance->window_width, instance->window_height, 0, -1, -1);
	camera2d->updateProjectionMatrix();
	Matrix44 viewproj = camera2d->viewprojection_matrix;

	material.shader->setUniform("u_model", model);
	material.shader->setUniform("u_viewprojection", viewproj); //viewproj = null??
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


	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	Entity::render(camera2d);
}

void EntityUI::update(double seconds_elapsed) {
	Vector2 mouse_pos = Input::mouse_position;

	if (button_id != UI_BUTTON_UNIDENTIFIED &&
		mouse_pos.x > (position.x - size.x * 0.5f) &&
		mouse_pos.x < (position.x + size.x * 0.5f) &&
		mouse_pos.y > (position.y - size.y * 0.5f) &&
		mouse_pos.y < (position.y + size.y * 0.5f)) {

		material.color = Vector4(1.25);
		
		if (Input::wasMouseReleased(SDL_BUTTON_LEFT)) {
			switch (button_id)
			{
			case UI_BUTTON_PLAY:
				Game::instance->setStage(PLAY_STAGE);
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
		material.color = Vector4::WHITE;
	}
}