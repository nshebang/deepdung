#pragma once
#include<raylib.h>
#include<vector>
#include"entity.hpp"
#include"bullet.hpp"

class Snake : public Entity {
public:
	Snake(Sprite* sprite, Vector3 position,
		bool isWall, std::vector<Bullet*>* bullets) :
		Entity(sprite, position, isWall, 4) {
			this->bullets = bullets;
	}
	~Snake() {}

	void Behavior(bool isPlayerAttacking, Camera* camera,
		Image* map, Inventory* inventory) override;

private:
	float moveFactor = 2.f;
	std::vector<Bullet*>* bullets;

	void CheckCollisions(Image* map);
};

