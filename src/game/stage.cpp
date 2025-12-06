
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
        //Audio::Play("data/atmospheric.wav", 1.0f, BASS_SAMPLE_LOOP);
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
    /* -------------- AIXÒ ÉS NOU --------------*/
    //He afegit tot això inicialitza els botons del menú d'inici
    Game* instance = Game::instance;
    Vector2 position_a = Vector2(instance->window_width / 2, instance->window_height - 150);
    Vector2 position_b = Vector2(instance->window_width / 2, instance->window_height - 80);
    Vector2 size = Vector2(150, 40);

    Material material;
    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
    //material.color = Vector4::WHITE;
    //material.diffuse = Texture::Get("data/textures/play.png");

    start_button = new EntityUI(position_a, size, material, eUIButtonID::UI_BUTTON_PLAY);
    exit_button = new EntityUI(position_b, size, material, eUIButtonID::UI_BUTTON_EXIT);
}

void MenuStage::render(Camera* camera) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* -------------- AIXÒ ÉS NOU --------------*/
    //Guarda linstancies de la camara 2d i del joc
    Game* instance = Game::instance;
    Camera* camera2d = instance->camera2d;

    /**/
    if (background_tex) {
        //toViewport dibuixa la textura a tota la pantalla
        background_tex->toViewport();
    }
    /* -------------- AIXÒ ÉS NOU --------------*/
    //utilitza la camara 2d per a renderitzar cada un dels botons
    start_button->render(camera2d);
    exit_button->render(camera2d);
    /* -------------- AIXÒ ÉS NOU --------------*/
    //Draw Text
    drawText(instance->window_width / 2, instance->window_height / 2, "Play", Vector3(0.0, 0.0, 0.0), 20);
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
    /*
    if ((Input::isKeyPressed(SDL_SCANCODE_X) || Input::isKeyPressed(SDL_SCANCODE_Z) || (Input::gamepads[0].isButtonPressed(A_BUTTON)) || (Input::gamepads[0].isButtonPressed(B_BUTTON)) || (Input::gamepads[0].isButtonPressed(Y_BUTTON)) || (Input::gamepads[0].isButtonPressed(X_BUTTON)))) {
        instance->audio->Play("sounds/coin.wav");
        instance->setStage(PLAY_STAGE, MAIN_MENU);
    }
    */

    /* -------------- AIXÒ ÉS NOU --------------*/
    //update del start button i exit button
    start_button->update(seconds_elapsed);
    exit_button->update(seconds_elapsed);
}

PlayStage::PlayStage() {
    world = new World();

    Game* instance = Game::instance;

    /*----------------Això és nou----------------*/
    //Bàsicament es crean totes els EntityUI que utilitza el Play Stage copia tot el que hi ha a partir d'aquí

    Vector2 position_a;
    Vector2 size;
    Material material_a;

    /*
    // -------------Barra de Vida - Position/Size-------------
    position_a = Vector2(instance->window_width / 2, instance->window_height - 45);
    size = Vector2(350, 30);

    material_a.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/status_bar.fs"); //healthbar.fs // abans: status_bar.fs
    material_a.diffuse = Texture::Get("data/textures/Healthy1_Large.png");
    material_a.color = Vector4(0.25);

    health_bar = new EntityUI(position_a, size, material_a, eUIButtonID::UI_HEALTHBAR);

    // -------------Barra de Turbo - Position/Size-------------
    position_a = Vector2(instance->window_width - 25, instance->window_height / 2);
    size = Vector2(25, 267);

    Material material_b;
    material_b.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
    material_b.color = Vector4(1.0, 1.0, 0.0, 1.0);

    turbo_bar = new EntityUI(position_a, size, material_b, eUIButtonID::UI_TURBO);
    */

    // -------------Marcador Turbo y/n - Position/Size-------------
    /*
    Vector2 position = Vector2(instance->window_width - 35, instance->window_height - 40);
    size = Vector2(40, 40);

    Material material;
    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
    material.color = Vector4::WHITE;

    turbo_indicator = new EntityUI(position, size, material, eUIButtonID::UI_TURBO_YN);
    */

    // -------------Marcador Temps fins el final del Nivell - Position/Size-------------
    /*
    position = Vector2(40, instance->window_height - 35);
    size = Vector2(40, 40);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
    material.color = Vector4::WHITE;

    level_indicator = new EntityUI(position, size, material, eUIButtonID::UI_TIMETOEND);
    */

    // -------------Bg-------------
    Vector2 position = Vector2(instance->window_width / 2, instance->window_height - 45);
    size = Vector2(450, 90);

    //Material material_a;
    material_a.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material_a.diffuse = Texture::Get("data/textures/SingleEdge_Large.png");

    healthbar_background = new EntityUI(position, size, material_a, eUIButtonID::UI_BACKGROUND);

    // -------------Bg-T-------------
    /**/
    position = Vector2(instance->window_width - 25, instance->window_height / 2);
    size = Vector2(450, 90);

    material_a.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture_inv.fs");
    material_a.diffuse = Texture::Get("data/textures/MetalEnd_Small.png");

    turbo_bg = new EntityUI(position, size, material_a, eUIButtonID::UI_BACKGROUND);

    //SlotStripe_Black
    position = Vector2(55, instance->window_height - 55);
    size = Vector2(140, 140);

    material_a.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material_a.diffuse = Texture::Get("data/textures/SlotStripe_Green.png");

    level_indicator_bg = new EntityUI(position, size, material_a, eUIButtonID::UI_BACKGROUND);

    //SlotStripe_Black
    position = Vector2(instance->window_width - 55, instance->window_height - 55);

    material_a.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture_inv.fs");
    material_a.diffuse = Texture::Get("data/textures/SlotStripe_Green_inv.png");

    turbo_indicator_bg = new EntityUI(position, size, material_a, eUIButtonID::UI_BACKGROUND);

    // Botó Resume i 
    position = Vector2(instance->window_width / 2, instance->window_height - 150);
    size = Vector2(150, 40);

    Material material;
    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
    //material.color = Vector4::WHITE;
    //material.diffuse = Texture::Get("data/textures/play.png");

    resume_button = new EntityUI(position, size, material, eUIButtonID::UI_BUTTON_RESUME);

    position = Vector2(instance->window_width / 2, instance->window_height - 80);

    exit_button = new EntityUI(position, size, material, eUIButtonID::UI_BUTTON_EXIT);
}

