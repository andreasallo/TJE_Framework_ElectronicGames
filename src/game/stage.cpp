
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
#include "graphics/shader.h"
#include "graphics/fbo.h"

enum MenuOptions {
    MENU_START,
    MENU_EXIT,
    MENU_TOTAL
};

HCHANNEL title_screen;
HCHANNEL gameplay;

/* -------------- AIXÒ ÉS NOU --------------*/
void Stage::onEnter(eStage next_stage) {
    PlayStage* play_stage = Game::instance->play_stage;
    Stage* stage = Game::instance->current_stage;

    switch (next_stage) {
    case PLAY_STAGE:
        //comprova que la partida anterior ha acabat abans de iniciar el joc, i si ho ha fet, crea un nou world, reutilitzant la instància de world del play stage.
        if (play_stage->world->level_finished) {
            play_stage->world = new World();
        }
        gameplay = Audio::Play("data/atmospheric.wav", 1.0f, BASS_SAMPLE_LOOP);
        break;
    case MAIN_MENU:
        title_screen = Audio::Play("data/mus_theme_rep.mp3", 1.0f, BASS_SAMPLE_LOOP);
        break;
    case TUTORIAL_STAGE:
        break;
    case END_STAGE:
        break;
    }
}


void Stage::onExit(eStage last_stage) {
    Stage* stage = Game::instance->current_stage;

    switch (last_stage) {
    case PLAY_STAGE:
        Audio::Stop(gameplay);
        break;
    case MAIN_MENU:
        break;
    case TUTORIAL_STAGE:
        Audio::Stop(title_screen);
        break;
    case END_STAGE:
        break;
    }
}


MenuStage::MenuStage() {
    /*
    background.loadTGA("data/2.tga");

    background.flipY();

    if (background.data) {
        background_tex = new Texture(&background);
    }
    */

    /* -------------- AIXÒ ÉS NOU --------------*/
    //He afegit tot això inicialitza els botons del menú d'inici
    Game* instance = Game::instance;

    Vector2 position_a = Vector2(instance->window_width / 2, instance->window_height - 150);
    Vector2 position_b = Vector2(instance->window_width / 2, instance->window_height - 100);
    Vector2 size = Vector2(150, 40);

    Material material_a;
    material_a.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
    material_a.color = Vector4::BLUE;
    //material.diffuse = Texture::Get("data/textures/play.png");

    start_button = new EntityUI(position_a, size, material_a, eUIButtonID::UI_BUTTON_PLAY);
    exit_button = new EntityUI(position_b, size, material_a, eUIButtonID::UI_BUTTON_EXIT);

    //TITLE
    //Galactic
    Vector2 position = Vector2(instance->window_width / 2, instance->window_height / 2 - 153);
    size = Vector2(472, 66);

    Material material;
    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Galactic1.png");

    title1 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //Conqueror
    position = Vector2(instance->window_width / 2, instance->window_height / 2 - 85);
    size = Vector2(613, 68);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Conqueror1.png");

    title2 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //Sol Novus
    position = Vector2(instance->window_width / 2, instance->window_height / 2);
    size = Vector2(543, 92);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/SolNovus1.png");

    title3 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //Fons Sol Novus
    position = Vector2(instance->window_width / 2, instance->window_height / 2 - 10);
    size = Vector2(705, 155);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/textures/MetalEnd_Small.png");

    title4 = new EntityUI(position, size, material, eUIButtonID::UI_BACKGROUND);

    //BACKGROUND
    Texture* cube_texture = new Texture();
    cube_texture->loadCubemap("ProbaCubeMap", {
        "data/space_cube_new/px.png",
        "data/space_cube_new/nx.png",
        "data/space_cube_new/ny.png",
        "data/space_cube_new/py.png",
        "data/space_cube_new/pz.png",
        "data/space_cube_new/nz.png"
        });

    Material cubemap_material;
    cubemap_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/cubemap.fs");
    cubemap_material.diffuse = cube_texture;

    skybox = new EntityMesh(Mesh::Get("data/cubemap.ASE"), cubemap_material);
}

