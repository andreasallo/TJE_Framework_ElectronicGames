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
#include "graphics/texture.h"

enum MenuOptions {
    MENU_START,
    MENU_EXIT,
    MENU_TOTAL
};


/* -------------- AIXÒ ÉS NOU --------------*/
void Stage::onEnter(eStage next_stage) {
    Game* instance = Game::instance;

    switch (next_stage) {
    case PLAY_STAGE:
        Audio::Play("data/atmospheric.wav", 1.0f, BASS_SAMPLE_LOOP);
        break;
    case MAIN_MENU:
        break;
    }
}


void Stage::onExit(eStage last_stage) {

}


MenuStage::MenuStage() {
    background.loadTGA("data/2.tga");

    background.flipY();

    if (background.data) {
        background_tex = new Texture(&background);
    }
}

void MenuStage::render(Camera* camera) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    if (background_tex) {
        //toViewport dibuixa la textura a tota la pantalla
        background_tex->toViewport();
    }

    //start_button->render(camera);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MenuStage::update(double seconds_elapsed, Camera* camera) {
    Game* instance = Game::instance;
    //start_button->update();
    //SDL_PollEvent(&event);

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

        /* -------------- AIXÒ ÉS NOU --------------*/
        // setStage passa a ser un mètode amb dues entrades
        instance->setStage(PLAY_STAGE, MAIN_MENU);
    }

}

PlayStage::PlayStage() {
    world = new World();

    Game* instance = Game::instance;
    /* -------------- AIXÒ ÉS NOU --------------*/
    // he creat un vector position per passar-li al constructor i noves dades de size
    Vector2 position = Vector2(instance->window_width / 2, instance->window_height - 25);
    Vector2 size = Vector2(350, 30);

    Material material;
    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
    /* -------------- AIXÒ ÉS NOU --------------*/
    // el color de la barra de vida
    material.color = Vector4::GREEN;

    health_bar = new EntityUI(position, size, material, eUIButtonID::UI_BUTTON_UNIDENTIFIED);
}

void PlayStage::render(Camera* camera) {
    Camera* camera2d = Game::instance->camera2d;
    if (world) {
        world->render(camera);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        health_bar->render(camera2d);
    }
}

void PlayStage::update(double seconds_elapsed, Camera* camera) {
    if (world) {
        world->update(seconds_elapsed);
        health_bar->update(seconds_elapsed);
    }
    Player* player = Player::getInstance();
}