void PlayStage::render(Camera* camera) {
    /* -------------- AIXÒ ÉS NOU --------------*/
    //Instanciar la camara i el joc
    Game* instance = Game::instance;
    Camera* camera2d = instance->camera2d;
    Player* player = Player::getInstance();

    if (world) {
        world->render(camera);


        /* -------------- AIXÒ ÉS NOU --------------*/
        //render de cada EntityUI que hem creat al constructor
        player->health_bar->render(camera2d);
        player->turbo_bar->render(camera2d);
        healthbar_background->render(camera2d);
        level_indicator_bg->render(camera2d);
        turbo_indicator_bg->render(camera2d);
        turbo_bg->render(camera2d);

        //turbo_indicator->render(camera2d);
        //level_indicator->render(camera2d);

        /*----------------Això és nou----------------*/
        //is està pausat el joc, renderitza els botons del menú de pausa
        if (instance->isPaused) {
            //renderitzes el menú de pause
            resume_button->render(camera2d);
            exit_button->render(camera2d);
        }

        /*----------------Això és nou----------------*/
        //he provat a renderitzar un comptador per lo de la distància fins al final del nivell, de moment no funciona
        /*
        std::string t = std::to_string(instance->elapsed_time);
        drawText(instance->window_width / 2, instance->window_height / 2, t, Vector3(1.0), 10);*/
    }
}

void PlayStage::update(double seconds_elapsed, Camera* camera) {
    /*----------------Això és nou----------------*/
    //instancies de game i player
    Game* instance = Game::instance;
    Player* player = Player::getInstance();

    /*----------------Això és nou----------------*/
    //si cliques P, s'activa la bandera del menú de pausa
    if (Input::isKeyPressed(SDL_SCANCODE_P) && pWasPressed == false) { instance->isPaused = !instance->isPaused; }

    if (world) {
        /*----------------Això és nou----------------*/
        //si està pausat, deixem d'actualitzar el mon i es deixa de moure el joc
        //si està pausat, actualitzem també els botons del menu de pausa
        if (!instance->isPaused) {
            world->update(seconds_elapsed);
        }
        else {
            resume_button->update(seconds_elapsed);
            exit_button->update(seconds_elapsed);
        }

        /* -------------- AIXÒ ÉS NOU --------------*/
        //update de les entities de player
        player->health_bar->update(seconds_elapsed);
        //healthbar_background->update(seconds_elapsed);
        player->turbo_bar->update(seconds_elapsed);

        //turbo_indicator->update(seconds_elapsed);
        //level_indicator->update(seconds_elapsed);
        /*
        std::string t = std::to_string(seconds_elapsed);
        drawText(instance->window_width / 2, instance->window_height / 2, t, Vector3(1.0), 10);*/
    }
    //Player* player = Player::getInstance();

    /*----------------Això és nou----------------*/
    //sense això, no funciona laopció de pausa
    pWasPressed = Input::isKeyPressed(SDL_SCANCODE_P);
}


/*----------------Això és nou----------------*/
//metodes dels stages que ens falten, el de lore i el tutorial i el del final del nivell.
TutorialStage::TutorialStage() {

}

void TutorialStage::render(Camera* camera) {

}

void TutorialStage::update(double seconds_elapsed, Camera* camera) {
    Game* instance = Game::instance;
    /**/
    if ((Input::isKeyPressed(SDL_SCANCODE_X) || Input::isKeyPressed(SDL_SCANCODE_Z) || (Input::gamepads[0].isButtonPressed(A_BUTTON)) || (Input::gamepads[0].isButtonPressed(B_BUTTON)) || (Input::gamepads[0].isButtonPressed(Y_BUTTON)) || (Input::gamepads[0].isButtonPressed(X_BUTTON)))) {
        instance->audio->Play("sounds/coin.wav");
        instance->setStage(PLAY_STAGE, TUTORIAL_STAGE);
    }

}

EndStage::EndStage() {

}

void EndStage::render(Camera* camera) {

}

void EndStage::update(double seconds_elapsed, Camera* camera) {

}