void MenuStage::render(Camera* camera) {

    //camera->enable();
    /*nou*/
    if (skybox) {
        skybox->model.setTranslation(camera->eye);

        glDisable(GL_DEPTH_TEST);
        skybox->render(camera);
        glEnable(GL_DEPTH_TEST);
    }

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

    /*nou*/
    title4->render(camera2d);
    title1->render(camera2d);
    title2->render(camera2d);
    title3->render(camera2d);

    /* -------------- AIXÒ ÉS NOU --------------*/
    //utilitza la camara 2d per a renderitzar cada un dels botons
    start_button->render(camera2d);
    exit_button->render(camera2d);
    /* -------------- AIXÒ ÉS NOU --------------*/
    //Draw Text
    Vector3 text_color = Vector3(0.0);
    drawText((instance->window_width / 2) - 55, (instance->window_height - 167), "PLAY", text_color, 5);
    drawText((instance->window_width / 2) - 55, (instance->window_height - 117), "EXIT", text_color, 5);
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

/*----------------Això és nou----------------*/
//metodes dels stages que ens falten, el de lore i el tutorial i el del final del nivell.
TutorialStage::TutorialStage() {
    //BACKGROUND
    Texture* cube_texture = new Texture();
    cube_texture->loadCubemap("ProbaCubeMap", {
        "data/space_cube_new/px.png",
        "data/space_cube_new/nx.png",
        "data/space_cube_new/ny.png",
        "data/space_cube_new/py.png",
        "data/space_cube_new/pz.png",
        "data/space_cube_new/nz.png"
        });

    Material cubemap_material;
    cubemap_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/cubemap.fs");
    cubemap_material.diffuse = cube_texture;

    skybox = new EntityMesh(Mesh::Get("data/cubemap.ASE"), cubemap_material);

    Game* instance = Game::instance;

    //LORE DROP
    // BLOC 1
    Vector2 position = Vector2(instance->window_width / 2, 50);
    Vector2 size = Vector2(720, 20);

    Material material;
    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore1.png");

    lore1 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2, 100);
    size = Vector2(720, 20);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore2.png");

    lore2 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2, 125);
    size = Vector2(720, 20);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore3.png");

    lore3 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2 - 178, 150);
    size = Vector2(370, 18);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore4.png");

    lore4 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2 - 5, 200);
    size = Vector2(720, 20);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore5.png");

    lore5 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2 - 3, 225);
    size = Vector2(720, 20);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore6.png");

    lore6 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2 - 3, 250);
    size = Vector2(720, 20);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore7.png");

    lore7 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2 - 305, 270);
    size = Vector2(110, 15);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore8.png");

    lore8 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2 - 10, 325);
    size = Vector2(700, 15);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore9.png");

    lore9 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2 + 3, 350);
    size = Vector2(720, 20);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore10.png");

    lore10 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2 - 185, 370);
    size = Vector2(345, 15);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore11.png");

    lore11 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2 - 50, 425);
    size = Vector2(600, 20);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore12.png");

    lore12 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //BLOC 2

    position = Vector2(instance->window_width / 2, 150);
    size = Vector2(750, 25);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore13.png");

    lore13 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2, 175);
    size = Vector2(667, 20);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore14.png");

    lore14 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2, 200);
    size = Vector2(620, 20);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore15.png");

    lore15 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2, 250);
    size = Vector2(326, 26);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Lore16.png");

    lore16 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //TUTORIAL BUTTONS
    //Spacebar
    position = Vector2(260, instance->window_height / 2 + 15);
    size = Vector2(398, 74.67); //597, 112

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Spacebar.png");

    spacebar = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //P
    position = Vector2(100, instance->window_height / 2 - 80);
    size = Vector2(80, 80);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/P.png");

    p = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //Z
    position = Vector2(100, instance->window_height / 2 - 175);
    size = Vector2(80, 80);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Z.png");

    z = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //Arrows
    position = Vector2(200, instance->window_height / 2 + 115);
    size = Vector2(259.11, 80); //366, 113

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Arrows.png");

    arrows = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //Title2
    position = Vector2(instance->window_width / 2, 50);
    size = Vector2(331.51, 40);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Controls.png");

    title2 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //how to play 1
    position = Vector2(instance->window_width / 2, instance->window_height - 50);
    size = Vector2(395, 21.16);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/tutorial_text_1.png");

    play1 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //how to play 2
    position = Vector2(instance->window_width / 2, instance->window_height - 80);
    size = Vector2(500, 21.16);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/tutorial_text_2.png");

    play2 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //Key Info 1 - Turbo
    position = Vector2(instance->window_width / 2 - 100, instance->window_height / 2 - 175);
    size = Vector2(251.32, 21.16);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/ActTurbo.png");

    info1 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //Key Info 2 - Pausa
    position = Vector2(instance->window_width / 2 - 100, instance->window_height / 2 - 80);
    size = Vector2(232.78, 21.16);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/PauseMenu.png");

    info2 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //Key Info 3 - Shoot
    position = Vector2(instance->window_width / 2 + 150, instance->window_height / 2 + 15);
    size = Vector2(94.70, 21.16);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Shoot.png");

    info3 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //Key Info 4
    position = Vector2(instance->window_width / 2 + 50, instance->window_height / 2 + 115);
    size = Vector2(168.25, 21.16);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Movement.png");

    info4 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

}

