#include "stage.h"

void MenuStage::render(Camera* camera) {

}

void MenuStage::update(double seconds_elapsed, Camera* camera) {
	if (Input::isKeyPressed(SDL_SCANCODE_X) || Input::isKeyPressed(SDL_SCANCODE_Z) || (Input::gamepads[0].isButtonPressed(A_BUTTON)) || (Input::gamepads[0].isButtonPressed(B_BUTTON)) || (Input::gamepads[0].isButtonPressed(Y_BUTTON)) || (Input::gamepads[0].isButtonPressed(X_BUTTON))) {
		Game* instance = Game::instance;
		instance->setStage(PLAY_STAGE);
	}
}

void PlayStage::render(Camera* camera) {
	World* instance = World::instance;
	instance->render(camera);
}

void PlayStage::update(double seconds_elapsed, Camera* camera) {
	World* instance = World::instance;
	instance->update(seconds_elapsed, camera);
}