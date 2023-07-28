#pragma once
#include<raylib.h>
#include"entity.hpp"


class Particle : public Entity {
public:
	Particle(Sprite* sprite, Vector3 position, bool isWall) :
		Entity(sprite, position, isWall, 1) {
			this->creationTime = GetTime();
	}
	~Particle() {}

	void Behavior(bool isPlayerAttacking, Camera* camera,
		Image* map, Inventory* inventory) override;

	bool queuedForDeletion = false;

private:
	double creationTime = 0;
};