void TutorialStage::render(Camera* camera) {
    Game* instance = Game::instance;
    Camera* camera2d = instance->camera2d;

    if (skybox) {
        skybox->model.setTranslation(camera->eye);

        glDisable(GL_DEPTH_TEST);
        skybox->render(camera);
        glEnable(GL_DEPTH_TEST);
    }

    if (block == 0) {
        //bloc1
        lore1->render(camera2d);
    }
    else if (block == 1) {
        //bloc2
        lore1->render(camera2d);
        lore2->render(camera2d);
        lore3->render(camera2d);
        lore4->render(camera2d);
    }
    else if (block == 2) {
        //bloc3
        lore1->render(camera2d);
        lore2->render(camera2d);
        lore3->render(camera2d);
        lore4->render(camera2d);
        lore5->render(camera2d);
        lore6->render(camera2d);
        lore7->render(camera2d);
        lore8->render(camera2d);
    }
    else if (block == 3) {
        //bloc4
        lore1->render(camera2d);
        lore2->render(camera2d);
        lore3->render(camera2d);
        lore4->render(camera2d);
        lore5->render(camera2d);
        lore6->render(camera2d);
        lore7->render(camera2d);
        lore8->render(camera2d);
        lore9->render(camera2d);
        lore10->render(camera2d);
        lore11->render(camera2d);
    }
    else if (block == 4) {
        //bloc5
        lore1->render(camera2d);
        lore2->render(camera2d);
        lore3->render(camera2d);
        lore4->render(camera2d);
        lore5->render(camera2d);
        lore6->render(camera2d);
        lore7->render(camera2d);
        lore8->render(camera2d);
        lore9->render(camera2d);
        lore10->render(camera2d);
        lore11->render(camera2d);
        lore12->render(camera2d);
    }
    else if (block == 5) {
        //bloc6
        lore13->render(camera2d);
        lore14->render(camera2d);
        lore15->render(camera2d);
    }
    else if (block == 6) {
        //bloc7
        lore13->render(camera2d);
        lore14->render(camera2d);
        lore15->render(camera2d);
        lore16->render(camera2d);
    }
    else if (block == 7) {
        //bloc9 - tutorial 2
        title2->render(camera2d);
        spacebar->render(camera2d);
        p->render(camera2d);
        z->render(camera2d);
        arrows->render(camera2d);
        play1->render(camera2d);
        play2->render(camera2d);
        info1->render(camera2d);
        info2->render(camera2d);
        info3->render(camera2d);
        info4->render(camera2d);
    }
}

void TutorialStage::update(double seconds_elapsed, Camera* camera) {
    Game* instance = Game::instance;
    /*
    if ((Input::isKeyPressed(SDL_SCANCODE_X) || Input::isKeyPressed(SDL_SCANCODE_Z) || (Input::gamepads[0].isButtonPressed(A_BUTTON)) || (Input::gamepads[0].isButtonPressed(B_BUTTON)) || (Input::gamepads[0].isButtonPressed(Y_BUTTON)) || (Input::gamepads[0].isButtonPressed(X_BUTTON)))) {
        instance->audio->Play("sounds/coin.wav");
        instance->setStage(PLAY_STAGE, TUTORIAL_STAGE);
    }
    */

    if (Input::isKeyPressed(SDL_SCANCODE_S)) { block = 7; }
    if (Input::isKeyPressed(SDL_SCANCODE_Z) && zWasPressed == false) { block++; }

    if (block > 7) {
        instance->setStage(PLAY_STAGE, TUTORIAL_STAGE);
    }

    zWasPressed = Input::isKeyPressed(SDL_SCANCODE_Z);
}

