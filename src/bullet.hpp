#pragma once
#include<raylib.h>
#include"entity.hpp"
#include"staticobj.hpp"

class Bullet : public Entity {
public:
	Bullet(Sprite* sprite, Vector3 position, bool isWall, 
		std::vector<StaticObject*>* objects) :
		Entity(sprite, position, isWall, 0) {
			this->objects = objects;
	}
	~Bullet() {}

	void Behavior(bool isPlayerAttacking, Camera* camera,
		Image* map, Inventory* inventory) override;

	Vector3 direction;
	bool playerIsOwner = false;
	bool queuedForDeletion = false;
private:
	std::vector<StaticObject*>* objects;

	void CheckCollisions(Image* map);
};

