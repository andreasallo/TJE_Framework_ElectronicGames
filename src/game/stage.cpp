#include "stage.h"
#include "framework/input.h"
#include "framework/framework.h"
#include "player.h"
#include "game/game.h"
#include"framework/utils.h"
#include "framework/audio.h"
#include <cmath>
#include "framework/world.h"
#include "framework/camera.h"

enum MenuOptions {
    MENU_START,
    MENU_EXIT,
    MENU_TOTAL
};


void MenuStage::render(Camera* camera) {

}

void MenuStage::update(double seconds_elapsed, Camera* camera) {   
    //SDL_PollEvent(&event);
    Game* instance = Game::instance;
    
    // W- selected_option = (selected_option - 1 + MENU_TOTAL) % MENU_TOTAL on menu total és la posició final del menú
    // S- selected_option = (selected_option + 1) % MENU_TOTAL
    /*while (SDL_PollEvent(&event)) { 
        if (event.type == SDL_KEYDOWN) {

        }
    }*/
    /*
    switch (event.keysym.sym)
    {
    case SDL_SCANCODE_W:
    case SDL_SCANCODE_UP:
        std::cout << "UP!!";
        break;
    case SDL_SCANCODE_S:
    case SDL_SCANCODE_DOWN:
        std::cout << "DOWN!!";
        break;
    case SDL_SCANCODE_X:
    case SDL_SCANCODE_Z:
        instance->audio->Play("sounds/coin.wav");
        instance->setStage(PLAY_STAGE);
        break;
    default:
        break;
    }
    */
    // W- selected_option = (selected_option - 1 + MENU_TOTAL) % MENU_TOTAL on menu total és la posició final del menú
    // S- selected_option = (selected_option + 1) % MENU_TOTAL
    /*
    if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP) || Input::gamepads[0].isButtonPressed(PAD_UP)) {
        std::cout << "UP";
        selected_option = (selected_option - 1 + MENU_TOTAL) % MENU_TOTAL;
        if (selected_option == MENU_START) { std::cout << "start"; }
        else if (selected_option == MENU_EXIT) { std::cout << "exit"; }
    }
    else if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN) || Input::gamepads[0].isButtonPressed(PAD_DOWN)) {
        selected_option = (selected_option + 1) % MENU_TOTAL;
        if (selected_option == MENU_START) { std::cout << "start"; }
        else if (selected_option == MENU_EXIT) { std::cout << "exit"; }
    }
    else if ((Input::isKeyPressed(SDL_SCANCODE_X) || Input::isKeyPressed(SDL_SCANCODE_Z) || (Input::gamepads[0].isButtonPressed(A_BUTTON)) || (Input::gamepads[0].isButtonPressed(B_BUTTON)) || (Input::gamepads[0].isButtonPressed(Y_BUTTON)) || (Input::gamepads[0].isButtonPressed(X_BUTTON))) && selected_option == MENU_START) {
        instance->audio->Play("sounds/coin.wav");
        instance->setStage(PLAY_STAGE);
    }
    else if ((Input::isKeyPressed(SDL_SCANCODE_X) || Input::isKeyPressed(SDL_SCANCODE_Z) || (Input::gamepads[0].isButtonPressed(A_BUTTON)) || (Input::gamepads[0].isButtonPressed(B_BUTTON)) || (Input::gamepads[0].isButtonPressed(Y_BUTTON)) || (Input::gamepads[0].isButtonPressed(X_BUTTON))) && selected_option == MENU_EXIT) {
        instance->audio->Play("sounds/coin.wav");
        instance->must_exit = true;
    }
    */
    
    // ANTIC
    /**/
    if ((Input::isKeyPressed(SDL_SCANCODE_X) || Input::isKeyPressed(SDL_SCANCODE_Z) || (Input::gamepads[0].isButtonPressed(A_BUTTON)) || (Input::gamepads[0].isButtonPressed(B_BUTTON)) || (Input::gamepads[0].isButtonPressed(Y_BUTTON)) || (Input::gamepads[0].isButtonPressed(X_BUTTON)))) {
        instance->audio->Play("sounds/coin.wav");
        instance->setStage(PLAY_STAGE);
    }
    
}

PlayStage::PlayStage() {
    world = new World();

    Audio::Play("data/atmospheric.wav", 1.0f, BASS_SAMPLE_LOOP);
}

void PlayStage::render(Camera* camera) {
    if (world)
        world->render(camera);
}

void PlayStage::update(double seconds_elapsed, Camera* camera) {
    if (world)
        world->update(seconds_elapsed);
    Player* player = Player::getInstance();
}