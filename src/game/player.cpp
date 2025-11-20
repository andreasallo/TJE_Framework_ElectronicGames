
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
    : EntityCollider(mesh, material, name)
{
    instance = this;
    layer = eCollisionFilter::PLAYER;

	position = model.getTranslation();
	
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

    // Calcular direcci� de moviment segons c�mera
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
            std::cout << �Collision!� << std::endl;
        }
    }
}
*/

   
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

/*
void Player::update(float delta_time)
{

    //PER ACCELARACIO PER QUAN UTILITZI EL TURBO
    if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) {
        targetSpeed = 50.0f; // Velocidad turbo
    }
    else {
        targetSpeed = 20.0f; // Velocidad normal
    }

    //variable q me diga si estoy en modo turbo. default_speed i normal_seppd.

	targetSpeed = 15.0f; // Velocidad constante


    position.x += move2.x * lateralSpeed * delta_time;
    position.y += move2.y * lateralSpeed * delta_time;

    // clamp dins límits
    if (position.x < minX) position.x = minX;
    if (position.x > maxX) position.x = maxX;
    if (position.y < minY) position.y = minY;
    if (position.y > maxY) position.y = maxY;
    
    //velocidad poco a poco
    //speed = speed + (targetSpeed - speed) * delta_time * 2.0f;
	//interpolacion exponencial, lerp fa que suavitzi la velocitat
    float k = explerpFactor(delta_time, 4.0f);   //4.0f → resposta ràpida però suau
    speed = lerp(speed, targetSpeed, k);

    //explerpFactor -> usa delta time. DENTRO UPDATE USAR ESTE
    
    Vector3 old_position = model.getTranslation();
    model.translate(0.0f, 0.0f, speed * delta_time);
    Vector3 new_position = model.getTranslation();
    
    if (canMove(new_position) == false) {
        model.setTranslation(old_position);
        //detener velocidad si chocas?????????????
        targetSpeed = 0.0f;
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

    


    // Actualizar matriz global
    EntityMesh::update(delta_time);
}*/

void Player::update(float dt)
{
    // ---- INPUT ----

    float x = 0, y = 0;
    if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT))  x += 1.0f;
    if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) x -= 1.0f;
    if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP))    y -= 1.0f;
    if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN))  y += 1.0f;

    Vector3 move(x, y, 0);
    if (move.length() > 0.001f)
        move.normalize();

    // ---- MOVIMENT LATERAL ----
    position.x += move.x * lateralSpeed * dt;
    position.y += move.y * lateralSpeed * dt;

    // ---- LIMITS ----
    position.x = clamp(position.x, minX, maxX);
    position.y = clamp(position.y, minY, maxY);

    // ---- ROTACIÓ VISUAL ----
    float maxRoll = 20.0f * DEG2RAD;
    float maxPitch = 18.0f * DEG2RAD;

    float desiredRoll = move.x * maxRoll;
    float desiredPitch = move.y * maxPitch;

    float k = explerpFactor(dt, 6.0f);

    visualRoll = lerp(visualRoll, desiredRoll, k);
    visualPitch = lerp(visualPitch, desiredPitch, k);

    // ---- MODEL ----
    model.setIdentity();
    model.setTranslation(position);
    model.rotate(visualPitch, Vector3(1, 0, 0));
    model.rotate(visualRoll, Vector3(0, 0, 1));

    EntityCollider::update(dt);
}

void Player::render(Camera* camera)
{
    EntityCollider::render(camera);
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