PlayStage::PlayStage() {
    world = new World();

    Game* instance = Game::instance;

    /*----------------Això és nou----------------*/
    //Bàsicament es crean totes els EntityUI que utilitza el Play Stage copia tot el que hi ha a partir d'aquí

    Vector2 position_a;
    Vector2 size;
    Material material_a;

    // -------------Bg-------------
    Vector2 position = Vector2(instance->window_width / 2, instance->window_height - 45);
    size = Vector2(450, 90);

    //Material material_a;
    material_a.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material_a.diffuse = Texture::Get("data/textures/SingleEdge_Large.png");

    healthbar_background = new EntityUI(position, size, material_a, eUIButtonID::UI_BACKGROUND);

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

    //PAUSE MENU

    // Botó Resume i Exit
    position = Vector2(instance->window_width / 2, instance->window_height - 250);
    size = Vector2(200, 40);

    Material material;
    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");

    resume_button = new EntityUI(position, size, material, eUIButtonID::UI_BUTTON_RESUME);

    position = Vector2(instance->window_width / 2, instance->window_height - 200);

    exit_button = new EntityUI(position, size, material, eUIButtonID::UI_BUTTON_EXIT_TITLE); //canviar aquest botó per què et torni al main menu

    //Shader fosc per a la pausa
    position = Vector2(instance->window_width / 2, instance->window_height / 2);
    size = Vector2(instance->window_width, instance->window_height);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
    material.color = Vector4(0.0, 0.0, 0.0, 0.5);

    pause_shader = new EntityUI(position, size, material, eUIButtonID::UI_BACKGROUND);

    //Pause Text
    position = Vector2(instance->window_width / 2, 150);
    size = Vector2(321, 66);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Pause.png");
    material.color = Vector4(1.0);

    pause_text = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    // Inicialitzar FBO i Quad PER EFECTES D SHADER
    fbo = new FBO();
    fbo->create(instance->window_width, instance->window_height);

    screen_quad = new Mesh();
    screen_quad->createQuad(0, 0, 2, 2, false);
}

void PlayStage::render(Camera* camera) {
    /* -------------- AIXÒ ÉS NOU --------------*/
    //Instanciar la camara i el joc
    Game* instance = Game::instance;
    Camera* camera2d = instance->camera2d;
    Player* player = Player::getInstance();

    if (world) {
        world->render(camera);

		//render to texture for postprocess effects
        fbo->bind(); // Activem el "mode gravar a textura"
        world->render(camera); // Renderitzem el 3D normal
        fbo->unbind();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        Shader* shader = Shader::Get("data/shaders/screen.vs", "data/shaders/chromatic.fs");
        shader->enable();
        shader->setTexture("u_texture", fbo->color_textures[0], 0);
        float intensity = 0.0f;
        if (world->chromatic_aberration_timer > 0.0f) {
            intensity = world->chromatic_aberration_timer;
        }
        shader->setUniform("u_amount", intensity);
        screen_quad->render(GL_TRIANGLES);
        shader->disable();

        if (vignette_intensity > 0.01f) // Optimització: només si es veu
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_DEPTH_TEST);
            Shader* v_shader = Shader::Get("data/shaders/screen.vs", "data/shaders/vignetting.fs");
            v_shader->enable();
            v_shader->setUniform("u_intensity", vignette_intensity);
            screen_quad->render(GL_TRIANGLES);

            v_shader->disable();
        }


        glEnable(GL_BLEND);

        /* -------------- AIXÒ ÉS NOU --------------*/
        //render de cada EntityUI que hem creat al constructor
        player->health_bar->render(camera2d);
        player->turbo_bar->render(camera2d);
        healthbar_background->render(camera2d);
        level_indicator_bg->render(camera2d);
        turbo_indicator_bg->render(camera2d);
        //turbo_bg->render(camera2d);

        //turbo_indicator->render(camera2d);
        //level_indicator->render(camera2d);

        /*----------------Això és nou----------------*/
        //is està pausat el joc, renderitza els botons del menú de pausa
        if (instance->isPaused) {
            //renderitzes el menú de pause
            pause_shader->render(camera2d);
            pause_text->render(camera2d);
            resume_button->render(camera2d);
            exit_button->render(camera2d);

            Vector3 text_color = Vector3(1.0);
            drawText((instance->window_width / 2) - 85, (instance->window_height - 267), "RESUME", text_color, 5);
            drawText((instance->window_width / 2) - 55, (instance->window_height - 217), "EXIT", text_color, 5);
        }

        /*----------------Això és nou----------------*/
        //he provat a renderitzar un comptador per lo de la distància fins al final del nivell, de moment no funciona
        /*
        std::string t = std::to_string(instance->elapsed_time);
        drawText(instance->window_width / 2, instance->window_height / 2, t, Vector3(1.0), 10);*/

        glEnable(GL_DEPTH_TEST);
    }
}

