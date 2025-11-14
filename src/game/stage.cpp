#include "stage.h"
#include "framework/input.h"
#include "framework/framework.h"
#include "player.h"
#include "game/game.h"
#include"framework/utils.h"

void MenuStage::render(Camera* camera) {

}

void MenuStage::update(double seconds_elapsed, Camera* camera) {
    if (Input::isKeyPressed(SDL_SCANCODE_X) || Input::isKeyPressed(SDL_SCANCODE_Z) || (Input::gamepads[0].isButtonPressed(A_BUTTON)) || (Input::gamepads[0].isButtonPressed(B_BUTTON)) || (Input::gamepads[0].isButtonPressed(Y_BUTTON)) || (Input::gamepads[0].isButtonPressed(X_BUTTON))) {
        Game* instance = Game::instance;
        instance->setStage(PLAY_STAGE);
    }
}

PlayStage::PlayStage() {
    world = new World();
}

void PlayStage::render(Camera* camera) {
    if (world)
        world->render(camera);
}

void PlayStage::update(double seconds_elapsed, Camera* camera) {
    if (world)
        world->update(seconds_elapsed);
    Player* player = Player::getInstance();
    //LÓGICA DE CÁMARA
    /*
    if (player && camera)
    {
        // --- Posición OBJETIVO de la cámara ---
        Vector3 player_pos = player->model.getTranslation();
        Vector3 player_front = player->model.frontVector();
        //Vector3 player_up = player->model.rotateVector(Vector3(0, 1, 0)); // ¡Clave!
        // Vector3 player_up = player->model.rotateVector(Vector3(0,1,0)); // COMENTA ESTO
        Vector3 player_up = Vector3(0.0f, 1.0f, 0.0f); // PON ESTO FIJO

        Vector3 target_eye = player_pos - (player_front * 25.0f) + (player_up * 7.0f);

        //on mira la camera al punto delante del jugador
        Vector3 target_center = player_pos + (player_front * 10.0f);

        //iniliciacion
        if (first_frame) {
            camera_current_eye = target_eye;
            camera_current_center = target_center;
            first_frame = false;
        }

        //INTERPOLACIÓN
        // En lugar de saltar, suavizamos el movimiento
        float smoothness = 5.0f;
        camera_current_eye = lerp(camera_current_eye, target_eye, seconds_elapsed * smoothness);
        camera_current_center = lerp(camera_current_center, target_center, seconds_elapsed * smoothness);

        //Actualizar la cámara real
        camera->lookAt(camera_current_eye, camera_current_center, player_up);

    }*/

    player->update(seconds_elapsed);
}