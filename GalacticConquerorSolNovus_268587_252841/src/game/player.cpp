
#include "player.h"
#include "framework/input.h"
#include "framework/camera.h"
#include "game/game.h"
#include "framework/collision.h"
#include "framework/world.h"
#include "graphics/shader.h"
#include "graphics/mesh.h"
#include <cmath>
#include "game/asteroid.h"
#include "game/asteroidControl.h"
#include "framework/audio.h"
#include "game/projectile.h"
#include "graphics/particles.h"



#define RENDER_DEBUG

Player* Player::instance = nullptr;

Player::Player(Mesh* mesh, const Material& material, const std::string& name)
    : EntityCollider(mesh, material, name)
{
    instance = this;
    layer = eCollisionFilter::PLAYER;

    position = model.getTranslation();

    if (mesh) {
        // .length() nos da la distancia del centro a la esquina de la caja
        collision_radius = mesh->box.halfsize.length() * 0.2; //la bounidng box es enorme a causa d les ales del avio, multipliquem per 0.35 per ajustar millor. si la vull enccar ames petita 
    }

    /* -------------- AIXÒ ÉS NOU --------------*/
    //Inicialització de les entityUI del menú de pausa

    turbo_timer = 0.0f;

    Game* instance = Game::instance;

    // -------------Barra de Vida - Position/Size-------------
    Vector2 position_a = Vector2(instance->window_width / 2, instance->window_height - 45);
    Vector2 size = Vector2(350, 30);

    Material material_a;
    material_a.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/health_bar.fs"); //healthbar.fs // abans: status_bar.fs
    //material_a.diffuse = Texture::Get("data/textures/Healthy1_Large.png");
    material_a.color = Vector4(0.25);

    health_bar = new EntityUI(position_a, size, material_a, eUIButtonID::UI_HEALTHBAR);

    // -------------Barra de Turbo - Position/Size-------------
    position_a = Vector2(instance->window_width / 2, instance->window_height / 2);
    size = Vector2(350, 30);
    position_a = Vector2(instance->window_width - 25, instance->window_height / 2);
    size = Vector2(25, 267);


    Material material_b;
    material_b.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/turbo_bar.fs");
    material_b.color = Vector4(0.25);

    turbo_bar = new EntityUI(position_a, size, material_b, eUIButtonID::UI_TURBO);
    turbo_bar->mask = 0.0f;
}



void Player::render(Camera* camera)
{
    // Render mesh
    
    EntityMesh::render(camera);

    if (Game::IsDebugMode())
    {

        // Render sphere using a basic.vs + flat.fs shader
        Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
        Mesh* mesh = Mesh::Get("data/meshes/sphere.obj");

        shader->enable();

        Matrix44 m;
        float pivot_offset = -0.29f;  // ajusta fins que quedi al centre
        m.setTranslation(getGlobalMatrix().getTranslation() + Vector3(0, -pivot_offset, 0));

        //m.setTranslation(getGlobalMatrix().getTranslation());
        m.scale(collision_radius, collision_radius, collision_radius);


        shader->setUniform("u_color", Vector4(0.0f, 1.0f, 0.0f, 1.0f));
        shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
        shader->setUniform("u_model", m);

        mesh->render(GL_LINES);

        shader->disable();
    }
}


   
//alfinal NOT USED
const Vector3& Player::getMovementDirection() {

    // Calcular direcci� de moviment segons c�mera
    Camera* camera = World::getInstance()->camera;
    Vector3 move_dir(0.0f, 0.0f, 0.0f);
    Matrix44 mYaw;
    mYaw.setRotation(camera->yaw, Vector3(0, 1, 0));
    Vector3 front = mYaw.frontVector();
    Vector3 right = -mYaw.rightVector();

    if ( Input::isKeyPressed(SDL_SCANCODE_UP)) move_dir -= front;
    if ( Input::isKeyPressed(SDL_SCANCODE_DOWN)) move_dir += front;
    if ( Input::isKeyPressed(SDL_SCANCODE_LEFT)) move_dir -= right;
    if ( Input::isKeyPressed(SDL_SCANCODE_RIGHT)) move_dir += right;

    return move_dir.normalize();
}



   

