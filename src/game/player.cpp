

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
#include <cmath>

Player* Player::instance = nullptr;

Player::Player(Mesh* mesh, const Material& material, const std::string& name)
    : EntityMesh(mesh, material, name)
{
    instance = this;
}


void Player::update(float delta_time)
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



bool Player::canMove(const Vector3& new_position) {
    // Implement collision detection logic here

	std::vector<sCollisionData> collisions;
	World* world = World::getInstance();

    bool collided = false;

    for (Entity* e : world->root->children) {
        collided != Collision::TestEntitySphere(e,sphere_radius, new_position + Vector3(0.0f,height,0.0f), collisions, eCollisionFilter::SCENARIO);
    }

	return !collided;

}

