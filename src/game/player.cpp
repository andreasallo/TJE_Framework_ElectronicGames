
//front=nYaw.frontVector;
//right=-nYaw.rightVector;

//collisions
//model.setTranslation(position);
// model.rotate() //cojer yaw anteriror, cojer delta y acumular dt
//EntityMesh::update(delta_time);

//exponential interpolation frmae-rate; usar lerp
//foto: eye center eye center
#include "player.h"
#include "framework/input.h"
#include "framework/camera.h"
#include "game/game.h"
#include "framework/collision.h"
#include "framework/world.h"
#include "graphics/shader.h"
#include "graphics/mesh.h"
#include <cmath>

#define RENDER_DEBUG

Player* Player::instance = nullptr;

Player::Player(Mesh* mesh, const Material& material, const std::string& name)
    : EntityMesh(mesh, material, name)
{
    instance = this;
}

void Player::render(Camera* camera)
{
    // Render mesh
    EntityMesh::render(camera);

#ifdef RENDER_DEBUG
    // Render sphere using a basic.vs + flat.fs shader
    Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
    Mesh * mesh = Mesh::Get("data/meshes/sphere.obj");

    shader->enable();

    Matrix44 m;
    m.setTranslation(getGlobalMatrix().getTranslation());
    m.translate(0.0f, height, 0.0f);
    m.scale(sphere_radius, sphere_radius, sphere_radius);

    shader->setUniform("u_color", Vector4(0.0f, 1.0f, 0.0f, 1.0f));
    shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
    shader->setUniform("u_model", m);

    mesh->render(GL_LINES);

    shader->disable();
#endif RENDER_DEBUG
}


   
const Vector3& Player::getMovementDirection() {

    // Calcular direcció de moviment segons càmera
    Camera* camera = World::getInstance()->camera;
    Vector3 move_dir(0.0f, 0.0f, 0.0f);
    Matrix44 mYaw;
    mYaw.setRotation(camera->yaw, Vector3(0, 1, 0));
    Vector3 front = mYaw.frontVector();
    Vector3 right = -mYaw.rightVector();

    if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) move_dir -= front;
    if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) move_dir += front;
    if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) move_dir -= right;
    if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) move_dir += right;

    return move_dir.normalize();
}


void Player::update(float delta_time)
{

    //PER ACCELARACIO PER QUAN UTILITZI EL TURBO
    /*if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) {
        targetSpeed = 50.0f; // Velocidad turbo
    }
    else {
        targetSpeed = 20.0f; // Velocidad normal
    }*/

	targetSpeed = 0.0f; // Velocidad constante

    
    //velocidad poco a poco
    //speed = speed + (targetSpeed - speed) * delta_time * 2.0f;
	speed = lerp(speed, targetSpeed, 2.0f * delta_time);
    
    Vector3 old_position = model.getTranslation();
    model.translate(0.0f, 0.0f, speed * delta_time);
    Vector3 new_position = model.getTranslation();

    if (canMove(new_position) == false) {
        model.setTranslation(old_position);
        //detener velocidad si chocas?????????????

    }
    
    float rot_speed_adjusted = 90.0f * delta_time; // Velocidad de giro
    float rad_factor = (float)DEG2RAD;

    
    //float rot_speed_adjusted = rotation_speed * delta_time * 50.0f; // Ajustado para delta_time

    if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {
        model.rotate(rot_speed_adjusted * rad_factor, Vector3(-1, 0, 0)); // Morro arriba
    }
    if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
        model.rotate(rot_speed_adjusted * rad_factor, Vector3(1, 0, 0));  // Morro abajo
    }

    
    if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
        model.rotate(rot_speed_adjusted * rad_factor, Vector3(0, -1, 0)); // Girar izq
        model.rotate(rot_speed_adjusted * 0.5f * rad_factor, Vector3(0, 0, 1)); // Inclinar un poco
    }
    if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
        model.rotate(rot_speed_adjusted * rad_factor, Vector3(0, 1, 0)); // Girar der
        model.rotate(rot_speed_adjusted * 0.5f * rad_factor, Vector3(0, 0, -1)); // Inclinar un poco
    }

    //new_eye = lerp(old_eye, new_eye, lerp_factor);


    // Actualizar matriz global
    EntityMesh::update(delta_time);
}

bool Player::canMove(const Vector3& new_position) {

    std::vector<sCollisionData> collisions;
    World* world = World::getInstance();

    bool collided = false;

    for (Entity* e : world->root->children) {
        if (e != this && Collision::TestEntitySphere(e, sphere_radius, new_position + Vector3(0.0f, height, 0.0f), collisions, eCollisionFilter::SCENARIO)) {
            return false;
        }
    }
    return true;

}

/*
void Player::collison(Vector3& position) {
    // Get sphere center from character position
    Vector3 character_center = position + Vector3(0, 1, 0);    
    World* world = World::getInstance();

    for (EntityMesh* e : world->root->children) {
        Mesh* mesh =
        // Check if collides with wall using sphere (radius = 1)
        if (mesh->testSphereCollision(entity->model, character_center, 1.0, col_point, col_normal)) {
            std::cout << “Collision!” << std::endl;
        }
    }
}
*/

/*void Player::update(float delta_time)
{
    Vector3 position = model.getTranslation();
    Camera* camera = Game::instance->camera;

    velocity = { 0.0f,0.0f,0.0f };

    Vector3 move_dir = getMovementDirection();

    //velocity
    float current_speed = speed;
    move_dir += current_speed;
    velocity += move_dir;

    //still no colision
    position += velocity * delta_time;



    //rotate player model based on camera yaw
    if (velocity.length() > 0.0f) {

        Vector3 dir = velocity;
        dir.normalize();
        float delta_yaw = atan2(-velocity.x, velocity.z);

        float smoothing = 0.1f;
        float new_yaw = lerpAngleRad(camera->yaw + delta_yaw, delta_yaw, smoothing);
        model.setRotation(new_yaw, Vector3(0, 1, 0));

        //player getiona su movimiento, world solo la camera se mueve

        //model.rotate(camera->yaw, Vector3(0, 1, 0));  // gira el model segons càmera

        //update new position
        model.setTranslation(position);
        EntityMesh::update(delta_time); // actualitza base
    }
    // Mou el jugador
    //model.translate(velocity.x, velocity.y, velocity.z);
}*/

/*

void Player::update(float delta_time)
{
    //Obtener los vectores de orientación actuales del avión
    Vector3 position = model.getTranslation();
    Vector3 front = model.frontVector().normalize();
    Vector3 up = model.topVector().normalize();
    Vector3 right = model.rightVector().normalize();

    float roll_speed = 50.0f;

    //Controlar la ROTACIÓN con las teclas

    //pitch
    if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {
        model.rotate(pitch_speed * delta_time, right);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
        model.rotate(-pitch_speed * delta_time, right);
    }

    //yaw and roll?
    if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
        model.rotate(roll_speed* delta_time, front);
        model.rotate(yaw_speed * 0.5f * delta_time, up);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
        model.rotate(-roll_speed * delta_time, front);
        model.rotate(-yaw_speed * 0.5f * delta_time , up);
    }


    
    front = model.frontVector().normalize();

    velocity = front * forward_speed;


    Vector3 new_position = position + velocity * delta_time;


    // 6. Comprobar colisiones ANTES de moverse
    if (canMove(new_position)) {
        position = new_position;
        model.setTranslation(position);
    }


    EntityMesh::update(delta_time);
}*/