void Player::update(float dt)
{
    /* -------------- AIXÒ ÉS NOU --------------*/
    Game* instance = Game::instance;

    turbo = false;
    float x = 0, y = 0;
    if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT))  x += 1.0f;
    if (Input::isKeyPressed(SDL_SCANCODE_X) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) x -= 1.0f;
    if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP))    y -= 1.0f;
    if (Input::isKeyPressed(SDL_SCANCODE_F) || Input::isKeyPressed(SDL_SCANCODE_DOWN))  y += 1.0f;


    Vector3 move(x, y, 0);
    if (move.length() > 0.001f) {
        move.normalize();
    }

    //MOVIMENT LATERAL
    position.x += move.x * lateralSpeed * dt;
    position.y += move.y * lateralSpeed * dt;

	//AVISAR JUGADOR SI TOCA ELS LÍMITS
    if (limit_sound_timer > 0.0f) {
        limit_sound_timer -= dt;
    }
    bool hit_limit = false;
    if (position.x < minX || position.x > maxX) {
        hit_limit = true;
    }
    if (position.y < minY || position.y > maxY) {
        hit_limit = true;
    }
    if (hit_limit && limit_sound_timer <= 0.0f) {
 
        Audio::Play("data/pip.wav", 0.8f, BASS_SAMPLE_MONO);
        limit_sound_timer = 0.5f;
    }

    //limitar pantalla
    position.x = clamp(position.x, minX, maxX);
    position.y = clamp(position.y, minY, maxY);

    float forwardSpeed = turbo ? 80.0f : 40.0f; //turbo->doble velocitat
    position.z += forwardSpeed * dt;

    //ROTACIÓ VISUAL 
    float maxRoll = 20.0f * DEG2RAD;
    float maxPitch = 18.0f * DEG2RAD;

    float desiredRoll = move.x * maxRoll;
    float desiredPitch = move.y * maxPitch;

    float k = explerpFactor(dt, 12.0f); //6.0????

    visualRoll = lerp(visualRoll, desiredRoll, k); //lerp per suavitza la transicio, el valor k l'he anat provant
    visualPitch = lerp(visualPitch, desiredPitch, k);


    model.setIdentity();
    model.setTranslation(position);
    model.rotate(visualPitch, Vector3(1, 0, 0));
    model.rotate(visualRoll, Vector3(0, 0, 1));

    if (lives != previous_lives) {
        time_to_heal = 3.0f;
    }

    //Al cap de 3 segons recupera la vida del jugador. Des de l'últim cop que ha rebut la nau.
    if (lives < max_lives) {
        time_to_heal -= dt;

        if (time_to_heal < 0.0f) {
            time_to_heal = 3.0f;
            lives = max_lives;
            health_bar->mask = max_lives / lives;
        }
    }

    previous_lives = lives;

    //turbo
    bool trying_to_turbo = Input::isKeyPressed(SDL_SCANCODE_Z);

    if (trying_to_turbo && turbo_timer > 0.0f) {
        turbo = true;
        Audio::Play("data/turbo.wav", 2.0f, BASS_SAMPLE_MONO);
        turbo_timer -= dt;
    }
    else {
        // Si no prems la tecla O se t'ha acabat el temps -> Turbo OFF
        turbo = false;
    }

    //protecció: que el timer no baixi de 0
    if (turbo_timer < 0.0f) {
        turbo_timer = 0.0f;
        turbo = false; 
    }

    if (turbo_bar) {
        turbo_bar->mask = turbo_timer / turbo_duration;
    }

    if (Input::wasKeyPressed(SDL_SCANCODE_SPACE))
    {
        World::instance->requestShoot();
    }

    //ENGINE FIRE POSITION
    {
        // posició del motor (lleugerament darrere la nau)
        Vector3 planePos = model.getTranslation();
        Vector3 back = model.frontVector().normalize() * -1.0f;
        Vector3 enginePos =
            planePos
            + back * 2.5f   // separació en Z
            + Vector3(0, 0.2f, 0); // lleugerament amunt

        World::instance->engine_fire_emitter->setEmitPosition(enginePos);

        // Direcció del foc → darrere
        World::instance->engine_fire_emitter->setEmitVelocity(back * 15.0f);

        // Turbo intensifica el foc
        if (turbo)
        {
            World::instance->engine_fire_emitter->setEmitRate(0.025f);
            World::instance->engine_fire_emitter->setSizesCurve({ 0.4f, 1.2f, 0.0f });
        }
        else
        {
            World::instance->engine_fire_emitter->setEmitRate(0.01f);
            World::instance->engine_fire_emitter->setSizesCurve({ 0.3f, 0.8f, 0.0f });
        }
    }
    EntityCollider::update(dt);
}



