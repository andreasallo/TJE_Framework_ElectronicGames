#pragma once

#include "framework/entities/entityMesh.h"

#include "framework/entities/entityMesh.h"

//hereda de EntityMesh

//hago singleton-> todo static, pero solo lo pongo en la instancia

class Mesh;
class Material;
class Camera;

class Player : public EntityMesh {
private:


	bool render_mesh = true;
	Vector3 velocity = Vector3(0.0f);

	
	float sphere_radius = 1.5f; //probar valors
	float height = 0.0f;

	float speed = 0.0f;          // Velocidad actual
	float targetSpeed = 0.0f;    // Velocidad a la que queremos llegar
	float acceleration = 40.0f;  // Qué tan rápido acelera
	float rotation_speed = 1.5f; // Velocidad de giro

	

	const Vector3& getMovementDirection();



public:
	static Player* instance;
	Player() {};
	Player(Mesh* mesh, const Material& material, const std::string& name = "");

	static Player* getInstance() { return instance; }


	//void render(Camera* camera) override;
	void update(float delta_time) override;


	void SetRenderMesh(bool new_render_mesh) { render_mesh = new_render_mesh; }
	bool canMove(const Vector3& new_position);
	void isGrounded(const Vector3& new_position, float max_ray_dist, Vector3& col_point);

	//for(Entity* e: world->root->children){

};
