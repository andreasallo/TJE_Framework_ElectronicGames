#include "game/asteroid.h"

void Asteroid::addChild(Asteroid* child)
{
	if (child->parent) {
		std::cerr << "Child has already a parent, remove it first!" << std::endl;
		return;
	}

	// Checks if it's already a child
	auto it = std::find(children.begin(), children.end(), child);
	if (it != children.end()) {
		std::cerr << "Entity is already one of the children!" << std::endl;
		return;
	}

	child->parent = this;
	children.push_back(child);
}

void Asteroid::removeChild(Asteroid* child)
{
	// Checks if it's a child
	auto it = std::find(children.begin(), children.end(), child);
	if (it == children.end()) {
		std::cerr << "Entity is not a child!!" << std::endl;
		return;
	}

	children.erase(it);
	child->parent = nullptr;
}

void Asteroid::render(Camera* camera) {
	for (int i = 0; i < children.size(); ++i) {
		children[i]->render(camera);
	}
}

void Asteroid::update(double delta_time) {
	Vector3 target = Vector3(0, 10, 0);
	float targetSpeed = 1.0f; // Velocidad constante

	//velocidad poco a poco
	//speed = speed + (targetSpeed - speed) * delta_time * 2.0f;
	speed = lerp(speed, targetSpeed, 2.0f * delta_time);

	Vector3 old_position = model.getTranslation();
	model.translate(0.0f, 0.0f, speed * delta_time);
	Vector3 new_position = model.getTranslation();
}