void PlayStage::update(double seconds_elapsed, Camera* camera) {
    /*----------------Això és nou----------------*/
    //instancies de game i player
    Game* instance = Game::instance;
    Player* player = Player::getInstance();

    //VINYETATGE TURBO 
    float target_vignette = 0.0f;
    if (player->turbo) {
        target_vignette = 1.2f;
    }
    float speed = 3.0f * (float)seconds_elapsed;
    vignette_intensity = lerp(vignette_intensity, target_vignette, speed);

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

EndStage::EndStage() {
    Game* instance = Game::instance;
    /*
    // Imatge que ocupa tota la pantalla
    Vector2 position = Vector2(instance->window_width * 0.5f, instance->window_height * 0.5f);
    Vector2 size = Vector2(instance->window_width, instance->window_height);

    Material mat;
    mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    mat.diffuse = Texture::Get("data/endpic.png");  

    end_screen = new EntityUI(position, size, mat, eUIButtonID::UI_BACKGROUND);
    */

    //BACKGROUND
    Texture* cube_texture = new Texture();
    cube_texture->loadCubemap("ProbaCubeMap", {
        "data/space_cube_new/px.png",
        "data/space_cube_new/nx.png",
        "data/space_cube_new/ny.png",
        "data/space_cube_new/py.png",
        "data/space_cube_new/pz.png",
        "data/space_cube_new/nz.png"
        });

    Material cubemap_material;
    cubemap_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/cubemap.fs");
    cubemap_material.diffuse = cube_texture;

    skybox = new EntityMesh(Mesh::Get("data/cubemap.ASE"), cubemap_material);

    //Game Over
    Vector2 position = Vector2(instance->window_width / 2, instance->window_height / 2);
    Vector2 size = Vector2(741, 66);

    Material material;
    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/MissionFailed2.png");

    title1 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //Game Complete
    position = Vector2(instance->window_width / 2, instance->window_height / 2 - 250);
    size = Vector2(358, 66);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Mission.png");

    title2_1 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    position = Vector2(instance->window_width / 2, instance->window_height / 2 - 160);
    size = Vector2(744, 66);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Accomplished.png");

    title2_2 = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);

    //Results:
    position = Vector2(instance->window_width / 2, instance->window_height / 2 - 50);
    size = Vector2(243, 40);

    material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    material.diffuse = Texture::Get("data/Title/Results.png");

    results = new EntityUI(position, size, material, eUIButtonID::UI_TEXT);
}


void EndStage::render(Camera* camera) {
    Game* instance = Game::instance;
    Player* player = Player::instance;
    Camera* cam2d = Game::instance->camera2d;

    /*
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Fons negre (per si la imatge té transparència)
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // La teva imatge a pantalla completa
    if (end_screen) {
        end_screen->render(cam2d);
    }

    glEnable(GL_DEPTH_TEST);
    */

    if (skybox) {
        skybox->model.setTranslation(camera->eye);

        glDisable(GL_DEPTH_TEST);
        skybox->render(camera);
        glEnable(GL_DEPTH_TEST);
    }

    if (player->isDestroyed) {
        title1->render(cam2d);
    }
    else {
        title2_1->render(cam2d);
        title2_2->render(cam2d);
        results->render(cam2d);

        drawText(instance->window_width / 2, instance->window_height / 2, "Score:", Vector3(1.0), 5);
    }
}

void EndStage::update(double seconds_elapsed, Camera* camera) {
    Game* instance = Game::instance;
    /*
    if ((Input::isKeyPressed(SDL_SCANCODE_X) || Input::isKeyPressed(SDL_SCANCODE_Z) || (Input::gamepads[0].isButtonPressed(A_BUTTON)) || (Input::gamepads[0].isButtonPressed(B_BUTTON)) || (Input::gamepads[0].isButtonPressed(Y_BUTTON)) || (Input::gamepads[0].isButtonPressed(X_BUTTON)))) {
        instance->audio->Play("sounds/coin.wav");
        instance->setStage(MAIN_MENU, END_STAGE);
    }
    */

    //temporitzador de 1s per què no s'en vagi directament al menú principal si estàs presionant Z
    if (Input::isKeyPressed(SDL_SCANCODE_Z) && zWasPressed == false) { instance->setStage(MAIN_MENU, END_STAGE); }
    
    zWasPressed = Input::isKeyPressed(SDL_SCANCODE_Z);
}
